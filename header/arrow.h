#ifndef ARROW_H
#define ARROW_H
#include "mininode.h"
#include "BaseArrow.h"

class Arrow : public BaseArrow
{
    Q_OBJECT
public:
    explicit Arrow(QObject *parent = 0);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // ARROW_H
