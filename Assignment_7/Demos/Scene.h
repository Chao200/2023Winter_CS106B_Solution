/******************************************************************************
 * File: Scene.h
 *
 * Types used to implement graphics scenes.
 */

#pragma once
#include "Color.h"
#include "gcanvas.h"
#include "gtimer.h"
#include <string>
#include <typeinfo>

class SceneImpl {
public:
    virtual ~SceneImpl() = default;

    /* Advances the scene forward one step. */
    virtual void tick();

    /* Draws the scene. */
    virtual void draw();

    /* React to mouse events. */
    virtual void mousePressed(double x, double y);
    virtual void mouseReleased(double x, double y);
    virtual void mouseDragged(double x, double y);
    virtual void mouseMoved(double x, double y);

protected:
    /* Drawing Functions */
    static void setColor(const Color& color);
    static void setColor(const std::string& color);

    static void drawRect(double x, double y, double width, double height);
    static void fillRect(double x, double y, double width, double height);

    static void drawOval(double x, double y, double width, double height);
    static void fillOval(double x, double y, double width, double height);

    static void drawLine(double x0, double y0, double x1, double y1);

    /* Simulation speed. Default is 50FPS */
    static void setFramesPerSecond(int fps);

private:
    static GCanvas* theCanvas;
    static GTimer* theTimer;
    friend class SceneRendererGUI;

    /* Transform coordinates. */
    static GPoint     toScreen(const GPoint& pt);
    static GRectangle toScreen(const GRectangle& rect);
    static GPoint     toAbstract(const GPoint& pt);
    static GRectangle toAbstract(const GRectangle& rect);

    static GCanvas* canvas();
    static GTimer*  timer();
};


/* * * * * Scene detection and registry. * * * * */

#include <functional>
#include <memory>
#include <cstdlib>
#include <cxxabi.h> // Nonstandard, but supported on g++ and clang.
#include <string>
#include "error.h"

namespace SceneRegistry {
    /* The actual registry. */
    using Constructor = std::function<std::shared_ptr<SceneImpl>()>;
    std::map<std::string, Constructor>& scenes();
}

template <typename T> class Scene: public SceneImpl {
public:
    ~Scene() {
        _initializer++;
    }

private:
    static int _initializer;
    static int registerType() {
        /* std::type_info does not guarantee that .name() will be at all human-readable.
         * Use this g++/clang-specific logic to "demangle" the name back into a human-readable
         * format.
         */
        int statusCode;
        auto* realName = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &statusCode);
        if (statusCode != 0) error("Internal error: Couldn't demangle name?");

        /* Store the constructor. */
        SceneRegistry::scenes().insert(make_pair(std::string(realName), [] {
            return std::make_shared<T>();
        }));

        std::free(realName);
        return 137; // They said I could return anything, so I returned 137.
    }
};

template <typename T>
int Scene<T>::_initializer = Scene<T>::registerType();
