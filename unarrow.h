#ifndef UNARROW_H
#define UNARROW_H
#include <arrow.h>

class UnArrow : public Arrow
{
    Q_OBJECT
public:
    explicit UnArrow(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // UNARROW_H
