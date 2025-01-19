//
// Created by aigle on 08/12/2024.
//

#ifndef UPDATE_H
#define UPDATE_H
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <list>
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "Settings.h"
#include "Score.h"

typedef struct EnvItem {
    Rectangle rect;
    int blocking;
    Color color;
} EnvItem;

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);
void UpdateEnemies(std::list<Enemy>& enemies, Player* player, Score& score);
void UpdateCameraCenter(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterInsideMap(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterSmoothFollow(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraEvenOutOnLanding(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraPlayerBoundsPush(MS::Camera *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);

#endif // UPDATE_H


