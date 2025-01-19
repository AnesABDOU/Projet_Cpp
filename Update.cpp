#include "Update.h"
#include "raylib.h"
#include "raymath.h"
#include <list>

bool wasPressed;

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta) {
    if (IsKeyDown(KEY_LEFT)|| IsKeyDown(KEY_A)) player->move(-PLAYER_HORIZONTAL_SPEED * delta, 0);
    if (IsKeyDown(KEY_RIGHT)|| IsKeyDown(KEY_D)) player->move(PLAYER_HORIZONTAL_SPEED * delta, 0);
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

#include "Score.h"

void UpdateEnemies(std::list<Enemy>& enemies, Player* player, Score& score) {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemy.checkCollisionWithPlayer(player->getPosition(), 50.0f);
        } else if (enemy.wasAliveLastFrame()) {
            score.addScoreForEnemy(); // Appel direct à la méthode addScoreForEnemy
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
