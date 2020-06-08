#ifndef ARROW_H
#define ARROW_H
#include "mininode.h"

class Arrow : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Arrow(QObject *parent = 0);
    enum { Type = UserType + 3 };
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void initBetweenNodes(Node *first, Node *second);
    void setFirstNode(Node *node);
    void setSecondNode(Node *node);
    Node* getFirstNode();
    Node* getSecondNode();
    void prepareUpdate();
    void setColor(const QColor color);
signals:
    void delete_from_node(Arrow *arrow);
public slots:
    void slotFirstMove();
    void slotSecondMove();
    void slot_delete();
protected:
    int width;
    int height;
    qreal length;
    Node *first;
    Node *second;
    qreal calculateRotation();
    void setBetweenNodes();
    qreal calculateLength( qreal x,  qreal y)const;
    QBrush brush;
    QPen pen;

    int type() const override;
};

#endif // ARROW_H
