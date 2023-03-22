#pragma once

#include "grid.h"
#include "random.h"
/* Maximum temperature of a cell in the fire. */
const int kMaxTemp = 36;

/* Advances the simulation forward one step. */
void updateFire(Grid<int>& fire);
