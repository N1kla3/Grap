#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTabWidget>
#include <QInputDialog>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QRandomGenerator>

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
    view->resize(this->width(), this->height());
    view->scene()->setSceneRect(0,0, this->width(), this->height());
    tabber->resize(this->width(), this->height());
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
        view->resize(this->width(), this->height());
        view->scene()->setSceneRect(0,0, this->width(), this->height());
        tabber->addTab(view, text);
    }
}

void MainWindow::tab_changes(int index){
    on_actionSelect_triggered();
    Q_UNUSED(index);
}

void MainWindow::tab_close(int index){
    tabber->removeTab(index);
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
            if(node){
                currentGraph->addNode(node);
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

    PaintScene *scene = new PaintScene();
    scene->setFlag(SELECTION);
    QGraphicsView *view = new QGraphicsView();
    view->setScene(scene);
    view->resize(this->width(), this->height());
    view->scene()->setSceneRect(0,0, this->width(), this->height());

    QString fileName = QFileDialog::getOpenFileName(this, "open", ".grap", tr("Grap file (*.grap)"));
    QFile file(fileName);
    QString graphName = "";
    file.open(QIODevice::ReadOnly);
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
        for(int i = 0; i < size; ++i){
            Node *node = new Node();
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
    }
    file.close();
    tabber->addTab(view, graphName);
}
