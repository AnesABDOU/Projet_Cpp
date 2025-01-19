#ifndef CAMERA_H
#define CAMERA_H
#include "raylib.h"
#include "Settings.h"


namespace MS {
    class Camera {
    public:
        Camera(){}

        Camera(Vector2 target) {
            _camera2D.target = target;
            _camera2D.offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
            _camera2D.rotation = 0.0f;
            _camera2D.zoom = 1.0f;
        }

        void move(float x, float y) {
            _camera2D.target.x += x;
            _camera2D.target.y += y;
        }

        Camera2D getCamera2D() const {
            return _camera2D;
        }

        void setCamera2D(const Camera2D &camera2d) {
            _camera2D = camera2d;
        }

        void setZoom(float zoom) {
            _camera2D.zoom = zoom;
        }

        float getZoom() const {
            return _camera2D.zoom;
        }

        Vector2 getOffset() const {
            return _camera2D.offset;
        }

        void setOffset(Vector2 offset) {
            _camera2D.offset = offset;
        }

        void setOffset(float x, float y) {
            setOffset({x, y});
        }

        void setTarget(Vector2 target) {
            _camera2D.target = target;
        }

        void setTarget(float x, float y) {
            setTarget({x, y});
        }

        Vector2 getTarget() {
            return _camera2D.target;
        }

    private:
        Camera2D _camera2D = {};
    };
}


#endif //CAMERA_H
