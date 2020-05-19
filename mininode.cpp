#include "mininode.h"
#include <QPainter>

Mininode::Mininode(QObject *parent):
    Node(parent)
{
    setZValue(3);
}

QRectF Mininode::boundingRect()const{
    return QRectF(-10, -10, 20, 20);
}

void Mininode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(Qt::darkRed);
    painter->setBrush(Qt::darkRed);
    painter->drawEllipse(-10, -10, 20, 20);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

int Mininode::type() const{
    return Type;
}
