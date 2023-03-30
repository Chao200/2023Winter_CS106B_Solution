#pragma once

#include "Demos/Color.h"

/* Draws a particle on the screen at the given (x, y) coordinate. */
void drawParticle(double x, double y, Color color);





/* These functions are used internally by the graphics system and ParticleCatcher.
 * You are not expected to make use of them in the course of solving this
 * assignment.
 */
#include <functional>

using DrawFunction = std::function<void (double x, double y, Color color)>;
void setDrawFunction(DrawFunction fn);
