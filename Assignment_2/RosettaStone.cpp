#include "RosettaStone.h"
#include "GUI/SimpleTest.h"
#include "priorityqueue.h"
using namespace std;

Map<string, double> kGramsIn(const string& str, int kGramLength) {
    /* TODO: Delete this comment and the other lines here, then implement
     * this function.
     */
//    (void) str;
//    (void) kGramLength;
//    return {};
    Map<string, double> map = {};
    if (kGramLength <= 0) {   // 长度为负数或 0
        error("The value of kGramLength not positive!");
        return {};
    } else if (kGramLength > str.size()) {    //  如果 kGramLength 比字符串长
        return {};
    } else {  //  满足条件
        for (int i = 0; i <= str.size() - kGramLength; i++) {
            // 得到子串
            string sub = str.substr(i, kGramLength);
            if (map.containsKey(sub)) {   // 如果包含子串
                map[sub]++;
            } else {
                map.put(sub, 1);
            }
        }
    }
    return map;
}

Map<string, double> normalize(const Map<string, double>& input) {
    /* TODO: Delete this comment and the other lines here, then implement
     * this function.
     */
//    (void) input;
//    return {};
    if (input.equals({})) {  // 为空
        error("the input is empty!");
        return {};
    }
    // 求频率平方和
    double sum_all = 0;
    for (double freq: input.values()) {
        sum_all += pow(freq, 2);
    }
    // 如果所有频率均为 0
    if (!sum_all) {
        error("the values are all zero!");
        return {};
    }
    double root = sqrt(sum_all);
    //  由于声明为 const，所以需要重新定义一个 Map
    Map<string, double> result = input;
    for (string s: result) {
        result[s] /= root;
    }
    return result;
}

Map<string, double> topKGramsIn(const Map<string, double>& source, int numToKeep) {
    /* TODO: Delete this comment and the other lines here, then implement
     * this function.
     */
//    (void) source;
//    (void) numToKeep;
//    return {};
    // special case
    if (numToKeep < 0) {
        error("num is negative!");
        return {};
    }
    if (numToKeep == 0) {
        return {};
    }
    if (numToKeep > source.size()) {
        return source;
    }

    // normal case
    PriorityQueue<string> pq;
    Map<string, double> result;
    // 添加到栈中
    for (string str: source) {
        pq.enqueue(str, source[str]);
    }
    // 出栈，直到栈中剩下 numToKeep 个元素
    while (numToKeep < pq.size() && !pq.isEmpty()) {
        pq.dequeue();
    }
    // 存储到新的 Map
    for (int i = 0; i < numToKeep; i++) {
        string targetstr = pq.dequeue();
        result.put(targetstr, source[targetstr]);
    }
    return result;
}

double cosineSimilarityOf(const Map<string, double>& lhs, const Map<string, double>& rhs) {
    /* TODO: Delete this comment and the other lines here, then implement
     * this function.
     */
//    (void) lhs;
//    (void) rhs;
//    return {};
    double cosine = 0.0;
    for (string str: lhs) {
        if (rhs.containsKey(str)) {
            cosine += (lhs[str] * rhs[str]);
        }
    }
    return cosine;
}

string guessLanguageOf(const Map<string, double>& textProfile,
                       const Set<Corpus>& corpora) {
    /* TODO: Delete this comment and the other lines here, then implement
     * this function.
     */
//    (void) textProfile;
//    (void) corpora;
//    return "";
    if (textProfile.isEmpty() || corpora.isEmpty()) {
        error("Empty!");
        return "";
    }
    string guess = "";
    double similarity = 0.0;
    for (Corpus cor: corpora) {
        double temp = cosineSimilarityOf(textProfile, cor.profile);
        if (temp > similarity) {
            similarity = temp;
            guess = cor.name;
        }
    }
    return guess;

}




/* * * * *   Test Cases Below This Point   * * * * */







/* * * * *   Provided Tests Below This Point   * * * * */
PROVIDED_TEST("kGramsIn works when the text length exactly matches the k-gram length.") {
    Map<string, double> expected = {
        { "^_^", 1 }
    };

    EXPECT_EQUAL(kGramsIn("^_^", 3), expected);
}

