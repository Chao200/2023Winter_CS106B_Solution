/***************************************************************
 * File: RosettaStone.h
 *
 * Functions and utilities for manipulating trigrams and guessing
 * the language of a text.
 */
#pragma once

#include "map.h"
#include "set.h"
#include <string>

/* Type representing a text corpus. */
struct Corpus {
    /* Name of the corpus (e.g. "Finnish") */
    std::string name;

    /* Normalized k-gram profile for that language. */
    Map<std::string, double> profile;
};

/**
 * Given a string, returns a frequency map of the k-grams in that string.
 *
 * @param str The string to obtain k-grams from.
 * @param kGramLength The length of the k-grams to extract.
 *        If this is negative or less than zero, this function calls
 *        error() to report an error.
 * @return A map from k-grams to their frequency.
 */
Map<std::string, double> kGramsIn(const std::string& str, int kGramLength);

/**
 * Returns a normalized version of the given k-grams map so that the sum of
 * the squares of the weights sums to one.
 *
 * @param profile The trigrams profile.
 * @return A normalized version of that profile.
 */
Map<std::string, double> normalize(const Map<std::string, double>& profile);

/**
 * Given two k-gram frequency maps, returns their cosine similarity.
 *
 * @param lhs The first frequency map, assumed to be normalized.
 * @param rhs The second frequency map, assumed to be normalized.
 * @return Their cosine similarity.
 */
double cosineSimilarityOf(const Map<std::string, double>& lhs, const Map<std::string, double>& rhs);

/**
 * Given a map of k-gram frequencies, returns the numToKeep highest-frequency
 * k-grams. Ties are broken arbitrarily.
 *
 * @param profile The k-gram profile to get the top k-grams from.
 * @param numToKeep How many k-grams are to be retained.
 * @return A map containing the top numToKeep k-grams from the original
 *         profile (or up to numToKeep if numToKeep is bigger than the
 *         number of k-grams in the map).
 */
Map<std::string, double> topKGramsIn(const Map<std::string, double>& profile, int numToKeep);

/**
 * Given a text profile and a set of corpora, returns the best guess of the
 * language of text represented by that profile.
 *
 * @param textProfile The profile of the text whose language is to be identified.
 * @param corpora A set of corpora to compare against.
 * @return The name of the language the text has the highest similarity to.
 */
std::string guessLanguageOf(const Map<std::string, double>& textProfile,
                            const Set<Corpus>& corpora);




/* This function is needed to store a Corpus in a Set or as a key in
 * a Map. Those types require that the stored items be comparable
 * via the < operator, and so this function tells C++ "here's what
 * the < sign means when comparing two Corpus objects against one
 * another."
 */
bool operator< (const Corpus& lhs, const Corpus& rhs);
