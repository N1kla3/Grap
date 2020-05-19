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
        if(Node *a = qgraphicsitem_cast<Node*>(selectedItem)){
            a->setPos(event->scenePos());
            a->moved();
        }else if(Arrow *b = qgraphicsitem_cast<Arrow*>(selectedItem)){

        }
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event){

    switch(flag){
    case SELECTION:
    {
        bDrawArrow = false;
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        Arrow *arrow = qgraphicsitem_cast<Arrow*>(item);
        Node *node = qgraphicsitem_cast<Node*>(item);

        if(node){
            selectedItem = node;
        }else if(arrow){
            selectedItem = arrow;
        }

        break;
    }

    case CREATE_NODE:
    {
        bDrawArrow = false;
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
                firstNodeOfArrow = node;
                bDrawArrow = true;
            }
        }else{
            Node *node = qgraphicsitem_cast<Node*>(itemAt(event->scenePos(), QTransform()));
            if(node){
                Arrow *arrow = new Arrow();
                arrow->initBetweenNodes(firstNodeOfArrow, node);
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
