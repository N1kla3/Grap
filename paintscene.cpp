#include "paintscene.h"
#include <arrow.h>

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

    switch(flag){
    case SELECTION:
    {
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        Arrow *arrow = qgraphicsitem_cast<Arrow*>(item);
        Node *node = qgraphicsitem_cast<Node*>(item);

        if(node){
            selectedItem = node;
        }else if(arrow){
            selectedItem = arrow;
        }
        QGraphicsScene::mousePressEvent(event);
        break;
    }

    case CREATE_NODE:
    {
        Node *node = new Node();
        node->setPos(event->scenePos().x(), event->scenePos().y());
        this->addItem(node);
        break;
    }

    case CREATE_ARROW:
    {

        Arrow *arrow = new Arrow();
        arrow->setFirstNode(QPointF(event->scenePos().x(), event->scenePos().y()));
        arrow->setFirstNode(QPointF(event->scenePos().x()+50, event->scenePos().y()+50));
        arrow->setPos(event->scenePos().x(), event->scenePos().y());
        this->addItem(arrow);
        break;
    }

    }
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    selectedItem = nullptr;
    Q_UNUSED(event)
}

void PaintScene::setFlag(EActionToDo mode){
    flag = mode;
}
