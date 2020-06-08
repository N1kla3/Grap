#ifndef UNARROW_H
#define UNARROW_H
#include "arrow.h"

class UnArrow : public Arrow
{
    Q_OBJECT
public:
    explicit UnArrow(QObject *parent = 0);
    enum { Type = UserType + 4 };
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    int type() const override;
};

#endif // UNARROW_H
