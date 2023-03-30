#include "DrawParticle.h"
#include "error.h"

namespace {
    static DrawFunction theDrawFunction;
}

void drawParticle(double x, double y, Color color) {
    if (!theDrawFunction) {
        error("No draw function has been set. This can happen if, for example, you try using drawParticle in a test case without setting up a ParticleCatcher first.");
    }
    theDrawFunction(x, y, color);
}

void setDrawFunction(DrawFunction fn) {
    theDrawFunction = fn;
}
