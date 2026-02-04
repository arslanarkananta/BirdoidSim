#include "boid.h"
#include "raymath.h"

void initBoid(Boid* boid) {
    int mapSizeInt = (int)MAP_SIZE;
    int maxSpeedInt = (int)maxspeed;
    
    boid->pos = (Vector3){
        (float)GetRandomValue(-mapSizeInt, mapSizeInt),
        (float)GetRandomValue(-mapSizeInt, mapSizeInt),
        (float)GetRandomValue(-mapSizeInt, mapSizeInt)
    };
    boid->vel = (Vector3){
        (float)GetRandomValue(-maxSpeedInt, maxSpeedInt),
        (float)GetRandomValue(-maxSpeedInt, maxSpeedInt),
        (float)GetRandomValue(-maxSpeedInt, maxSpeedInt)
    };
}

void updateBoidPosition(Boid* boid, Boid* boids) {
    Vector3 closeVec = { 0.0f, 0.0f, 0.0f };
    Vector3 alignVec = { 0.0f, 0.0f, 0.0f };
    Vector3 cohesionVec = { 0.0f, 0.0f, 0.0f };
    int closeCount = 0, alignCount = 0, cohesionCount = 0;

    for (int i = 0; i < BOID_NUM; ++i) {
        if (&boids[i] != boid) {
            float dist = Vector3Distance(boids[i].pos, boid->pos);
            if (dist < protectedRange) {
                closeVec = Vector3Add(closeVec, Vector3Subtract(boid->pos, boids[i].pos));
                closeCount++;
            }
            if (dist < visualRange) {
                alignVec = Vector3Add(alignVec, boids[i].vel);
                cohesionVec = Vector3Add(cohesionVec, boids[i].pos);
                alignCount++;
                cohesionCount++;
            }
        }
    }

    if (closeCount > 0) closeVec = Vector3Scale(Vector3Normalize(closeVec), 1.0f / closeCount);
    if (alignCount > 0) alignVec = Vector3Scale(Vector3Normalize(alignVec), 1.0f / alignCount);
    if (cohesionCount > 0) {
        cohesionVec = Vector3Scale(cohesionVec, 1.0f / cohesionCount);
        cohesionVec = Vector3Scale(Vector3Subtract(cohesionVec, boid->pos), 1.0f);
    }

    boid->vel = Vector3Add(boid->vel, Vector3Scale(closeVec, avoidFactor));
    boid->vel = Vector3Add(boid->vel, Vector3Scale(alignVec, alignFactor));
    boid->vel = Vector3Add(boid->vel, Vector3Scale(cohesionVec, cohesionFactor));

    if (boid->pos.x < -MAP_SIZE || boid->pos.x > MAP_SIZE) 
        boid->vel.x += (boid->pos.x < -MAP_SIZE) ? TURN_FACTOR : -TURN_FACTOR;
    if (boid->pos.y < -MAP_SIZE || boid->pos.y > MAP_SIZE) 
        boid->vel.y += (boid->pos.y < -MAP_SIZE) ? TURN_FACTOR : -TURN_FACTOR;
    if (boid->pos.z < -MAP_SIZE || boid->pos.z > MAP_SIZE) 
        boid->vel.z += (boid->pos.z < -MAP_SIZE) ? TURN_FACTOR : -TURN_FACTOR;

    float speed = Vector3Length(boid->vel);
    if (speed > maxspeed) boid->vel = Vector3Scale(boid->vel, maxspeed / speed);
    else if (speed < minspeed) boid->vel = Vector3Scale(boid->vel, minspeed / speed);

    boid->pos = Vector3Add(boid->pos, boid->vel);
}

void drawBoid(Boid* boid, const Camera3D* camera) {
    Vector3 forward = Vector3Normalize(boid->vel);
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, (Vector3){ 0.0f, 1.0f, 0.0f }));
    Vector3 up = Vector3CrossProduct(right, forward);

    float pyramidHeight = 6.0f;
    float pyramidBaseRadius = 2.0f;
    int numSegments = 36;

    Vector3 apex = Vector3Add(boid->pos, Vector3Scale(forward, pyramidHeight));
    Vector3 baseCenter = boid->pos;

    Color color = (Color){ 100, 100, 100, 255 };

    for (int i = 0; i < numSegments; i++) {
        float angle1 = 2.0f * PI * (float)i / (float)numSegments;
        float angle2 = 2.0f * PI * (float)(i + 1) / (float)numSegments;

        Vector3 point1 = Vector3Add(baseCenter, Vector3Scale(right, pyramidBaseRadius * cosf(angle1)));
        point1 = Vector3Add(point1, Vector3Scale(up, pyramidBaseRadius * sinf(angle1)));

        Vector3 point2 = Vector3Add(baseCenter, Vector3Scale(right, pyramidBaseRadius * cosf(angle2)));
        point2 = Vector3Add(point2, Vector3Scale(up, pyramidBaseRadius * sinf(angle2)));

        DrawTriangle3D(point1, point2, apex, color);
    }

    for (int i = 0; i < numSegments; i++) {
        float angle1 = 2.0f * PI * i / (float)numSegments;
        float angle2 = 2.0f * PI * (i + 1) / (float)numSegments;

        Vector3 point1 = Vector3Add(baseCenter, Vector3Scale(right, pyramidBaseRadius * cosf(angle1)));
        point1 = Vector3Add(point1, Vector3Scale(up, pyramidBaseRadius * sinf(angle1)));

        Vector3 point2 = Vector3Add(baseCenter, Vector3Scale(right, pyramidBaseRadius * cosf(angle2)));
        point2 = Vector3Add(point2, Vector3Scale(up, pyramidBaseRadius * sinf(angle2)));

        DrawTriangle3D(baseCenter, point2, point1, color);
    }
}

Vector3 getBoidPosition(Boid* boid) {
    return boid->pos;
}