/*******************************************************************************************
*
*   raylib [core] example - 2D Camera platformer
*
*   Example originally created with raylib 2.5, last time updated with raylib 3.0
*
*   Example contributed by arvyy (@arvyy) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 arvyy (@arvyy)
*
********************************************************************************************/

#include <iostream>
#include "Camera.h"
#include "raylib.h"
#include "raymath.h"
#include "Player.h"
#include "Settings.h"
#include "Enemy.h"
#include "Score.h"
#include "Update.h"

#include <list>



Player *player;
MS::Camera camera;
std::list<Enemy> enemies;
Score score;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mario Shab");

    //auto *player = new Player({400, 280});
    //moi:
    auto *player = new Player({0, 0}); //standard spawn 0,0
    camera = MS::Camera(player->getPosition());
    enemies = {
            Enemy({3600, -150}, 0),
            Enemy({8700, -700}, 0)
    };

    //int envItemsLength = std::size(envItems);
    EnvItem envItems[] = {
            /*{{0,   400,   1000, 400}, 1, GREEN},
            {{300, 200, 400,  10},  1, RED},
            {{250, 300, 100,  10},  1, GRAY},
            {{650, 300, 100,  10},  1, GRAY}*/
            //ma map1::
            {{-1000,   400,   3000, 1900}, 1, GREEN},
            {{300, 200, 400,  10},  1, RED},
            {{2000+100, 400, 1000,  1900},  1, GREEN},
            {{3000+100+100, 300, 100,  10},  1, GRAY},
            {{3000+100+100+100, 200, 100,  10},  1, GRAY},
            {{3000+100+100, 100, 100,  10},  1, GRAY},
            {{3000+100+100+100, 0, 100,  10},  1, GRAY},
            {{3500, -100, 1000,  1900},  1, GREEN},
            {{4500+100+100, -200, 100,  10},  1, GRAY},
            {{4500+600, -250, 100,  10},  1, GRAY},
            {{5500, -275, 1000,  1900},  1, GREEN},
            {{5500, -1000, 1000,  600},  1, GREEN},
            {{7000, -100, 100,  10},  1, GRAY},
            {{7350, -225, 100,  10},  1, GRAY},
            {{7800, -400, 1000,  1900},  1, GREEN},
            {{7800, -1000, 700,  550},  1, GREEN},
            {{7700, -300, 100,  10},  1, LIGHTGRAY},
            //BLOCS INVISIBLES AVANT DE FINIR LE JEU:
            //SI TROP DIFFICILE : CHANGER LIGHTGRAY EN GREEN
            {{8500, -500, 100,  10},  1, LIGHTGRAY},
            {{8700, -600, 100,  10},  1, LIGHTGRAY},
            {{8900, -700, 100,  10},  1, LIGHTGRAY},
            {{8700, -800, 100,  10},  1, LIGHTGRAY},
            {{8900, -900, 100,  10},  1, LIGHTGRAY},

            //PORTE DE FIN DE JEU
            {{8000, -1100, 50, 100},  1, GOLD},
    };
    int envItemsLength = std::size(envItems);
    // Store pointers to the multiple update camera functions
    void (*cameraUpdaters[])(MS::Camera *, Player *, EnvItem *, int, float, int, int) = {
            UpdateCameraCenter,
            UpdateCameraCenterInsideMap,
            UpdateCameraCenterSmoothFollow,
            UpdateCameraEvenOutOnLanding,
            UpdateCameraPlayerBoundsPush
    };

    int cameraOption = 0;
    int cameraUpdatersLength = std::size(cameraUpdaters);

    std::string cameraDescriptions[] = {
            "Follow player center",
            "Follow player center, but clamp to map edges",
            "Follow player center; smoothed",
            "Follow player center horizontally; update player center vertically after landing",
            "Player push camera on getting too close to screen edge"
    };

    SetTargetFPS(60);

#ifdef FULL_SCREEN
    ToggleFullscreen();
#endif

    Vector2 lastPlayerPosition = player->getPosition();
    static float accumulatedDistance = 0;

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        float deltaTime = GetFrameTime();

        UpdatePlayer(player, envItems, envItemsLength, deltaTime);

        float distanceTraveled = Vector2Distance(player->getPosition(), lastPlayerPosition);
        accumulatedDistance += distanceTraveled;
        if (accumulatedDistance >= 10.0f) {
            int points = static_cast<int>(accumulatedDistance / 10) * 100;
            score.addScoreForDistance(points);
            accumulatedDistance = fmod(accumulatedDistance, 10.0f);
        }
        lastPlayerPosition = player->getPosition();

        UpdateEnemies(enemies, player,score);

        camera.setZoom(camera.getZoom() + GetMouseWheelMove() * 0.05f);

        if (camera.getZoom() > 3.0f) camera.setZoom(3.0f);
        else if (camera.getZoom() < 0.25f) camera.setZoom(0.25f);

        if (IsKeyPressed(KEY_R)) {
            camera.setZoom(1.0f);
            player->setPosition({400, 280});
            score.reset();
        }

        if (IsKeyPressed(KEY_C)) cameraOption = (cameraOption + 1) % cameraUpdatersLength;

        // Call update camera function by its pointer
        cameraUpdaters[cameraOption](&camera, player, envItems, envItemsLength, deltaTime, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Draw
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        BeginMode2D(camera.getCamera2D());

        for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);

        Rectangle playerRect = {player->getPosition().x - 20, player->getPosition().y - 40, 40.0f, 40.0f};
        DrawRectangleRec(playerRect, RED);

        for (const auto& enemy : enemies) {
            if (enemy.isAlive()) {
                DrawCircleV(enemy.getPosition(), 20.0f, DARKPURPLE);
            }
        }

        DrawCircleV(player->getPosition(), 5.0f, GOLD);

        EndMode2D();

        score.render();

        DrawText("Controls:", 20, 20, 10, BLACK);
        DrawText("- Right/Left to move", 40, 40, 10, DARKGRAY);
        DrawText("- Space to jump", 40, 60, 10, DARKGRAY);
        DrawText("- Mouse Wheel to Zoom in-out, R to reset zoom", 40, 80, 10, DARKGRAY);
        DrawText("- C to change camera mode", 40, 100, 10, DARKGRAY);
        DrawText("Current camera mode:", 20, 120, 10, BLACK);
        DrawText(cameraDescriptions[cameraOption].c_str(), 40, 140, 10, DARKGRAY);

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}