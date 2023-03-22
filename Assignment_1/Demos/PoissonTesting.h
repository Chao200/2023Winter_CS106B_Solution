#pragma once

#include <vector>
#include <functional>

namespace PoissonTesting {
    bool isClose(const std::vector<double>& probabilities,
                 std::function<int ()> experiment,
                 int meanSamples = 100000,
                 double stdevMax = 5.0);
}
