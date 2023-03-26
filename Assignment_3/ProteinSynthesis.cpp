#include "ProteinSynthesis.h"
using namespace std;

string toProtein(const string& rna, const Map<string, char>& codonMap) {
    /* TODO: Delete this comment and the next few lines, then implement
     * this function.
     */
//    (void) rna;
//    (void) codonMap;
//    return {};
    if (rna.size() % 3) {  // 如果长度不对
        error("the length of rna is not right!");
        return {};
    }

    if (codonMap.containsKey(rna.substr(0, 3))) {
        return codonMap[rna.substr(0, 3)] + toProtein(rna.substr(3, rna.size() - 3), codonMap);
    } else if (!rna.size()) { // 没有 rna 序列了
        return "";
    } else {  // 不存在该 key
        error("not exist that key!");
        return {};
    }
}

/* * * * * Test Cases Below This Point * * * * */
#include "GUI/SimpleTest.h"

/*** Tests for toProtein ***/

PROVIDED_TEST("toProtein works on empty strands.") {
    /* Should work when there's nothing in the coding map. */
    EXPECT_EQUAL(toProtein("", {}), "");

    /* Should also work when there is something in the coding map. */
    EXPECT_EQUAL(toProtein("", standardCodonMap()), "");
}

PROVIDED_TEST("toProtein reports errors on invalid inputs.") {
    /* Should fail if the input string has a bad length. */
    EXPECT_ERROR(toProtein("AAAAAAAAAA", standardCodonMap()));

    /* Should fail if the input string has a bad codon. */
    EXPECT_ERROR(toProtein("GUACAMOLE", standardCodonMap()));
}

PROVIDED_TEST("toProtein converts the examples from the assignment description.") {
    EXPECT_EQUAL(toProtein("GACAUAAAAGAUAUCAAG", standardCodonMap()), "DIKDIK");
    EXPECT_EQUAL(toProtein("CAGUGAUAGAAGAAGGCU", standardCodonMap()), "QUOKKA");
}

PROVIDED_TEST("toProtein can convert all codons.") {
    Map<string, char> codonMap = standardCodonMap();

    /* Build the input and output strings. */
    string rna, protein;
    for (string codon: codonMap) {
        rna     += codon;
        protein += codonMap[codon];
    }

    /* Confirm it works. */
    EXPECT_EQUAL(toProtein(rna, codonMap), protein);
}

PROVIDED_TEST("toProtein works with nonstandard codons.") {
    Map<string, char> notRealCodons = {
        { "CS1", 'A' },
        { "06B", '+' }
    };

    EXPECT_EQUAL(toProtein("CS106B", notRealCodons), "A+");
}
