#include "graph.h"
#include <QFile>
#include <QTextStream>

Graph::Graph(int size):
    size(size),
    arrows(0),
    name("")
{
    matrix = QVector<QVector<int>>(size, QVector<int>(size, 0));
    nodes = QVector<Node*>(size, nullptr);
}

void Graph::addNode(Node *node){
    nodes[node->getIndex()] = node;
}

void Graph::addOrientedArrow(const int first, const int second){
    ++arrows;
    matrix[first][second] = 1;
}

void Graph::addUnOrientedArrow(const int first, const int second){
    ++arrows;
    matrix[first][second] = 1;
    matrix[second][first] = 1;
}

void Graph::setName(const QString name){
    this->name = name;
}

QString Graph::getName(){
    return name;
}

void Graph::writeFile(const QString fileName){
    QFile file(fileName);
    if(!file.exists())return;
    file.open( QIODevice::WriteOnly );
    if(file.isOpen()){
        QTextStream outStream(&file);
        outStream << name.size() << " " << name << "\n";
        outStream << size << "\n";
        for(int i = 0; i < size; ++i){
            outStream << nodes[i]->getName().size() << " ";
            outStream << nodes[i]->getName() << "\n";
        }
        for(int i = 0; i < size; ++i){
            for(int j = 0; j < size; ++j){
                outStream << matrix[i][j] << " ";
            }
        }
    }
    file.close();
}

int Graph::matrixAt(const int i, const int j){
    return matrix[i][j];
}

void Graph::setMatrixAt(const int i, const int j, const int num){
    matrix[i][j] = num;
}

int Graph::getArrows(){
    return arrows;
}

int Graph::getNodes(){
    return size;
}
