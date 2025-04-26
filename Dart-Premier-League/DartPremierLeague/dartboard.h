#ifndef DARTBOARD_H
#define DARTBOARD_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QPaintEvent>
#include <QPixmap>


class DartBoard : public QObject, public QGraphicsItem
{

    Q_OBJECT

public:
    DartBoard(QGraphicsItem* parent = NULL);
//    //--------------
//    DartBoard(QWidget *parent = 0);//-----------------
//    //-----------------

signals:
    void zoneClicked(int zone);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
//    //------------------------------
//    void paintEvent(QPaintEvent* e);//--------------
//    //-----------------

private:
    int baseWidth;
    int baseHeight;
};

#endif // DARTBOARD_H
