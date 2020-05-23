#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <paintscene.h>
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createGraph();
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTabWidget *tabber;


    PaintScene *scene;

private:
    Graph *currentGraph;
    void resizeEvent(QResizeEvent * event);

private slots:
    void slotTimer();
    void on_actionSelect_triggered();
    void on_actionNode_triggered();
    void on_actionArrow_triggered();
    void on_actionNew_Graph_triggered();
    void tab_changes(int index);
    void tab_close(int index);
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionunoriented_triggered();
};
#endif // MAINWINDOW_H