PROVIDED_TEST("kGramsIn works when the text length is one more than the k-gram length.") {
    Map<string, double> expected = {
        { "the", 1 },
        { "hem", 1 }
    };

    EXPECT_EQUAL(kGramsIn("them", 3), expected);
}

PROVIDED_TEST("kGramsIn works when the text length is one more than the k-gram length.") {
    /* First check: when no k-grams are repeated. */
    Map<string, double> expected = {
        { "the", 1 },
        { "hem", 1 }
    };

    EXPECT_EQUAL(kGramsIn("them", 3), expected);

    /* Next check: when the k-grams are repeated. */
    expected = {
        { "aaa", 2 }
    };

    EXPECT_EQUAL(kGramsIn("aaaa", 3), expected);
}

PROVIDED_TEST("kGramsIn works on a sample when k = 1.") {
    Map<string, double> expected = {
        { "A", 7 },
        { "B", 2 },
        { "D", 1 },
        { "N", 4 },
    };

    EXPECT_EQUAL(kGramsIn("ABANANABANDANA", 1), expected);
}

PROVIDED_TEST("kGramsIn works on a sample when k = 2.") {
    Map<string, double> expected = {
        { "AB", 2 },
        { "AN", 4 },
        { "BA", 2 },
        { "DA", 1 },
        { "NA", 3 },
        { "ND", 1 }
    };

    EXPECT_EQUAL(kGramsIn("ABANANABANDANA", 2), expected);
}

PROVIDED_TEST("kGramsIn handles non-English strings.") {
    /* The characters in these strings are expressed in UTF-8. This
     * means that splitting the string into k-grams will produce some
     * k-grams containing fragments of individual characters. See
     * the assignment description for more details about what's
     * going on here.
     */
    string devanagari = "दे";
    Map<string, double> expected = {
      {"\244\246\340", 1},
      {"\246\340\245", 1},
      {"\340\244\246", 1},
      {"\340\245\207", 1}
    };
    EXPECT_EQUAL(kGramsIn(devanagari, 3), expected);

    string ktavAshuri = "דָג";
    expected = {
        {"\223\326\270", 1},
        {"\270\327\222", 1},
        {"\326\270\327", 1},
        {"\327\223\326", 1}
    };
    EXPECT_EQUAL(kGramsIn(ktavAshuri, 3), expected);
}

PROVIDED_TEST("kGramsIn works when the input is shorter than the k-gram length.") {
    EXPECT_EQUAL(kGramsIn("ABANANABANDANA", 137), {});
}

PROVIDED_TEST("kGramsIn reports errors on bad inputs.") {
    EXPECT_ERROR(kGramsIn("Oops, negative", -137));
    EXPECT_ERROR(kGramsIn("Oops, zero",     0));
}

PROVIDED_TEST("kGramsIn works on large inputs.") {
    /* This test is designed to make sure your implementation of
     * kGramsIn works on large inputs and runs efficiently.
     *
     * If this test runs and fails due to running out of time, it
     * might indicate that your strategy for finding k-grams is not
     * efficient enough to handle very large strings. If that's the
     * case, we recommend posting on EdStem or stopping by the LaIR
     * to get some guidance about why your code isn't running quickly
     * enough and what you can do to fix it.
     *
     * If this test fails with a stack overflow, it likely means that
     * you're trying to solve this problem recursively. The challenge
     * with using recursion here is that stack space is a limited
     * resource, and given very, very large strings (say, 100Kb of
     * text or more) you may run out of stack space. To fix this,
     * you'll need to rewrite your implementation iteratively rather
     * than recursively.
     */

    const int kLargeStringLength = 100000;

    string largeString(kLargeStringLength, ' ');
    for (int i = 0; i < kLargeStringLength; i++) {
        largeString[i] = i * i * i + 3 * i * i + 2 * i + i + 4 * i + 5;
    }

    EXPECT_COMPLETES_IN(1.0,
        EXPECT_NO_ERROR(kGramsIn(largeString, 50));
    );
}

