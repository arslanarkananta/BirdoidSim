#ifndef BOID_H
#define BOID_H

#include "raylib.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define BOID_NUM 500
#define MAP_SIZE 200.0f
#define TRAIL_LENGTH 30
#define TRAIL_UPDATE_RATE 3
#define TURN_FACTOR 0.2f

#define DEF_MIN_SPEED 2.0f
#define DEF_MAX_SPEED 3.0f
#define DEF_VISUAL_RANGE 60.0f
#define DEF_PROTECTED_RANGE 10.0f
#define DEF_COHESION 0.0005f
#define DEF_AVOID 0.15f
#define DEF_ALIGN 0.6f

extern float visualRange;
extern float protectedRange;
extern float cohesionFactor;
extern float avoidFactor;
extern float alignFactor;
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