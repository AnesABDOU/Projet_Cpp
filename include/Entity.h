// Updated Entity.h
#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "raymath.h"

class Entity {
public:
    Entity() : _position({0, 0}), _speed(0) {}

    Entity(Vector2 startPos, float speed) : _position(startPos), _speed(speed) {}

    virtual ~Entity() = default;

    virtual void move(const Vector2 &delta) {
        _position = Vector2Add(_position, delta);
    }

    virtual void move(float x, float y) {
        move({x, y});
    }

    Vector2 getPosition() const {
        return _position;
    }

    float getSpeed() const {
        return _speed;
    }

    void setPosition(const Vector2 &position) {
        _position = position;
    }

    void setSpeed(float speed) {
        _speed = speed;
    }

protected:
    Vector2 _position;
    float _speed;
};

#endif // ENTITY_H
