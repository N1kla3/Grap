//
// Created by nicolas on 16/06/2020.
//


#include "header/BaseArrow.h"
#include <QPainter>
#include <QtMath>

BaseArrow::BaseArrow(QObject *parent):
        QObject(parent),
        QGraphicsItem(),
        height(10),
        first(nullptr),
        second(nullptr),
        brush(Qt::darkYellow),
        pen(Qt::darkYellow)
{
    setZValue(1);
    setTransformOriginPoint(0, 5);
}

void BaseArrow::setBetweenNodes(){
    qreal rotation = calculateRotation();
    this->setRotation(rotation);
}

qreal BaseArrow::calculateRotation() {
    qreal result = 0;
    qreal x = first->scenePos().x() - second->scenePos().x();
    qreal y = first->scenePos().y() - second->scenePos().y();
    length = calculateLength(x, y);
    if((x > 0 && y > 0) || (x < 0 && y > 0)){
        result = qFabs(y) / length;
    }else if((x > 0 && y < 0) || (x < 0 && y < 0)){
        result = qFabs(x) / length;
    }
    qreal res = (qAsin(result)*(180/M_PI));
    if(x >= 0 && y < 0){
        res = 90 + res;
    }else if (x > 0 && y >= 0){
        res += 180;
    }else if(x < 0 && y >= 0){
        res = 360 - res;
    }else if (x < 0 && y <= 0){
        res = 90 - res;
    }else if(x == 0 && y > 0){
        res -= 90;
    }
    return res;
}

qreal BaseArrow::calculateLength(qreal x, qreal y)const {
    return qSqrt(x*x + y*y);
}

void BaseArrow::initBetweenNodes(Node *first, Node *second){
    setFirstNode(first);
    setSecondNode(second);
    setPos(QPointF(first->scenePos().x(), first->scenePos().y()-5));
    setBetweenNodes();
}

void BaseArrow::slotFirstMove(){
    setPos(QPointF(first->scenePos().x(), first->scenePos().y()-5));
    setBetweenNodes();
    prepareGeometryChange();
    update();
}

void BaseArrow::slotSecondMove(){
    setBetweenNodes();
    prepareGeometryChange();
    update();
}

void BaseArrow::slot_delete(){
    delete_from_node(this);
}

int BaseArrow::type() const{
    return Type;
}

void BaseArrow::setFirstNode(Node *node){
    if(first != nullptr){
        disconnect(this->first, &Node::moved, this, &BaseArrow::slotFirstMove);
    }
    if(first != nullptr){
        disconnect(this->first, &Node::deleted, this, &BaseArrow::slot_delete);
    }
    this->first = node;
    if(node)connect(this->first, &Node::deleted, this, &BaseArrow::slot_delete);
    if(node)connect(this->first, &Node::moved, this, &BaseArrow::slotFirstMove);
}

void BaseArrow::setSecondNode(Node *node){
    if(second != nullptr){
        disconnect(this->second, &Node::moved, this, &BaseArrow::slotSecondMove);
    }
    if(second != nullptr){
        disconnect(this->second, &Node::deleted, this, &BaseArrow::slot_delete);
    }
    this->second = node;
    if(node)connect(this->second, &Node::deleted, this, &BaseArrow::slot_delete);
    if(node)connect(this->second, &Node::moved, this, &BaseArrow::slotSecondMove);
}

void BaseArrow::prepareUpdate(){
    setPos(QPointF(first->scenePos().x(), first->scenePos().y()-5));
    setBetweenNodes();
    prepareGeometryChange();
}

Node* BaseArrow::getFirstNode(){
    return first;
}

Node* BaseArrow::getSecondNode(){
    return second;
}

void BaseArrow::setColor(const QColor& color){
    brush.setColor(color);
    pen.setColor(color);
    update();
}
