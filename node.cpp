#include "node.h"
#include "QPainter"
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

Node::Node(QObject *parent):
    QObject(parent), QGraphicsItem(){
    setZValue(2);
}

QRectF Node::boundingRect() const{
    return QRectF (-15,-15,ELLIPSE_SIZE,ELLIPSE_SIZE);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(Qt::darkGreen);
    painter->setBrush(Qt::darkGreen);
    painter->drawEllipse(-15, -15, ELLIPSE_SIZE, ELLIPSE_SIZE);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

int Node::type()const{
    return Type;
}
