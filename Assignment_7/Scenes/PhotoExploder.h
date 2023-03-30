/******************************************************************************
 * File: PhotoExploder.h
 *
 * A scene in which an image is "exploded" and then pulled back together to
 * reveal the final picture.
 */
#include "Demos/Scene.h"
#include "ParticleSystem.h"
#include "queue.h"

class PhotoExploder: public Scene<PhotoExploder> {
public:
    PhotoExploder();

    void tick();
    void draw();

private:
    ParticleSystem system;
    Queue<std::string> files; // Image files to process

    int countdown;  // Number of ticks before the animation starts
    int stepsLeft;  // Steps before we're done.
    int imagePause; // Number of ticks the image remains for

    void loadNextImage();
    void setImage(const std::string& filename);
    void addParticle(double x, double y, Color color);
};