PROVIDED_TEST("normalize does not add or remove keys.") {
    Map<string, double> incomplete = {
        { "C", 1 },
        { "O", 1 },
        { "N", 1 },
        { "E", 1 }
    };

    auto result = normalize(incomplete);
    EXPECT(result.containsKey("C"));
    EXPECT(result.containsKey("O"));
    EXPECT(result.containsKey("N"));
    EXPECT(result.containsKey("E"));
    EXPECT_EQUAL(result.size(), 4);
}

PROVIDED_TEST("normalize works on positive numbers.") {
    Map<string, double> scores = {
        { "C", 1 },
        { "O", 1 },
        { "N", 1 },
        { "E", 1 },
    };
    Map<string, double> expected = {
        { "C", 0.5 },
        { "O", 0.5 },
        { "N", 0.5 },
        { "E", 0.5 },
    };

    EXPECT_EQUAL(normalize(scores), expected);
}

PROVIDED_TEST("normalize works on negative numbers.") {
    Map<string, double> scores = {
        { "C", -1 },
        { "O", -1 },
        { "N", -1 },
        { "E", -1 },
    };
    Map<string, double> expected = {
        { "C", -0.5 },
        { "O", -0.5 },
        { "N", -0.5 },
        { "E", -0.5 },
    };

    EXPECT_EQUAL(normalize(scores), expected);
}

PROVIDED_TEST("normalize works on unequal values.") {
    Map<string, double> scores = {
        { "O",  3 },
        { "N", -4 },
    };
    Map<string, double> expected = {
        { "O",  0.6 },
        { "N", -0.8 },
    };

    EXPECT_EQUAL(normalize(scores), expected);
}

PROVIDED_TEST("normalize works on huge numbers.") {
    /* As mentioned in the assignment description, the int type
     * is (usually) limited to a range of roughly negative two
     * billion to roughly positive two billion. The double type,
     * however, can handle this just fine. This test case checks
     * to make sure that large numbers are processed correctly.
     * If you store any intermediary calculation values in an
     * int rather than a double, it may cause this test
     * to fail.
     */
    Map<string, double> scores = {
        { "singleton", 1e18 }
    };

    auto result = normalize(scores);
    EXPECT_EQUAL(result.size(), 1);
    EXPECT(result.containsKey("singleton"));

    /* doubles store rounded values, so with large values being
     * calculated we may end up with a value that's slightly
     * wrong. These error bounds should catch this.
     */
    EXPECT_LESS_THAN_OR_EQUAL_TO(result["singleton"], 10);
    EXPECT_GREATER_THAN_OR_EQUAL_TO(result["singleton"], 0.0);
}

PROVIDED_TEST("normalize reports errors on inputs that are all zero.") {
    EXPECT_ERROR(normalize({}));
    EXPECT_ERROR(normalize({{"A", 0}, {"C", 0}, {"E", 0}}));
}

PROVIDED_TEST("normalize handles large inputs.") {
    /* This next piece of code computes a frequency map
     * consisting of all 8! = 40,320 permutations of the
     * string "abcdefgh". This is a large but managable
     * number of keys to have in a Map that's passed off
     * to normalize.
     *
     * If your implementation is reasonably efficient, then
     * normalizing the map should take much less than a second.
     * If you're failing this test because your code takes too
     * long, it likely means your code isn't algorithmically
     * efficient. If that happens, feel free to ask for help at
     * the LaIR or to post on EdStem.
     *
     * If this test causes a stack overflow, it may mean that
     * you're approaching this problem recursively and ran out
     * of stack space. Stack space is limited, and when handling
     * huge inputs like these you may not have enough of it to
     * handle large inputs. If that's the case, you'll need to
     * rewrite your function iteratively.
     */
    string chars = "abcdefgh";

    Map<string, double> freqMap;
    do {
        freqMap[chars] = 1;
    } while (next_permutation(chars.begin(), chars.end()));

    EXPECT_COMPLETES_IN(2.0,
        EXPECT_NO_ERROR(normalize(freqMap));
    );
}

PROVIDED_TEST("topKGramsIn finds the most frequent k-gram.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    Map<string, double> expected = {
        { "B", 30 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 1), expected);
}

