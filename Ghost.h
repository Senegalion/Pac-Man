#pragma once

#include <QTimer>
#include <vector>
#include <QGraphicsPixmapItem>
#include "Player.h"

enum class Direction { Up, Down, Left, Right };
enum class GhostMode { Normal, Frightened, Scatter, Chase };

class Ghost : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Ghost(int startX, int startY, Player* player, const std::vector<std::string>& mapSketch, const QString& imagePath);
    virtual ~Ghost();
    int getXCoordinate() const;
    int getYCoordinate() const;

public slots:
    virtual void moveGhost() = 0;
    void switchToFrightenedMode();
    void switchToNormalMode();

protected:
    void setTarget(int x, int y);
    bool canMoveTo(int x, int y);

    int xCoordinate;
    int yCoordinate;
    int targetX;
    int targetY;
    int cellSide;
    int direction;
    GhostMode currentMode;
    bool isGateOpen;
    QTimer* moveTimer;
    Player* player;
    const std::vector<std::string>& mapSketch;

    QTimer* frightenedTimer;
    int frightenedDuration;
    bool isNormalMode = 1;

    void resetToInitialPosition(int coordinateX, int coordinateY);
};
