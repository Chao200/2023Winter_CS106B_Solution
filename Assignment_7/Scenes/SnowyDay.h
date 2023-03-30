/******************************************************************************
 * File: SnowyDay.h
 *
 * A scene of looking out the window on a snowy day.
 */
#pragma once

#include "Demos/Scene.h"
#include "ParticleSystem.h"

class SnowyDay: public Scene<SnowyDay> {
public:
    void tick();
    void draw();

private:
    ParticleSystem system;

    void drawWindow();
};
