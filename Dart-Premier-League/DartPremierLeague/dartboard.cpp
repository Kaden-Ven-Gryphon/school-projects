#include "dartboard.h"

DartBoard::DartBoard(QGraphicsItem* parent) : QGraphicsItem(parent)
{
    baseWidth = 400;
    baseHeight = 400;

//    void DartBoard::paintEvent(QPaintEvent *e){
//        QPixmap pixmap2(":/new/prefix1/dartboard.png");
//        QPainter painter(this);
//        painter.drawPixmap(10,70,50,50, pixmap2);


//    }
}

QRectF DartBoard::boundingRect() const
{
    return QRectF(0, 0, 400, 400);
}


void DartBoard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QPixmap pixmap(":/new/prefix1/dartboard.png");
    painter->drawPixmap(0,0,400,400,pixmap);
    //*
    float scaleX = float(painter->window().width()) / float(baseWidth);
    float scaleY = float(painter->window().height()) / float(baseHeight);

    //scale by smaller factor to maintian circular dartboard
    float scaler = scaleY;
    if(scaleX < scaleY)
        scaler = scaleX;

    //center of Window
    int centerX = 200;
    int centerY = 200;
    //*/


    //determine the radius of each circle on the board
    //multipliers are found by dividing the radii of official dartboard dimensions

    //radius of dartBoard
    float boardRadius = 380;
    //radius of outer double ring
    float outDoubRad = boardRadius * 0.765;
    //radius of inner double ring
    float inDoubRad = boardRadius * 0.72;
    //radius of outer triple ring
    float outTripRad = boardRadius * 0.48;
    //radius of innter triple ring
    float inTripRad = boardRadius * 0.439;
    //radius of outer Bull ring
    float outBullRad = boardRadius * 0.071;
    //radius of inner Bull ring
    float inBullRad = boardRadius * 0.028;


    //painter->scale(scaler, scaler);
//    painter->fillRect(boundingRect(),Qt::red);

    //draw circles
    painter->drawEllipse(QRect(centerX - (boardRadius / 2), centerY - (boardRadius / 2), boardRadius, boardRadius));
    painter->drawEllipse(QRect(centerX - (outDoubRad / 2), centerY - (outDoubRad / 2), outDoubRad, outDoubRad));
    painter->drawEllipse(QRect(centerX - (inDoubRad / 2), centerY - (inDoubRad / 2), inDoubRad, inDoubRad));
    painter->drawEllipse(QRect(centerX - (outTripRad / 2), centerY - (outTripRad / 2), outTripRad, outTripRad));
    painter->drawEllipse(QRect(centerX - (outBullRad / 2), centerY - (outBullRad / 2), outBullRad, outBullRad));
    painter->drawEllipse(QRect(centerX - (inTripRad / 2), centerY - (inTripRad / 2), inTripRad, inTripRad));
    painter->drawEllipse(QRect(centerX - (inBullRad / 2), centerY - (inBullRad / 2), inBullRad, inBullRad));

    //draw line segments
    for(int i = 0; i < 10; i++)
    {
        //start 9 degress off straigt up, rotate by 18 degrees to get 20 segments
        float slope = 1 / qTan(qDegreesToRadians(9 +(18 * i)));

        float startX = (outBullRad / 2) * qCos(qAtan(slope));
        float startY = (outBullRad / 2) * qSin(qAtan(slope));
        float endX = (outDoubRad / 2) * qCos(qAtan(slope));
        float endY = (outDoubRad / 2) * qSin(qAtan(slope));

        painter->drawLine(startX + centerX, startY + centerY, endX + centerX, endY + centerY);
        painter->drawLine(-startX + centerX, -startY + centerY, -endX + centerX, -endY + centerY);
    }
}

void DartBoard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    float x = event->lastPos().rx();
    float y = event->lastPos().ry();

    float centerX = 200;
    float centerY = 200;

    x = x - centerX;
    y = -(y) + centerY;
    qInfo() << "click: " << x << y;

    float radius = qSqrt(qPow(x, 2) + qPow(y, 2));
    float theta = qAtan(y/ x);

    int zone = 1;
    for(int i = 1; i <= 10; i++)
    {
        if(theta > qDegreesToRadians(99 -(18 * i)))
        {
            zone = i;
            if(x < 0)
                zone += 10;
            break;
        }
    }

    if(zone == 1 && y < 0)
        zone += 10;

    //radius of dartBoard
    float boardRadius = 190;
    //radius of outer double ring
    float outDoubRad = boardRadius * 0.765;
    //radius of inner double ring
    float inDoubRad = boardRadius * 0.72;
    //radius of outer triple ring
    float outTripRad = boardRadius * 0.48;
    //radius of innter triple ring
    float inTripRad = boardRadius * 0.439;
    //radius of outer Bull ring
    float outBullRad = boardRadius * 0.071;
    //radius of inner Bull ring
    float inBullRad = boardRadius * 0.028;

    if(radius > outDoubRad)
        zone = 0;
    if(radius < inDoubRad)
        zone += 20;
    if(radius < outTripRad)
        zone += 20;
    if(radius < inTripRad)
        zone += 20;
    if(radius < outBullRad)
        zone = 81;
    if(radius < inBullRad)
        zone = 82;

    //qInfo() << "zone: " << zone;
    emit zoneClicked(zone);
}

/*
void DartBoard::zoneClicked(int zone)
{
    qInfo() << "here with zone: " << zone;
}*/
