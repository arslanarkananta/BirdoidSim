#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "boid.h"
#include <math.h>

float visualRange = DEF_VISUAL_RANGE;
float protectedRange = DEF_PROTECTED_RANGE;
float minspeed = DEF_MIN_SPEED;
float maxspeed = DEF_MAX_SPEED;
float cohesionFactor = DEF_COHESION;
float avoidFactor = DEF_AVOID;
float alignFactor = DEF_ALIGN;

Vector3 OldPos[TRAIL_LENGTH][500];
bool showTrail = true;
float speedfactor = 1.0f;

void ShiftTrailPositions(int boidIndex, Vector3 newPos) {
    for (int i = TRAIL_LENGTH - 1; i > 0; i--) {
        OldPos[i][boidIndex] = OldPos[i-1][boidIndex];
    }
    OldPos[0][boidIndex] = newPos;
}

void updateTrailPositions(Boid* boids, Camera3D camera) {
    static int frameCount = 0;
    frameCount++;
    for (int i = 0; i < BOID_NUM; ++i) {
        if (frameCount % TRAIL_UPDATE_RATE == 0) {
            ShiftTrailPositions(i, getBoidPosition(&boids[i]));
        }
    }
}

void drawTrail(void) {
    if (!showTrail) return;
    for (int i = 0; i < BOID_NUM; ++i) {
        for (int j = 1; j < TRAIL_LENGTH; ++j) {
            Color trailColor = (Color){ 0, 0, 255, (unsigned char)(100 * (1.0f - (j / (float)TRAIL_LENGTH))) };
            DrawLine3D(OldPos[j-1][i], OldPos[j][i], trailColor);
        }
    }
}

void resetParameters(void) {
    visualRange = DEF_VISUAL_RANGE;
    protectedRange = DEF_PROTECTED_RANGE;
    cohesionFactor = DEF_COHESION;
    avoidFactor = DEF_AVOID;
    alignFactor = DEF_ALIGN;
    speedfactor = 1.0f;
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BirdoidSim");
    SetTargetFPS(60);

    Boid* boids = malloc(sizeof(Boid) * BOID_NUM);
    
    for (int i = 0; i < BOID_NUM; i++) {
        initBoid(&boids[i]);
        Vector3 initialPos = getBoidPosition(&boids[i]);
        for (int j = 0; j < TRAIL_LENGTH; j++) {
            OldPos[j][i] = initialPos;
        }
    }

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 200.0f, 400.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    float cameraAngle = 25.0f;
    float cameraRadius = 800.0f;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) showTrail = !showTrail;
        minspeed = DEF_MIN_SPEED * speedfactor;
        maxspeed = DEF_MAX_SPEED * speedfactor;
        
        cameraAngle += 0.05f;
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) cameraRadius -= 2.0f;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) cameraRadius += 2.0f;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) cameraAngle -= 1.0f;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) cameraAngle += 1.0f;
        camera.position.x = cameraRadius * sinf(cameraAngle * DEG2RAD);
        camera.position.z = cameraRadius * cosf(cameraAngle * DEG2RAD);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawGrid(20, 50.0f);
        for (int i = 0; i < BOID_NUM; ++i) {
            updateBoidPosition(&boids[i], boids);
            drawBoid(&boids[i], &camera);
        }
        updateTrailPositions(boids, camera);
        drawTrail();
        EndMode3D();
        
        DrawRectangle(5, 5, 325, 80, Fade(LIGHTGRAY, 0.3f));
        DrawText("Use WASD to move the camera", 20, 20, 10, DARKGRAY);
        DrawText("Press Space to toggle the trails", 20, 40, 10, DARKGRAY);
        DrawText("Control the parameters with the sliders below", 20, 60, 10, DARKGRAY);
        
        DrawRectangle(GetScreenWidth() - 320, 5, 315, 300, Fade(LIGHTGRAY, 0.3f));
        if (GuiButton((Rectangle){ GetScreenWidth() - 300, 20, 280, 30 }, "Reset")) resetParameters();
        GuiSliderBar((Rectangle){ GetScreenWidth() - 250, 60, 200, 30 }, "Visual", TextFormat("%.2f", visualRange), &visualRange, 40.0f, 100.0f);
        GuiSliderBar((Rectangle){ GetScreenWidth() - 250, 100, 200, 30 }, "Protected", TextFormat("%.2f", protectedRange), &protectedRange, 0.0f, 60.0f);
        GuiSliderBar((Rectangle){ GetScreenWidth() - 250, 140, 200, 30 }, "Coherence", TextFormat("%.2f", cohesionFactor * 1000), &cohesionFactor, 0.0f, 0.001f);
        GuiSliderBar((Rectangle){ GetScreenWidth() - 250, 180, 200, 30 }, "Separation", TextFormat("%.2f", avoidFactor), &avoidFactor, 0.0f, 1.0f);
        GuiSliderBar((Rectangle){ GetScreenWidth() - 250, 220, 200, 30 }, "Alignment", TextFormat("%.2f", alignFactor), &alignFactor, 0.0f, 1.0f);
        GuiSliderBar((Rectangle){ GetScreenWidth() - 250, 260, 200, 30 }, "Speed", TextFormat("%.2f", speedfactor), &speedfactor, 0.0f, 2.0f);
        EndDrawing();
    }

    free(boids);
    CloseWindow();
    return 0;
}