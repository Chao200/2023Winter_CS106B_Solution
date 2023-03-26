#pragma once

#include <string>
#include "map.h"

std::string toProtein(const std::string& rna,
                      const Map<std::string, char>& codonMap);

Map<std::string, char> standardCodonMap();
