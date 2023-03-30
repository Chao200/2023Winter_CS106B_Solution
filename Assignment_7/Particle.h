#pragma once

#include "Demos/Color.h"
#include <ostream>

/* Enumerated type indicating a type of particle. There are three
 * options:
 *
 *   ParticleType::STREAMER:  The default. The particle moves in a straight line
 *                            with a fixed velocity.
 *   ParticleType::BALLISTIC: A particle that moves on a trajectory that
 *                            accelerates downward with time.
 *   ParticleType::FIREWORK:  Like a ballistic particle, but when its lifetime
 *                            ends, a firework explodes into a bunch of
 *                            streamers.
 */
enum class ParticleType {
    STREAMER, BALLISTIC, FIREWORK
};

/* Length of time a particle lives if nothing is specified. The default
 * value is a very, very long time. :-)
 */
const int A_LONG_TIME = 123456789;

/* Type representing a particle in space. */
struct Particle {
    /* The particle's position in space. Default is (0, 0). */
    double x = 0, y = 0;

    /* Particle color. Default is black. */
    Color color;

    /* Particle velocity. Default is stationary. */
    double dx = 0, dy = 0;

    /* Particle lifetime. Default is a very long time. */
    int lifetime = A_LONG_TIME;

    /* Particle type. Default is streamer. */
    ParticleType type = ParticleType::STREAMER;
};

/* Helper functions that allow us to print out ParticleType and Particle
 * values. These are defined as overloads of the << operator. Check
 * Chapter 6.2 of the textbook for more information.
 */
std::ostream& operator<< (std::ostream& out, ParticleType type);
std::ostream& operator<< (std::ostream& out, const Particle& particle);

/* Comparison operators so that Particles can be compared against one
 * another.
 */
bool operator== (const Particle& lhs, const Particle& rhs);
bool operator!= (const Particle& lhs, const Particle& rhs);
