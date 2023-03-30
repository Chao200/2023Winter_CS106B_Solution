#include "MagicWand.h"
#include "random.h"
using namespace std;

/* Size of the tip of the magic wand. */
const double kTipWidth = 10;
const double kTipHeight = 20;

/* How tall the wand body - excluding the tip - is. */
const double kWandHeight = 100;

/* How many particles to create per frame when the mouse is down. */
const int kDownRate = 50;

/* How many particles to create when the mouse moves. */
const int kMoveRate = 1;

/* Speed of a particle. */
const double kMinStreamerSpeed = 3;
const double kMaxStreamerSpeed = 20;

/* Lifetime of a particle. */
const int kMinLifetime = 0;
const int kMaxLifetime = 20;

/* Speed controls for a falling particle. */
const double kMinMoveX = -3;
const double kMaxMoveX = +3;

const double kMinMoveY = -6;
const double kMaxMoveY = -3;

void MagicWand::tick()  {
    /* If the mouse is down, create a shower of particles from the mouse
     * position - which is also where the tip of the magic wand is.
     */
    if (mouseDown) {
        for (int i = 0; i < kDownRate; i++) {
            /* Random angle / speed to fire the particle. */
            double theta  = randomReal(0, 2 * M_PI);
            double speed = randomReal(kMinStreamerSpeed, kMaxStreamerSpeed);

            /* How long the particle lives for. */
            int lifetime  = randomInteger(kMinLifetime, kMaxLifetime);

            Particle particle;

            /* Center on the mouse. */
            particle.x = mouse.x;
            particle.y = mouse.y;

            /* Polar coordinates to the rescue! */
            particle.dx = speed * cos(theta);
            particle.dy = speed * sin(theta);

            particle.lifetime = lifetime;
            particle.type = ParticleType::STREAMER;
            particle.color = Color::random();

            system.add(particle);
        }
    }

    system.moveParticles();
}

void MagicWand::draw() {
    system.drawParticles();

    /* Draw the tip of the wand. */
    setColor(Color::WHITE);
    fillRect(mouse.x - kTipWidth / 2, mouse.y - kTipHeight / 2, kTipWidth, kTipHeight);

    /* Draw the body of the wand, and the wand border. */
    setColor(Color::BLACK);
    drawRect(mouse.x - kTipWidth / 2, mouse.y - kTipHeight / 2, kTipWidth, kTipHeight);
    fillRect(mouse.x - kTipWidth / 2, mouse.y + kTipHeight / 2, kTipWidth, kWandHeight);
}

/* Mouse motion. In respond to mouse events, remember where the mouse is. */
void MagicWand::mouseMoved(double x, double y) {
    /* Remember where the mouse is. */
    mouse = { x, y };

    /* Create particles that emanates from the mouse position. */
    for (int i = 0; i < kMoveRate; i++) {
        Particle particle;
        particle.x = x;
        particle.y = y;

        particle.dx = randomReal(kMinMoveX, kMaxMoveX);
        particle.dy = randomReal(kMinMoveY, kMaxMoveY);

        particle.lifetime = INT_MAX; // Live forever, basically
        particle.color = Color::random();

        particle.type = ParticleType::BALLISTIC;
        system.add(particle);
    }
}

void MagicWand::mousePressed(double x, double y) {
    /* Record that the mouse is down. */
    mouseDown = true;
    mouse = { x, y };
}

void MagicWand::mouseReleased(double, double) {
    /* Record that the mouse is up. */
    mouseDown = false;
}

void MagicWand::mouseDragged(double x, double y) {
    /* Record where the mouse is. */
    mouse = { x, y };
}
