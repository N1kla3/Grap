#include "paintscene.h"

PaintScene::PaintScene(QObject* parent):
    QGraphicsScene(parent)
{
    selectedItem = nullptr;
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(selectedItem)
        selectedItem->setPos(event->scenePos());
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    if(!item){
        Node *a = new Node();
        a->setPos(event->scenePos().x(), event->scenePos().y());
        this->addItem(a);
    }else{
        selectedItem = item;
    }
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    selectedItem = nullptr;
    Q_UNUSED(event)
}
