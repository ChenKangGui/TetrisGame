#include "box.h"
#include <QDebug>

Box::Box(QColor color) : color(color)
{

}

Box::~Box()
{

}

QRectF Box::boundingRect() const
{
    qreal pen_width = 1;
    return QRectF(pen_width/2, pen_width/2, 39+pen_width/2, 39+pen_width/2);
}

void Box::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(0, 0, 40, 40, QPixmap(":/box.gif"));
    painter->setBrush(color);
    painter->drawRect(QRect(0, 0, 40, 40));
}

QPainterPath Box::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 39, 39);
    return path;
}

GroupBox::GroupBox()
{
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    setFlags(QGraphicsItem::ItemIsFocusable);
    old_transform = transform();
}

GroupBox::~GroupBox()
{

}

void GroupBox::createBox(QPoint pos, Shape shape)
{
    static const QColor color_table[7] =
        {
            QColor(200, 0, 0, 100), QColor(255, 200, 0, 100), QColor(0, 0, 200, 100),
            QColor(0, 200, 0, 100), QColor(0, 200, 255, 100), QColor(200, 0, 255, 100),
            QColor(150, 100, 100, 100)
        };
    angle = 0;
    setTransform(old_transform);
    setRotation(0);
    resetTransform();

    int shape_id = shape;
    if(shape_id == RANDOMSHAPE)
    {
        shape_id = qrand() % 7;
    }
    current_shape = (Shape)shape_id;

    QList<Box *> boxList;
    for(int i=0; i<4; i++)
    {
        Box *box = new Box(color_table[shape_id]);
        boxList.append(box);
        addToGroup(box);
    }

    switch(shape_id)
    {
        case SHAPE_I:
        {
            //I
             boxList.at(0)->setPos(0, 0);
             boxList.at(1)->setPos(0, 40);
             boxList.at(2)->setPos(0, 80);
             boxList.at(3)->setPos(0, 120);
            break;
        }
        case SHAPE_T:
        {
            //T
            boxList.at(0)->setPos(0, 0);
            boxList.at(1)->setPos(40, 0);
            boxList.at(2)->setPos(80, 0);
            boxList.at(3)->setPos(40, 40);
            break;
        }
        case SHAPE_O:
        {
            //O
            boxList.at(0)->setPos(0, 0);
            boxList.at(1)->setPos(40, 0);
            boxList.at(2)->setPos(0, 40);
            boxList.at(3)->setPos(40, 40);
            break;
        }
        case SHAPE_Z:
        {
            //Z
            boxList.at(0)->setPos(0, 0);
            boxList.at(1)->setPos(0, 40);
            boxList.at(2)->setPos(40, 40);
            boxList.at(3)->setPos(40, 80);
            break;
        }
        case SHAPE_FZ:
        {
            //~Z
            boxList.at(0)->setPos(40, 0);
            boxList.at(1)->setPos(40, 40);
            boxList.at(2)->setPos(0, 40);
            boxList.at(3)->setPos(0, 80);
            break;
        }
        case SHAPE_L:
        {
            //L
            boxList.at(0)->setPos(0, 0);
            boxList.at(1)->setPos(0, 40);
            boxList.at(2)->setPos(0, 80);
            boxList.at(3)->setPos(40, 80);
            break;
        }
        case SHAPE_FL:
        {
            //~L
            boxList.at(0)->setPos(0, 0);
            boxList.at(1)->setPos(40, 0);
            boxList.at(2)->setPos(80, 0);
            boxList.at(3)->setPos(40, 40);
            break;
        }
        default:
            break;
    }

    setPos(pos);

    if(isColliding())
        emit gameOver_signal();
}

void GroupBox::keyPressEvent(QKeyEvent *e)
{
    QList<QGraphicsItem *> itemList = childItems();
    if(itemList.size() <= 0)
    {
        return ;
    }

    switch (e->key())
    {
        case Qt::Key_Up:
        {
            angle += 90;
            setRotation(angle);
            setTransformOriginPoint(40,40);

            if(isColliding())
            {
                angle -= 90;
                setRotation(angle);
            }
            if(angle >= 360)
            {
                angle = 0;
            }
            break;
        }
        case Qt::Key_Down:
        {
            while(!isColliding())
            {
                moveBy(0, 40);
            }
            moveBy(0, -40);
            break;
        }
        case Qt::Key_Left:
        {
            moveBy(-40, 0);

            if(isColliding())
                moveBy(40, 0);
            break;
        }
        case Qt::Key_Right:
        {
            moveBy(40, 0);
            if(isColliding())
                moveBy(-40, 0);
            break;
        }
        default:
            break;
    }
}

bool GroupBox::isColliding()
{
    QList<QGraphicsItem *> boxList = childItems();
    foreach (QGraphicsItem *box, boxList)
    {
        if(box->collidingItems().count()>1)
        {            
            return true;
        }
    }
    return false;
}

void GroupBox::moveOneStep()
{
    setFocus();
    moveBy(0, 40);

    if(isColliding())
    {
        moveBy(0, -40);
        clearBox();
        emit needNewBox();
    }
}

void GroupBox::clearBox(bool isDestroy)
{
    QList<QGraphicsItem *> itemList = childItems();
    foreach (QGraphicsItem *item, itemList)
    {
        removeFromGroup(item);
        if(isDestroy)
        {
            Box *box = (Box *)item;
            delete box;
        }
    }

}

QRectF GroupBox::boundingRect() const
{
    qreal pen_width = 1;
    return QRectF(-pen_width/2, -pen_width/2, 160+pen_width/2, 160+pen_width/2);
}


