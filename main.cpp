#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "boid.h"
#include <cmath>

float visualRange = 60.0f;
float protectedRange = 10.0f;
float minspeed = 2.0f;
float maxspeed = 3.0f;
float turnFactor = 0.2f;
float centeringFactor = 0.0005f;
float avoidFactor = 0.15f;
float matchingFactor = 0.6f;

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "3D Birds Simulation");
    SetTargetFPS(60);

    Boid boids[boidNum];

    Camera3D camera = { 0 };
    camera.position = Vector3{ 0.0f, 200.0f, 400.0f };
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    float cameraAngle = 25.0f;
    float cameraRadius = 800.0f;

    while (!WindowShouldClose()) {
        cameraAngle += 0.05f;
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) cameraRadius -= 2.0f;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) cameraRadius += 2.0f;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) cameraAngle -= 1.0f;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) cameraAngle += 1.0f;
        camera.position.x = cameraRadius * sinf(cameraAngle * DEG2RAD);
        camera.position.z = cameraRadius * cosf(cameraAngle * DEG2RAD);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        GuiSliderBar({ static_cast<float>(GetScreenWidth() - 240), 20, 200, 30 }, "Visual Range", TextFormat("%.2f", visualRange), &visualRange, 40.0f, 100.0f);
        GuiSliderBar({ static_cast<float>(GetScreenWidth() - 240), 60, 200, 30 }, "Protected Range", TextFormat("%.2f", protectedRange), &protectedRange, .0f, 60.0f);

        GuiSliderBar({ static_cast<float>(GetScreenWidth() - 240), 120, 200, 30 }, "Coherence", TextFormat("%.2f", centeringFactor * 1000), &centeringFactor, .0f, .001f);
        GuiSliderBar({ static_cast<float>(GetScreenWidth() - 240), 160, 200, 30 }, "Separation", TextFormat("%.2f", avoidFactor), &avoidFactor, .0f, 1.0f);
        GuiSliderBar({ static_cast<float>(GetScreenWidth() - 240), 200, 200, 30 }, "Alignment", TextFormat("%.2f", matchingFactor), &matchingFactor, .0f, 1.0f);

        GuiSliderBar({ static_cast<float>(GetScreenWidth() - 240), 260, 200, 30 }, "Minspeed", TextFormat("%.2f", minspeed), &minspeed, .0f, 10.0f);
        GuiSliderBar({ static_cast<float>(GetScreenWidth() - 240), 300, 200, 30 }, "Maxspeed", TextFormat("%.2f", maxspeed), &maxspeed, .0f, 10.0f);

        BeginMode3D(camera);
        DrawGrid(20, 50.0f);
        for (int i = 0; i < boidNum; ++i) {
            boids[i].UpdatePosition(boids);
            boids[i].DrawBoid(camera);
        }
        EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
