#include "Inky.h"
#include <QRandomGenerator>
#include "Player.h"
#include "Blinky.h"
#include "math.h"

Inky::Inky(int startX, int startY, Player* player, Blinky* blinky, const std::vector<std::string>& mapSketch)
    : Ghost(startX, startY, player, mapSketch, "/home/lucas/Desktop/Pac-Man/r1I.png")
{
    isGateOpen = true;
    this->blinky = blinky;
    scatterMode();

    frightenedTimer = new QTimer(this);
    connect(frightenedTimer, &QTimer::timeout, this, &Inky::updateFrightenedDuration);
    connect(player, &Player::energizerEaten, this, &Inky::onEnergizerEaten);
}

Inky::~Inky(){}

void Inky::startGame()
{
    scatterMode();
}

void Inky::scatterMode()
{
    currentMode = GhostMode::Scatter;
    setTarget(360, 380);
    QTimer::singleShot(7 * 1000, this, &Inky::switchToChaseMode);
    qDebug() << "Scatter mode: Inky";
    startGhostMovement();
}

void Inky::chaseMode()
{
    currentMode = GhostMode::Chase;
    setTarget(player->getXCoordinate(), player->getYCoordinate());
    QTimer::singleShot(20 * 1000, this, &Inky::switchToScatterMode);
    qDebug() << "Chase mode: Inky";
    startGhostMovement();
}

void Inky::switchToChaseMode()
{
    stopGhostMovement();

    chaseMode();
}

void Inky::switchToScatterMode()
{
    stopGhostMovement();

    scatterMode();
}

void Inky::updateFrightenedDuration()
{
    if (frightenedDuration > 0) {
        frightenedDuration -= 1000;
    } else {
        switchToNormalMode();  // Switch back to normal mode when duration is over
    }
}

void Inky::onEnergizerEaten()
{
    switchToFrightenedMode();
}

void Inky::startGhostMovement()
{
    moveTimer->start(500);
}

void Inky::stopGhostMovement()
{
    moveTimer->stop();
}

void Inky::onPacManEaten()
{
    resetToInitialPosition(180, 180);
    emit pacManEaten();
}

void Inky::checkTimeToRelease() {
    if (iterator == 28) {
        isCorrectTimeToRelease = true;
        iterator = 0;
    }
    iterator++;
}

void Inky::moveGhost()
{
    checkTimeToRelease();
    if (!isCorrectTimeToRelease) {
        return;
    }
    if (isGateOpen && isCorrectTimeToRelease) {
        if (xCoordinate == 9 * cellSide && yCoordinate == 9 * cellSide) {
            direction = 4;
            xCoordinate += cellSide;
        } else if (xCoordinate == 10 * cellSide && (yCoordinate == 9 * cellSide || yCoordinate == 8 * cellSide)) {
            direction = 1;
            yCoordinate -= cellSide;
        } else if (xCoordinate == 10 * cellSide && yCoordinate == 7 * cellSide) {
            direction = 4;
            xCoordinate = xCoordinate + cellSide;
            isGateOpen = false;
        }
        if (direction == 1) {
            QPixmap pixmap("/home/lucas/Desktop/Pac-Man/u1I.png");
            setPixmap(pixmap.scaled(cellSide, cellSide));
        } else if (direction == 2) {
            QPixmap pixmap("/home/lucas/Desktop/Pac-Man/d1I.png");
            setPixmap(pixmap.scaled(cellSide, cellSide));
        } else if (direction == 3) {
            QPixmap pixmap("/home/lucas/Desktop/Pac-Man/l1I.png");
            setPixmap(pixmap.scaled(cellSide, cellSide));
        } else {
            QPixmap pixmap("/home/lucas/Desktop/Pac-Man/r1I.png");
            setPixmap(pixmap.scaled(cellSide, cellSide));
        }
        setPos(xCoordinate, yCoordinate);
        return;
    }

    int currentX = xCoordinate;
    int currentY = yCoordinate;

    if (currentMode == GhostMode::Scatter) {
        setTarget(360, 380);
    } else {
        QPointF playerPosition(player->getXCoordinate(), player->getYCoordinate());
        QPointF blinkyPosition(blinky->getXCoordinate(), blinky->getYCoordinate());

        QPointF vectorToTarget = 2 * calculateVector(blinkyPosition.x(), blinkyPosition.y(), playerPosition.x(), playerPosition.y());

        QPointF targetPosition = blinkyPosition + vectorToTarget;

        setTarget(targetPosition.x(), targetPosition.y());
    }

    // double currentDistance = sqrt(pow(currentX - targetX, 2) + pow(currentY - targetY, 2));

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
                xCoordinate = 180;
                yCoordinate = 180;
                if (direction == 1) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/u1I.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 2) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/d1I.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 3) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/l1I.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/r1I.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                }
                setPos(xCoordinate, yCoordinate);
                isGateOpen = true;
                currentMode = GhostMode::Normal;
                isNormalMode = 1;
                ghostWasEaten = 1;
                return;
            } else {
                player->decreaseLives();
                if (direction == 1) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/u1I.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 2) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/d1I.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 3) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/l1I.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/r1I.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                }
                player->setXCoordinate(200);
                player->setYCoordinate(300);
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
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/u1I.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            } else if (direction == 2) {
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/d1I.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            } else if (direction == 3) {
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/l1I.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            } else {
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/r1I.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            }
        }
        setPos(xCoordinate, yCoordinate);
    }
}

QPointF Inky::calculateVector(int x1, int y1, int x2, int y2)
{
    return QPointF(x2 - x1, y2 - y1);
}

