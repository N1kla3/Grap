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
    void writeFile(const QString fileName);
    int matrixAt(const int i, const int j);
    void setMatrixAt(const int i, const int j, const int num);
    int getArrows();
    int getNodes();
    int getDegree();
    int getSpecDegree(Node *node);
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
