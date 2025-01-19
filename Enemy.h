// Enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "raylib.h"

class Enemy : public Entity {
public:
    Enemy() : Entity({0, 0}, 0), _isAlive(true) {}

    Enemy(Vector2 startPos, float speed) : Entity(startPos, speed), _isAlive(true) {}

    // Check if the player jumps on the enemy
    void checkCollisionWithPlayer(const Vector2 &playerPosition, float playerJumpHeight) {
        // If the player lands on top of the enemy
        if (_isAlive &&
            playerPosition.x > _position.x - _size / 2 &&
            playerPosition.x < _position.x + _size / 2 &&
            playerPosition.y < _position.y &&
            playerPosition.y > _position.y - playerJumpHeight) {
            _isAlive = false;
        }
    }

    bool isAlive() const {
        return _isAlive;
    }

    void setSize(float size) {
        _size = size;
    }

    float getSize() const {
        return _size;
    }
    bool wasAliveLastFrame() const{
        return _wasAliveLastFrame;
    }
    void setWasAliveLastFrame(bool state) {
        _wasAliveLastFrame = state;
    }
private:
    bool _isAlive;
    float _size = 50.0f; // Default size of the enemy
    bool _wasAliveLastFrame; // Variable pour suivre l'état précédent de l'ennemi

};

#endif // ENEMY_H
