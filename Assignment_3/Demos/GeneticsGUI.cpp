#include "ProteinSynthesis.h"
#include "GUI/MiniGUI.h"
#include "ginteractors.h"
#include <fstream>
using namespace std;

namespace {
    constexpr char kCodonsFile[] = "res/codons.txt";

    class GeneticsGUI: public ProblemHandler {
    public:
        GeneticsGUI(GWindow& window);
        void actionPerformed(GObservable* source) override;

    private:
        Temporary<GColorConsole> console;
        Temporary<GLabel>        desc;
        Temporary<GTextField>    input;
        Temporary<GButton>       rnaButton;

        void convert(function<string (string, Map<string, char>)> converter);
    };

    GeneticsGUI::GeneticsGUI(GWindow& window) : ProblemHandler(window) {
        console       = make_temporary<GColorConsole>(window, "CENTER");
        desc          = make_temporary<GLabel>(window, "SOUTH", "Text to convert: ");
        input         = make_temporary<GTextField>(window, "SOUTH");
        rnaButton     = make_temporary<GButton>(window, "SOUTH", "Go!");
    }

    void GeneticsGUI::actionPerformed(GObservable* source) {
        if (source == rnaButton) {
            convert(toProtein);
        }
    }

    void GeneticsGUI::convert(function<string (string, Map<string, char>)> converter) {
        try {
            string result = converter(input->getText(), standardCodonMap());
            *console << "Result: " << result << endl;
        } catch (const exception& e) {
            console->doWithStyle("#800000", GColorConsole::BOLD, [&] {
                *console << "Error: " << e.what() << endl;
            });
        }
    }
}

GRAPHICS_HANDLER("Genetics", GWindow& window) {
    return make_shared<GeneticsGUI>(window);
}

/* TODO: Console handler. */

Map<string, char> standardCodonMap() {
    /* Just load it once for efficiency's sake. */
    static Map<string, char>* theMap = nullptr;
    if (theMap == nullptr) {
        ifstream input(kCodonsFile);
        if (!input) {
            error("Error opening codons file " + string(kCodonsFile));
        }

        /* Pull the data from the file. */
        Map<string, char> result;
        string codon;
        char   amino;
        while (input >> codon >> amino) {
            result[codon] = amino;
        }

        /* Stash it for later. */
        theMap = new Map<string, char>(result);
    }

    return *theMap;
}
