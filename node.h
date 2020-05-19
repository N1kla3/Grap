#ifndef NODE_H
#define NODE_H
#include <QGraphicsItem>
#include <QObject>

class Node : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Node(QObject *parent = 0);
    ~Node(){}
    enum{Type = UserType + 2 };
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
signals:
    void moved();
private:
    const int ELLIPSE_SIZE = 30;
    int type() const override;
};

#endif // NODE_H
