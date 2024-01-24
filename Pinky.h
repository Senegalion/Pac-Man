#pragma once

#include "Ghost.h"

class Pinky : public Ghost {
    Q_OBJECT
public:
    Pinky(int startX, int startY, Player* player, const std::vector<std::string>& mapSketch);
    virtual ~Pinky();

    virtual void moveGhost() override;

signals:
    void pacManEaten();

public slots:
    void startGame();
    void scatterMode();
    void chaseMode();
    void switchToChaseMode();
    void switchToScatterMode();
    void updateFrightenedDuration();
    void onEnergizerEaten();
    void onPacManEaten();

private:
    void startGhostMovement();
    void stopGhostMovement();
    int ghostWasEaten = 0;
    bool isCorrectTimeToRelease = false;
    void checkTimeToRelease();
    int iterator = 0;
    int wasPacManEaten = 0;
};
