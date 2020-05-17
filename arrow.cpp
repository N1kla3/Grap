#include "arrow.h"
#include <QPainter>
#include <QtMath>

Arrow::Arrow(QObject *parent):
    QObject(parent),
    QGraphicsItem(),
    height(10)
{

}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(Qt::black);
    painter->setBrush(Qt::black);
    painter->drawRect(0, 3, length-7, 4);

    QPolygon polygon;
    polygon << QPoint(length-10,0) << QPoint(length-10,10) << QPoint(length,5);
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
    qreal x = firstNode.x() - secondNode.x();
    qreal y = firstNode.y() - secondNode.y();
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

void Arrow::setFirstNode(QPointF point){
    firstNode = point;
}

void Arrow::setSecondNode(QPointF point){
    secondNode = point;
}
