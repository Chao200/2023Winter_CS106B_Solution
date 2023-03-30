#include "Fireworks.h"
using namespace std;

Fireworks::Fireworks() {
    /* To make this more enjoyable, decrease the frame rate to 20FPS
     * rather than the default 50. Otherwise it's way too fast!
     */
    setFramesPerSecond(20);
}

void Fireworks::tick() {
    /* Maybe launch another rocket! */
    if (randomChance(0.2)) {
        /* Pick a random x coordinate. */
        double x = randomReal(0, SCENE_WIDTH - 1);

        /* Launch from the bottom. */
        double y = SCENE_HEIGHT - 1;

        /* Horizontal movement is random. */
        double dx = randomReal(-5, +5);

        /* Determine a launch speed to put us toward the upper region of the window
         * (between the 1/4 and 5/6 point). Thanks, physics!
         *
         * The minus sign is here because positive y moves down, but we want this
         * rocket to move upward.
         */
        double dy = -sqrt(2 * randomReal(y / 4, 5 * y / 6));

        /* Set it up as a particle. */
        Particle particle;
        particle.x = x;
        particle.y = y;

        particle.dx = dx;
        particle.dy = dy;

        /* At each time step dy decreases by one. When it reaches zero it will be
         * hovering, and that's a great time for it to explode.
         */
        particle.lifetime = -dy;

        particle.color = Color::WHITE;
        particle.type  = ParticleType::FIREWORK;

        /* Launch! */
        system.add(particle);
    }

    system.moveParticles();
}

void Fireworks::draw()  {
    /* Make the whole scene area black. */
    setColor(Color::BLACK);
    fillRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);

    /* Draw the particles, including both the fireworks and the
     * streamers.
     */
    system.drawParticles();
}
