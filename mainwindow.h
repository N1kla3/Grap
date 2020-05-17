#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <paintscene.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer;      /* Определяем таймер для подготовки актуальных размеров
                         * графической сцены
                         * */
    PaintScene *scene;  // Объявляем кастомную графическую сцену

private:
    /* Переопределяем событие изменения размера окна
     * для пересчёта размеров графической сцены
     * */
    void resizeEvent(QResizeEvent * event);

private slots:
    void slotTimer();
    void on_actionSelect_triggered();
    void on_actionNode_triggered();
    void on_actionArrow_triggered();
};
#endif // MAINWINDOW_H
