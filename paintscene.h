#ifndef PAINTSCENE_H
#define PAINTSCENE_H
#include<QGraphicsScene>
#include<QGraphicsSceneMouseEvent>
#include <QTimer>
#include <arrow.h>


enum EActionToDo{
    SELECTION,
    CREATE_NODE,
    CREATE_ARROW
};
class PaintScene : public QGraphicsScene
{
       Q_OBJECT
public:
    explicit PaintScene(QObject* parent = 0);
    ~PaintScene(){}
    void setFlag(EActionToDo);
public slots:
    void slot_color();
    void slot_delete();
    void slot_delete_arrow(Arrow *arrow);
private:
    bool bDrawArrow;
    Node *firstNodeOfArrow;
    Node *secondNodeOfArrow;
    Mininode *one;
    Mininode *two;
    Mininode *currMininode;
    EActionToDo flag;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void deleteMiniNodes();
    QGraphicsItem *selectedItem;
    Node *selectedNodeForPopupMenu;
    Arrow *selectedArrowForPopupMenu;
};

#endif // PAINTSCENE_H
