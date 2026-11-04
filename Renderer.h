#pragma once

#include "Cube.h"

#include <vector>

class Renderer {
public:
    void initialize() const;
    void draw(const std::vector<Cube>& cubes) const;
    void resize(int width, int height) const;
    bool groundPointAt(int mouseX, int mouseY, int windowHeight,
                       float& worldX, float& worldZ) const;
};
