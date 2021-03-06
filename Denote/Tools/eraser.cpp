#include "eraser.h"

#include "Ui/ui.h"
#include "Framework/document.h"
#include "Framework/toolmenu.h"
#include "Graphics/pagelayoutscene.h"
#include "Graphics/page.h"
#include "Graphics/pageportal.h"
#include "Framework/History/undodeletion.h"

#include <QPainter>


Eraser::Eraser(UI* ui) : Tool(ui)
{
    width_slider = new QSlider(Qt::Horizontal);
    width_slider->setValue(width);
    width_slider->setMaximum(200);

    menu_layout = new QGridLayout();
    menu_layout->addWidget(width_slider,0,0);

    tool_menu->setLayout(menu_layout);

    connect(width_slider, &QSlider::valueChanged, this, &Eraser::updateWidth);

    setWidth(30);
}


void Eraser::documentProximityEvent(QEvent *event)
{
    if(event->type() == QEvent::Enter) activate();
    else if(event->type() == QEvent::Leave) deactivate();
}


void Eraser::drawPressEvent(ToolEvent event)
{
    if(event.button() == Qt::LeftButton){
        erasing = true;
    }
}


void Eraser::drawMoveEvent(ToolEvent event)
{
    if(visible){
        setPos(event.layoutPos());
        if(erasing){
            QPainterPath page_shape = mapToScene(shape()).translated(-ui->getActivePortal()->scenePos());
            foreach(QGraphicsItem* item, ui->getActivePage()->items()){
                PageItem* page_item = static_cast<PageItem*>(item);
                if(page_item != nullptr){
                    if(page_item->isPresent() and page_shape.intersects(page_item->mapToScene(page_item->shape()))){
                        page_item->removeItem();
                        erased.append(page_item);
                    }
                }
            }
        }
    }
}


void Eraser::drawReleaseEvent(ToolEvent event)
{
    if(event.button() == Qt::LeftButton){
        if(not erased.isEmpty()){
            QString text = (erased.length() == 1) ? "Erase Item" : "Erase Items";
            new UndoDeletion(ui->getActiveDocument()->getHistoryManager(), erased, text);
            ui->getActivePage()->updatePortals();
            erased.clear();
        }
        erasing = false;
    }
}


void Eraser::activate()
{
    if(not visible and ui->getActiveLayout() != nullptr){
        ui->getActiveLayout()->addItem(this);
        visible = true;
    }
}


void Eraser::deactivate()
{
    if(visible and ui->getActiveLayout() != nullptr){
        ui->getActiveLayout()->removeItem(this);
        visible = false;
    }
}


void Eraser::paintPreset(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(tool_preset);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(QColor("white")));
    painter.setPen(QPen(QColor("black")));
    painter.drawRect(QRectF(0,0,tool_preset->width(),tool_preset->height()));
    painter.setBrush(QBrush(QColor("Pink")));
    painter.drawRect((tool_preset->width()-width)/2,(tool_preset->height()-width)/2, width,width);
    painter.drawText(QPointF(2,12),"Eraser");
}


QRectF Eraser::boundingRect() const
{
    return bounds;
}


QPainterPath Eraser::shape() const
{
    QPainterPath path;
    path.addEllipse(bounds);
    return path;
}


void Eraser::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen = QPen(QColor(122,122,122,75), 2, Qt::SolidLine, Qt::RoundCap);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(QPointF(0,0),0.5*width,0.5*width);
}


void Eraser::setWidth(float width)
{
    this->width = width;
    width_slider->setValue(width);
    bounds = QRectF(-0.5*width,-0.5*width, width, width).adjusted(-2,-2,2,2);
    tool_preset->update();
    prepareGeometryChange();
}


void Eraser::updateWidth(int width)
{
    setWidth(float(width));
}
