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
    name = new QGraphicsTextItem(this);
    name->setPos(-10,10);
}

QRectF Node::boundingRect() const{
    return QRectF (-15,-15,ELLIPSE_SIZE,ELLIPSE_SIZE);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(Qt::black);
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

int Node::getIndex(){
    return index;
}

void Node::setIndex(int index){
    this->index = index;
}

void Node::setName(const QString name){
    this->name->setPlainText(name);
    update();
}

QString Node::getName(){
    return name->toPlainText();
}

void Node::setExcent(int e){
    excent = e;
}

int Node::getExcent(){
    return excent;
}
