#include "node.h"
#include "QPainter"
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

Node::Node(QObject *parent):
    QObject(parent),
    QGraphicsItem(),
    brush(Qt::darkGreen),
    pen(Qt::darkGreen)
{
    setZValue(2);
}

QRectF Node::boundingRect() const{
    return QRectF (-15,-15,ELLIPSE_SIZE,ELLIPSE_SIZE);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(-15, -15, ELLIPSE_SIZE, ELLIPSE_SIZE);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

int Node::type()const{
    return Type;
}

void Node::setColor(const QColor color){
    pen.setColor(color);
    brush.setColor(color);
    update();
}
