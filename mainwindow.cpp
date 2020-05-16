#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new PaintScene();
    scene->setFlag(CREATE_ARROW);
    ui->graphicsView->setScene(scene);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);
}

void MainWindow::slotTimer()
{
    timer->stop();
    scene->setSceneRect(0,0, this->width(), this->height());
    ui->graphicsView->resize(this->width()-40, this->height()-100);
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


void MainWindow::on_actionCursor_toggled(bool arg1)
{
    if(arg1)scene->setFlag(SELECTION);
}
