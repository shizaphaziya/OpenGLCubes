#include "Physics.h"

#include <algorithm>
#include <cmath>

namespace {
constexpr float kCubeSize = 1.0f;
constexpr float kHalfCube = kCubeSize / 2.0f;
constexpr float kGravity = 9.81f;
constexpr float kRestitution = 0.15f;

float& component(Vec3& vector, int axis) {
    if (axis == 0) return vector.x;
    if (axis == 1) return vector.y;
    return vector.z;
}

void resolveCubeCollision(Cube& first, Cube& second) {
    const float dx = first.position.x - second.position.x;
    const float dy = first.position.y - second.position.y;
    const float dz = first.position.z - second.position.z;
    const float overlapX = kCubeSize - std::abs(dx);
    const float overlapY = kCubeSize - std::abs(dy);
    const float overlapZ = kCubeSize - std::abs(dz);

    if (overlapX <= 0.0f || overlapY <= 0.0f || overlapZ <= 0.0f) {
        return;
    }

    int axis = 1;
    float penetration = overlapY;
    if (overlapX < penetration) {
        axis = 0;
        penetration = overlapX;
    }
    if (overlapZ < penetration) {
        axis = 2;
        penetration = overlapZ;
    }

    const float difference = component(first.position, axis) -
                             component(second.position, axis);
    const float direction = difference >= 0.0f ? 1.0f : -1.0f;
    component(first.position, axis) += direction * penetration / 2.0f;
    component(second.position, axis) -= direction * penetration / 2.0f;

    const float relativeVelocity = component(first.velocity, axis) -
                                  component(second.velocity, axis);
    const float relativeAlongNormal = relativeVelocity * direction;
    if (relativeAlongNormal < 0.0f) {
        const float impulse =
            -(1.0f + kRestitution) * relativeAlongNormal / 2.0f;
        component(first.velocity, axis) += impulse * direction;
        component(second.velocity, axis) -= impulse * direction;
    }
}
}  // namespace

void Physics::update(std::vector<Cube>& cubes, float deltaTime) const {
    for (Cube& cube : cubes) {
        cube.velocity.y -= kGravity * deltaTime;
        cube.position.x += cube.velocity.x * deltaTime;
        cube.position.y += cube.velocity.y * deltaTime;
        cube.position.z += cube.velocity.z * deltaTime;
    }

    for (std::size_t i = 0; i < cubes.size(); ++i) {
        for (std::size_t j = i + 1; j < cubes.size(); ++j) {
            resolveCubeCollision(cubes[i], cubes[j]);
        }
    }

    for (Cube& cube : cubes) {
        if (cube.position.y < kHalfCube) {
            cube.position.y = kHalfCube;
            cube.velocity.y = cube.velocity.y < -0.5f
                                 ? -cube.velocity.y * kRestitution
                                 : 0.0f;
            cube.velocity.x *= 0.98f;
            cube.velocity.z *= 0.98f;
        }
    }
}
