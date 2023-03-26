#pragma once

#include "map.h"
#include "set.h"

Set<std::string> allRNAStrandsFor(const std::string& protein,
                                  const Map<std::string, char>& codonMap);

/* Returns the standard codon map, the one that's actually used by RNA. */
Map<std::string, char> standardCodonMap();
