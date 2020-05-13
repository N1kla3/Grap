#include "node.h"
#include "QPainter"

Node::Node(QObject *parent):
    QObject(parent), QGraphicsItem()
{}

QRectF Node::boundingRect() const{
    return QRectF (0,0,30,30);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(Qt::black);
    painter->setBrush(Qt::green);
    painter->drawEllipse(0, 0, 30, 30);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event)
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event)
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event)
}
