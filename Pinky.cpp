#include "Pinky.h"
#include <QRandomGenerator>
#include "Player.h"
#include "math.h"

Pinky::Pinky(int startX, int startY, Player* player, const std::vector<std::string>& mapSketch)
    : Ghost(startX, startY, player, mapSketch, "/home/lucas/Desktop/Pac-Man/r1P.png")
{
    isGateOpen = true;
    scatterMode();

    frightenedTimer = new QTimer(this);
    connect(frightenedTimer, &QTimer::timeout, this, &Pinky::updateFrightenedDuration);
    connect(player, &Player::energizerEaten, this, &Pinky::onEnergizerEaten);
}

Pinky::~Pinky(){}

void Pinky::startGame()
{
    scatterMode();
}

void Pinky::scatterMode()
{
    currentMode = GhostMode::Scatter;
    setTarget(40, 20);
    QTimer::singleShot(7 * 1000, this, &Pinky::switchToChaseMode);
    qDebug() << "Scatter mode: Pinky";
    startGhostMovement();
}

void Pinky::chaseMode()
{
    currentMode = GhostMode::Chase;
    setTarget(player->getXCoordinate(), player->getYCoordinate());
    QTimer::singleShot(20 * 1000, this, &Pinky::switchToScatterMode);
    qDebug() << "Chase mode: Pinky";
    startGhostMovement();
}

void Pinky::switchToChaseMode()
{
    stopGhostMovement();

    chaseMode();
}

void Pinky::switchToScatterMode()
{
    stopGhostMovement();

    scatterMode();
}

void Pinky::updateFrightenedDuration()
{
    if (frightenedDuration > 0) {
        frightenedDuration -= 1000;
    } else {
        switchToNormalMode();  // Switch back to normal mode when duration is over
    }
}

void Pinky::onEnergizerEaten()
{
    switchToFrightenedMode();
}

void Pinky::startGhostMovement()
{
    moveTimer->start(500);
}

void Pinky::stopGhostMovement()
{
    moveTimer->stop();
}

void Pinky::onPacManEaten()
{
    resetToInitialPosition(200, 180);
    emit pacManEaten();
}

void Pinky::checkTimeToRelease() {
    if (iterator == 14) {
        isCorrectTimeToRelease = true;
        iterator = 0;
    }
    iterator++;
}

void Pinky::moveGhost()
{
    checkTimeToRelease();
    if (!isCorrectTimeToRelease) {
        return;
    }
    if (isGateOpen && isCorrectTimeToRelease) {
        if (xCoordinate == 10 * cellSide && (yCoordinate == 9 * cellSide || yCoordinate == 8 * cellSide)) {
            direction = 1;
            yCoordinate -= cellSide;
        } else if (xCoordinate == 10 * cellSide && yCoordinate == 7 * cellSide) {
            direction = 4;
            xCoordinate += cellSide;
            isGateOpen = false;
        }
        if (direction == 1) {
            QPixmap pixmap("/home/lucas/Desktop/Pac-Man/u1P.png");
            setPixmap(pixmap.scaled(cellSide, cellSide));
        } else if (direction == 2) {
            QPixmap pixmap("/home/lucas/Desktop/Pac-Man/d1P.png");
            setPixmap(pixmap.scaled(cellSide, cellSide));
        } else if (direction == 3) {
            QPixmap pixmap("/home/lucas/Desktop/Pac-Man/l1P.png");
            setPixmap(pixmap.scaled(cellSide, cellSide));
        } else {
            QPixmap pixmap("/home/lucas/Desktop/Pac-Man/r1P.png");
            setPixmap(pixmap.scaled(cellSide, cellSide));
        }
        setPos(xCoordinate, yCoordinate);
        return;
    }
    int currentX = xCoordinate;
    int currentY = yCoordinate;

    if (currentMode == GhostMode::Scatter) {
        setTarget(40, 20);
    } else {
        switch (player->getCurrentDirection()) {
        case Qt::Key_W:
            setTarget(player->getXCoordinate(), player->getYCoordinate() - 4 * cellSide);
            break;
        case Qt::Key_S:
            setTarget(player->getXCoordinate(), player->getYCoordinate() + 4 * cellSide);
            break;
        case Qt::Key_A:
            setTarget(player->getXCoordinate() - 4 * cellSide, player->getYCoordinate());
            break;
        case Qt::Key_D:
            setTarget(player->getXCoordinate() + 4 * cellSide, player->getYCoordinate());
            break;
        default:
            // Invalid direction, handle accordingly
            break;
        }
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
                xCoordinate = 200;
                yCoordinate = 180;
                if (direction == 1) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/u1P.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 2) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/d1P.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 3) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/l1P.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/r1P.png");
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
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/u1P.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 2) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/d1P.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else if (direction == 3) {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/l1P.png");
                    setPixmap(pixmap.scaled(cellSide, cellSide));
                } else {
                    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/r1P.png");
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
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/u1P.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            } else if (direction == 2) {
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/d1P.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            } else if (direction == 3) {
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/l1P.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            } else {
                QPixmap pixmap("/home/lucas/Desktop/Pac-Man/r1P.png");
                setPixmap(pixmap.scaled(cellSide, cellSide));
            }
        }
        setPos(xCoordinate, yCoordinate);
    }
}
