#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTabWidget>
#include <QInputDialog>
#include <QFileDialog>
#include <QDir>

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
        currentGraph = new Graph(i);
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
    QString fileName = QFileDialog::getSaveFileName(this, "save", ".txt.grap", tr("Graph file (*.txt.grap)"));
    createGraph();
    currentGraph->writwFile(fileName);
}
