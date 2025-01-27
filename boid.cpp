#include "boid.h"
#include "raymath.h"

void Boid::UpdatePosition(Boid* boids) {
    Vector3 closeVec = { 0.0f, 0.0f, 0.0f };
    Vector3 alignVec = { 0.0f, 0.0f, 0.0f };
    Vector3 cohesionVec = { 0.0f, 0.0f, 0.0f };
    int closeCount = 0, alignCount = 0, cohesionCount = 0;

    for (int i = 0; i < boidNum; ++i) {
        if (&boids[i] != this) {
            float dist = Vector3Distance(boids[i].pos, pos);
            if (dist < protectedRange) {
                closeVec = Vector3Add(closeVec, Vector3Subtract(pos, boids[i].pos));
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
    if (cohesionCount > 0) cohesionVec = Vector3Scale(Vector3Subtract(Vector3Scale(cohesionVec, 1.0f / cohesionCount), pos), 1.0f);

    vel = Vector3Add(vel, Vector3Scale(closeVec, avoidFactor));
    vel = Vector3Add(vel, Vector3Scale(alignVec, matchingFactor));
    vel = Vector3Add(vel, Vector3Scale(cohesionVec, centeringFactor));

    if (pos.x < -mapSize || pos.x > mapSize) vel.x += (pos.x < -mapSize) ? turnFactor : -turnFactor;
    if (pos.y < -mapSize || pos.y > mapSize) vel.y += (pos.y < -mapSize) ? turnFactor : -turnFactor;
    if (pos.z < -mapSize || pos.z > mapSize) vel.z += (pos.z < -mapSize) ? turnFactor : -turnFactor;

    float speed = Vector3Length(vel);
    if (speed > maxspeed) vel = Vector3Scale(vel, maxspeed / speed);
    else if (speed < minspeed) vel = Vector3Scale(vel, minspeed / speed);

    pos = Vector3Add(pos, vel);
}

void Boid::DrawBoid(const Camera3D& camera) {
    Vector3 forward = Vector3Normalize(vel);
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, { 0.0f, 1.0f, 0.0f }));
    Vector3 up = Vector3CrossProduct(right, forward);

    float pyramidHeight = 6.0f;
    float pyramidBaseRadius = 2.0f;
    int numSegments = 36;

    Vector3 apex = Vector3Add(pos, Vector3Scale(forward, pyramidHeight));
    Vector3 baseCenter = pos;

    Color color = { 100, 100, 100, 255 };

    for (int i = 0; i < numSegments; i++) {
        float angle1 = 2.0f * PI * static_cast<float>(i) / static_cast<float>(numSegments);
        float angle2 = 2.0f * PI * static_cast<float>(i + 1) / static_cast<float>(numSegments);

        Vector3 point1 = Vector3Add(baseCenter, Vector3Scale(right, pyramidBaseRadius * cosf(angle1)));
        point1 = Vector3Add(point1, Vector3Scale(up, pyramidBaseRadius * sinf(angle1)));

        Vector3 point2 = Vector3Add(baseCenter, Vector3Scale(right, pyramidBaseRadius * cosf(angle2)));
        point2 = Vector3Add(point2, Vector3Scale(up, pyramidBaseRadius * sinf(angle2)));

        DrawTriangle3D(point1, point2, apex, color);
    }

    for (int i = 0; i < numSegments; i++) {
        float angle1 = 2.0f * PI * i / static_cast<float>(numSegments);
        float angle2 = 2.0f * PI * (i + 1) / static_cast<float>(numSegments);

        Vector3 point1 = Vector3Add(baseCenter, Vector3Scale(right, pyramidBaseRadius * cosf(angle1)));
        point1 = Vector3Add(point1, Vector3Scale(up, pyramidBaseRadius * sinf(angle1)));

        Vector3 point2 = Vector3Add(baseCenter, Vector3Scale(right, pyramidBaseRadius * cosf(angle2)));
        point2 = Vector3Add(point2, Vector3Scale(up, pyramidBaseRadius * sinf(angle2)));

        DrawTriangle3D(baseCenter, point2, point1, color);
    }
}