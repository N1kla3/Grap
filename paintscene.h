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

private:
    bool bDrawArrow;
    Node *firstNodeOfArrow;
    EActionToDo flag;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QGraphicsItem *selectedItem;
};

#endif // PAINTSCENE_H
