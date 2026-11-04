#pragma once

#include "Cube.h"

#include <vector>

class Physics {
public:
    void update(std::vector<Cube>& cubes, float deltaTime) const;
};
