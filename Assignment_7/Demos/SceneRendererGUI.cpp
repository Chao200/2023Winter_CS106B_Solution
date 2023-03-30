#include "GUI/MiniGUI.h"
#include "DrawParticle.h"
#include "ParticleSystem.h"
#include "Scene.h"
#include "gtimer.h"
#include "gbutton.h"
using namespace std;

namespace {
    const double kParticleRadius = 2;
    const double kDefaultDelay = 20;
}

class SceneRendererGUI: public ProblemHandler {
public:
    SceneRendererGUI(GWindow& window);
    ~SceneRendererGUI();

    void timerFired()override;

    void mousePressed(double x, double y) override;
    void mouseReleased(double x, double y) override;
    void mouseMoved(double x, double y) override;
    void mouseDragged(double x, double y) override;

    void actionPerformed(GObservable* source) override;

protected:
    void repaint() override;

private:
    GTimer* timer;
    shared_ptr<SceneImpl> scene;

    Temporary<GComboBox> sceneSelector;
    Temporary<GButton>   loadButton;

    /* Loads the selected scene from the dropdown. */
    void loadSelectedScene();
};

/* Set up the scene and install the canvas. */
SceneRendererGUI::SceneRendererGUI(GWindow& window) : ProblemHandler(window) {
    timer = new GTimer(20);
    timer->start();

    /* Stash the canvas / timer where the scene can get it. */
    SceneImpl::theCanvas = window.getCanvas();
    SceneImpl::theTimer  = timer;

    /* Set up the particle-drawing logic. */
    setDrawFunction([&] (double x, double y, Color color) {
        double maxX = SCENE_WIDTH;
        double maxY = SCENE_HEIGHT;
        if (x >= 0 && y >= 0 && x < maxX && y < maxY) {
            window.setColor(color.rgbValue());
            window.fillOval(SceneImpl::toScreen({ x - kParticleRadius, y - kParticleRadius, kParticleRadius * 2, kParticleRadius * 2 }));
        } else {
            error("Particle out of bounds.");
        }
    });

    /* Create a list of all scenes. */
    sceneSelector = make_temporary<GComboBox>(window, "SOUTH");
    sceneSelector->setEditable(false);
    for (const auto& entry: SceneRegistry::scenes()) {
        sceneSelector->addItem(entry.first);
    }

    loadButton = make_temporary<GButton>(window, "SOUTH", "Load Scene");
}

SceneRendererGUI::~SceneRendererGUI() {
    /* BUG: Leak the timer to prevent a race condition. */
    timer->stop();

    /* Release the scene resource, just in case anything there is trying to access
     * the draw functions.
     */
    scene.reset();

    /* Disable drawing. */
    setDrawFunction(nullptr);

    /* Clear the canvas/timer, since it won't be used any more. */
    SceneImpl::theCanvas = nullptr;
    SceneImpl::theTimer  = nullptr;
}

/* When the timer fires, advance the scene. */
void SceneRendererGUI::timerFired() {
    /* If there is a scene, make it advance. */
    if (scene) {
        scene->tick();
        requestRepaint();
    }
}

/* Render the underlying scene. */
void SceneRendererGUI::repaint() {
    /* Wipe everything that's already there. */
    clearDisplay(window(), "white");

    /* If there's no scene, we're done. */
    if (!scene) return;

    /* Render the scene. */
    scene->draw();

    /* Draw four rectangles around the content area to ensure that nothing
     * overdraws out of the allotted area.
     */
    window().setColor(0xFFFFFF);

    double xMargin = (window().getCanvasWidth()  - SCENE_WIDTH)  / 2;
    double yMargin = (window().getCanvasHeight() - SCENE_HEIGHT) / 2;

    /* Top */
    window().fillRect(0, 0, window().getCanvasWidth(), yMargin);

    /* Left */
    window().fillRect(0, 0, xMargin, window().getCanvasHeight());

    /* Right */
    window().fillRect(window().getCanvasWidth()  - xMargin, 0, xMargin, window().getCanvasHeight());

    /* Bottom. */
    window().fillRect(0, window().getCanvasHeight() - yMargin, window().getCanvasWidth(),  yMargin);

    /* Draw a box around the scene. */
    window().setColor(0x000000);
    window().drawRect((window().getCanvasWidth()  - SCENE_WIDTH)  / 2.0,
                      (window().getCanvasHeight() - SCENE_HEIGHT) / 2.0,
                      SCENE_WIDTH, SCENE_HEIGHT);
}

/* Translate from world space to local space. */
void SceneRendererGUI::mouseDragged(double x, double y) {
    if (!scene) return;

    auto pt = SceneImpl::toAbstract({ x, y });
    scene->mouseDragged(pt.x, pt.y);
}

void SceneRendererGUI::mousePressed(double x, double y) {
    if (!scene) return;

    auto pt = SceneImpl::toAbstract({ x, y });
    scene->mousePressed(pt.x, pt.y);
}

void SceneRendererGUI::mouseMoved(double x, double y) {
    if (!scene) return;

    auto pt = SceneImpl::toAbstract({ x, y });
    scene->mouseMoved(pt.x, pt.y);
}

void SceneRendererGUI::mouseReleased(double x, double y) {
    if (!scene) return;

    auto pt = SceneImpl::toAbstract({ x, y });
    scene->mouseReleased(pt.x, pt.y);
}

void SceneRendererGUI::loadSelectedScene() {
    string curr = sceneSelector->getSelectedItem();

    /* Wipe any existing scene in case dtor logic has side effects. */
    scene.reset();

    /* Reset the timer. */
    timer->setDelay(kDefaultDelay);

    /* Create the new scene. */
    scene = SceneRegistry::scenes().at(curr)();
    requestRepaint();
}

void SceneRendererGUI::actionPerformed(GObservable* source) {
    if (source == loadButton) {
        loadSelectedScene();
    }
}

GRAPHICS_HANDLER("Scenes", GWindow& window) {
    return make_shared<SceneRendererGUI>(window);
}
