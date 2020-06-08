#include "header/graph.h"
#include <QFile>
#include <QTextStream>
#include <QQueue>
#include <QDebug>
#include<QStack>

Graph::Graph(int size):
    size(size),
    arrows(0),
    name("")
{
    matrix = QVector<QVector<int>>(size, QVector<int>(size, 0));
    nodes = QVector<Node*>(size, nullptr);
    check = QVector<bool>(size, false);
}

void Graph::addNode(Node *node){
    nodes[node->getIndex()] = node;
}

void Graph::addOrientedArrow(const int first, const int second, Arrow *arrow){
    orArrows[qMakePair(first, second)] = arrow;
    ++arrows;
    matrix[first][second] = 1;
}

void Graph::addUnOrientedArrow(const int first, const int second, UnArrow *arrow){
    unArrows[qMakePair(first, second)] = arrow;
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

int Graph::getDegree(){
    int res = 0;
    for(int index = 0; index < size; ++index){
        for(int i = 0; i < size; ++i){
            if(matrix[index][i] != 0){
                ++res;
            }
            if(matrix[i][index] != 0){
                if(matrix[index][i] == 0){
                    ++res;
                }
            }
        }
    }
    return res;
}

int Graph::getSpecDegree(Node *node){
    int res = 0;
    int index = node->getIndex();
    for(int i = 0; i < size; ++i){
        if(matrix[index][i] != 0){
            ++res;
        }
        if(matrix[i][index] != 0){
            if(matrix[index][i] == 0){
                ++res;
            }
        }
    }
    return res;
}

QString Graph::getMatrix(){
    QString res = "";
    QTextStream out(&res);
    for(auto i : matrix){
        out << "\n";
        for(auto k : i){
            out << k << " ";
        }
    }
    return res;
}

void Graph::bfs(){
    QVector<QVector<int>> mat(size, QVector<int>(size, 0));
    for(int i = 0; i < size; ++i){
        check = QVector<bool>(size, false);
        QQueue<int> q;
        check[i] = true;
        q.enqueue(i);
        while(!q.empty()){
            int s = q.dequeue();
            for(int k = 0; k < size; ++k){
                if(matrix[s][k] && !check[k]){
                    mat[s][k] = 1;
                    q.enqueue(k);
                    check[k] = true;
                }
            }
        }
        bool bCheck = true;
        for(auto k : check){
            if(!k){
                bCheck = false;
                break;
            }
        }
        if(bCheck){
            for(int k = 0; k < size; k++){
                for(int j = 0; j < size; j++){
                    if(!mat[k][j] && matrix[k][j]){
                        if(matrix[j][k] && mat[j][k]){
                            matrix[j][k] = 0;
                            continue;
                        }

                        if(unArrows.contains(qMakePair(k,j))){
                            unArrows[qMakePair(k,j)]->slot_delete();
                        }else if(unArrows.contains(qMakePair(j, k))){
                            unArrows[qMakePair(j, k)]->slot_delete();
                        }
                        if(orArrows.contains(qMakePair(k,j))){
                            orArrows[qMakePair(k,j)]->slot_delete();
                        }
                    }
                }
            }

            return;
        }else mat = QVector<QVector<int>>(size, QVector<int>(size, 0));
    }
}

void Graph::dfs(int i) {
    if (check[i]) {
    return;
    }
    check[i] = true;
    for (int j = 0; j < size; ++j) {
        if (matrix[i][j]) {
            dfs(j);
        }
    }
}

void Graph::bfsDepth(int i, int depth){
    check = QVector<bool>(size, false);
    QQueue<QPair<int, int>> q;
    check[i] = true;
    q.enqueue(qMakePair(i, depth));
    while(!q.empty()){
        auto s = q.dequeue();
        radius = qMax(radius, s.second);
        diametr = qMax(diametr, s.second);
        for(int k = 0; k < size; ++k){
            if(matrix[s.first][k] && !check[k]){
                q.enqueue(qMakePair(k, s.second+1));
                check[k] = true;
            }
        }
    }

}

QString Graph::isTree() {
    int edges = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (matrix[i][j] == 1) {
                if(matrix[j][i])matrix[j][i] = 2;
                edges++;
            }else if(matrix[i][j] == 2)matrix[i][j] = 1;
        }
    }
    if (edges != size - 1) {
        return "False";
    }
    for(int index = 0; index < size; ++index){
        dfs(index);
        bool iscorrect = true;
        for (int i = 0; i < size; ++i) {
            if (!check[i]) {
                iscorrect = false;
            }
        }
        if(iscorrect)return "True";
        check = QVector<bool>(size, false);
    }
    return "False";
}

bool Graph::dfsHamil(int i, int root, QStack<QPair<int, int>> &q){
    bool res = false;
    for(int k = 0; k < size; ++k){
        if(matrix[i][k] && !check[k]){
            check[k] = true;
            q.push(qMakePair(i,k));
            qDebug() << "enq " << i << " " << k;
            if(dfsHamil(k, root, q)){
                res = true;
                break;
            }else {
                check[k] = false;
                q.pop();
                qDebug() << "deq " << i << " " << k;
            }
        }
    }
    if(i == root){
        res = true;
        for(auto i : check)
            if(!i)res = i;
    }
    return res;
}

bool Graph::hamilton(){
    for(int vertex = 0; vertex < size; ++vertex){
        QStack<QPair<int, int>> q;
        check = QVector<bool>(size, false);
        if(dfsHamil(vertex, vertex, q)){
            while(!q.empty()){
                auto pair = q.pop();
                qDebug() << pair.first << " " << pair.second << "\n";
                if(orArrows.contains(pair))
                    orArrows[pair]->setColor(Qt::red);
                else if(unArrows.contains(pair))
                    unArrows[pair]->setColor(Qt::red);
                else if(unArrows.contains(qMakePair(pair.second, pair.first)))
                    unArrows[qMakePair(pair.second, pair.first)]->setColor(Qt::red);

            }
            return true;
        }
    }
    return false;
}

void Graph::centerGraph(int &radius, int &diametr){
    this->diametr = 0;
    int rad = 10e8;
    for(int i = 0; i < size; ++i){
        this->radius = 0;
        bfsDepth(i, 0);
        nodes[i]->setExcent(this->radius);
        rad = qMin(rad, this->radius);
    }
    radius = rad;
    diametr = this->diametr;
    for(auto i : nodes){
        if(i->getExcent() == rad)
            nodes[i->getIndex()]->setColor(Qt::red);
    }
}

QVector<QVector<int>> Graph::getMatrix2(){
    return  matrix;
}
