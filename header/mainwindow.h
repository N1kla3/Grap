#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintscene.h"
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
    void createGraph(QGraphicsView *view);
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTabWidget *tabber;


    PaintScene *scene;

private:
    Graph *currentGraph;
    void resizeEvent(QResizeEvent * event);
    QVector<QVector<int>> kroneker(const QVector<QVector<int>> &first, const QVector<QVector<int>> &second);
    QVector<QVector<int>> kronekerHelper(const QVector<QVector<int>> &first, const QVector<QVector<int>> &second);
    QVector<QVector<int>> graphUnion(const QVector<QVector<int>> &first, const QVector<QVector<int>> &second);
    QVector<QVector<int>> graphSection(const QVector<QVector<int>> &first, const QVector<QVector<int>> &second);
    QVector<QVector<int>> graphAddition(const QVector<QVector<int>> &first);

private slots:
    void slot_degree(Node *node);
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
    void on_action_triggered();
    void on_actionItems_triggered();
    void on_actionMatrix_triggered();
    void on_actionMake_Tree_triggered();
    void on_actionHamilton_Cycle_triggered();
    void on_actionDiametr_Radius_triggered();
    void on_actionDekart_triggered();
    void on_actionUnion_triggered();
    void on_actionIntersection_triggered();
    void on_actionAddition_triggered();
};
#endif // MAINWINDOW_H
