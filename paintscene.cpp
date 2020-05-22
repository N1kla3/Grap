#include "paintscene.h"
#include <arrow.h>
#include<QMenu>
#include<QColorDialog>
#include<QInputDialog>

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
                connect(arrow, &Arrow::delete_from_node, this, &PaintScene::slot_delete_arrow);
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

void PaintScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(flag != SELECTION)return;
    QGraphicsItem *selectedItemForPopupMenu = itemAt(event->scenePos(), QTransform());
    selectedNodeForPopupMenu = qgraphicsitem_cast<Node*>(selectedItemForPopupMenu);
    selectedArrowForPopupMenu = qgraphicsitem_cast<Arrow*>(selectedItemForPopupMenu);
    if(selectedItemForPopupMenu){
        QMenu menu(event->widget());
        menu.addAction("Change color", this, &PaintScene::slot_color);
        menu.addAction("Delete", this, &PaintScene::slot_delete);
        if(selectedNodeForPopupMenu){
            menu.addAction("sssd");
            menu.addAction("Set Name", this, &PaintScene::slot_set_name);
        }
        menu.exec(event->screenPos());
    }
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
    a->prepareUpdate();
    a->update();
    currMininode = nullptr;
}

void PaintScene::slot_color(){
    if(selectedNodeForPopupMenu){
        selectedNodeForPopupMenu->setColor(QColorDialog::getColor(Qt::darkGreen));
    }else if(selectedArrowForPopupMenu){
        selectedArrowForPopupMenu->setColor(QColorDialog::getColor(Qt::darkYellow));
    }
}

void PaintScene::slot_delete(){
    if(selectedNodeForPopupMenu){
        selectedNodeForPopupMenu->deleted();
        removeItem(selectedNodeForPopupMenu);
        delete selectedNodeForPopupMenu;
        selectedNodeForPopupMenu = nullptr;
        selectedItem = nullptr;
    }else if(selectedArrowForPopupMenu){
        deleteMiniNodes();
        removeItem(selectedArrowForPopupMenu);
        delete selectedArrowForPopupMenu;
        selectedArrowForPopupMenu = nullptr;
        selectedItem = nullptr;
    }
}

void PaintScene::slot_delete_arrow(Arrow *arrow){
    removeItem(arrow);
    delete arrow;
}

void PaintScene::slot_set_name(){
    bool ok;
    selectedNodeForPopupMenu->setName(QInputDialog::getText(nullptr, "add", "set"));
}
