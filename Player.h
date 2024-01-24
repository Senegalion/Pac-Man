#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsEllipseItem>
#include <QKeyEvent>
#include <QBrush>
#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <vector>

class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Player(qreal x, qreal y, std::vector<std::string>& mapSketch);
    void keyPressEvent(QKeyEvent *event, int mapWidth, int mapHeight);
    void move();
    int getXCoordinate() const;
    int getYCoordinate() const;
    void setXCoordinate(int newXCoordinate);
    void setYCoordinate(int newYCoordinate);
    int getNumberOfLives() const;
    int getCurrentDirection() const;
    int getScore() const;
    void increaseScore(int points);
    void decreaseLives();

signals:
    void energizerEaten();
    void gameWon();
    void gameLost();

private:
    int xCoordinate;
    int yCoordinate;
    int cellSide;
    int numberOfLives;
    int score;
    std::vector<std::string>& mapSketch;
    int currentDirection;
    bool isMoving;
    int movementSpeed;
    QTimer* moveTimer;
    void checkGameStatus();
};

#endif // PLAYER_H
