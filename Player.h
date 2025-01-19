// Updated Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "raylib.h"
#include "raymath.h"

class Player : public Entity {
public:
    Player() : Entity({0, 0}, 0), _canJump(false), _jumpCounter(0) {}

    Player(Vector2 startPos) : Entity(startPos, 0), _canJump(false), _jumpCounter(0) {}

    void accelerate(float speed) {
        _speed += speed;
    }

    bool canJump() const {
        return _canJump;
    }

    unsigned int getJumpCounter() const {
        return _jumpCounter;
    }

    void setCanJump(bool canJump) {
        _canJump = canJump;
    }

    void increaseJumpCounter() {
        _jumpCounter++;
    }

    void resetJumpCounter() {
        _jumpCounter = 0;
    }

private:
    bool _canJump;
    unsigned int _jumpCounter;
};

#endif // PLAYER_H
