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
private:
    int width;
    int height;
    QPointF firstNode;
    QPointF secondNode;
    qreal calculateRotation()const;
    qreal calculateLength(const qreal x, const qreal y)const;
};

#endif // ARROW_H
