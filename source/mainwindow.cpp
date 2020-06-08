
#include "ui_mainwindow.h"
#include <QTabWidget>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QTextStream>
#include <QRandomGenerator>
#include <header/unarrow.h>
#include <header/mainwindow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentGraph(nullptr)
{
    ui->setupUi(this);
    scene = new PaintScene();
    scene->setFlag(SELECTION);
    ui->graphicsView->setScene(scene);
    tabber = new QTabWidget(ui->centralwidget);
    connect(scene, &PaintScene::calc_degree, this, &MainWindow::slot_degree);
    tabber->insertTab(1, ui->graphicsView, "view");
    tabber->setTabsClosable(true);
    tabber->setMovable(true);
    connect(tabber, &QTabWidget::tabBarClicked, this, &MainWindow::tab_changes);
    connect(tabber, &QTabWidget::tabCloseRequested, this, &MainWindow::tab_close);
    //updatesEnabled(bool);
    tabber->show();
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);
}

void MainWindow::slotTimer()
{
    timer->stop();
    QGraphicsView *view = qobject_cast<QGraphicsView*>(tabber->currentWidget());
    view->resize(this->width()-20, this->height()-100);
    view->scene()->setSceneRect(0,0, this->width()-20, this->height()-100);
    tabber->resize(this->width(), this->height()-60);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    timer->start(20);
    QWidget::resizeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSelect_triggered()
{
    for(auto action : ui->toolBar->actions()){
        if(action->objectName() != "actionSelect")
            action->setChecked(false);
        else{
            action->setChecked(true);
        }
    }
    QGraphicsView *view = qobject_cast<QGraphicsView*>(tabber->currentWidget());
    if(view){
        PaintScene *currScene = qobject_cast<PaintScene*>(view->scene());
        currScene->setFlag(SELECTION);
    }
}

void MainWindow::on_actionNode_triggered()
{
    for(auto action : ui->toolBar->actions()){
        if(action->objectName() != "actionNode")
            action->setChecked(false);
        else{
            action->setChecked(true);
        }
    }
    QGraphicsView *view = qobject_cast<QGraphicsView*>(tabber->currentWidget());
    if(view){
        PaintScene *currScene = qobject_cast<PaintScene*>(view->scene());
        currScene->setFlag(CREATE_NODE);
    }

}

void MainWindow::on_actionArrow_triggered()
{
    for(auto action : ui->toolBar->actions()){
        if(action->objectName() != "actionArrow")
            action->setChecked(false);
        else{
            action->setChecked(true);
        }
    }
    QGraphicsView *view = qobject_cast<QGraphicsView*>(tabber->currentWidget());
    if(view){
        PaintScene *currScene = qobject_cast<PaintScene*>(view->scene());
        currScene->setFlag(CREATE_ARROW);
    }
}

void MainWindow::on_actionNew_Graph_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("File Name"),
                                             tr("Enter filename:"), QLineEdit::Normal,
                                             "graph", &ok);

    if(ok){
        PaintScene *scene = new PaintScene();
        scene->setFlag(SELECTION);
        connect(scene, &PaintScene::calc_degree, this, &MainWindow::slot_degree);
        QGraphicsView *view = new QGraphicsView();
        view->setScene(scene);
        view->resize(this->width()-20, this->height()-100);
        view->setRenderHint(QPainter::Antialiasing);
        view->setRenderHint(QPainter::TextAntialiasing);
        view->setRenderHint(QPainter::SmoothPixmapTransform);
        view->setRenderHint(QPainter::HighQualityAntialiasing);
        view->scene()->setSceneRect(0,0, this->width()-20, this->height()-100);
        tabber->addTab(view, text);
    }
}

void MainWindow::tab_changes(int index){
    on_actionSelect_triggered();
    Q_UNUSED(index);
}

void MainWindow::tab_close(int index){    
    QGraphicsView *view = qobject_cast<QGraphicsView*>(tabber->widget(index));
    tabber->removeTab(index);
    auto scene = view->scene();
    for(QGraphicsItem *i : scene->items()){
        delete i;
    }
    scene->deleteLater();
    view->deleteLater();
}

