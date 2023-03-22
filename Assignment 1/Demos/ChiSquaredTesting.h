#pragma once

#include <vector>
#include <functional>

namespace ChiSquaredTesting {
    bool isClose(const std::vector<double>& probabilities,
                 std::function<int ()> experiment);
}
