#include "paintscene.h"
#include <arrow.h>

PaintScene::PaintScene(QObject* parent):
    QGraphicsScene(parent),
    bDrawArrow(false),
    flag(SELECTION),
    selectedItem(nullptr)
{
    firstNodeOfArrow = nullptr;
    secondNodeOfArrow = nullptr;
    one = nullptr;
    two = nullptr;
    currMininode = nullptr;
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(currMininode){
        currMininode->setPos(event->scenePos());
        currMininode->moved();
    }else if(selectedItem)
        if(Node *a = qgraphicsitem_cast<Node*>(selectedItem)){
            a->setPos(event->scenePos());
            a->moved();
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
        Mininode *mininode = qgraphicsitem_cast<Mininode*>(item);
        if(mininode){
            currMininode = mininode;
        }else if(node){
            if(qgraphicsitem_cast<Arrow*>(selectedItem)){
                deleteMiniNodes();
            }
            selectedItem = node;
        }else if(arrow){
            if(qgraphicsitem_cast<Arrow*>(selectedItem)){
                deleteMiniNodes();
            }
            selectedItem = arrow;
            firstNodeOfArrow = arrow->getFirstNode();
            secondNodeOfArrow = arrow->getSecondNode();
            one = new Mininode();
            one->setPos(firstNodeOfArrow->pos());
            two = new Mininode();
            two->setPos(secondNodeOfArrow->pos());
            arrow->setFirstNode(one);
            arrow->setSecondNode(two);
            arrow->update();
            this->addItem(one);
            this->addItem(two);
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

    if(currMininode != nullptr){
        deleteMiniNodes();
    }
    if(!qgraphicsitem_cast<Arrow*>(selectedItem)){
        selectedItem = nullptr;
    }
    Q_UNUSED(event)
}

void PaintScene::setFlag(EActionToDo mode){
    flag = mode;
}

void PaintScene::deleteMiniNodes(){
    if(!selectedItem)return;
    if(!one)return;
    if(!two)return;
    Arrow *a = qgraphicsitem_cast<Arrow*>(selectedItem);
    a->setFirstNode(nullptr);
    a->setSecondNode(nullptr);
    auto pos1 = one->pos();
    auto pos2 = two->pos();
    this->removeItem(one);
    delete one;one = nullptr;
    delete two;two = nullptr;
    Node *node = qgraphicsitem_cast<Node*>(itemAt(pos1, QTransform()));
    if(node){
        a->setFirstNode(node);
    }else{
        a->setFirstNode(firstNodeOfArrow);
    }
    node = qgraphicsitem_cast<Node*>(itemAt(pos2, QTransform()));
    if(node){
        a->setSecondNode(node);
    }else {
        a->setSecondNode(secondNodeOfArrow);
    }
    currMininode = nullptr;
}
