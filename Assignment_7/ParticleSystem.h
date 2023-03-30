#pragma once

#include "Particle.h"
#include "GUI/SimpleTest.h"
#include "GUI/MemoryDiagnostics.h"

/* Dimensions of a graphics scene. Particles that move outside the bounding
 * box [0, SCENE_WIDTH) x [0, SCENE_HEIGHT) disappear.
 */
const double SCENE_WIDTH  = 800;
const double SCENE_HEIGHT = 600;

/* Type representing a particle system: a collection of particles that can
 * be moved around the screen.
 */
class ParticleSystem {
public:
    /* Creates a new, empty particle system. */
    ParticleSystem();

    /* Cleans up all memory used by the particle system. */
    ~ParticleSystem();

    /* Adds a new particle to the scene. If the particle is out of bounds or
     * has a negative lifetime, has no effect. The particle is placed at
     * the end of the list of particles, and this function runs in time O(1).
     */
    void add(const Particle& particle);

    /* Returns how many particles are in the particle system. Runs in time
     * O(1).
     */
    int numParticles() const;

    /* Draws all the particles in the system. */
    void drawParticles() const;

    /* Moves all particles in the system. This may cause some particles
     * to be removed (if their lifetimes end or the particles move out of
     * bounds) or added (if firework particles explode).
     */
    void moveParticles();

private:
    /* Doubly-linked list type made of particles. */
    struct ParticleCell {
        Particle particle;
        ParticleCell* next;
        ParticleCell* prev;

        TRACK_ALLOCATIONS_OF(ParticleCell);
    };

    /* Pointer to the first cell in the list of particles, or nullptr if there
     * are no particles in the list.
     */
    ParticleCell* head;
    ParticleCell* cur;
    int numofParticle = 0;
    /* TODO: Add any new member variables or helper functions here. Make sure
     * to delete this comment before submitting.
     */



    /* Allows SimpleTest to peek inside the ParticleSystem type. */
    ALLOW_TEST_ACCESS();
};
