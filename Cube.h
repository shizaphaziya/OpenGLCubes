#pragma once

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct Cube {
    Vec3 position;
    Vec3 velocity;
};
