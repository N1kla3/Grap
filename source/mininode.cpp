#include "header/mininode.h"
#include <QPainter>

Mininode::Mininode(QObject *parent):
    Node(parent)
{
    setZValue(3);
}

QRectF Mininode::boundingRect()const{
    return QRectF(ALEFT, ATOP, DIAMETR, DIAMETR);
}

void Mininode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(Qt::darkRed);
    painter->setBrush(Qt::darkRed);
    painter->drawEllipse(ALEFT, ATOP, DIAMETR, DIAMETR);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

int Mininode::type() const{
    return Type;
}
