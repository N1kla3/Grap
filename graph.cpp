#include "graph.h"

Graph::Graph(int size):
    size(size)
{
    matrix = QVector<QVector<int>>(size, QVector<int>(size, 0));
    nodes.reserve(size);
}

void Graph::addNode(Node *node){
    nodes[node->getIndex()] = node;
}

void Graph::addOrientedArrow(const int first, const int second){
    matrix[first][second] = 1;
}

void Graph::addUnOrientedArrow(const int first, const int second){
    matrix[first][second] = 1;
    matrix[second][first] = 1;
}
