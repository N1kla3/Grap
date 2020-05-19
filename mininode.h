#ifndef MININODE_H
#define MININODE_H
#include "node.h"

class Mininode : public Node
{
    Q_OBJECT
public:
    Mininode(QObject *parent = 0);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // MININODE_H