PROVIDED_TEST("topKGramsIn finds the top two k-grams.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    Map<string, double> expected = {
        { "B", 30 },
        { "C", 20 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 2), expected);
}

PROVIDED_TEST("topKGramsIn works if all k-grams are requested.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 3), input);
}

PROVIDED_TEST("topKGramsIn works if more k-grams are requested than exist.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 137), input);
}

PROVIDED_TEST("topKGramsIn works if no k-grams are requested.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 0), {});
}

PROVIDED_TEST("topKGramsIn reports errors on bad inputs.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    EXPECT_ERROR(topKGramsIn(input, -137));
}

PROVIDED_TEST("topKGramsIn works even if frequencies are negative.") {
    Map<string, double> input = {
        { "A",  2 },
        { "B",  1 },
        { "C",  0 },
        { "D", -1 },
        { "E", -2 },
        { "F", -3 },
        { "G", -4 },
        { "H", -5 }
    };

    Map<string, double> expected = {
        { "A",  2 },
        { "B",  1 },
        { "C",  0 },
        { "D", -1 },
        { "E", -2 },
    };

    EXPECT_EQUAL(topKGramsIn(input, 5), expected);
}

PROVIDED_TEST("topKGramsIn works with fractional weights.") {
    Map<string, double> input = {
        { "A", 10.1 },
        { "B", 10.2 },
        { "C", 10.3 }
    };
    Map<string, double> expected = {
        { "C", 10.3 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 1), expected);

    input = {
        { "A", 10.3 },
        { "B", 10.2 },
        { "C", 10.1 }
    };
    expected = {
        { "A", 10.3 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 1), expected);

    input = {
        { "A", 10.2 },
        { "B", 10.3 },
        { "C", 10.1 }
    };
    expected = {
        { "B", 10.3 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 1), expected);
}

PROVIDED_TEST("topKGramsIn handles ties.") {
    Map<string, double> tiered = {
        { "first",  10 },
        { "best",   10 },
        { "last",   5  },
        { "worst",  5  }
    };

    /* Test 1: Do we pick up the top two if we ask for the
     * top two?
     */
    Map<string, double> expected = {
        { "first",  10 },
        { "best",   10 }
    };

    EXPECT_EQUAL(topKGramsIn(tiered, 2), expected);

    /* Test 2: If we pick the top 1, do we get one of the
     * two items of weight 10?
     */
    Map<string, double> top = topKGramsIn(tiered, 1);
    EXPECT_EQUAL(top.size(), 1);
    EXPECT(tiered.containsKey(top.firstKey()));
    EXPECT_EQUAL(tiered[top.firstKey()], 10);

    /* Test 3: If we pick to the top 3, do we get both items
     * of top weight and one of the bottom two?
     */
    Map<string, double> three = topKGramsIn(tiered, 3);
    EXPECT_EQUAL(three.size(), 3);
    EXPECT(three.containsKey("first"));
    EXPECT(three.containsKey("best"));
    EXPECT_EQUAL(three["first"], 10);
    EXPECT_EQUAL(three["best"],  10);

    if (three.containsKey("worst")) {
        EXPECT_EQUAL(three["worst"], 5);
    } else if (three.containsKey("last")) {
        EXPECT_EQUAL(three["last"], 5);
    } else {
        SHOW_ERROR("Top three k-grams should contain either 'worst' or 'last'.");
    }
}

PROVIDED_TEST("topKGramsIn handles large inputs.") {
    /* This next piece of code computes a frequency map
     * consisting of all 8! = 40,320 permutations of the
     * string "abcdefgh". This is a large but managable
     * number of keys to have in a Map that's passed off
     * to normalize.
     *
     * If your implementation is reasonably efficient, then
     * finding the top k-grams should take much less than a second.
     * If you're failing this test because your code takes too
     * long, it likely means your code isn't algorithmically
     * efficient. If that happens, feel free to ask for help at
     * the LaIR or to post on EdStem.
     *
     * If this test causes a stack overflow, it may mean that
     * you're approaching this problem recursively and ran out
     * of stack space. Stack space is limited, and when handling
     * huge inputs like these you may not have enough of it to
     * handle large inputs. If that's the case, you'll need to
     * rewrite your function iteratively.
     */
    string chars = "abcdefgh";

    Map<string, double> freqMap;
    int counter = 1;
    do {
        freqMap[chars] = counter;
        counter++;
    } while (next_permutation(chars.begin(), chars.end()));


    EXPECT_COMPLETES_IN(2.0,
        EXPECT_EQUAL(topKGramsIn(freqMap, 137).size(), 137);
    );
}

