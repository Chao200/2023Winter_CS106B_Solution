#include "GUI/MiniGUI.h"
#include "Fire.h"
#include "gtimer.h"
#include "gbutton.h"
#include <iomanip>
#include <sstream>
using namespace std;

namespace {
    /* Number of rows/columns to use in the fire for the animation. */
    constexpr int kNumRows = 80;
    constexpr int kNumCols = 150;

    /* Background color. */
    constexpr char kBackgroundColor[] = "#727472"; // Nickel

    /* Window padding. */
    constexpr double kPadding = 20;

    /* Aspect ratio of the display. */
    constexpr double kAspectRatio = double(kNumCols) / kNumRows;

    /* Pixels per fire cell. */
    constexpr double kPixelsPerCell = 4;

    /* Frames to wait when warming up or cooling down. */
    constexpr int kUpdateDelay = 2;

    /* Text for starting/ending fire. */
    constexpr char kOnText[]  = "Ignite Fire";
    constexpr char kOffText[] = "Extinguish Fire";

    /* Map from temperatures to colors. This list is taken from the original
     * blog post referenced in the assignment instructions, which in turn took
     * them from a port of the game DOOM.
     */
    constexpr int kColorRGB[kMaxTemp + 1] = {
        0x070707,
        0x1F0707,
        0x2F0F07,
        0x470F07,
        0x571707,
        0x671F07,
        0x771F07,
        0x8F2707,
        0x9F2F07,
        0xAF3F07,
        0xBF4707,
        0xC74707,
        0xDF4F07,
        0xDF5707,
        0xDF5707,
        0xD75F07,
        0xD75F07,
        0xD7670F,
        0xCF6F0F,
        0xCF770F,
        0xCF7F0F,
        0xCF8717,
        0xC78717,
        0xC78F17,
        0xC7971F,
        0xBF9F1F,
        0xBF9F1F,
        0xBFA727,
        0xBFA727,
        0xBFAF2F,
        0xB7AF2F,
        0xB7B72F,
        0xB7B737,
        0xCFCF6F,
        0xDFDF9F,
        0xEFEFC7,
        0xFFFFFF
    };

    /* Checks that all cells in the fire are in the proper range. */
    void validateFire(const Grid<int>& fire) {
        for (int value: fire) {
            if (value < 0) {
                error("Negative temperature occurred in the fire.");
            }
            if (value > kMaxTemp) {
                error("Fire temperature exceeds kMaxTemp.");
            }
        }
    }

    class FireGUI: public ProblemHandler {
    public:
        FireGUI(GWindow& window);

        ~FireGUI() {
            /* TODO: There is a bug (as of 7/5/22) in StanfordCPPLib that introduces
             * a race condition when timers are being stopped while they're being
             * destroyed. Leak the timer as a result.
             */
            timer->stop();
        }

        /* When the timer fires, update the simulation. */
        void timerFired() override;
        void actionPerformed(GObservable* source) override;

    protected:
        void repaint() override;

    private:
        Temporary<GButton> toggleButton;

        GRectangle displayBounds() const;

        Grid<int> world;
        GTimer* timer;

        /* Temperature that we're aiming for. */
        int targetTemp = 0;

        /* Frame counter. */
        int frame = 0;
    };

    FireGUI::FireGUI(GWindow& window) : ProblemHandler(window) {
        /* Start a fire! */
        world = Grid<int>(kNumRows, kNumCols);

        /* Set a timer that will repeatedly update the animation. */
        timer = new GTimer(50);
        timer->start();

        /* Controls. */
        toggleButton = make_temporary<GButton>(window, "SOUTH", kOnText);
    }

    GRectangle FireGUI::displayBounds() const {
        /* Shrink from the sides, but don't go negative. */
        double width  = max(0.0, window().getCanvasWidth()  - 2 * kPadding);
        double height = max(0.0, window().getCanvasHeight() - 2 * kPadding);

        /* Scale to the intended size of a pixel. */
        width  = min(width,  kPixelsPerCell * kNumCols);
        height = min(height, kPixelsPerCell * kNumRows);

        /* Accounts for the possibility that we're too small. */
        double x = (window().getCanvasWidth()  -  width) / 2;
        double y = (window().getCanvasHeight() - height) / 2;

        return fitToBounds({ x, y, width, height }, kAspectRatio);
    }

    void FireGUI::repaint() {
        clearDisplay(window(), kBackgroundColor);
        auto bounds = displayBounds();

        /* Set each pixel in the region to the proper color. */
        for (int x = bounds.x; x < bounds.x + bounds.width; x++) {
            for (int y = bounds.y; y < bounds.y + bounds.height; y++) {
                /* Transform screen space to fire space. */
                int col = (x - bounds.x) * world.numCols() / (bounds.width + 1);
                int row = (y - bounds.y) * world.numRows() / (bounds.height + 1);
                window().getCanvas()->setPixel(x, y, kColorRGB[world[row][col]]);
            }
        }
    }

    /* Respond to buttons. */
    void FireGUI::actionPerformed(GObservable* source) {
        if (source == toggleButton) {
            if (targetTemp == 0) {
                targetTemp = kMaxTemp;
                toggleButton->setText(kOffText);
            } else {
                targetTemp = 0;
                toggleButton->setText(kOnText);
            }
        }
    }

    void FireGUI::timerFired() {
        /* Next animation frame. */
        frame++;

        /* If the fire runs for several years, frame overflows. Fix this. :-) */
        if (frame == 0x7FFFFFFF) {
            frame = 0;
        }

        /* Advance the simulation forward. */
        updateFire(world);
        validateFire(world);

        /* Nudge the fire toward the target temperature. */
        if (frame % kUpdateDelay == 0) {
            for (int col = 0; col < world.numCols(); col++) {
                int& temp = world[world.numRows() - 1][col];
                if (temp < targetTemp) {
                    temp++;
                } else if (temp > targetTemp) {
                    temp--;
                }
            }
        }

        /* Need to redraw this. */
        requestRepaint();
    }
}

GRAPHICS_HANDLER("Fire", GWindow& window) {
    return make_shared<FireGUI>(window);
}

/* TODO: Console handlers. */
