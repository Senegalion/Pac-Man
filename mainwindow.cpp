#include "mainwindow.h"
#include <QBrush>
#include <QDebug>
#include <QMovie>
#include <QPixmap>
#include "GhostManager.h"
#include "Energizer.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"

MainWindow::MainWindow(QWidget *parent)
    : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);

    mapSketch = {
        " ################### ",
        " #........#........# ",
        " #o##.###.#.###.##o# ",
        " #.................# ",
        " #.##.#.#####.#.##.# ",
        " #....#...#...#....# ",
        " #### ### # ### #### ",
        "    #.#       #.#    ",
        "#####.# ##-## #.#####",
        "     .  #   #  .     ",
        "#####.# ##### #.#####",
        "    #.#       #.#    ",
        " ####.# ##### #.#### ",
        " #........#........# ",
        " #.##.###.#.###.##.# ",
        " #o.#..... .....#.o# ",
        " ##.#.#.#####.#.#.## ",
        " #....#...#...#....# ",
        " #.######.#.######.# ",
        " #.................# ",
        " ################### "
    };
    cells.resize(MAP_HEIGHT, std::vector<QGraphicsRectItem*>(MAP_WIDTH, nullptr));

    createObjects();
    createPelletsAndEnergizers();
    createPacMan();
    createGhosts();
    createUIElements();

    connect(pacMan, &Player::energizerEaten, this, &MainWindow::updateUI);
    connect(pacMan, &Player::gameWon, this, &MainWindow::displayGameWonScreen);
    connect(pacMan, &Player::gameLost, this, &MainWindow::displayGameLostScreen);
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            delete cells[i][j];
        }
    }

    for (Pellet* pellet : pellets)
    {
        delete pellet;
    }
}

void MainWindow::createObjects()
{
    QBrush borderBrush(Qt::blue);

    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            if (mapSketch[i][j] == '#')
            {
                QGraphicsRectItem *cell = new QGraphicsRectItem(j * CELL_WIDTH, i * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
                cell->setBrush(borderBrush);
                cell->setPen(Qt::NoPen);
                scene->addItem(cell);
                cells[i][j] = cell;
            } else if (mapSketch[i][j] == '-') {
                QGraphicsRectItem *gate = new QGraphicsRectItem(j * CELL_WIDTH, i * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
                gate->setBrush(Qt::gray);
                gate->setPen(Qt::NoPen);
                scene->addItem(gate);
                cells[i][j] = gate;
            }
        }
    }
}

void MainWindow::createPacMan() {
    pacMan = new Player(10 * CELL_WIDTH, 15 * CELL_WIDTH, mapSketch);
    scene->addItem(pacMan);
    pacMan->setZValue(10);
}

void MainWindow::createGhosts() {
    Blinky* redGhost = new Blinky(10 * CELL_WIDTH, 7 * CELL_WIDTH, pacMan, mapSketch);
    redGhost->scatterMode();
    scene->addItem(redGhost);
    redGhost->setZValue(1);

    Pinky* pinkGhost = new Pinky(10 * CELL_WIDTH, 9 * CELL_WIDTH, pacMan, mapSketch);
    pinkGhost->scatterMode();
    scene->addItem(pinkGhost);
    pinkGhost->setZValue(1);

    Inky* blueGhost = new Inky(9 * CELL_WIDTH, 9 * CELL_WIDTH, pacMan, redGhost, mapSketch);
    blueGhost->scatterMode();
    scene->addItem(blueGhost);
    blueGhost->setZValue(1);

    Clyde* orangeGhost = new Clyde(11 * CELL_WIDTH, 9 * CELL_WIDTH, pacMan, mapSketch);
    orangeGhost->scatterMode();
    scene->addItem(orangeGhost);
    orangeGhost->setZValue(1);
}

