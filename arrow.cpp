#include "arrow.h"
#include <QPainter>
#include <QtMath>

Arrow::Arrow(QObject *parent):
    QObject(parent),
    QGraphicsItem()
{

}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(Qt::black);
    painter->setBrush(Qt::black);
    painter->drawRect(5, 5, width-5, height-5);

    QPolygon polygon;
    polygon << QPoint(width-10,0) << QPoint(width-10,10) << QPoint(width,5);
    painter->drawPolygon(polygon);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Arrow::boundingRect() const{
    return QRectF(0, 0, width, height);
}

void Arrow::setBetweenNodes(){

}

qreal Arrow::calculateRotation()const {
    qreal res = 0;
    qreal result = 0;
    qreal x = firstNode.x() - secondNode.y();
    qreal y = firstNode.y() - secondNode.y();
    qreal length = calculateLength(x, y);
    if((x > 0 && y > 0) || (x < 0 && y > 0)){
        result = qFabs(y) / length;
    }else if((x > 0 && y < 0) || (x < 0 && y < 0)){
        result = qFabs(x) / length;
    }
    res = (qAsin(result)*(180/3.14159265359));
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

qreal Arrow::calculateLength(const qreal x, const qreal y)const {
    qreal res = (qSqrt(x*x + y*y));
    return res;
}
