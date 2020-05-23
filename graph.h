#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include <QVector>

class Graph
{
public:
    Graph(int size);
    void addNode(Node *node);
    void addOrientedArrow(const int first, const int second);
    void addUnOrientedArrow(const int first, const int second);
    QString getName();
    void setName(const QString name);
    void writwFile(const QString fileName);
public slots:
    void slot_degree(Node *node);
private:
    int size;
    int arrows;
    QString name;
    QVector<Node*> nodes;
    QVector<QVector<int>> matrix;
};

#endif // GRAPH_H
