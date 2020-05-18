#include "arrow.h"
#include <QPainter>
#include <QtMath>

Arrow::Arrow(QObject *parent):
    QObject(parent),
    QGraphicsItem(),
    height(10)
{
    setZValue(1);
    setTransformOriginPoint(0, 5);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(Qt::darkYellow);
    painter->setBrush(Qt::darkYellow);
    painter->drawRect(0, 3, length-30, 4);

    QPolygon polygon;
    polygon << QPoint(length-30,0) << QPoint(length-30,10) << QPoint(length-20,5);
    painter->drawPolygon(polygon);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Arrow::boundingRect() const{
    return QRectF(0, 0, length, height);
}

void Arrow::setBetweenNodes(){
    qreal rotation = calculateRotation();
    this->setRotation(rotation);
}

qreal Arrow::calculateRotation() {
    qreal res = 0;
    qreal result = 0;
    qreal x = first->scenePos().x() - second->scenePos().x();
    qreal y = first->scenePos().y() - second->scenePos().y();
    length = calculateLength(x, y);
    if((x > 0 && y > 0) || (x < 0 && y > 0)){
        result = qFabs(y) / length;
    }else if((x > 0 && y < 0) || (x < 0 && y < 0)){
        result = qFabs(x) / length;
    }
    res = (qAsin(result)*(180/M_PI));
    if(x > 0 && y < 0){
        res = 90 + res;
    }else if (x >= 0 && y >= 0){
        res += 180;
    }else if(x < 0 && y > 0){
        res = 360 - res;
    }else if (x <= 0 && y <= 0){
        res = 90 - res;
    }
    return res;
}

qreal Arrow::calculateLength(qreal x, qreal y)const {
    qreal res = (qSqrt(x*x + y*y));
    return res;
}

void Arrow::initBetweenNodes(Node *first, Node *second){
    this->first = first;
    this->second = second;
    connect(this->first, &Node::moved, this, &Arrow::slotFirstMove);
    connect(this->second, &Node::moved, this, &Arrow::slotSecondMove);
    setPos(QPointF(first->scenePos().x(), first->scenePos().y()-5));
    setBetweenNodes();
}

void Arrow::slotFirstMove(){
    setPos(QPointF(first->scenePos().x(), first->scenePos().y()-5));
    setBetweenNodes();
    prepareGeometryChange();
    update();
}

void Arrow::slotSecondMove(){
    setBetweenNodes();
    prepareGeometryChange();
    update();
}
