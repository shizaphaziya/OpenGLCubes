#pragma once

#include "Physics.h"
#include "Renderer.h"

#include <vector>

class Application {
public:
    int run(int argc, char** argv);
    void draw() const;
    void resize(int width, int height) const;
    void onKey(unsigned char key);
    void onMouseClick(int button, int state, int x, int y);
    void update();

private:
    void spawnCube(float x, float z);

    Renderer renderer_;
    Physics physics_;
    std::vector<Cube> cubes_;
    int previousTimeMs_ = 0;
    float timeAccumulator_ = 0.0f;
};
