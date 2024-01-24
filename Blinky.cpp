#include "Blinky.h"
#include <QRandomGenerator>
#include "Player.h"
#include "math.h"

Blinky::Blinky(int startX, int startY, Player* player, const std::vector<std::string>& mapSketch)
    : Ghost(startX, startY, player, mapSketch, "/home/lucas/Desktop/Pac-Man/r1B.png")
{
    scatterMode();

    frightenedTimer = new QTimer(this);
    connect(frightenedTimer, &QTimer::timeout, this, &Blinky::updateFrightenedDuration);
    connect(player, &Player::energizerEaten, this, &Blinky::onEnergizerEaten);

    connect(this, &Blinky::pacManEaten, this, &Blinky::onPacManEaten);
}

Blinky::~Blinky(){}

void Blinky::startGame()
{
    scatterMode();
}

void Blinky::scatterMode()
{
    currentMode = GhostMode::Scatter;
    setTarget(360, 20);
    QTimer::singleShot(7 * 1000, this, &Blinky::switchToChaseMode);
    qDebug() << "Scatter mode: Blinky";
    startGhostMovement();
}

void Blinky::chaseMode()
{
    currentMode = GhostMode::Chase;
    setTarget(player->getXCoordinate(), player->getYCoordinate());
    QTimer::singleShot(20 * 1000, this, &Blinky::switchToScatterMode);
    qDebug() << "Chase mode: Blinky";
    startGhostMovement();
}

void Blinky::switchToChaseMode()
{
    stopGhostMovement();
    chaseMode();
}

void Blinky::switchToScatterMode()
{
    stopGhostMovement();
    scatterMode();
}

void Blinky::updateFrightenedDuration()
{
    isNormalMode = 0;
    if (frightenedDuration > 0) {
        frightenedDuration -= 1000;
    } else {
        switchToNormalMode();
    }
}

void Blinky::onEnergizerEaten()
{
    switchToFrightenedMode();
}

void Blinky::startGhostMovement()
{
    moveTimer->start(500);
}

void Blinky::stopGhostMovement()
{
    moveTimer->stop();
}

void Blinky::onPacManEaten()
{
    resetToInitialPosition(200, 140);
    emit pacManEaten();
}

void Blinky::moveGhost()
{
    int currentX = xCoordinate;
    int currentY = yCoordinate;

    if (currentMode == GhostMode::Scatter) {
        setTarget(360, 20);
    } else {
        setTarget(player->getXCoordinate(), player->getYCoordinate());
    }

    int dxW = currentX;
    int dyW = currentY - cellSide;
    double distanceW = sqrt(pow(dxW - targetX, 2) + pow(dyW - targetY, 2));

    int dxS = currentX;
    int dyS = currentY + cellSide;
    double distanceS = sqrt(pow(dxS - targetX, 2) + pow(dyS - targetY, 2));

    int dxA = currentX - cellSide;
    int dyA = currentY;
    double distanceA = sqrt(pow(dxA - targetX, 2) + pow(dyA - targetY, 2));

    int dxD = currentX + cellSide;
    int dyD = currentY;
    double distanceD = sqrt(pow(dxD - targetX, 2) + pow(dyD - targetY, 2));

    std::vector<std::pair<int, double>> availableDirections;

    if (canMoveTo(dxW, dyW) && direction != 2) {
        availableDirections.emplace_back(1, distanceW);
    }
    if (canMoveTo(dxS, dyS) && direction != 1) {
        availableDirections.emplace_back(2, distanceS);
    }
    if (canMoveTo(dxA, dyA) && direction != 4) {
        availableDirections.emplace_back(3, distanceA);
    }
    if (canMoveTo(dxD, dyD) && direction != 3) {
        availableDirections.emplace_back(4, distanceD);
    }

    if (!availableDirections.empty()) {
        std::sort(availableDirections.begin(), availableDirections.end(),
                  [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
                      return a.second < b.second;
                  });

        int chosenDirection = availableDirections[0].first;

        int chosenDirectionX, chosenDirectionY;

        switch (chosenDirection) {
        case 1:
            chosenDirectionX = dxW;
            chosenDirectionY = dyW;
            break;
        case 2:
            chosenDirectionX = dxS;
            chosenDirectionY = dyS;
            break;
        case 3:
            chosenDirectionX = dxA;
            chosenDirectionY = dyA;
            break;
        case 4:
            chosenDirectionX = dxD;
            chosenDirectionY = dyD;
            break;
        default:
            // Invalid direction, should not happen
            return;
        }

        if (currentX == player->getXCoordinate() && currentY == player->getYCoordinate())
        {
            if (currentMode == GhostMode::Frightened) {
                player->increaseScore(100);
                xCoordinate = 200;
                yCoordinate = 140;
                if (direction == 1) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/u1B.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 2) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/d1B.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 3) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/l1B.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/r1B.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                }
                setPos(xCoordinate, yCoordinate);
                currentMode = GhostMode::Normal;
                isNormalMode = 1;
                ghostWasEaten = 1;
                return;
            } else {
                player->decreaseLives();
                if (direction == 1) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/u1B.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 2) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/d1B.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 3) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/l1B.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/r1B.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                }
                player->setXCoordinate(200);
                player->setYCoordinate(300);
                // emit pacManEaten();
                return;
            }
        }

        if (isNormalMode) {
            ghostWasEaten = 0;
        }

        if (direction == 3 && !canMoveTo(dxA, dyA) && !canMoveTo(dxW, dyW) && !canMoveTo(dxS, dyS)) {
            chosenDirection = 3;
            chosenDirectionX = 400;
        } else if (direction == 4 && !canMoveTo(dxD, dyD) && !canMoveTo(dxW, dyW) && !canMoveTo(dxS, dyS)) {
            chosenDirection = 4;
            chosenDirectionX = 0;
        }

        direction = chosenDirection;
        xCoordinate = chosenDirectionX;
        yCoordinate = chosenDirectionY;

        if (!isNormalMode && !ghostWasEaten) {
            QPixmap pixmap("/home/lucas/Desktop/Pac-Man/mortal.png");
            setPixmap(pixmap.scaled(cellSide, cellSide));
        } else {
            if (direction == 1) {
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/u1B.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            } else if (direction == 2) {
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/d1B.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            } else if (direction == 3) {
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/l1B.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            } else {
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/r1B.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            }
        }
        setPos(xCoordinate, yCoordinate);
    }
}
