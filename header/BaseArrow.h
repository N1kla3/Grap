//
// Created by nicolas on 16/06/2020.
//

#ifndef GRAP_BASEARROW_H
#define GRAP_BASEARROW_H


#include <QtCore/QObject>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QBrush>
#include <QtGui/QPen>
#include <header/node.h>

class BaseArrow : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
public:
    explicit BaseArrow(QObject *parent = 0);
    enum { Type = UserType + 3 };
    void initBetweenNodes(Node *first, Node *second);
    void setFirstNode(Node *node);
    void setSecondNode(Node *node);
    Node* getFirstNode();
    Node* getSecondNode();
    void prepareUpdate();
    void setColor(const QColor& color);
signals:
    void delete_from_node(BaseArrow *arrow);
public slots:
    void slotFirstMove();
    void slotSecondMove();
    void slot_delete();
protected:
    int height;
    qreal length;
    Node *first;
    Node *second;
    qreal calculateRotation();
    void setBetweenNodes();
    qreal calculateLength( qreal x,  qreal y)const;
    QBrush brush;
    QPen pen;

    int type() const override;
};


#endif //GRAP_BASEARROW_H
