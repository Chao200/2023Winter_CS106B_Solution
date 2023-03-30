#include "Particle.h"
#include "gmath.h"
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;

/* Printers */
ostream& operator<< (ostream& out, ParticleType type) {
    if (type == ParticleType::STREAMER) {
        return out << "ParticleType::STREAMER";
    } else if (type == ParticleType::BALLISTIC) {
        return out << "ParticleType::BALLISTIC";
    } else if (type == ParticleType::FIREWORK) {
        return out << "ParticleType::FIREWORK";
    } else {
        return out << "<unknown particle type>";
    }
}

ostream& operator<< (ostream& out, const Particle& particle) {
    ostringstream builder;
    builder << fixed << setprecision(numeric_limits<double>::digits10 + 1)
            << "{ "
            << particle.x << ", "
            << particle.y << ", "
            << particle.color << ", "
            << particle.dx << ", "
            << particle.dy << ", "
            << particle.lifetime << ", "
            << particle.type << " }";
    return out << builder.str();
}

/* Comparators */
bool operator== (const Particle& lhs, const Particle& rhs) {
    return floatingPointEqual(lhs.x, rhs.x) &&
           floatingPointEqual(lhs.y, rhs.y) &&
           lhs.color == rhs.color &&
           floatingPointEqual(lhs.dx, rhs.dx) &&
           floatingPointEqual(lhs.dy, rhs.dy) &&
           lhs.lifetime == rhs.lifetime &&
           lhs.type == rhs.type;
}

bool operator!= (const Particle& lhs, const Particle& rhs) {
    return !(lhs == rhs);
}
