#include "SnowyDay.h"
using namespace std;

/* Constants controlling the width and height of the window. */
const double kWindowWidth  = SCENE_WIDTH  * 0.5;
const double kWindowHeight = SCENE_HEIGHT * 0.8;

/* Width of the crossbars on the window. */
const double kCrossbarSize = 10;

/* Colors. */
const Color kWallColor   = Color(127, 127, 127);
const Color kBorderColor = Color(127,  63,   0);
const Color kSnowColor   = Color(255, 255, 255);
const Color kSkyColor    = Color(  0,   0,  63);

/* Particle parameters. */

/* Probability of a particle being generated at each pixel across the
 * width of the scene. This is chosen so we average a target number of
 * particles per frame.
 */
const double kParticleProbability = 20 / SCENE_WIDTH;

/* Velocity parameters. */
const double kDxRange = 2;
const double kDownSpeed = 3;

void SnowyDay::tick() {
    /* Possibly add particles all across the top row. */
    for (int x = 0; x < SCENE_WIDTH; x++) {
        if (randomChance(kParticleProbability)) {
            Particle snowflake;

            /* Originate from the top. */
            snowflake.x     = x;
            snowflake.y     = 0;

            /* Wind effects. */
            snowflake.dy    = kDownSpeed;
            snowflake.dx    = randomReal(-kDxRange, +kDxRange);

            /* Proper color. */
            snowflake.color = kSnowColor;
            system.add(snowflake);
        }
    }

    /* Now move all the particles. */
    system.moveParticles();
}

void SnowyDay::draw() {
    /* Fill the background with the sky color. */
    setColor(kSkyColor);
    fillRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);

    /* Draw all the snowflakes. */
    system.drawParticles();

    /* Overdraw the walls, leaving a gap for the window. */
    setColor(kWallColor);

    fillRect(0, 0,  SCENE_WIDTH, (SCENE_HEIGHT - kWindowHeight) / 2.0); // Top
    fillRect(0, 0, (SCENE_WIDTH  -  kWindowWidth) / 2.0, SCENE_HEIGHT); // Left
    fillRect(0, (SCENE_HEIGHT + kWindowHeight) / 2.0,
             SCENE_WIDTH, (SCENE_HEIGHT - kWindowHeight) / 2.0); // Bottom
    fillRect((SCENE_WIDTH  +  kWindowWidth) / 2.0, 0,
             (SCENE_WIDTH  -  kWindowWidth) / 2.0, SCENE_HEIGHT); // Right

    /* Overdraw the crossbars. */
    setColor(kBorderColor);

    /* Horizontal. */
    fillRect((SCENE_WIDTH  -  kWindowWidth) / 2.0,
             (SCENE_HEIGHT - kCrossbarSize) / 2.0,
             kWindowWidth,  kCrossbarSize);

    /* Vertical. */
    fillRect((SCENE_WIDTH  - kCrossbarSize) / 2.0,
             (SCENE_HEIGHT - kWindowHeight) / 2.0,
             kCrossbarSize, kWindowHeight);

    /* Overdraw the window frame. */

    /* Left. */
    fillRect((SCENE_WIDTH  -  kWindowWidth) / 2.0, (SCENE_HEIGHT - kWindowHeight) / 2.0,
             kCrossbarSize, kWindowHeight);
    /* Right. */
    fillRect((SCENE_WIDTH  +  kWindowWidth) / 2.0, (SCENE_HEIGHT - kWindowHeight) / 2.0,
             kCrossbarSize, kWindowHeight);
    /* Top. */
    fillRect((SCENE_WIDTH  -  kWindowWidth) / 2.0, (SCENE_HEIGHT - kWindowHeight) / 2.0,
             kWindowWidth, kCrossbarSize);
    /* Bottom. */
    fillRect((SCENE_WIDTH  -  kWindowWidth) / 2.0, (SCENE_HEIGHT + kWindowHeight) / 2.0,
             kWindowWidth, kCrossbarSize);

}