void MainWindow::createGraph(QGraphicsView *view = nullptr){
    if(!view)view = qobject_cast<QGraphicsView*>(tabber->currentWidget());
    if(view){
        auto scene = qobject_cast<PaintScene*>(view->scene());
        scene->deleteMiniNodes();
        int i = 0;
        QList<QGraphicsItem*> items = view->items();
        for(QGraphicsItem *item : items){
            Node *node = qgraphicsitem_cast<Node*>(item);
            if(node){
                node->setIndex(i);
                ++i;
            }
        }

        int index = tabber->currentIndex();
        QString currentTabText = tabber->tabText(index);
        currentGraph = new Graph(i);
        currentGraph->setName(currentTabText);
        for(QGraphicsItem *item : items){
            Node *node = qgraphicsitem_cast<Node*>(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow*>(item);
            UnArrow *unarrow = qgraphicsitem_cast<UnArrow*>(item);
            if(node){
                currentGraph->addNode(node);
            }else if(unarrow){
                currentGraph->addUnOrientedArrow(unarrow->getFirstNode()->getIndex(), unarrow->getSecondNode()->getIndex(), unarrow);
            }else if(arrow){
                currentGraph->addOrientedArrow(arrow->getFirstNode()->getIndex(), arrow->getSecondNode()->getIndex(), arrow);
            }
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "save", ".grap", tr("Grap file (*.grap)"));
    createGraph();
    currentGraph->writeFile(fileName);
}

void MainWindow::on_actionOpen_triggered()
{
    QRandomGenerator generator;

    QString fileName = QFileDialog::getOpenFileName(this, "open", ".grap", tr("Grap file (*.grap)"));

    QFile file(fileName);
    if(!file.exists())return;
    QString graphName = "";
    file.open(QIODevice::ReadOnly);

    PaintScene *scene = new PaintScene();
    scene->setFlag(SELECTION);
    connect(scene, &PaintScene::calc_degree, this, &MainWindow::slot_degree);
    QGraphicsView *view = new QGraphicsView();
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::TextAntialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);
    view->setRenderHint(QPainter::HighQualityAntialiasing);
    view->setScene(scene);
    view->resize(this->width()-20, this->height()-100);
    view->scene()->setSceneRect(0,0, this->width()-20, this->height()-100);



    if(file.isOpen()){

        int nameSize;
        QTextStream input(&file);
        input >> nameSize;
        QChar a;
        input >> a;
        for(int i = 0; i < nameSize; ++i){
            input >> a;
            graphName.append(a);
        }
        int size = 0;
        input >> size;
        QVector<Node*> nodes(size);
        for(int i = 0; i < size; ++i){
            Node *node = new Node();
            nodes[i] = node;
            int strSize;
            QString nodeName = "";
            input >> strSize;
            input >> a;
            for(int j = 0; j < strSize; ++j){
                input >> a;
                nodeName.append(a);
            }
            node->setPos(generator.bounded(view->width()), generator.bounded(view->height()));
            scene->addItem(node);
            node->setName(nodeName);

        }
        QVector<QVector<int>> matrix(size, QVector<int>(size, 0));
        for(int i = 0; i < size; ++i){
            for(int j = 0; j < size; ++j){
                input >> matrix[i][j];
            }
        }
        for(int i = 0; i < size; ++i){
            for(int j = 0; j < size; ++j){
                if(matrix[i][j]){
                    if(matrix[j][i]){
                        UnArrow *arrow = new UnArrow();
                        arrow->initBetweenNodes(nodes[i], nodes[j]);
                        connect(arrow, &Arrow::delete_from_node, scene, &PaintScene::slot_delete_arrow);
                        scene->addItem(arrow);
                        matrix[j][i] = 0;
                    }else{
                        Arrow *arrow = new Arrow();
                        arrow->initBetweenNodes(nodes[i], nodes[j]);
                        connect(arrow, &Arrow::delete_from_node, scene, &PaintScene::slot_delete_arrow);
                        scene->addItem(arrow);
                    }
                }
            }
        }
    }
    file.close();
    tabber->insertTab(1, view, graphName);
}

void MainWindow::on_actionunoriented_triggered()
{
    for(auto action : ui->toolBar->actions()){
        if(action->objectName() != "actionunoriented")
            action->setChecked(false);
        else{
            action->setChecked(true);
        }
    }
    QGraphicsView *view = qobject_cast<QGraphicsView*>(tabber->currentWidget());
    if(view){
        PaintScene *currScene = qobject_cast<PaintScene*>(view->scene());
        currScene->setFlag(CREATE_UNORIENTED);
    }
}

void MainWindow::on_action_triggered()
{
    if(tabber->currentWidget()){bool ok;
        QString name = QInputDialog::getText(this, tr("Set Name"),tr("Enter name:"),
                                             QLineEdit::Normal,"name", &ok);
        if(ok)
            tabber->setTabText(tabber->currentIndex(), name);
    }
}

void MainWindow::on_actionItems_triggered()
{
    if(currentGraph)delete currentGraph;
    if(tabber->currentWidget()){
        createGraph();
        QMessageBox::information(this, "Items of Graph", "vertices :"
                                 +  QString::number(currentGraph->getNodes()) + "\narrows :"
                                 + QString::number(currentGraph->getArrows()));
    }
}

void MainWindow::slot_degree(Node *node){
    if(currentGraph)delete currentGraph;
    createGraph();
    QMessageBox::information(this, "Degree of Graph", "All :"
                             +  QString::number(currentGraph->getDegree())
                             + "\nFor selected :"
                             + QString::number(currentGraph->getSpecDegree(node)));
}

void MainWindow::on_actionMatrix_triggered()
{
    if(currentGraph)delete currentGraph;
    createGraph();
    QMessageBox::information(this, "Matrix", "Is a Tree :"
                             +  currentGraph->isTree()
                             + "\n\nMatrix :"
                             + currentGraph->getMatrix());
}


void MainWindow::on_actionMake_Tree_triggered()
{
    if(currentGraph)delete currentGraph;
    createGraph();
    currentGraph->bfs();
}

void MainWindow::on_actionHamilton_Cycle_triggered()
{
    if(currentGraph)delete currentGraph;
    createGraph();
    if(!currentGraph->hamilton()){
        QMessageBox::information(this, "Hamilton", "NO HAMILTON CYCLE");
    }
}

void MainWindow::on_actionDiametr_Radius_triggered()
{
    if(currentGraph)delete currentGraph;
    createGraph();
    int radius = 0;
    int diametr = 0;
    currentGraph->centerGraph(radius, diametr);
    QMessageBox::information(this, "Center Graph", "Radius :"
                             +  QString::number(radius)
                             + "\n\nDiametr :"
                             + QString::number(diametr));
}

void MainWindow::on_actionDekart_triggered()
{
    QRandomGenerator generator;
    createGraph();
    Graph *graph1 = currentGraph;
    on_actionOpen_triggered();
    createGraph(qobject_cast<QGraphicsView*>(tabber->widget(1)));
    Graph *graph2 = currentGraph;
    auto res = kroneker(graph1->getMatrix2(), graph2->getMatrix2());
    const int size = res.size();

    PaintScene *scene = new PaintScene();
    scene->setFlag(SELECTION);
    connect(scene, &PaintScene::calc_degree, this, &MainWindow::slot_degree);
    QGraphicsView *view = new QGraphicsView();
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::TextAntialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);
    view->setRenderHint(QPainter::HighQualityAntialiasing);
    view->setScene(scene);
    view->resize(this->width()-20, this->height()-100);
    view->scene()->setSceneRect(0,0, this->width()-20, this->height()-100);

    QVector<Node*> nodes(size);
    for(int i = 0; i < size; ++i){
        Node *node = new Node();
        nodes[i] = node;
        node->setPos(generator.bounded(view->width()), generator.bounded(view->height()));
        scene->addItem(node);
    }
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            if(res[i][j]){
                if(res[j][i]){
                    UnArrow *arrow = new UnArrow();
                    arrow->initBetweenNodes(nodes[i], nodes[j]);
                    connect(arrow, &Arrow::delete_from_node, scene, &PaintScene::slot_delete_arrow);
                    scene->addItem(arrow);
                    res[j][i] = 0;
                }else{
                    Arrow *arrow = new Arrow();
                    arrow->initBetweenNodes(nodes[i], nodes[j]);
                    connect(arrow, &Arrow::delete_from_node, scene, &PaintScene::slot_delete_arrow);
                    scene->addItem(arrow);
                }
            }
        }
    }

    tabber->insertTab(1, view, "Decart");
}

