#include "PoissonTesting.h"
#include "error.h"
#include <random>
#include <cmath>
#include <limits>
using namespace std;

namespace {
    /* Generates a Poisson random variable with the given mean. */
    int numSamples(int mean) {
        static mt19937 generator(137);
        return poisson_distribution<int>(mean)(generator);
    }

#if 0
    /* Checks if the results are within tolerance. */
    bool isCloseLegacy(int meanSamples,
                 const vector<int>& frequencies,
                 const vector<double>& probabilities,
                 double stdevTolerance) {
        if (frequencies.size() != probabilities.size()) {
            error("Frequency vector size must match probability vector size.");
        }
        if (meanSamples < 0) {
            error("Negative number of mean samples?");
        }

        /* Check each pair. */
        for (size_t i = 0; i < frequencies.size(); i++) {
            /* Get the lambda parameter for our Poisson distribution. */
            double lambda = meanSamples * probabilities[i];

            /* We can approximate the Poisson distribution as a
             * Normal(mu = lambda, sigma^2 = lambda) distribution, then count
             * how many standard deviations away we are.
             */
            double stdevs = fabs(lambda - frequencies[i]) / sqrt(lambda);
            if (stdevs > stdevTolerance) {
                return false;
            }
        }

        /* It all checks out! */
        return true;
    }
#endif
}

bool PoissonTesting::isClose(const vector<double>& probabilities,
                             function<int ()> experiment,
                             int meanSamples,
                             double stdevMax) {
    /* Get a Poisson-distributed number of samples. */
    int samples = numSamples(meanSamples);

    /* Run the experiment and build a frequency histogram. */
    vector<int> frequencies(probabilities.size());
    for (int i = 0; i < samples; i++) {
        int result = experiment();
        if (result < 0 || result > int(frequencies.size())) {
            error("Illegal experiment outcome.");
        }
        frequencies[result]++;
    }

    /* Check each pair. */
    for (size_t i = 0; i < probabilities.size(); i++) {
        /* Get the lambda parameter for our Poisson distribution. */
        double lambda = meanSamples * probabilities[i];

        /* We can approximate the Poisson distribution as a
         * Normal(mu = lambda, sigma^2 = lambda) distribution, then count
         * how many standard deviations away we are.
         */
        double stdevs = fabs(lambda - frequencies[i]) / sqrt(lambda);
        if (stdevs > stdevMax) {
            return false;
        }
    }

    /* It all checks out! */
    return true;
}
