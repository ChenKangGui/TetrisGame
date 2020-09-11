#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QPropertyAnimation>
#include <QPixmap>
#include <QGraphicsBlurEffect>
#include <QGraphicsProxyWidget>
#include "box.h"
#include <QMessageBox>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    GroupBox *groupBox;
    GroupBox *nextGroupBox;

    QTimer *timer;
    QGraphicsScene *mainscene;
    QGraphicsTextItem *scoreItem;

    QList<int > rows;

public slots:
    void createNewBox();
    void moveBox();
    void gameOver();
};

#endif // MAINWINDOW_H