QVector<QVector<int>> MainWindow::kroneker(const QVector<QVector<int> > &first, const QVector<QVector<int> > &second){
    const int firstSize = first.size();
    const int secondSize = second.size();
    const int size = firstSize * secondSize;
    QVector<QVector<int>> firstOne(firstSize, QVector<int>(firstSize, 0));
    for(int i = 0; i < firstSize; ++i){firstOne[i][i] = 1;}
    QVector<QVector<int>> secondOne(secondSize, QVector<int>(secondSize, 0));
    for(int i = 0; i < secondSize; ++i){secondOne[i][i] = 1;}
    auto onePlus = kronekerHelper(first, secondOne);
    auto twoPlus = kronekerHelper(firstOne, second);
    QVector<QVector<int>> result(size, QVector<int>(size, 0));
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            if(onePlus[i][j] || twoPlus[i][j])
                result[i][j] = 1;
        }
    }
    return result;
}

QVector<QVector<int>> MainWindow::kronekerHelper(const QVector<QVector<int>> &first, const QVector<QVector<int>> &second){
    int fSize = first.size();
    int sSize = second.size();
    int size = fSize*sSize;
    QVector<QVector<int>> result(size, QVector<int>(size, 0));
    for (int z1(0); z1 < fSize; ++z1) {
            for (int z2(0); z2 < sSize; ++z2) {
                for (int z3(0); z3 < fSize; ++z3) {
                    for (int z4(0); z4 < sSize; ++z4) {
                        result[z1*sSize + z2][z3*sSize + z4] = first[z1][z3] * second[z2][z4];
                    }
                }
            }
        }
    return result;
}

