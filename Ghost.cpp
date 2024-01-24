#include "Ghost.h"

Ghost::Ghost(int startX, int startY, Player* player, const std::vector<std::string>& mapSketch, const QString& imagePath)
    : xCoordinate(startX), yCoordinate(startY), player(player), mapSketch(mapSketch)
{
    cellSide = 20;
    direction = 4;
    QPixmap pixmap(imagePath);
    setPixmap(pixmap.scaled(cellSide, cellSide));
    setPos(xCoordinate, yCoordinate);

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Ghost::moveGhost);
}

Ghost::~Ghost()
{
    delete moveTimer;
}

void Ghost::resetToInitialPosition(int coordinateX, int coordinateY)
{
    xCoordinate = coordinateX;  // Assuming initialX is a member variable storing the starting X coordinate
    yCoordinate = coordinateY;  // Assuming initialY is a member variable storing the starting Y coordinate
    setPos(xCoordinate, yCoordinate);
}

void Ghost::switchToFrightenedMode()
{
    isNormalMode = 0;
    qDebug() << "Frightened Mode";
    currentMode = GhostMode::Frightened;

    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/mortal.png");
    setPixmap(pixmap.scaled(cellSide, cellSide));

    moveTimer->setInterval(500);

    frightenedDuration = 10000;  // Set the duration to 10 seconds
    frightenedTimer->start(1000);  // Start the timer to decrement duration every second
    moveTimer->start(500);
}

void Ghost::switchToNormalMode()
{
    isNormalMode = 1;
    qDebug() << "Normal Mode";
    currentMode = GhostMode::Normal;

    moveTimer->setInterval(250);
    frightenedTimer->stop();  // Stop the frightened mode timer
}

void Ghost::moveGhost()
{
    if (currentMode == GhostMode::Normal)
    {
        // Normal ghost behavior
        // ...
    }
    else if (currentMode == GhostMode::Frightened)
    {
        // Frightened ghost behavior
        // ...
    }
    // Add more cases for Scatter and Chase modes as needed
}

int Ghost::getXCoordinate() const {
    return xCoordinate;
}

int Ghost::getYCoordinate() const {
    return yCoordinate;
}

void Ghost::setTarget(int x, int y)
{
    targetX = x;
    targetY = y;
}

bool Ghost::canMoveTo(int x, int y)
{
    int MAP_WIDTH = 21, MAP_HEIGHT = 21;
    int cellX = x / cellSide;
    int cellY = y / cellSide;

    if (cellX < 0 || cellX >= MAP_WIDTH || cellY < 0 || cellY >= MAP_HEIGHT)
        return false;

    return mapSketch[cellY][cellX] != '#' && mapSketch[cellY][cellX] != '-';
}
