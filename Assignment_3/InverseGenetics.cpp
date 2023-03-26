#include "InverseGenetics.h"
using namespace std;

Set<string> allRNAStrandsFor(const string& protein,
                             const Map<string, char>& codonMap) {
    /* TODO: Delete this comment and the next few lines, then
     * implement this function.
     */
//    (void) protein;
//    (void) codonMap;
//    return {};
    Map<char, Set<string>> acidMap;
    // 得到碱基对应的氨基酸
    for (string str: codonMap) {
        acidMap[codonMap[str]].add(str);
    }

    if (protein.size() == 1) {
        return acidMap[protein[0]];
    } else {
        Set<string> lefts = acidMap[protein[0]];
        Set<string> rights = allRNAStrandsFor(protein.substr(1, protein.size() - 1), codonMap);
        Set<string> result = {};
        for (string left: lefts) {
            for (string right: rights) {
                result.add(left + right);
            }
        }
        return result;
    }
}


/* * * * * Test Cases Below This Point * * * * */

#include "GUI/SimpleTest.h"

PROVIDED_TEST("Converts a single amino acid when only one option exists.") {
    /* Custom codon map with just one option. */
    Map<string, char> codonMap = {
        { "AAA", 'K' }
    };

    EXPECT_EQUAL(allRNAStrandsFor("K", codonMap), { "AAA" });
}

PROVIDED_TEST("Handles the case where an amino acid has nothing that codes it.") {
    /* Custom codon map with just one option. */
    Map<string, char> codonMap = {
        { "AAA", 'K' }
    };

    EXPECT_EQUAL(allRNAStrandsFor("X", codonMap), { });
}

PROVIDED_TEST("Handles a single amino acid with multiple options.") {
    Map<string, char> codonMap = {
        { "AAA", 'K' },
        { "AAG", 'K' },
    };

    EXPECT_EQUAL(allRNAStrandsFor("K", codonMap), { "AAA", "AAG" });
}

PROVIDED_TEST("Handles a single amino acid using the standard codon table.") {
    EXPECT_EQUAL(allRNAStrandsFor("K", standardCodonMap()), { "AAA", "AAG" });
}

PROVIDED_TEST("Converts two amino acids when only one option exists for each.") {
    /* Custom codon map with just one option. */
    Map<string, char> codonMap = {
        { "UAG", 'O' },
        { "AAA", 'K' },
    };

    EXPECT_EQUAL(allRNAStrandsFor("OK", codonMap), { "UAGAAA" });
}

PROVIDED_TEST("Converts two amino acids when no solution exists.") {
    /* Custom codon map with just one option. */
    Map<string, char> codonMap = {
        { "AAA", 'K' },
    };

    EXPECT_EQUAL(allRNAStrandsFor("OK", codonMap), { });
}

PROVIDED_TEST("Converts two amino acids when multiple options exist.") {
    /* Custom codon map with just one option. */
    Map<string, char> codonMap = {
        { "CAC", 'H' },
        { "CAU", 'H' },
        { "AUA", 'I' },
        { "AUC", 'I' },
        { "AUU", 'I' },
    };

    EXPECT_EQUAL(allRNAStrandsFor("HI", codonMap), {
        "CACAUA", "CACAUC", "CACAUU", "CAUAUA", "CAUAUC", "CAUAUU"
    });
}

PROVIDED_TEST("Can handle medium-sized strings.") {
    EXPECT_EQUAL(allRNAStrandsFor("QUOKKA", standardCodonMap()), {
        "CAAUGAUAGAAAAAAGCA",
        "CAAUGAUAGAAAAAAGCC",
        "CAAUGAUAGAAAAAAGCG",
        "CAAUGAUAGAAAAAAGCU",
        "CAAUGAUAGAAAAAGGCA",
        "CAAUGAUAGAAAAAGGCC",
        "CAAUGAUAGAAAAAGGCG",
        "CAAUGAUAGAAAAAGGCU",
        "CAAUGAUAGAAGAAAGCA",
        "CAAUGAUAGAAGAAAGCC",
        "CAAUGAUAGAAGAAAGCG",
        "CAAUGAUAGAAGAAAGCU",
        "CAAUGAUAGAAGAAGGCA",
        "CAAUGAUAGAAGAAGGCC",
        "CAAUGAUAGAAGAAGGCG",
        "CAAUGAUAGAAGAAGGCU",
        "CAGUGAUAGAAAAAAGCA",
        "CAGUGAUAGAAAAAAGCC",
        "CAGUGAUAGAAAAAAGCG",
        "CAGUGAUAGAAAAAAGCU",
        "CAGUGAUAGAAAAAGGCA",
        "CAGUGAUAGAAAAAGGCC",
        "CAGUGAUAGAAAAAGGCG",
        "CAGUGAUAGAAAAAGGCU",
        "CAGUGAUAGAAGAAAGCA",
        "CAGUGAUAGAAGAAAGCC",
        "CAGUGAUAGAAGAAAGCG",
        "CAGUGAUAGAAGAAAGCU",
        "CAGUGAUAGAAGAAGGCA",
        "CAGUGAUAGAAGAAGGCC",
        "CAGUGAUAGAAGAAGGCG",
        "CAGUGAUAGAAGAAGGCU"
    });
}

PROVIDED_TEST("Stress Test: Converts a long string with only one option.") {
    const int kNumCopies = 100;

    /* There's only one codon for U, which is UGA. */
    string protein(kNumCopies, 'U');

    /* That means this is the only protein that should work. */
    string expected;
    for (int i = 0; i < kNumCopies; i++) {
        expected += "UGA";
    }

    /* 10x margin of safety against our reference implementation run on a very
     * middle-of-the-line computer.
     */
    EXPECT_COMPLETES_IN(0.005, {
        EXPECT_EQUAL(allRNAStrandsFor(protein, standardCodonMap()), { expected });
    });
}

PROVIDED_TEST("Stress Test: Generates all options for a medium-length string.") {
    /* There are 4,608 RNA strands for the protein "OROPENDOLA." Generating all
     * of them should take very little time provided that your implementation
     * generates each option once and exactly once.
     *
     * We've given a 10x margin of safety above our reference implementation,
     * which was run on a very middle-of-the-line computer.
     */
    EXPECT_COMPLETES_IN(0.5, {
        EXPECT_EQUAL(allRNAStrandsFor("OROPENDOLA", standardCodonMap()).size(), 4608);
    });
}