QVector<QVector<int>> MainWindow::graphUnion(const QVector<QVector<int>> &first, const QVector<QVector<int>> &second){
    int size = qMax(first.size(), second.size());
    QVector<QVector<int>> res(size, QVector<int>(size, 0));
    for(int i = 0; i < second.size(); ++i){
        for(int j = 0; j < second.size(); ++j){
            if(second[i][j])res[i][j] = 1;
        }
    }
    for(int i = 0; i < first.size(); ++i){
        for(int j = 0; j < first.size(); ++j){
            if(first[i][j])res[i][j] = 1;
        }
    }
    return res;
}

QVector<QVector<int>> MainWindow::graphSection(const QVector<QVector<int>> &first, const QVector<QVector<int>> &second){
    int size = qMin(first.size(), second.size());
    QVector<QVector<int>> res(size, QVector<int>(size, 0));
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            if(second[i][j] && first[i][j])res[i][j] = 1;
        }
    }
    return res;
}

QVector<QVector<int>> MainWindow::graphAddition(const QVector<QVector<int>> &first){
    int size = first.size();
    QVector<QVector<int>> res(size, QVector<int>(size, 0));
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            if(i == j)continue;
            if(first[i][j])res[i][j] = 0;
            else res[i][j] = 1;
        }
    }
    return res;
}

void MainWindow::on_actionUnion_triggered()
{
    QRandomGenerator generator;
    createGraph();
    Graph *graph1 = currentGraph;
    on_actionOpen_triggered();
    createGraph(qobject_cast<QGraphicsView*>(tabber->widget(1)));
    Graph *graph2 = currentGraph;
    auto res = graphUnion(graph1->getMatrix2(), graph2->getMatrix2());
    const int size = res.size();

    PaintScene *scene = new PaintScene();
    scene->setFlag(SELECTION);
    connect(scene, &PaintScene::calc_degree, this, &MainWindow::slot_degree);
    QGraphicsView *view = new QGraphicsView();
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::TextAntialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);
    view->setRenderHint(QPainter::HighQualityAntialiasing);
    view->setScene(scene);
    view->resize(this->width()-20, this->height()-100);
    view->scene()->setSceneRect(0,0, this->width()-20, this->height()-100);

    QVector<Node*> nodes(size);
    for(int i = 0; i < size; ++i){
        Node *node = new Node();
        nodes[i] = node;
        node->setPos(generator.bounded(view->width()), generator.bounded(view->height()));
        scene->addItem(node);
    }
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            if(res[i][j]){
                if(res[j][i]){
                    UnArrow *arrow = new UnArrow();
                    arrow->initBetweenNodes(nodes[i], nodes[j]);
                    connect(arrow, &Arrow::delete_from_node, scene, &PaintScene::slot_delete_arrow);
                    scene->addItem(arrow);
                    res[j][i] = 0;
                }else{
                    Arrow *arrow = new Arrow();
                    arrow->initBetweenNodes(nodes[i], nodes[j]);
                    connect(arrow, &Arrow::delete_from_node, scene, &PaintScene::slot_delete_arrow);
                    scene->addItem(arrow);
                }
            }
        }
    }

    tabber->insertTab(1, view, "Union");
}

