#include "header/paintscene.h"
#include "header/unarrow.h"
#include<QMenu>
#include<QColorDialog>
#include<QInputDialog>

PaintScene::PaintScene(QObject* parent):
    QGraphicsScene(parent),
    bDrawArrow(false),
    bUnOrient(false),
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
    if(event->scenePos().x() < 0 || event->scenePos().x() > width())return;
    if(event->scenePos().y() < 0 || event->scenePos().y() > height())return;
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
        bUnOrient = false;
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        Arrow *arrow = qgraphicsitem_cast<Arrow*>(item);
        Node *node = qgraphicsitem_cast<Node*>(item);
        Mininode *mininode = qgraphicsitem_cast<Mininode*>(item);
        UnArrow *unorient = qgraphicsitem_cast<UnArrow*>(item);

        if(mininode){
            currMininode = mininode;
        }else if(node){
            if(qgraphicsitem_cast<Arrow*>(selectedItem)){
                deleteMiniNodes();
            }else if(qgraphicsitem_cast<UnArrow*>(selectedItem)){
                deleteMiniNodes();
            }
            selectedItem = node;
        }else if(arrow){
            deleteMiniNodes();
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
        }else if(unorient){
            deleteMiniNodes();
            selectedItem = unorient;
            firstNodeOfArrow = unorient->getFirstNode();
            secondNodeOfArrow = unorient->getSecondNode();
            one = new Mininode();
            one->setPos(firstNodeOfArrow->pos());
            two = new Mininode();
            two->setPos(secondNodeOfArrow->pos());
            unorient->setFirstNode(one);
            unorient->setSecondNode(two);
            unorient->update();
            this->addItem(one);
            this->addItem(two);
        }
        break;
    }

    case CREATE_NODE:
    {
        bDrawArrow = false;
        bUnOrient = false;
        Node *node = new Node();
        node->setPos(event->scenePos().x(), event->scenePos().y());
        this->addItem(node);
        break;
    }

    case CREATE_ARROW:
    {
        bUnOrient = false;
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
    case CREATE_UNORIENTED:
    {
        bDrawArrow = false;
        if(!bUnOrient){
            Node *node = qgraphicsitem_cast<Node*>(itemAt(event->scenePos(), QTransform()));
            if(node){
                firstNodeOfArrow = node;
                bUnOrient = true;
            }
        }else{
            Node *node = qgraphicsitem_cast<Node*>(itemAt(event->scenePos(), QTransform()));
            if(node){
                UnArrow *arrow = new UnArrow();
                arrow->initBetweenNodes(firstNodeOfArrow, node);
                this->addItem(arrow);
                connect(arrow, &Arrow::delete_from_node, this, &PaintScene::slot_delete_arrow);
            }
            bUnOrient = false;
        }
        break;
    }

    }
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){

    if(currMininode != nullptr){
        deleteMiniNodes();
    }
    if(!qgraphicsitem_cast<Arrow*>(selectedItem)
            && !qgraphicsitem_cast<UnArrow*>(selectedItem)){
        selectedItem = nullptr;
    }
    Q_UNUSED(event)
}

void PaintScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(flag != SELECTION)return;
    QGraphicsItem *selectedItemForPopupMenu = itemAt(event->scenePos(), QTransform());
    selectedNodeForPopupMenu = qgraphicsitem_cast<Node*>(selectedItemForPopupMenu);
    selectedArrowForPopupMenu = qgraphicsitem_cast<Arrow*>(selectedItemForPopupMenu);
    selectedUnArrowPopup = qgraphicsitem_cast<UnArrow*>(selectedItemForPopupMenu);
    deleteMiniNodes();
    selectedItem = nullptr;
    if(selectedItemForPopupMenu){
        QMenu menu(event->widget());
        menu.addAction("Change color", this, &PaintScene::slot_color);
        menu.addAction("Delete", this, &PaintScene::slot_delete);
        if(selectedNodeForPopupMenu){
            menu.addAction("Degree", this, [this](){
                calc_degree(selectedNodeForPopupMenu);
            });
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
    if(!a)a = qgraphicsitem_cast<UnArrow*>(selectedItem);
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
        auto color = QColorDialog::getColor(Qt::darkGreen);
        if(color.isValid())selectedNodeForPopupMenu->setColor(color);
    }else if(selectedArrowForPopupMenu){
        auto color = QColorDialog::getColor(Qt::darkYellow);
        if(color.isValid())selectedArrowForPopupMenu->setColor(color);
    }else if(selectedUnArrowPopup){
        auto color = QColorDialog::getColor(Qt::darkYellow);
        if(color.isValid())selectedUnArrowPopup->setColor(color);
    }
}

void PaintScene::slot_delete(){
    if(selectedNodeForPopupMenu){
        selectedNodeForPopupMenu->deleted();
        removeItem(selectedNodeForPopupMenu);
        delete selectedNodeForPopupMenu;
        selectedNodeForPopupMenu = nullptr;
    }else if(selectedArrowForPopupMenu){
        deleteMiniNodes();
        removeItem(selectedArrowForPopupMenu);
        delete selectedArrowForPopupMenu;
        selectedArrowForPopupMenu = nullptr;
    }else if(selectedUnArrowPopup){
        deleteMiniNodes();
        removeItem(selectedUnArrowPopup);
        delete selectedUnArrowPopup;
        selectedUnArrowPopup = nullptr;
    }
    selectedItem = nullptr;
}

void PaintScene::slot_delete_arrow(Arrow *arrow){
    removeItem(arrow);
    delete arrow;
}

void PaintScene::slot_set_name(){
    bool ok;
    selectedNodeForPopupMenu->setName(QInputDialog::getText(nullptr, tr("Set Name"),
                                                            tr("Enter name:"), QLineEdit::Normal,
                                                            "name", &ok));
}
