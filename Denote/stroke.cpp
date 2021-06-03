#include "stroke.h"
#include "pen.h"


#include <qdebug.h>

Stroke::Stroke(Pen* pen)
{
   this->pen = pen;
}

void Stroke::init(QGraphicsSceneMouseEvent *event)
{
    points.push_back(event->scenePos());
}

void Stroke::addpoint(QGraphicsSceneMouseEvent *event)
{
    points.push_back(event->scenePos());
    qDebug() << event->scenePos();
}

void Stroke::finish(QGraphicsSceneMouseEvent *event)
{
    //points.push_back(event->scenePos());
}

void Stroke::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    color = QColor(Qt::blue);
    QPen pen = QPen(color, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen.setCosmetic(true);
    painter->setPen(pen);

    if(points.length()>1){
        for(int i = 1; i < points.size()-1; i++){
        painter->drawLine(points.at(i-1),points.at(i));
        }
    }
    this->show();

}

QRectF Stroke::boundingRect() const{
    return QRectF(-1000,-1000,2000,2000);
}
