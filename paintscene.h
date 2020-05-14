#ifndef PAINTSCENE_H
#define PAINTSCENE_H
#include<QGraphicsScene>
#include<QGraphicsSceneMouseEvent>
#include <QTimer>
#include <node.h>


enum EActionToDo{
    CREATE_NODE,
    CREATE_ARROW
};
class PaintScene : public QGraphicsScene
{
       Q_OBJECT
public:
    explicit PaintScene(QObject* parent = 0);
    ~PaintScene(){}

private:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QGraphicsItem *selectedItem;
};

#endif // PAINTSCENE_H