PROVIDED_TEST("cosineSimilarityOf works with one shared key.") {
    Map<string, double> one = {
        { "A",  1 },
    };
    Map<string, double> two = {
        { "A",  1 },
    };
    EXPECT_EQUAL(cosineSimilarityOf(one, two), 1);
}

PROVIDED_TEST("cosineSimilarityOf works with two maps with non-overlapping keys.") {
    Map<string, double> one = {
        { "A",  1 },
    };
    Map<string, double> two = {
        { "B",  1 },
    };
    EXPECT_EQUAL(cosineSimilarityOf(one, two), 0);
}

PROVIDED_TEST("cosineSimilarityOf works where keys are present in RHS but not LHS.") {
    /* Yes, we know these aren't normalized and that we promised that we would normalize
     * all inputs to cosineSimilarityOf. This is just for testing purposes.
     */
    Map<string, double> one = {
        { "O",  1 },
        { "C",  2 },
        { "E",  3 },
        { "A",  4 },
        { "N",  5 },
    };
    Map<string, double> two = {
        { "C", -2 },
        { "E", -3 },
        { "A", -4 },
        { "N", -5 },
    };
    EXPECT_EQUAL(cosineSimilarityOf(one, two), -54);
}

PROVIDED_TEST("cosineSimilarityOf works where keys are present in LHS but not RHS.") {
    /* Yes, we know these aren't normalized and that we promised that we would normalize
     * all inputs to cosineSimilarityOf. This is just for testing purposes.
     */
    Map<string, double> one = {
        { "C",  2 },
        { "E",  3 },
        { "A",  4 },
        { "N",  5 },
    };
    Map<string, double> two = {
        { "O",  1 },
        { "C", -2 },
        { "E", -3 },
        { "A", -4 },
        { "N", -5 },
    };
    EXPECT_EQUAL(cosineSimilarityOf(one, two), -54);
}

PROVIDED_TEST("cosineSimilarityOf works with keys missing from both sides.") {
    /* Yes, we know these aren't normalized and that we promised that we would normalize
     * all inputs to cosineSimilarityOf. This is just for testing purposes.
     */
    Map<string, double> one = {
        { "C",  2 },
        { "E",  3 },
        { "A",  4 },
    };
    Map<string, double> two = {
        { "O",  1 },
        { "E", -3 },
        { "A", -4 },
    };
    EXPECT_EQUAL(cosineSimilarityOf(one, two), -25);
}

PROVIDED_TEST("cosineSimilarityOf works with disjoint keys.") {
    Map<string, double> first = {
        { "auk", 1 }
    };
    Map<string, double> second = {
        { "moa", 1 }
    };

    EXPECT_EQUAL(cosineSimilarityOf(first, second), 0.0);
}

PROVIDED_TEST("cosineSimilarityOf works on large inputs.") {
    /* This next piece of code computes a frequency map
     * consisting of all 8! = 40,320 permutations of the
     * string "abcdefgh". This is a large but managable
     * number of keys to have in a Map that's passed off
     * to normalize.
     *
     * If your implementation is reasonably efficient, then
     * finding cosine similarities should take much less than a second.
     * If you're failing this test because your code takes too
     * long, it likely means your code isn't algorithmically
     * efficient. If that happens, feel free to ask for help at
     * the LaIR or to post on EdStem.
     *
     * If this test causes a stack overflow, it may mean that
     * you're approaching this problem recursively and ran out
     * of stack space. Stack space is limited, and when handling
     * huge inputs like these you may not have enough of it to
     * handle large inputs. If that's the case, you'll need to
     * rewrite your function iteratively.
     */
    string chars = "abcdefgh";

    Map<string, double> freqMap;
    int counter = 1;
    do {
        freqMap[chars] = counter;
        counter++;
    } while (next_permutation(chars.begin(), chars.end()));

    EXPECT_COMPLETES_IN(2.0,
        EXPECT_NO_ERROR(cosineSimilarityOf(freqMap, freqMap));
    );
}

