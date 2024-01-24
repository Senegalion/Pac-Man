#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>

class Energizer : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Energizer(qreal x, qreal y);

private:
    int cellWidth = 20;
};
