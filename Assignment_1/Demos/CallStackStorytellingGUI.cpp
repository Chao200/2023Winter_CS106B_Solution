#include "DebuggerWarmups.h"
#include "GUI/MiniGUI.h"
#include "simpio.h"
#include "gbutton.h"
#include <cstdint>
using namespace std;
using namespace MiniGUI;

namespace {
    const string kMessage = "Click the button below to call the function that tells your story. Make sure you've set a breakpoint at the indicated line in CallStackStorytelling.cpp.";
    const Font kMessageFont(FontFamily::SERIF, FontStyle::NORMAL, 24, "#A50021"); // Madder
    const string kBackgroundColor = "white";

    class CallStackStorytellingGUI: public ProblemHandler {
    public:
        CallStackStorytellingGUI(GWindow& window);

        void actionPerformed(GObservable* source) override;

    protected:
        void repaint() override;

    private:
        Temporary<GButton> tellStoryButton;
    };

    CallStackStorytellingGUI::CallStackStorytellingGUI(GWindow& window): ProblemHandler(window) {
        tellStoryButton = make_temporary<GButton>(window, "SOUTH", "Go!");
    }

    void CallStackStorytellingGUI::repaint() {
        clearDisplay(window(), kBackgroundColor);

        auto render = TextRender::construct(kMessage, { 0, 0, window().getCanvasWidth(), window().getCanvasHeight()}, kMessageFont);
        render->alignCenterHorizontally();
        render->alignCenterVertically();
        render->draw(window());
    }

    void CallStackStorytellingGUI::actionPerformed(GObservable* source) {
        if (source == tellStoryButton) {
           tellStory();
        }
    }
}

GRAPHICS_HANDLER("Storytelling", GWindow& window) {
    return make_shared<CallStackStorytellingGUI>(window);
}

CONSOLE_HANDLER("Storytelling") {
    (void) getLine("Press ENTER to call the function that tells a story. Make sure you've set a breakpoint at the appropriate spot in CallStackStorytelling.cpp");
    tellStory();
}

/* This is a function called xorshift that takes in an integer, scrambles it, and returns another
 * integer. We use it to derive different values as the story unfolds so that the story isn't
 * too predictable.
 */
int scramble(int value) {
    uint32_t uValue = value;

    /* These values are here for a specific reason. For more details, see
     *
     * https://stackoverflow.com/questions/71522815/why-xorshift-random-number-generator-uses-the-same-amount-of-sbr-sbl-in-all-ex
     */
    uValue ^= uValue << 13;
    uValue ^= uValue >> 17;
    uValue ^= uValue << 5;
    return uValue & 0x7FFFFFFF;
}