PROVIDED_TEST("guessLanguageOf works with perfect similarity.") {
    Corpus o = { "Language O", {{ "O", 1 }} },
           c = { "Language C", {{ "C", 1 }} },
           e = { "Language E", {{ "E", 1 }} },
           a = { "Language A", {{ "A", 1 }} },
           n = { "Language N", {{ "N", 1 }} };

    EXPECT_EQUAL(guessLanguageOf({{ "O", 1 }}, {o, c, e, a, n}), o.name);
    EXPECT_EQUAL(guessLanguageOf({{ "C", 1 }}, {o, c, e, a, n}), c.name);
    EXPECT_EQUAL(guessLanguageOf({{ "E", 1 }}, {o, c, e, a, n}), e.name);
    EXPECT_EQUAL(guessLanguageOf({{ "A", 1 }}, {o, c, e, a, n}), a.name);
    EXPECT_EQUAL(guessLanguageOf({{ "N", 1 }}, {o, c, e, a, n}), n.name);
}

PROVIDED_TEST("guessLanguageOf works with imperfect similarity.") {
    Corpus o = { "Language O", {{ "O", 0.8 }, { "C", 0.6 }} },
           c = { "Language C", {{ "C", 0.8 }, { "E", 0.6 }} },
           e = { "Language E", {{ "E", 0.8 }, { "A", 0.6 }} },
           a = { "Language A", {{ "A", 0.8 }, { "N", 0.6 }} },
           n = { "Language N", {{ "N", 0.8 }, { "O", 0.6 }} };

    EXPECT_EQUAL(guessLanguageOf({{"O", 1}}, {o, c, e, a, n}), o.name);
    EXPECT_EQUAL(guessLanguageOf({{"C", 1}}, {o, c, e, a, n}), c.name);
    EXPECT_EQUAL(guessLanguageOf({{"E", 1}}, {o, c, e, a, n}), e.name);
    EXPECT_EQUAL(guessLanguageOf({{"A", 1}}, {o, c, e, a, n}), a.name);
    EXPECT_EQUAL(guessLanguageOf({{"N", 1}}, {o, c, e, a, n}), n.name);
}

PROVIDED_TEST("guessLanguageOf works on more complex inputs.") {
    Corpus karel  = { "Karel",  {{ "R", 1.0 / sqrt(7.0) }, { "O", 2.0 / sqrt(7.0) }, { "B", 1.0 / sqrt(7.0) }, { "T", 1.0 / sqrt(7.0) }} };
    Corpus trisha = { "Trisha", {{ "T", 1.0 / sqrt(6.0) }, { "R", 1.0 / sqrt(6.0) }, { "I", 1.0 / sqrt(6.0) }, { "S", 1.0 / sqrt(6.0) }, { "H", 1.0 / sqrt(6.0) }, { "A", 1.0 / sqrt(6.0) }} };
    Corpus chris  = { "Chris",  {{ "C", 1.0 / sqrt(5.0) }, { "H", 1.0 / sqrt(5.0) }, { "R", 1.0 / sqrt(5.0) }, { "I", 1.0 / sqrt(5.0) }, { "S", 1.0 / sqrt(5.0) }} };
    Corpus jerry  = { "Jerry",  {{ "J", 1.0 / sqrt(5.0) }, { "E", 1.0 / sqrt(5.0) }, { "R", 2.0 / sqrt(5.0) }, { "Y", 1.0 / sqrt(5.0) }} };

    EXPECT_EQUAL(guessLanguageOf({{"R", 1}}, {karel, trisha, chris, jerry}), jerry.name);
}

PROVIDED_TEST("guessLanguageOf reports errors if no corpora are provided.") {
    EXPECT_ERROR(guessLanguageOf({}, {}));
    EXPECT_ERROR(guessLanguageOf({{"C", -1}}, {}));
}
