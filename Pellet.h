// pellet.h
#ifndef PELLET_H
#define PELLET_H

#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>

class Pellet : public QGraphicsPixmapItem {
public:
    Pellet(qreal x, qreal y);
};

#endif // PELLET_H
