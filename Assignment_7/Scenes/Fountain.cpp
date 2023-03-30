#include "Fountain.h"
using namespace std;

/* Spacing between elements of the fountain. */
const double kSpacing  = SCENE_HEIGHT / 9;

/* Thickness of the central column. */
const double kColumnThickness = 20;

/* Thickness of each branch. */
const double kBranchThickness = 10;

/* Angles at which water can be sprayed from the emitters. */
const double kMinAngle = M_PI / 2 - M_PI / 12;
const double kMaxAngle = M_PI / 2 + M_PI / 12;

/* Water speed. */
const double kMinWaterSpeed = 10;
const double kMaxWaterSpeed = 20;

/* Particles per emitter per frame. */
const int kFlowRate = 10;

/* Minimum and maximum white components for water particles. */
const int kMinWhite = 60;
const int kMaxWhite = 210;

Fountain::Fountain() {
    /* Rough sketch of the fountain:
     *
     *            *
     *            |
     *            |
     *         *--+--*
     *            |
     *            |
     *      *--*--+--*--*
     *            |
     *            |
     *
     * Each * here is an emitter. The | and -- lines represent the same
     * spacing.
     */
    double bottomRowY = SCENE_HEIGHT - 1 - 2 * kSpacing;
    emitters.add({ SCENE_WIDTH / 2 - 2 * kSpacing, bottomRowY });
    emitters.add({ SCENE_WIDTH / 2 - 1 * kSpacing, bottomRowY });
    emitters.add({ SCENE_WIDTH / 2 + 1 * kSpacing, bottomRowY });
    emitters.add({ SCENE_WIDTH / 2 + 2 * kSpacing, bottomRowY });

    double middleRowY = SCENE_HEIGHT - 1 - 4 * kSpacing;
    emitters.add({ SCENE_WIDTH / 2 - 1 * kSpacing, middleRowY });
    emitters.add({ SCENE_WIDTH / 2 + 1 * kSpacing, middleRowY });

    double topRowY = SCENE_HEIGHT - 1 - 6 * kSpacing;
    emitters.add({ SCENE_WIDTH / 2, topRowY });
    emitters.add({ SCENE_WIDTH / 2, topRowY });
}

void Fountain::tick() {
    /* Each source emits water. */
    for (GPoint source: emitters) {
        /* Each source emits multiple particles. */
        for (int i = 0; i < kFlowRate; i++) {
            /* Pick an angle and fire water at that angle. */
            double theta = randomReal(kMinAngle, kMaxAngle);

            /* Select a speed. */
            double speed = randomReal(kMinWaterSpeed, kMaxWaterSpeed);

            /* dy is negative because positive y corresponds to moving down. */
            double dx =  speed * cos(theta);
            double dy = -speed * sin(theta);

            /* Set this up as a particle. */
            Particle data;
            data.color = waterColor();
            data.x = source.x;
            data.y = source.y;
            data.dx = dx;
            data.dy = dy;
            data.type = ParticleType::BALLISTIC;
            data.lifetime = INT_MAX; // Particle only disappears when falling off the frame.

            system.add(data);
        }
    }

    system.moveParticles();
}

/* Picks a color for a particle of water. The particle always has the maximum
 * possible blue component and has a red/green component that is randomly
 * chosen.
 */
Color Fountain::waterColor() const {
    int white = randomInteger(kMinWhite, kMaxWhite);
    return Color(white, white, 255);
}

/* Draw the particles and the fountain itself. */
void Fountain::draw() {
    /* First, the fountain. */
    setColor(Color::GRAY);

    /* Central column. */
    fillRect((SCENE_WIDTH - kColumnThickness) / 2,
             SCENE_HEIGHT - 1 - 6 * kSpacing,
             kColumnThickness,
             6 * kSpacing);

    /* Middle branch. */
    fillRect((SCENE_WIDTH - 2 * kSpacing) / 2,
             SCENE_HEIGHT - 1 - 4 * kSpacing - kBranchThickness / 2,
             2 * kSpacing,
             kBranchThickness);

    /* Bottom branch. */
    fillRect((SCENE_WIDTH - 4 * kSpacing) / 2,
             SCENE_HEIGHT - 1 - 2 * kSpacing - kBranchThickness / 2,
             4 * kSpacing,
             kBranchThickness);

    system.drawParticles();
}
