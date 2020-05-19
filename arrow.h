#ifndef ARROW_H
#define ARROW_H
#include "mininode.h"

class Arrow : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Arrow(QObject *parent = 0);
    enum { Type = UserType + 1 };
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void initBetweenNodes(Node *first, Node *second);
    void setFirstNode(Node *node);
    void setSecondNode(Node *node);
    Node* getFirstNode();
    Node* getSecondNode();
public slots:
    void slotFirstMove();
    void slotSecondMove();
private:
    int width;
    int height;
    qreal length;
    Node *first;
    Node *second;
    qreal calculateRotation();
    void setBetweenNodes();
    qreal calculateLength( qreal x,  qreal y)const;

    int type() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // ARROW_H
