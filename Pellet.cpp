#include "Pellet.h"
#include <QBrush>

Pellet::Pellet(qreal x, qreal y)
{
    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/pellet.png");
    setPixmap(pixmap.scaled(20, 20));
    setPos(x, y);
}
