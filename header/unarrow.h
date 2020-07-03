#ifndef UNARROW_H
#define UNARROW_H
#include "arrow.h"

class UnArrow : public BaseArrow
{
    Q_OBJECT
public:
    explicit UnArrow(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
};

#endif // UNARROW_H
