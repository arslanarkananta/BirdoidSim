#include "raylib.h"
#pragma once

constexpr int screenWidth = 1200, screenHeight = 900, boidNum = 500;
constexpr float mapSize = 200.0f;
extern float visualRange, protectedRange, turnFactor;
extern float centeringFactor, avoidFactor, matchingFactor;
extern float maxspeed, minspeed;

class Boid {
public:
    Boid() {
        int mapSizeInt = static_cast<int>(mapSize);
        int maxSpeedInt = static_cast<int>(maxspeed);
    
        pos = { 
            static_cast<float>(GetRandomValue(-mapSizeInt, mapSizeInt)),
            static_cast<float>(GetRandomValue(-mapSizeInt, mapSizeInt)),
            static_cast<float>(GetRandomValue(-mapSizeInt, mapSizeInt))
        };
        vel = {
            static_cast<float>(GetRandomValue(-maxSpeedInt, maxSpeedInt)),
            static_cast<float>(GetRandomValue(-maxSpeedInt, maxSpeedInt)),
            static_cast<float>(GetRandomValue(-maxSpeedInt, maxSpeedInt))
        };
    }

    void UpdatePosition(Boid* boids);
    void DrawBoid(const Camera3D& camera);

private:
    Vector3 pos, vel;
};