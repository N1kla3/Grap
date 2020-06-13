#ifndef MININODE_H
#define MININODE_H
#include "node.h"

class Mininode : public Node
{
    Q_OBJECT
public:
    enum{Type = UserType + 2};
    Mininode(QObject *parent = 0);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    int type() const override;
    const int DIAMETR = 20;
    const int ALEFT = -10, ATOP = -10;
};

#endif // MININODE_H
