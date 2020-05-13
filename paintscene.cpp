#include "paintscene.h"

PaintScene::PaintScene(QObject* parent):
    QGraphicsScene(parent)
{

}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Node *a = new Node();
    a->setPos(event->scenePos().x(), event->scenePos().y());
    this->addItem(a);
}
