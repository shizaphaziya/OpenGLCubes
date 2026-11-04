#include "Renderer.h"

#include <GL/freeglut.h>

#include <algorithm>

namespace {
constexpr float kPlaneHalfSize = 10.0f;
constexpr float kCubeSize = 1.0f;

void setCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(12.0, 12.0, 16.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
}  // namespace

void Renderer::initialize() const {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.82f, 0.82f, 0.82f, 1.0f);
}

void Renderer::draw(const std::vector<Cube>& cubes) const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setCamera();

    glColor3f(0.58f, 0.58f, 0.58f);
    glBegin(GL_QUADS);
    glVertex3f(-kPlaneHalfSize, 0.0f, -kPlaneHalfSize);
    glVertex3f(-kPlaneHalfSize, 0.0f,  kPlaneHalfSize);
    glVertex3f( kPlaneHalfSize, 0.0f,  kPlaneHalfSize);
    glVertex3f( kPlaneHalfSize, 0.0f, -kPlaneHalfSize);
    glEnd();

    glColor3f(0.78f, 0.25f, 0.18f);
    for (const Cube& cube : cubes) {
        glPushMatrix();
        glTranslatef(cube.position.x, cube.position.y, cube.position.z);
        glutSolidCube(kCubeSize);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void Renderer::resize(int width, int height) const {
    height = std::max(height, 1);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<double>(width) / height, 0.1, 100.0);
    setCamera();
}

bool Renderer::groundPointAt(int mouseX, int mouseY, int windowHeight,
                             float& worldX, float& worldZ) const {
    GLdouble modelView[16];
    GLdouble projection[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble nearX, nearY, nearZ;
    GLdouble farX, farY, farZ;
    const GLint screenY = windowHeight - mouseY;
    if (gluUnProject(mouseX, screenY, 0.0, modelView, projection, viewport,
                     &nearX, &nearY, &nearZ) != GL_TRUE ||
        gluUnProject(mouseX, screenY, 1.0, modelView, projection, viewport,
                     &farX, &farY, &farZ) != GL_TRUE) {
        return false;
    }

    const GLdouble directionY = farY - nearY;
    if (directionY == 0.0) {
        return false;
    }

    const GLdouble distance = -nearY / directionY;
    if (distance < 0.0) {
        return false;
    }

    worldX = static_cast<float>(nearX + distance * (farX - nearX));
    worldZ = static_cast<float>(nearZ + distance * (farZ - nearZ));
    worldX = std::clamp(worldX, -kPlaneHalfSize + kCubeSize / 2.0f,
                        kPlaneHalfSize - kCubeSize / 2.0f);
    worldZ = std::clamp(worldZ, -kPlaneHalfSize + kCubeSize / 2.0f,
                        kPlaneHalfSize - kCubeSize / 2.0f);
    return true;
}
