#include "Energizer.h"
#include <QTimer>

Energizer::Energizer(qreal x, qreal y)
{
    QPixmap pixmap("/home/lucas/Desktop/Pac-Man/energizer.png");
    setPixmap(pixmap.scaled(cellWidth - 4, cellWidth - 4));
    setPos(x + 2, y + 2);
}
