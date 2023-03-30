/******************************************************************************
 * File: MagicWand.h
 *
 * A scene with a magic wand. Moving the mouse waves the wand, and pressing the
 * mouse emits a shower of particles.
 */
#pragma once

#include "Demos/Scene.h"
#include "ParticleSystem.h"

class MagicWand: public Scene<MagicWand> {
public:
    void tick();
    void draw();

    void mouseMoved(double x, double y);
    void mousePressed(double x, double y);
    void mouseReleased(double x, double y);
    void mouseDragged(double x, double y);

private:
    ParticleSystem system;

    bool mouseDown = false;
    GPoint mouse{ SCENE_WIDTH / 2.0, SCENE_HEIGHT / 2.0 };
};
