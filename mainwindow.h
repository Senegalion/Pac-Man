#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include "Player.h"
#include "Pellet.h"
#include "Energizer.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Clyde.h"
#include <QTimer>

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateUI();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    static const int MAP_WIDTH = 21;
    static const int MAP_HEIGHT = 21;
    static const int CELL_WIDTH = 20;

    QGraphicsScene *scene;
    std::vector<std::vector<QGraphicsRectItem*>> cells;
    std::vector<std::string> mapSketch;

    Player *pacMan;
    std::vector<Pellet*> pellets;
    std::vector<Energizer*> energizers;
    Blinky* redGhost;
    Pinky* pinkGhost;
    Clyde* orangeGhost;

    QGraphicsTextItem* scoreText;
    QGraphicsTextItem* livesText;

    void createObjects();
    void createPelletsAndEnergizers();
    void createPacMan();
    void updateMapSketch(int cellX, int cellY);
    void createGhosts();
    void createUIElements();
    void updateGhostPositions();
    void displayGameWonScreen();
    void displayGameLostScreen();
    void hideAllItems();
};

#endif // MAINWINDOW_H
