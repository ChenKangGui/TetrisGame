#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/icon.png"));
    setWindowTitle(tr("俄罗斯方块"));
    setFixedSize(800, 803);

    QGraphicsView *mainview = new QGraphicsView(this);
    mainview->setFixedSize(800, 803);
    mainview->setStyleSheet("padding: 0px; border: 0px;");
    mainview->setBackgroundBrush(QPixmap(":/1.png").scaled(mainview->width(), mainview->height()));
    mainview->setRenderHint(QPainter::Antialiasing);//使用抗锯齿的方式渲染
    mainview->setCacheMode(QGraphicsView::CacheBackground);//设置缓存背景，这样可以加快渲染速度

    mainscene = new QGraphicsScene();
    mainscene->setSceneRect(0, 0, 800, 803);
    mainview->setScene(mainscene);

    mainscene->addLine(-1, -1, 401, -1);
    mainscene->addLine(-1, -1, -1, 801);
    mainscene->addLine(-1, 801, 401, 801);
    mainscene->addLine(401, -1, 401, 801);

    groupBox = new GroupBox();
    mainscene->addItem(groupBox);
    groupBox->createBox(QPoint(160, 0));
    groupBox->setFocus();
    connect(groupBox, &GroupBox::needNewBox, this, &MainWindow::createNewBox);
    connect(groupBox, &GroupBox::gameOver_signal, this, &MainWindow::gameOver);

    nextGroupBox = new GroupBox();
    nextGroupBox->createBox(QPoint(600, 200));
    mainscene->addItem(nextGroupBox);

    scoreItem = new QGraphicsTextItem("0");
    scoreItem->setPos(500, 600);
    scoreItem->setFont(QFont("Times", 50, QFont::Bold));
    scoreItem->setDefaultTextColor(Qt::green);
    mainscene->addItem(scoreItem);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, groupBox, &GroupBox::moveOneStep);
    timer->start(800);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createNewBox()
{
    for(int y=801; y>=0; y -= 40)
    {
        QList<QGraphicsItem *> list = mainscene->items(QRectF(-1, y, 402, 80), Qt::ContainsItemShape, Qt::AscendingOrder);
        if(list.count() == 10)
        {
            foreach(QGraphicsItem *item, list)
            {
                Box *box = (Box *)item;
                QPropertyAnimation *animation = new QPropertyAnimation(box, "scale");
                animation->setDuration(200);
                animation->setEasingCurve(QEasingCurve::OutBounce);
                animation->setStartValue(0.25);
                animation->setEndValue(1);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                connect(animation, &QPropertyAnimation::finished, box, &Box::deleteLater);
            }
            rows << y;
        }
    }

    if(rows.size() > 0)
    {
        QTimer::singleShot(205, this, SLOT(moveBox()));
    }
    else
    {
        groupBox->createBox(QPoint(160, 0), nextGroupBox->current_shape);

        nextGroupBox->clearBox(true);
        nextGroupBox->createBox(QPoint(600, 200));
    }

}

void MainWindow::moveBox()
{
    for(int i=rows.size(); i>0; i--)
    {
        int y = rows.at(i-1);
        QList<QGraphicsItem *> list = mainscene->items(QRectF(-1, -1, 600, y+80), Qt::ContainsItemShape, Qt::AscendingOrder);
        foreach(QGraphicsItem *item, list)
        {
            item->moveBy(0, 40);
        }
    }
    int score = scoreItem->toPlainText().toInt();
    score += 100 * rows.size();
    scoreItem->setPlainText(QString::number(score));
    rows.clear();
    groupBox->createBox(QPoint(160, 0), nextGroupBox->current_shape);

    nextGroupBox->clearBox(true);
    nextGroupBox->createBox(QPoint(600, 200));
}

void MainWindow::gameOver()
{
    timer->stop();
    QMessageBox::about(this, "Tips", "Game Over");
}