void MainWindow::createPelletsAndEnergizers()
{
    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            if (mapSketch[i][j] == '.')
            {
                Pellet* pellet = new Pellet(j * CELL_WIDTH, i * CELL_WIDTH);
                scene->addItem(pellet);

                pellets.push_back(pellet);
            } else if (mapSketch[i][j] == 'o')
            {
                Energizer* energizer = new Energizer(j * CELL_WIDTH, i * CELL_WIDTH);
                scene->addItem(energizer);

                energizers.push_back(energizer);
            }
        }
    }
}

void MainWindow::createUIElements()
{
    scoreText = new QGraphicsTextItem("Score: 0");
    livesText = new QGraphicsTextItem("Lives: 3");

    QFont font("Arial", 12);
    scoreText->setFont(font);
    livesText->setFont(font);

    scoreText->setDefaultTextColor(Qt::white);
    livesText->setDefaultTextColor(Qt::white);

    scoreText->setPos(10, MAP_HEIGHT * CELL_WIDTH + 10);
    livesText->setPos(10, MAP_HEIGHT * CELL_WIDTH + 30);

    scene->addItem(scoreText);
    scene->addItem(livesText);
}

void MainWindow::updateUI()
{
    scoreText->setPlainText("Score: " + QString::number(pacMan->getScore()));
    livesText->setPlainText("Lives: " + QString::number(pacMan->getNumberOfLives()));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (pacMan)
    {
        pacMan->keyPressEvent(event, MAP_WIDTH, MAP_HEIGHT);

        for (auto it = pellets.begin(); it != pellets.end();)
        {
            if (pacMan->collidesWithItem(*it))
            {
                scene->removeItem(*it);
                delete *it;
                it = pellets.erase(it);

                pacMan->increaseScore(0);
                updateUI();
            }
            else
            {
                ++it;
            }
        }

        for (auto it = energizers.begin(); it != energizers.end();)
        {
            if (pacMan->collidesWithItem(*it))
            {
                scene->removeItem(*it);
                delete *it;
                it = energizers.erase(it);

                pacMan->increaseScore(20);
                updateUI();
            }
            else
            {
                ++it;
            }
        }
    }
}

void MainWindow::displayGameWonScreen()
{
    hideAllItems();

    scene->setBackgroundBrush(Qt::black);

    QMovie* winnerMovie = new QMovie("/home/lucas/Desktop/Pac-Man/winner_gif.gif");
    QGraphicsPixmapItem* winnerItem = new QGraphicsPixmapItem(QPixmap::fromImage(winnerMovie->currentImage()));
    scene->addItem(winnerItem);
    winnerMovie->start();

    connect(winnerMovie, &QMovie::frameChanged, [this, winnerMovie, winnerItem]() {
        winnerItem->setPixmap(QPixmap::fromImage(winnerMovie->currentImage()));
        // Fit the gif to the view
        fitInView(winnerItem, Qt::KeepAspectRatio);
    });
}

void MainWindow::displayGameLostScreen()
{
    hideAllItems();

    scene->setBackgroundBrush(Qt::black);

    QGraphicsTextItem* gameLostText = new QGraphicsTextItem("Game Over! You Lost!");
    gameLostText->setFont(QFont("Arial", 20));
    gameLostText->setDefaultTextColor(Qt::red);
    gameLostText->setPos(70, 200);
    scene->addItem(gameLostText);
}

void MainWindow::hideAllItems()
{
    // Hide or remove existing items from the scene
    for (int i = 0; i < MAP_HEIGHT; ++i)
    {
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            QGraphicsItem *item = cells[i][j];
            if (item)
            {
                item->setVisible(false);
            }
        }
    }

    for (Pellet* pellet : pellets)
    {
        pellet->setVisible(false);
    }

    for (Energizer* energizer : energizers)
    {
        energizer->setVisible(false);
    }

    pacMan->setVisible(false);

    // Hide ghosts
    QList<QGraphicsItem*> allItems = scene->items();
    for (QGraphicsItem* item : allItems)
    {
        Ghost* ghost = dynamic_cast<Ghost*>(item);
        if (ghost)
        {
            ghost->setVisible(false);
        }
    }

    // Hide score and numberOfLives text
    scoreText->setVisible(false);
    livesText->setVisible(false);
}
