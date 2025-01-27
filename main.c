#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "boid.h"
#include <math.h>

float visualRange = 60.0f;
float protectedRange = 10.0f;
float minspeed = 2.0f;
float maxspeed = 3.0f;
float turnFactor = 0.2f;
float centeringFactor = 0.0005f;
float avoidFactor = 0.15f;
float matchingFactor = 0.6f;
Vector3 OldPos[TRAIL_LENGTH][500];

void ShiftTrailPositions(int boidIndex, Vector3 newPos) {
    for (int i = TRAIL_LENGTH - 1; i > 0; i--) {
        OldPos[i][boidIndex] = OldPos[i-1][boidIndex];
    }
    OldPos[0][boidIndex] = newPos;
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BirdoidSim");
    SetTargetFPS(60);

    Boid* boids = malloc(sizeof(Boid) * BOID_NUM);
    int frameCount = 0;
    
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
        frameCount++;
        
        cameraAngle += 0.05f;
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) cameraRadius -= 2.0f;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) cameraRadius += 2.0f;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) cameraAngle -= 1.0f;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) cameraAngle += 1.0f;
        camera.position.x = cameraRadius * sinf(cameraAngle * DEG2RAD);
        camera.position.z = cameraRadius * cosf(cameraAngle * DEG2RAD);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        GuiSliderBar((Rectangle){ GetScreenWidth() - 240, 20, 200, 30 }, "Visual Range", TextFormat("%.2f", visualRange), &visualRange, 40.0f, 100.0f);
        GuiSliderBar((Rectangle){ GetScreenWidth() - 240, 60, 200, 30 }, "Protected Range", TextFormat("%.2f", protectedRange), &protectedRange, 0.0f, 60.0f);
        GuiSliderBar((Rectangle){ GetScreenWidth() - 240, 120, 200, 30 }, "Coherence", TextFormat("%.2f", centeringFactor * 1000), &centeringFactor, 0.0f, 0.001f);
        GuiSliderBar((Rectangle){ GetScreenWidth() - 240, 160, 200, 30 }, "Separation", TextFormat("%.2f", avoidFactor), &avoidFactor, 0.0f, 1.0f);
        GuiSliderBar((Rectangle){ GetScreenWidth() - 240, 200, 200, 30 }, "Alignment", TextFormat("%.2f", matchingFactor), &matchingFactor, 0.0f, 1.0f);
        GuiSliderBar((Rectangle){ GetScreenWidth() - 240, 260, 200, 30 }, "Minspeed", TextFormat("%.2f", minspeed), &minspeed, 0.0f, 10.0f);
        GuiSliderBar((Rectangle){ GetScreenWidth() - 240, 300, 200, 30 }, "Maxspeed", TextFormat("%.2f", maxspeed), &maxspeed, 0.0f, 10.0f);

        BeginMode3D(camera);
        DrawGrid(20, 50.0f);
        
        for (int i = 0; i < BOID_NUM; ++i) {
            updateBoidPosition(&boids[i], boids);
            drawBoid(&boids[i], &camera);
            
            if (frameCount % TRAIL_UPDATE_RATE == 0) {
                ShiftTrailPositions(i, getBoidPosition(&boids[i]));
            }
            
            for (int j = 1; j < TRAIL_LENGTH; ++j) {
                Color trailColor = (Color){ 0, 0, 255, (unsigned char)(100 * (1.0f - (j / (float)TRAIL_LENGTH))) };
                DrawLine3D(OldPos[j-1][i], OldPos[j][i], trailColor);
            }
        }
        
        EndMode3D();
        EndDrawing();
    }

    free(boids);
    CloseWindow();
    return 0;
}