#ifndef BOID_H
#define BOID_H

#include "raylib.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define BOID_NUM 500
#define MAP_SIZE 200.0f
#define TRAIL_LENGTH 30
#define TRAIL_UPDATE_RATE 3

extern float visualRange;
extern float protectedRange;
extern float turnFactor;
extern float centeringFactor;
extern float avoidFactor;
extern float matchingFactor;
extern float maxspeed;
extern float minspeed;

typedef struct {
    Vector3 pos;
    Vector3 vel;
} Boid;

void initBoid(Boid* boid);
void updateBoidPosition(Boid* boid, Boid* boids);
void drawBoid(Boid* boid, const Camera3D* camera);
Vector3 getBoidPosition(Boid* boid);

#endif