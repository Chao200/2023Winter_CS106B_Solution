#include "DebuggerWarmups.h"
#include "GUI/MiniGUI.h"
#include "simpio.h"
#include "set.h"
#include "gbutton.h"
using namespace std;
using namespace MiniGUI;

namespace {
    const string kMessage = "Click the button below to trigger a stack overflow. This will crash your program if the debugger is not enabled. If the debugger is enabled, it will jump you to the spot in the code where the stack overflow occurred.";
    const Font kMessageFont(FontFamily::SERIF, FontStyle::NORMAL, 24, "#A50021"); // Madder
    const string kBackgroundColor = "white";

    /* This is an unusual one - it doesn't actually visualize anything, and instead
     * either (1) crashes or (2) aborts with an exception.
     */
    class CallStackStorytellingGUI: public ProblemHandler {
    public:
        CallStackStorytellingGUI(GWindow& window);

        void actionPerformed(GObservable* source) override;

    protected:
        void repaint() override;

    private:
        Temporary<GButton> crashMe;
    };

    CallStackStorytellingGUI::CallStackStorytellingGUI(GWindow& window): ProblemHandler(window) {
        crashMe = make_temporary<GButton>(window, "SOUTH", "Go!");
    }

    void CallStackStorytellingGUI::repaint() {
        clearDisplay(window(), kBackgroundColor);

        auto render = TextRender::construct(kMessage, { 0, 0, window().getCanvasWidth(), window().getCanvasHeight()}, kMessageFont);
        render->alignCenterHorizontally();
        render->alignCenterVertically();
        render->draw(window());
    }

    void CallStackStorytellingGUI::actionPerformed(GObservable* source) {
        if (source == crashMe) {
            initiateStackOverflow();
        }
    }
}

GRAPHICS_HANDLER("Stack Overflows", GWindow& window) {
    return make_shared<CallStackStorytellingGUI>(window);
}

CONSOLE_HANDLER("Stack Overflows") {
    if (getYesOrNo("Do you want to trigger a stack overflow? ")) {
        initiateStackOverflow();
    }
}

namespace {
    /* Like randomInteger, but using scramble as a source. */
    int scrambleInteger(int low, int high, int& seed) {
        if (high < low) {
            error("Values out of range.");
        }

        /* Next random value. */
        seed = scramble(seed);

        /* Progress factor between 0 and maximum value + 1. */
        double alpha = seed / double(0x80000000u);

        /* Interpolate between the lower boundary of low and the upper
         * boundary of high.
         */
        return low + alpha * (double(high) + 1 - low);
    }

    /* Like std::shuffle, but using scrambleInteger as a source. */
    void scrambleShuffle(int elems[], int numElems, int& seed) {
        for (int i = 0; i < numElems; i++) {
            swap(elems[i], elems[scrambleInteger(i, numElems - 1, seed)]);
        }
    }
}


/* Shuffles the values until we produce a permutation that has a cycle of the
 * given length. Once that cycle has been generated, returns some number in
 * that cycle.
 */
int shuffleValues(int seed, int values[], int numValues, int target) {
    while (true) {
        scrambleShuffle(values, numValues, seed);

        /* Avoid rescanning the same items. */
        Set<int> used;
        for (int value = 0; value < numValues; value++) {
            /* Skip this if we already saw it. */
            if (used.contains(value)) continue;

            /* Trace through its cycle. */
            int length = 0;
            int curr = value;
            do {
                length++;
                used += curr;
                curr = values[curr];
            } while (curr != value);

            /* Right length? Then we're done. */
            if (length == target) {
                return value;
            }
        }

        /* Otherwise, shuffle again. */
    }
}
