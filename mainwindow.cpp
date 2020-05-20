#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTabWidget>

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

    //updatesEnabled(bool);
    tabber->show();
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);
}

void MainWindow::slotTimer()
{
    timer->stop();
    scene->setSceneRect(0,0, this->width(), this->height());
    ui->graphicsView->resize(this->width(), this->height());
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
    }
    scene->setFlag(SELECTION);
}

void MainWindow::on_actionNode_triggered()
{
    for(auto action : ui->toolBar->actions()){
        if(action->objectName() != "actionNode")
            action->setChecked(false);
    }
    scene->setFlag(CREATE_NODE);
}

void MainWindow::on_actionArrow_triggered()
{
    for(auto action : ui->toolBar->actions()){
        if(action->objectName() != "actionArrow")
            action->setChecked(false);
    }
    scene->setFlag(CREATE_ARROW);
}
