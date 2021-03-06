#include "toolevent.h"
#include "Graphics/documentinteractionview.h"


ToolEvent::ToolEvent(QMouseEvent *event, DocumentInteractionView* view) :
    QTabletEvent(event->type(),
                 event->pointingDevice(),
                 QPointF(float(event->position().x()), float(event->position().y())),
                 event->globalPosition(),
                 0.5, //pressure
                 0, //x_tilt
                 0, //y_tilt
                 0, //tangential_pressure
                 0, //rotation
                 0, //z
                 event->modifiers(),
                 event->button(),
                 event->buttons())
{
    layout_position = view->getViewInverse().map(event->position());
    page_position = layout_position + view->getPageInverse();
}


ToolEvent::ToolEvent(QTabletEvent *event, DocumentInteractionView* view) : QTabletEvent(*event)
{
    layout_position = view->getViewInverse().map(event->position());
    page_position = layout_position + view->getPageInverse();
}
