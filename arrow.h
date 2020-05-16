#ifndef ARROW_H
#define ARROW_H
#include <QGraphicsItem>
#include <QObject>

class Arrow : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Arrow(QObject *parent = 0);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void setBetweenNodes();
    void setFirstNode(QPointF point);
    void setSecondNode(QPointF point);
private:
    int width;
    int height;
    qreal length;
    QPointF firstNode;
    QPointF secondNode;
    qreal calculateRotation();
    qreal calculateLength( qreal x,  qreal y)const;
};

#endif // ARROW_H
