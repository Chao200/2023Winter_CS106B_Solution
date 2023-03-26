#include "InverseGenetics.h"
#include "GUI/MiniGUI.h"
#include "ginteractors.h"
#include <fstream>
using namespace std;

/* Graphics Handler */
namespace {
    class InverseGeneticsGUI: public ProblemHandler {
    public:
        InverseGeneticsGUI(GWindow& window);

        void actionPerformed(GObservable* source) override;

    private:
        Temporary<GColorConsole> console;
        Temporary<GLabel> desc;
        Temporary<GTextField> input;
        Temporary<GButton> go;
    };

    /* Number at which we won't let the user see all the options. */
    const int kTooMany = 100000;

    /* Returns how many strands generate a given protein, maxing out at kTooMany. */
    int numStrandsFor(const string& input) {
        auto codonMap = standardCodonMap();

        /* First, make sure that all the letters are actually amino acids. */
        Set<char> used;
        for (char ch: input) {
            used += ch;
        }
        for (string codon: codonMap) {
            used -= codonMap[codon];
        }

        /* If any characters weren't covered, then there are no ways to represent
         * this string.
         */
        if (!used.isEmpty()) {
            return 0;
        }

        /* Multiply in the number of items per character. */
        int result = 1;
        for (char amino: input) {
            int matches = 0;
            for (string codon: codonMap) {
                if (codonMap[codon] == amino) {
                    matches++;
                }
            }

            result *= matches;
            if (result >= kTooMany) {
                return kTooMany;
            }
        }
        return result;
    }

    /* Actually performs the conversion. */
    void convert(const string& protein, ostream& out) {
        /* Make sure there aren't too many options. */
        if (numStrandsFor(protein) == kTooMany) {
            out << "There are at least " << kTooMany << " strands, too many for us to show!" << endl;
            return;
        }

        /* Progress meter. */
        out << "Converting... " << flush;

        auto options = allRNAStrandsFor(protein, standardCodonMap());

        out << " done!\n";

        if (options.isEmpty()) {
            out << "  No RNA strands generate that protein." << endl;
        } else {
            if (options.size() == 1) {
                out << "  Here is the only RNA strand for that protein:\n";
            } else {
                out << "  There are " << options.size() << " strands for that protein:\n";
            }

            for (string option: options) {
                out << "    \"" << option << "\"\n";
            }

            out << flush;
        }
    }


    InverseGeneticsGUI::InverseGeneticsGUI(GWindow& window) : ProblemHandler(window) {
        console = make_temporary<GColorConsole>(window, "CENTER");

        desc  = make_temporary<GLabel>    (window, "SOUTH", "Protein: ");
        input = make_temporary<GTextField>(window, "SOUTH");
        go    = make_temporary<GButton>   (window, "SOUTH", "Convert");
    }

    void InverseGeneticsGUI::actionPerformed(GObservable* source) {
        if (source == go || source == input) {
            console->clearDisplay();

            /* Can take a while; don't queue up events. */
            setDemoOptionsEnabled(false);
            desc->setEnabled(false);
            input->setEnabled(false);
            go->setEnabled(false);


            convert(input->getText(), *console);

            setDemoOptionsEnabled(true);
            desc->setEnabled(true);
            input->setEnabled(true);
            go->setEnabled(true);
        }
    }
}

GRAPHICS_HANDLER("Inverse Genetics", GWindow& window) {
    return make_shared<InverseGeneticsGUI>(window);
}

