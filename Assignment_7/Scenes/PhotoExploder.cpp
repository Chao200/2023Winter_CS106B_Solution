#include "PhotoExploder.h"
#include "GUI/MiniGUI.h"
#include "gobjects.h"
#include "filelib.h"
#include "set.h"
#include "random.h"
using namespace std;

/* Countdown timer - in frames. */
const int kCountdownTime = 25;

/* Number of steps to put the photo back together. */
const int kNumSteps = 40;

/* Number of steps to wait on the photo for. */
const int kImageSteps = 50;

/* Background color. */
const Color kBackgroundColor(0x43, 0x44, 0x50);

/* Fraction of space dedicated to a border. */
const double kPadding = 0.3;

/* Speed range. */
const double kMinSpeed = 1;
const double kMaxSpeed = 4;

/* Pixel spacing. */
const double kPixelSpacing = 3.5;

namespace {
    /* Utility function to decode a GImage pixel into a color. */
    Color decode(int value) {
        return Color(GCanvas::getRed(value),
                     GCanvas::getGreen(value),
                     GCanvas::getBlue(value));
    }

    /* Gets all image files in a random order. */
    Queue<string> imageFilesIn(const string& directory) {
        Set<string> images;

        /* Find all image files. */
        for (string file: listDirectory(directory)) {
            if (endsWith(toLowerCase(file), ".png") ||
                endsWith(toLowerCase(file), ".jpg") ||
                endsWith(toLowerCase(file), ".jpeg") ||
                endsWith(toLowerCase(file), ".bmp") ||
                endsWith(toLowerCase(file), ".gif")) {
                images += directory + "/" + file;
            }
        }

        /* Scramble the order. */
        Queue<string> result;
        while (!images.isEmpty()) {
            string next = randomElement(images);
            result.enqueue(next);
            images -= next;
        }
        return result;
    }
}

PhotoExploder::PhotoExploder() {
    files = imageFilesIn("res/photos");
    loadNextImage();
}

void PhotoExploder::setImage(const string& filename) {
    /* Load the image. */
    GImage image(filename);

    /* Content area - where the image is allowed to go. */
    GRectangle contentArea = {
        kPadding * SCENE_WIDTH,
        kPadding * SCENE_HEIGHT,
        (1.0 - 2 * kPadding) * SCENE_WIDTH,
        (1.0 - 2 * kPadding) * SCENE_HEIGHT
    };

    /* Get a bounding box for the image. */
    auto bounds = fitToBounds(contentArea, image.getWidth() / image.getHeight());

    /* For each pixel in the bounds, create a particle. */
    for (double x = 0; x < bounds.width; x += kPixelSpacing) {
        for (double y = 0; y < bounds.height; y += kPixelSpacing) {
            /* Which pixel should we extract? */
            double srcX = x * image.getWidth()  / bounds.width;
            double srcY = y * image.getHeight() / bounds.height;

            addParticle(bounds.x + x, bounds.y + y, decode(image.getPixel(srcX, srcY)));
        }
    }

    /* Reset the countdown. */
    countdown  = kCountdownTime;
    stepsLeft  = kNumSteps;
    imagePause = kImageSteps;
}

void PhotoExploder::loadNextImage() {
    /* Using the 'looper' strategy from class, grab the next image from
     * the queue, load it, and put it back into the queue.
     */
    string next = files.dequeue();
    setImage(next);
    files.enqueue(next);
}

void PhotoExploder::addParticle(double x, double y, Color color) {
    Particle particle;

    /* Choose a random angle and speed. */
    double theta = randomReal(0, 2 * M_PI);
    double speed = randomReal(kMinSpeed, kMaxSpeed);

    particle.dx = speed * cos(theta);
    particle.dy = speed * sin(theta);

    /* Figure out where this particle will be in a certain number of time steps. */
    particle.x = x - particle.dx * kNumSteps;
    particle.y = y - particle.dy * kNumSteps;

    particle.lifetime = kNumSteps;

    particle.color = color;
    system.add(particle);
}

void PhotoExploder::tick() {
    /* If the countdown is still active, do nothing. */
    if (countdown > 0) {
        countdown--;
    }
    /* Otherwise, if we're in the active window, move particles. */
    else if (stepsLeft > 0) {
        system.moveParticles();
        stepsLeft--;
    }
    /* Otherwise, if we're at the pause window, marvel. */
    else if (imagePause > 0) {
        imagePause--;
    }
    /* Otherwise, reset the image. */
    else {
        system.moveParticles(); // Clear all particles
        loadNextImage();
    }
}

void PhotoExploder::draw() {
    setColor(kBackgroundColor);
    fillRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    system.drawParticles();
}
