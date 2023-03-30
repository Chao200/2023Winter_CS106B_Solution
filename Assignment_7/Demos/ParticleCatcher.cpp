#include "ParticleCatcher.h"
#include "DrawParticle.h"
#include <sstream>
#include <limits>
#include <iomanip>
#include "gmath.h"
using namespace std;

ParticleCatcher::ParticleCatcher() {
    /* Stash what's drawn here. */
    setDrawFunction([this] (double x, double y, Color c) {
        drawn.add({ x, y, c });
    });
}

ParticleCatcher::~ParticleCatcher() {
    setDrawFunction(nullptr);
}

int ParticleCatcher::numDrawn() const {
    return drawn.size();
}

DrawnParticle ParticleCatcher::operator [](int index) const {
    if (index < 0 || index >= numDrawn()) {
        error("ParticleCatcher: Index is out of range.");
    }
    return drawn[index];
}

void ParticleCatcher::reset() {
    drawn.clear();
}

/* Printer. */
ostream& operator<< (ostream& out, const DrawnParticle& particle) {
    ostringstream builder;
    builder << fixed << setprecision(numeric_limits<double>::digits10 + 1)
            << "{ "
            << particle.x << ", "
            << particle.y << ", "
            << particle.color
            << " }";
    return out << builder.str();
}

/* Comparators. */
bool operator== (const DrawnParticle& lhs, const DrawnParticle& rhs) {
    return floatingPointEqual(lhs.x, rhs.x) &&
           floatingPointEqual(lhs.y, rhs.y) &&
           lhs.color == rhs.color;
}
bool operator!= (const DrawnParticle& lhs, const DrawnParticle& rhs) {
    return !(lhs == rhs);
}
