#include "ChiSquaredTesting.h"
#include "error.h"
#include <iostream>
#include <cmath>
#include <cfloat>
#include <climits>
#include <algorithm>
#include <iomanip>
using namespace std;

namespace {
    constexpr int kNumSamples = 100000;

    /* Precomputed table of threshold chi-squared values
     * for the given number of degrees of freedom with
     * a p-value of 1e-6.
     */
    constexpr int kMaxProbabilities = 30;
    constexpr double kMaxChiSquaredValues[kMaxProbabilities + 1] = {
        -1,
        23.9281,
        27.6294,
        31.2164,
        33.3788,
        35.8882,
        38.2584,
        40.522,
        42.701,
        44.811,
        46.8632,
        48.8658,
        50.8254,
        52.7472,
        54.6354,
        56.4936,
        58.3244,
        60.1308,
        61.9144,
        63.6772,
        65.4208,
        67.1466,
        68.8558,
        70.5496,
        72.229,
        73.8947,
        75.5474,
        77.1882,
        78.8176,
        80.436,
        82.0442,
    };
}

bool ChiSquaredTesting::isClose(const vector<double>& probabilities,
                                function<int ()> experiment) {
    /* If there are no classes or just one class, then yes, we pass! */
    if (probabilities.size() <= 1) {
        return true;
    }

    /* We are limited in what sample sizes we can measure. */
    if (probabilities.size() > kMaxProbabilities) {
        error("Number of outcomes too large for chi squared testing.");
    }

    /* Run the experiment and build a frequency histogram. */
    vector<int> frequencies(probabilities.size());
    for (int i = 0; i < kNumSamples; i++) {
        int result = experiment();
        if (result < 0 || result > int(frequencies.size())) {
            error("Illegal experiment outcome.");
        }
        frequencies[result]++;
    }

    /* Compute the chi-squared statistic. */
    double chiSquared = 0.0;
    for (size_t i = 0; i < probabilities.size(); i++) {
        double deviation = (frequencies[i] - probabilities[i] * kNumSamples);
        chiSquared += deviation * deviation / (probabilities[i] * kNumSamples);
    }

    return chiSquared < kMaxChiSquaredValues[probabilities.size() - 1];
}
