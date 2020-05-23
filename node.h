#ifndef NODE_H
#define NODE_H
#include <QGraphicsItem>
#include <QObject>
#include <QBrush>
#include <QPen>

class Node : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Node(QObject *parent = 0);
    ~Node(){}
    enum{Type = UserType + 2 };
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    void setColor(const QColor color);
    void setIndex(int index);
    void setName(const QString name);
    QString getName();
    int getIndex();
signals:
    void moved();
    void deleted();
private:
    QGraphicsTextItem *name;
    int index;
    QBrush brush;
    QPen pen;
    const int ELLIPSE_SIZE = 30;
    int type() const override;
};

#endif // NODE_H
