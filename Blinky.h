#pragma once

#include "Ghost.h"

class Blinky : public Ghost {
    Q_OBJECT
public:
    Blinky(int startX, int startY, Player* player, const std::vector<std::string>& mapSketch);
    virtual ~Blinky();

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
};
