#include "Application.h"

#include <GL/freeglut.h>

#include <algorithm>

namespace {
Application* activeApplication = nullptr;
constexpr float kSpawnHeight = 5.0f;
constexpr float kFixedTimeStep = 1.0f / 120.0f;

void displayCallback() {
    activeApplication->draw();
}

void resizeCallback(int width, int height) {
    activeApplication->resize(width, height);
}

void keyboardCallback(unsigned char key, int, int) {
    activeApplication->onKey(key);
}

void mouseCallback(int button, int state, int x, int y) {
    activeApplication->onMouseClick(button, state, x, y);
}

void timerCallback(int) {
    activeApplication->update();
    glutTimerFunc(16, timerCallback, 0);
}
}  // namespace

int Application::run(int argc, char** argv) {
    activeApplication = this;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Space: cube at center | Left click: cube at cursor | Esc: exit");

    renderer_.initialize();
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(resizeCallback);
    glutKeyboardFunc(keyboardCallback);
    glutMouseFunc(mouseCallback);
    previousTimeMs_ = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(16, timerCallback, 0);
    glutMainLoop();
    activeApplication = nullptr;
    return 0;
}

void Application::draw() const {
    renderer_.draw(cubes_);
}

void Application::resize(int width, int height) const {
    renderer_.resize(width, height);
}

void Application::onKey(unsigned char key) {
    if (key == 27) {
        glutLeaveMainLoop();
    } else if (key == ' ') {
        spawnCube(0.0f, 0.0f);
    }
}

void Application::onMouseClick(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) {
        return;
    }

    float worldX;
    float worldZ;
    if (renderer_.groundPointAt(x, y, glutGet(GLUT_WINDOW_HEIGHT),
                                worldX, worldZ)) {
        spawnCube(worldX, worldZ);
    }
}

void Application::update() {
    const int currentTimeMs = glutGet(GLUT_ELAPSED_TIME);
    const float elapsed = std::min((currentTimeMs - previousTimeMs_) / 1000.0f,
                                   0.1f);
    previousTimeMs_ = currentTimeMs;
    timeAccumulator_ += elapsed;

    while (timeAccumulator_ >= kFixedTimeStep) {
        physics_.update(cubes_, kFixedTimeStep);
        timeAccumulator_ -= kFixedTimeStep;
    }

    glutPostRedisplay();
}

void Application::spawnCube(float x, float z) {
    cubes_.push_back({{x, kSpawnHeight, z}, {0.0f, 0.0f, 0.0f}});
}
