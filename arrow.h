#ifndef ARROW_H
#define ARROW_H
#include "node.h"

class Arrow : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Arrow(QObject *parent = 0);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void initBetweenNodes(Node *first, Node *second);
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
};

#endif // ARROW_H
