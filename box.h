#ifndef BOX_H
#define BOX_H

#include <QGraphicsItemGroup>
#include <QGraphicsObject>
#include <QObject>
#include <QPainter>
#include <QList>
#include <QTime>
#include <QColor>
#include <QKeyEvent>

enum Shape {SHAPE_I, SHAPE_T, SHAPE_O, SHAPE_Z, SHAPE_FZ, SHAPE_L, SHAPE_FL, RANDOMSHAPE};

class Box : public QGraphicsObject
{
    Q_OBJECT
public:
    Box(QColor color);
	~Box();
	
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *,QWidget *);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    QColor color;
};

class GroupBox : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public :
    GroupBox();
    ~GroupBox();

    Shape current_shape;
    int angle;
    QTransform old_transform;

	QRectF boundingRect() const override;
    void createBox(QPoint pos, Shape shape = RANDOMSHAPE);
    void keyPressEvent(QKeyEvent *e);
    bool isColliding();

    void clearBox(bool isDestroy = false);
public slots:
    void moveOneStep();

signals:
    void needNewBox();
    void gameOver_signal();
};



#endif // BOX_H
