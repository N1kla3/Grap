#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "unarrow.h"
#include <QVector>

class Graph
{
public:
    Graph(int size);
    void addNode(Node *node);
    void addOrientedArrow(const int first, const int second, Arrow *arrow);
    void addUnOrientedArrow(const int first, const int second, UnArrow *arrow);
    QString getName();
    void setName(const QString name);
    void writeFile(const QString fileName);
    int matrixAt(const int i, const int j);
    void setMatrixAt(const int i, const int j, const int num);
    int getArrows();
    int getNodes();
    int getDegree();
    int getSpecDegree(Node *node);
    QString getMatrix();
    void dfs(int i);
    void bfs();
    QString isTree();
public slots:
    void slot_degree(Node *node);
private:
    QVector<bool> check;
    int size;
    int arrows;
    QString name;
    QVector<Node*> nodes;
    QMap<QPair<int, int>, Arrow*> orArrows;
    QMap<QPair<int, int>, UnArrow*> unArrows;
    QVector<QVector<int>> matrix;
};

#endif // GRAPH_H
