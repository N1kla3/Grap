#ifndef PAINTSCENE_H
#define PAINTSCENE_H
#include<QGraphicsScene>
#include<QGraphicsSceneMouseEvent>
#include <QTimer>
#include <arrow.h>
#include <unarrow.h>

enum EActionToDo{
    SELECTION,
    CREATE_NODE,
    CREATE_ARROW,
    CREATE_UNORIENTED
};
class PaintScene : public QGraphicsScene
{
       Q_OBJECT
public:
    explicit PaintScene(QObject* parent = 0);
    ~PaintScene(){}
    void setFlag(EActionToDo);
    void deleteMiniNodes();
signals:
    void calc_degree(Node *node);
public slots:
    void slot_color();
    void slot_delete();
    void slot_delete_arrow(Arrow *arrow);
    void slot_set_name();
private:
    bool bDrawArrow;
    bool bUnOrient;
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

    QGraphicsItem *selectedItem;
    Node *selectedNodeForPopupMenu;
    Arrow *selectedArrowForPopupMenu;
    UnArrow *selectedUnArrowPopup;
};

#endif // PAINTSCENE_H
