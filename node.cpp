#include "node.h"
#include "QPainter"
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

Node::Node(QObject *parent):
    QObject(parent), QGraphicsItem(){
}

QRectF Node::boundingRect() const{
    return QRectF (-15,-15,ELLIPSE_SIZE,ELLIPSE_SIZE);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(Qt::black);
    painter->setBrush(Qt::green);
    painter->drawEllipse(-15, -15, ELLIPSE_SIZE, ELLIPSE_SIZE);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

//void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
//    this->setPos(mapToScene(event->pos()));
//}
//
//void Node::mousePressEvent(QGraphicsSceneMouseEvent *event){
//    this->setCursor(QCursor(Qt::ClosedHandCursor));
//    Q_UNUSED(event)
//}
//
//void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
//     this->setCursor(QCursor(Qt::ArrowCursor));
//    Q_UNUSED(event)
//}
