#pragma once

#include "Demos/Color.h"
#include "vector.h"
#include <ostream>

/* Type representing a particle drawn on the screen. Unlike a full Particle,
 * which has a ton of extra information (type, velocity, lifetime, etc.), a
 * DrawnParticle just has a location and a color.
 */
struct DrawnParticle {
    double x, y;
    Color color;
};

/* A type that intercepts all drawn particles and writes them down for
 * future inspection.
 */
class ParticleCatcher {
public:
    ParticleCatcher();
    ~ParticleCatcher();

    /* Returns how many particles were drawn. */
    int numDrawn() const;

    /* Returns the particle at a given index. */
    DrawnParticle operator[] (int index) const;

    /* Clears all memory of the particles seen thus far. */
    void reset();

private:
    Vector<DrawnParticle> drawn;
};

/* Printer. */
std::ostream& operator<< (std::ostream& out, const DrawnParticle& particle);

/* Comparators. */
bool operator== (const DrawnParticle& lhs, const DrawnParticle& rhs);
bool operator!= (const DrawnParticle& lhs, const DrawnParticle& rhs);
