#ifndef SCORE_H
#define SCORE_H

#include "raylib.h"
#include <string>

class Score {
private:
    int score;
    Vector2 position;  // Position du texte affiché
    int fontSize;      // Taille de la police
    Color color;       // Couleur du texte

public:
    // Constructeur
    Score()
            : score(0), position({10, 10}), fontSize(20), color(WHITE) {}

    // Ajouter des points pour la distance
    void addScoreForDistance(float distance) {
        int points = static_cast<int>(distance / 10) * 100; // 100 points tous les 10 unités
        score += points;
    }

    // Ajouter des points pour un ennemi éliminé
    void addScoreForEnemy() {
        score += 5000; // 5000 points par ennemi
    }

    // Afficher le score à l'écran
    void render() const {
        DrawText(("Score: " + std::to_string(score)).c_str(), position.x, position.y, fontSize, color);
    }

    // Obtenir le score actuel
    int getScore() const {
        return score;
    }

    // Modifier la position du texte
    void setPosition(Vector2 newPosition) {
        position = newPosition;
    }

    // Modifier la taille de la police
    void setFontSize(int newSize) {
        fontSize = newSize;
    }

    // Modifier la couleur du texte
    void setColor(Color newColor) {
        color = newColor;
    }

    void reset() {
        score = 0;
    }
};

#endif // SCORE_H
