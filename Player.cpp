#include "Player.h"
#include <QBrush>
#include <QColor>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <QMovie>

Player::Player(qreal x, qreal y, std::vector<std::string>& mapSketch)
    : xCoordinate(x), yCoordinate(y), cellSide(20), numberOfLives(3), score(0), mapSketch(mapSketch), currentDirection(Qt::Key_A), isMoving(true), movementSpeed(2)
{
    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/pacman_left_close.png");
    setPixmap(pixmap.scaled(cellSide, cellSide));
    setPos(xCoordinate, yCoordinate);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

int Player::getXCoordinate() const {
    return xCoordinate;
}

int Player::getYCoordinate() const {
    return yCoordinate;
}

int Player::getNumberOfLives() const
{
    return numberOfLives;
}

int Player::getScore() const
{
    return score;
}

void Player::setXCoordinate(int newXCoordinate) {
    xCoordinate = newXCoordinate;
}

void Player::setYCoordinate(int newYCoordinate) {
    yCoordinate = newYCoordinate;
}

void Player::increaseScore(int points)
{
    score += points;
}

int Player::getCurrentDirection() const {
    return currentDirection;
}

void Player::decreaseLives()
{
    numberOfLives--;

    if (numberOfLives <= 0)
    {
        qDebug() << "Game Over";
    }
}

void Player::keyPressEvent(QKeyEvent *event, int mapWidth, int mapHeight)
{
    checkGameStatus();

    int new_x = xCoordinate;
    int new_y = yCoordinate;

    int cellX = xCoordinate / cellSide;
    int cellY = yCoordinate / cellSide;

    if (event->key() == Qt::Key_W) {
        if (cellY > 0 && mapSketch[cellY - 1][cellX] != '#') {
            currentDirection = Qt::Key_W;
            new_y -= cellSide;
        }
    } else if (event->key() == Qt::Key_S) {
        if (cellY < mapHeight - 1 && mapSketch[cellY + 1][cellX] != '#') {
            currentDirection = Qt::Key_S;
            new_y += cellSide;
        }
    } else if (event->key() == Qt::Key_A) {
        if (cellX > 0 && mapSketch[cellY][cellX - 1] != '#') {
            currentDirection = Qt::Key_A;
            new_x -= cellSide;
        }
    } else if (event->key() == Qt::Key_D) {
        if (cellX < mapWidth - 1 && mapSketch[cellY][cellX + 1] != '#') {
            currentDirection = Qt::Key_D;
            new_x += cellSide;
        }
    }

    if (new_x >= 400) {
        new_x = 0;
    } else if (new_x <= 0) {
        new_x = 400;
    }

    if (currentDirection == Qt::Key_W) {
        QPixmap pixmap("/home/lucas/Desktop/Pac-Man/pacman_up_open_3.png");
        setPixmap(pixmap.scaled(cellSide, cellSide));
    } else if (currentDirection == Qt::Key_S) {
        QPixmap pixmap("/home/lucas/Desktop/Pac-Man/pacman_down_open_3.png");
        setPixmap(pixmap.scaled(cellSide, cellSide));
    } else if (currentDirection == Qt::Key_A) {
        QPixmap pixmap("/home/lucas/Desktop/Pac-Man/pacman_left_open_3.png");
        setPixmap(pixmap.scaled(cellSide, cellSide));
    } else {
        QPixmap pixmap("/home/lucas/Desktop/Pac-Man/pacman_right_open_3.png");
        setPixmap(pixmap.scaled(cellSide, cellSide));
    }

    setPos(new_x, new_y);  // Update the position

    xCoordinate = new_x;
    yCoordinate = new_y;

    cellX = xCoordinate / cellSide;
    cellY = yCoordinate / cellSide;

    if (mapSketch[cellY][cellX] == '.')
    {
        mapSketch[cellY][cellX] = ' ';
        increaseScore(10);
    } if (mapSketch[cellY][cellX] == 'o') {
        mapSketch[cellY][cellX] = ' ';
        emit energizerEaten();
        increaseScore(20);
    }
}

void Player::checkGameStatus()
{
    if (numberOfLives <= 0)
    {
        qDebug() << "Emit gameLost";
        emit gameLost();
        return;
    }

    bool allCollected = true;
    for (const auto& row : mapSketch)
    {
        if (row.find('.') != std::string::npos || row.find('o') != std::string::npos)
        {
            allCollected = false;
            break;
        }
    }

    if (allCollected)
    {
        qDebug() << "Emit gameWon";
        emit gameWon();
        return;
    }
}
