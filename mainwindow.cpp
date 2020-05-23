#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTabWidget>
#include <QInputDialog>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QRandomGenerator>
#include <unarrow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new PaintScene();
    scene->setFlag(SELECTION);
    ui->graphicsView->setScene(scene);
    tabber = new QTabWidget(ui->centralwidget);

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

void MainWindow::createGraph(){
    QGraphicsView *view = qobject_cast<QGraphicsView*>(tabber->currentWidget());
    if(view){
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
                currentGraph->addUnOrientedArrow(unarrow->getFirstNode()->getIndex(), unarrow->getSecondNode()->getIndex());
            }else if(arrow){
                currentGraph->addOrientedArrow(arrow->getFirstNode()->getIndex(), arrow->getSecondNode()->getIndex());
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
    tabber->addTab(view, graphName);
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
