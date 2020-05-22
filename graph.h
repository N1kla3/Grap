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
private:
    int size;
    QString name;
    QVector<Node*> nodes;
    QVector<QVector<int>> matrix;
};

#endif // GRAPH_H
