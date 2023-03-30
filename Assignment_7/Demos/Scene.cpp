#include "Scene.h"
#include "ParticleSystem.h"
using namespace std;

/***** Canvas Manipulation *****/

/* Definition of static members. */
GCanvas* SceneImpl::theCanvas = nullptr;
GTimer*  SceneImpl::theTimer  = nullptr;

/* Coordinate conversion. */
GPoint SceneImpl::toScreen(const GPoint& pt) {
    /* Translate so that (0, 0) locally is the upper-left corner of the drawing
     * surface.
     */
    return {
        pt.x + (canvas()->getWidth()  - SCENE_WIDTH)  / 2.0,
        pt.y + (canvas()->getHeight() - SCENE_HEIGHT) / 2.0
    };
}

GRectangle SceneImpl::toScreen(const GRectangle& rect) {
    auto corner = toScreen(GPoint{ rect.x, rect.y });
    return {
        corner.x, corner.y, rect.width, rect.height
    };
}

GPoint SceneImpl::toAbstract(const GPoint& pt) {
    /* Translate back based on where the upper-left corner of the display
     * content is.
     */
    return {
        pt.x - (canvas()->getWidth()  - SCENE_WIDTH)  / 2.0,
        pt.y - (canvas()->getHeight() - SCENE_HEIGHT) / 2.0
    };
}

GRectangle SceneImpl::toAbstract(const GRectangle& rect) {
    auto corner = toAbstract(GPoint{ rect.x, rect.y });
    return {
        corner.x, corner.y, rect.width, rect.height
    };
}

GCanvas* SceneImpl::canvas() {
    if (!theCanvas) {
        error("No canvas has been set. This can happen if, for example, you try using a Scene inside of a STUDENT_TEST.");
    }
    return theCanvas;
}

GTimer* SceneImpl::timer() {
    if (!theTimer) {
        error("No timer has been set. This can happen if, for example, you try using a Scene inside of a STUDENT_TEST.");
    }
    return theTimer;
}

/***** Graphics Logic *****/
void SceneImpl::setColor(const Color& color) {
    canvas()->setColor(color.rgbValue());
}

void SceneImpl::setColor(const std::string& color) {
    canvas()->setColor(color);
}

void SceneImpl::drawRect(double x, double y, double width, double height) {
    canvas()->drawRect(toScreen({ x, y, width, height }));
}

void SceneImpl::fillRect(double x, double y, double width, double height) {
    canvas()->fillRect(toScreen({ x, y, width, height }));
}

void SceneImpl::drawOval(double x, double y, double width, double height) {
    canvas()->drawOval(toScreen({ x, y, width, height }));
}

void SceneImpl::fillOval(double x, double y, double width, double height) {
    canvas()->fillOval(toScreen({ x, y, width, height }));
}

void SceneImpl::drawLine(double x0, double y0, double x1, double y1) {
    canvas()->drawLine(toScreen({ x0, y0 }), toScreen({ x1, y1 }));
}

/***** Frame Rate *****/
void SceneImpl::setFramesPerSecond(int fps) {
    if (fps <= 0) {
        error("Illegal frame rate.");
    }

    timer()->setDelay(1000.0 / fps);
}

/***** Default Handler *****/

void SceneImpl::tick() {
    // Do nothing by default
}

void SceneImpl::draw() {
    // Do nothing by default
}

void SceneImpl::mousePressed(double, double) {
    // Do nothing by default
}

void SceneImpl::mouseReleased(double, double) {
    // Do nothing by default
}

void SceneImpl::mouseDragged(double, double) {
    // Do nothing by default
}

void SceneImpl::mouseMoved(double, double) {
    // Do nothing by default
}

/***** Scene Registry *****/
namespace SceneRegistry {
    map<string, Constructor>& scenes() {
        static map<string, Constructor> theRegistry;
        return theRegistry;
    }
}
