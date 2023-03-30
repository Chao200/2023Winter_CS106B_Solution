/******************************************************************************
 * File: MyCustomScene.h
 *
 * Space for you to create your own graphics scene using your particle system,
 * if that's something you want to do.
 *
 * Each graphics scene is represented as a class that maintains internal
 * information about the state of the scene and contains functions that advance
 * and draw the scene. To create a scene class, use this syntax:
 *
 *     class NameOfYourSceneClass: public Scene<NameOfYourSceneClass> {
 *     public:
 *        void tick();
 *        void draw();
 *
 *     private:
 *        // whatever you want, probably involving a ParticleSystem
 *     };
 *
 * The 'public' bit here is some C++ sorcery we've concocted for this
 * assignment that will then hook your scene into the main graphics display
 * so that it'll be visible in the "Scenes" menu.
 *
 * The tick() function is called automatically around 50 times a second. Each
 * time the tick() function is called, it should update the scene. For example,
 * this would be a great place to call the moveParticles function to move the
 * particles in your particle system, or to create new particles, etc.
 *
 * The draw() function is where you'll draw the scene itself. This is where you
 * will likely want to call your drawParticles() function. You can also draw
 * ovals, lines, and rectangles by using this helper functions, which we've
 * provided to you:
 *
 *     void setColor(Color c); // Sets the color used to draw on the screen
 *
 *     // Draws various geometric figures
 *     void drawRect(double x, double y, double width, double height);
 *     void fillRect(double x, double y, double width, double height);
 *     void drawOval(double x, double y, double width, double height);
 *     void fillOval(double x, double y, double width, double height);
 *     void drawLine(double x0, double y0, double x1, double y1);
 *
 * Some scenes interact with the mouse. If you want to interact with the mouse,
 * you can add these functions to the public section of your class, and the
 * graphics system will call them whenever the user interacts with the mouse:
 *
 *     class NameOfYourSceneClass: public Scene<NameOfYourSceneClass> {
 *     public:
 *        void tick();
 *        void draw();
 *
 *        void mousePressed(double x, double y);  // Mouse button down
 *        void mouseReleased(double x, double y); // Mouse button up
 *        void mouseDragged(double x, double y);  // Mouse moved with button down
 *        void mouseMoved(double x, double y);    // Mouse moved with button up
 *
 *     private:
 *        // as before
 *     };
 *
 * Finally, you can adjust how frequently the tick() function is called. We use
 * this to slow down the fireworks scene a bit.
 *
 *     void setFramesPerSecond(int fps);
 *
 * Happy particling!
 */
#pragma once

#include "Demos/Scene.h"
#include "ParticleSystem.h"

/* Expand the comments at the top of this file to learn more about how to create
 * custom scenes.
 */

/* Uncomment the code below to create a scene class. You should rename it to whatever
 * name seems best to you.
 */
//class NameOfYourSceneClass: public Scene<NameOfYourSceneClass> {
//public:
//    void tick();
//    void draw();

////    /* Only uncomment this code if you want to interact with the mouse. */
////    void mousePressed(double x, double y);
////    void mouseReleased(double x, double y);
////    void mouseDragged(double x, double y);
////    void mouseMoved(double x, double y);

//private:
//    // up to you!
//};
