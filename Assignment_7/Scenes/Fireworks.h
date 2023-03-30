/******************************************************************************
 * File: Fireworks.h
 *
 * A scene containing a fireworks show!
 */

#pragma once

#include "Demos/Scene.h"
#include "ParticleSystem.h"

class Fireworks: public Scene<Fireworks> {
public:
    Fireworks();

    void tick();
    void draw();

private:
    ParticleSystem system;
};