void MainWindow::on_actionIntersection_triggered()
{
    QRandomGenerator generator;
    createGraph();
    Graph *graph1 = currentGraph;
    on_actionOpen_triggered();
    createGraph(qobject_cast<QGraphicsView*>(tabber->widget(1)));
    Graph *graph2 = currentGraph;
    auto res = graphSection(graph1->getMatrix2(), graph2->getMatrix2());
    const int size = res.size();

    PaintScene *scene = new PaintScene();
    scene->setFlag(SELECTION);
    connect(scene, &PaintScene::calc_degree, this, &MainWindow::slot_degree);
    QGraphicsView *view = new QGraphicsView();
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::TextAntialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);
    view->setRenderHint(QPainter::HighQualityAntialiasing);
    view->setScene(scene);
    view->resize(this->width()-20, this->height()-100);
    view->scene()->setSceneRect(0,0, this->width()-20, this->height()-100);

    QVector<Node*> nodes(size);
    for(int i = 0; i < size; ++i){
        Node *node = new Node();
        nodes[i] = node;
        node->setPos(generator.bounded(view->width()), generator.bounded(view->height()));
        scene->addItem(node);
    }
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            if(res[i][j]){
                if(res[j][i]){
                    UnArrow *arrow = new UnArrow();
                    arrow->initBetweenNodes(nodes[i], nodes[j]);
                    connect(arrow, &Arrow::delete_from_node, scene, &PaintScene::slot_delete_arrow);
                    scene->addItem(arrow);
                    res[j][i] = 0;
                }else{
                    Arrow *arrow = new Arrow();
                    arrow->initBetweenNodes(nodes[i], nodes[j]);
                    connect(arrow, &Arrow::delete_from_node, scene, &PaintScene::slot_delete_arrow);
                    scene->addItem(arrow);
                }
            }
        }
    }

    tabber->insertTab(1, view, "Intersection");
}

void MainWindow::on_actionAddition_triggered()
{
    QRandomGenerator generator;
    createGraph();
    auto res = graphAddition(currentGraph->getMatrix2());
    const int size = res.size();

    PaintScene *scene = new PaintScene();
    scene->setFlag(SELECTION);
    connect(scene, &PaintScene::calc_degree, this, &MainWindow::slot_degree);
    QGraphicsView *view = new QGraphicsView();
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::TextAntialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);
    view->setRenderHint(QPainter::HighQualityAntialiasing);
    view->setScene(scene);
    view->resize(this->width()-20, this->height()-100);
    view->scene()->setSceneRect(0,0, this->width()-20, this->height()-100);

    QVector<Node*> nodes(size);
    for(int i = 0; i < size; ++i){
        Node *node = new Node();
        nodes[i] = node;
        node->setPos(generator.bounded(view->width()), generator.bounded(view->height()));
        scene->addItem(node);
    }
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            if(res[i][j]){
                if(res[j][i]){
                    UnArrow *arrow = new UnArrow();
                    arrow->initBetweenNodes(nodes[i], nodes[j]);
                    connect(arrow, &Arrow::delete_from_node, scene, &PaintScene::slot_delete_arrow);
                    scene->addItem(arrow);
                    res[j][i] = 0;
                }else{
                    Arrow *arrow = new Arrow();
                    arrow->initBetweenNodes(nodes[i], nodes[j]);
                    connect(arrow, &Arrow::delete_from_node, scene, &PaintScene::slot_delete_arrow);
                    scene->addItem(arrow);
                }
            }
        }
    }

    tabber->insertTab(1, view, "Addition");
}
