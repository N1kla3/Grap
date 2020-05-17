#include "paintscene.h"
#include <arrow.h>

PaintScene::PaintScene(QObject* parent):
    QGraphicsScene(parent),
    bDrawArrow(false),
    flag(SELECTION),
    selectedItem(nullptr)
{

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
        if(!bDrawArrow){
            Node *node = qgraphicsitem_cast<Node*>(itemAt(event->scenePos(), QTransform()));
            if(node){
                firstNodeOfArrow = node->scenePos();
                bDrawArrow = true;
            }
        }else{
            Node *node = qgraphicsitem_cast<Node*>(itemAt(event->scenePos(), QTransform()));
            if(node){
                Arrow *arrow = new Arrow();
                arrow->setFirstNode(firstNodeOfArrow);
                arrow->setSecondNode(node->scenePos());
                arrow->setPos(firstNodeOfArrow);
                arrow->setBetweenNodes();
                this->addItem(arrow);
            }
            bDrawArrow = false;
        }

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
