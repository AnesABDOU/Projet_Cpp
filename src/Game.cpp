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

#include <list>

typedef struct EnvItem {
    Rectangle rect;
    int blocking;
    Color color;
} EnvItem;

Player *player;
MS::Camera camera;
std::list<Enemy> enemies;
Score score;

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);

void UpdateEnemies(std::list<Enemy>& enemies, Player *player);

void UpdateCameraCenter(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta,
                        int width, int height);

void UpdateCameraCenterInsideMap(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta,
                                 int width, int height);

void UpdateCameraCenterSmoothFollow(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength,
                                    float delta, int width, int height);

void UpdateCameraEvenOutOnLanding(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength,
                                  float delta, int width, int height);

void UpdateCameraPlayerBoundsPush(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength,
                                  float delta, int width, int height);

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mario Shab");

    //auto *player = new Player({400, 280});
    //moi:
    auto *player = new Player({8000, -1000}); //standard spawn 0,0
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

        UpdateEnemies(enemies, player);

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

bool wasPressed;

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta) {
    if (IsKeyDown(KEY_LEFT)|| IsKeyDown(KEY_A)) player->move(-PLAYER_HORIZONTAL_SPEED * delta, 0);
    if (IsKeyDown(KEY_RIGHT)|| IsKeyDown(KEY_D)) player->move(PLAYER_HORIZONTAL_SPEED * delta, 0);
    //ma modification pour rajouter le "courir":
    if ((IsKeyDown(KEY_LEFT)|| IsKeyDown(KEY_A))&& IsKeyDown(KEY_LEFT_SHIFT)) player->move(-1.5*PLAYER_HORIZONTAL_SPEED * delta, 0);
    if ((IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D))&& IsKeyDown(KEY_LEFT_SHIFT)) player->move(1.5*PLAYER_HORIZONTAL_SPEED * delta, 0);
    if ((IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_UP)) && player->canJump()) {
        wasPressed = true;
        if (player->getJumpCounter() < 10) {
            player->accelerate(-PLAYER_VERTICAL_SPEED / 10);
            player->increaseJumpCounter();
        } else if (player->getSpeed() < 1 && player->getSpeed() >= 0) {
            player->resetJumpCounter();
        }
    } else {
        player->resetJumpCounter();
    }

    if ((IsKeyReleased(KEY_SPACE) || IsKeyReleased(KEY_UP)) && wasPressed) {
        player->setCanJump(false);
    }

    bool hitObstacle = false;
    for (int i = 0; i < envItemsLength; i++) {
        EnvItem *ei = envItems + i;
        Vector2 p = player->getPosition();
        if (ei->blocking &&
            ei->rect.x <= p.x &&
            ei->rect.x + ei->rect.width >= p.x &&
            ei->rect.y >= p.y &&
            ei->rect.y <= p.y + player->getSpeed() * delta) {
            hitObstacle = true;
            player->setSpeed(0.0f);
            p.y = ei->rect.y;
            break;
        }
        if(ei->rect.x >= 8000 && ei->rect.y >= -1100 && IsKeyDown(KEY_E)){
            player->setSpeed(0);
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("T'as battu La MAIN4 c'est fini", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, 20, WHITE);
            EndDrawing();
        }
    }

    if (!hitObstacle) {
        player->move(0, player->getSpeed() * delta);
        player->accelerate(G * delta);
    } else {
        player->setCanJump(true);
    }
}


void UpdateEnemies(std::list<Enemy>& enemies, Player* player) {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemy.checkCollisionWithPlayer(player->getPosition(), 50.0f);
        } else if (enemy.wasAliveLastFrame()) {
            score.addScoreForEnemy();
            enemy.setWasAliveLastFrame(false);
        }
    }
}



void UpdateCameraCenter(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta,
                        int width, int height) {
    camera->setOffset(width / 2.0f, height / 2.0f);
    camera->setTarget(player->getPosition());
}

void UpdateCameraCenterInsideMap(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta,
                                 int width, int height) {
    camera->setTarget(player->getPosition());
    camera->setOffset(width / 2.0f, height / 2.0f);
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

    for (int i = 0; i < envItemsLength; i++) {
        EnvItem *ei = envItems + i;
        minX = fminf(ei->rect.x, minX);
        maxX = fmaxf(ei->rect.x + ei->rect.width, maxX);
        minY = fminf(ei->rect.y, minY);
        maxY = fmaxf(ei->rect.y + ei->rect.height, maxY);
    }

    Vector2 max = GetWorldToScreen2D((Vector2) {maxX, maxY}, camera->getCamera2D());
    Vector2 min = GetWorldToScreen2D((Vector2) {minX, minY}, camera->getCamera2D());

    if (max.x < width) camera->setOffset(width - (max.x - width / 2), 0);
    if (max.y < height) camera->setOffset(0, height - (max.y - height / 2));
    if (min.x > 0) camera->setOffset(width / 2 - min.x, 0);
    if (min.y > 0) camera->setOffset(0, height / 2 - min.y);
}

void UpdateCameraCenterSmoothFollow(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength,
                                    float delta, int width, int height) {
    static float minSpeed = 30;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;

    camera->setOffset((Vector2) {width / 2.0f, height / 2.0f});
    Vector2 diff = Vector2Subtract(player->getPosition(), camera->getTarget());
    float length = Vector2Length(diff);

    if (length > minEffectLength) {
        float speed = fmaxf(fractionSpeed * length, minSpeed);
        camera->setTarget(Vector2Add(camera->getTarget(), Vector2Scale(diff, speed * delta / length)));
    }
}

void UpdateCameraEvenOutOnLanding(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength,
                                  float delta, int width, int height) {
    static float evenOutSpeed = 700;
    static int eveningOut = false;
    static float evenOutTarget;

    camera->setOffset((Vector2) {width / 2.0f, height / 2.0f});
    camera->setTarget((Vector2) {player->getPosition().x, 0});

    if (eveningOut) {
        if (evenOutTarget > camera->getTarget().y) {
            camera->move(0, evenOutSpeed * delta);

            if (camera->getTarget().y > evenOutTarget) {
                camera->setTarget(0, evenOutTarget);
                eveningOut = 0;
            }
        } else {
            camera->move(0, evenOutSpeed * delta);

            if (camera->getTarget().y < evenOutTarget) {
                camera->setTarget(0, evenOutTarget);
                eveningOut = 0;
            }
        }
    } else {
        if (player->canJump() && (player->getSpeed() == 0) && (player->getPosition().y != camera->getTarget().y)) {
            eveningOut = 1;
            evenOutTarget = player->getPosition().y;
        }
    }
}

void UpdateCameraPlayerBoundsPush(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength,
                                  float delta, int width, int height) {
    static Vector2 bbox = {0.2f, 0.2f};

    Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2) {(1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height},
                                              camera->getCamera2D());
    Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2) {(1 + bbox.x) * 0.5f * width, (1 + bbox.y) * 0.5f * height},
                                              camera->getCamera2D());
    camera->setOffset((Vector2) {(1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height});

    if (player->getPosition().x < bboxWorldMin.x) camera->setTarget(player->getPosition().x, 0);
    if (player->getPosition().y < bboxWorldMin.y) camera->setTarget(0, player->getPosition().y);
    if (player->getPosition().x > bboxWorldMax.x)
        camera->setTarget(
                bboxWorldMin.x + (player->getPosition().x - bboxWorldMax.x), 0);
    if (player->getPosition().y > bboxWorldMax.y)
        camera->setTarget(
                0, bboxWorldMin.y + (player->getPosition().y - bboxWorldMax.y));
}
