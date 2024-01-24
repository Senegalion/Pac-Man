#pragma once

#include "Ghost.h"
#include "Blinky.h"

class Inky : public Ghost {
    Q_OBJECT
public:
    Inky(int startX, int startY, Player* player, Blinky* blinky, const std::vector<std::string>& mapSketch);
    virtual ~Inky();

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

    Blinky* blinky;
    QPointF calculateVector(int x1, int y1, int x2, int y2);
    bool isCorrectTimeToRelease = false;
    void checkTimeToRelease();
    int iterator = 0;
    int wasPacManEaten = 0;
};
