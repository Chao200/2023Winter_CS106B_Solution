#include "RosettaStone.h"
#include "GUI/MiniGUI.h"
#include "ginteractors.h"
#include "filelib.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

namespace {
    const int KGRAM_SIZE     = 3;
    const int kNumTrigrams = 2000;

    const string kBaseDir    = "res/trigrams/";

    const int kTextPadLength = 25;
}

bool operator< (const Corpus& lhs, const Corpus& rhs) {
    if (lhs.name != rhs.name) return lhs.name < rhs.name;
    return lhs.profile < rhs.profile;
}

namespace {
    class RosettaStoneGUI: public ProblemHandler {
    public:
        RosettaStoneGUI(GWindow& window);

        void actionPerformed(GObservable* source) override;
        void settingUp() override;

    private /* helpers */:
        void initChrome();
        void findBestMatch(const string& text);

    private /* state */:
        Set<Corpus> all_;

        Temporary<GColorConsole> console_;

        Temporary<GContainer>    panel_;
          GLabel*    result_;
          GTextArea* input_;
          GButton*   go_;
    };

    RosettaStoneGUI::RosettaStoneGUI(GWindow& window) : ProblemHandler(window) {
        initChrome();
    }

    void RosettaStoneGUI::initChrome() {
        console_ = make_temporary<GColorConsole>(window(), "CENTER");

        GContainer* mainPanel = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
        result_ = new GLabel("Loading Languages...");
        mainPanel->add(result_);

        GContainer* subPanel = new GContainer();

        input_   = new GTextArea;
        go_      = new GButton("Go!");

        subPanel->add(input_);
        subPanel->add(go_);

        mainPanel->add(subPanel);
        panel_ = Temporary<GContainer>(mainPanel, window(), "SOUTH");

        setDemoOptionsEnabled(false);
        input_->setEnabled(false);
        go_->setEnabled(false);
    }

    void RosettaStoneGUI::settingUp() {
        *console_ << "Loading language data... " << endl;
        for (string file: listDirectory(kBaseDir)) {
            if (!endsWith(file, ".3grams")) continue;

            string language = file.substr(0, file.find('.'));

            ifstream input("res/trigrams/" + file);
            if (!input) error("Bad!");

            *console_ << "  Loading data for " << setw(kTextPadLength) << left << (language + "...") << flush;

            Map<string, double> trigrams;
            input >> trigrams;
            if (!input) error("Worse!");

            Corpus corpus = { language, topKGramsIn(normalize(trigrams), kNumTrigrams) };
            all_.add(corpus);

            *console_ << " done!" << endl;
        }

        *console_ << "\nReady to guess languages.\n\n\n" << endl;
        result_->setText("Enter text to identify.");

        input_->setEnabled(true);
        go_->setEnabled(true);
        setDemoOptionsEnabled(true);
    }

    void RosettaStoneGUI::actionPerformed(GObservable* source) {
        if (source == go_) {
            findBestMatch(input_->getText());
        }
    }

    void RosettaStoneGUI::findBestMatch(const string& text) {
        try {
            string language = guessLanguageOf(topKGramsIn(normalize(kGramsIn(text, KGRAM_SIZE)), kNumTrigrams), all_);
            if (find_if(all_.begin(), all_.end(), [&](const Corpus& c) {
                return c.name == language;
            }) == all_.end()) {
                error("Guessed language was \"" + language + "\", but that language is not in our collection.");
            }

            result_->setText("Best guess: " + language);
        } catch (const exception& e) {
            result_->setText("Error: " + string(e.what()));
        }
    }
}

GRAPHICS_HANDLER("Rosetta Stone", GWindow& window) {
    return make_shared<RosettaStoneGUI>(window);
}
