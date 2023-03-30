/******************************************************************************
 * File: Fountain.h
 *
 * A scene of a water fountain.
 */
#pragma once

#include "Demos/Scene.h"
#include "ParticleSystem.h"
#include "vector.h"
#include "gobjects.h"

class Fountain: public Scene<Fountain> {
public:
    Fountain();

    void tick();
    void draw();

private:
    ParticleSystem system;

    /* Where the emitters are. */
    Vector<GPoint> emitters;

    /* Make a nice water color. */
    Color waterColor() const;
};
