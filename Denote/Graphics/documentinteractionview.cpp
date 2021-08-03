#include "documentinteractionview.h"
#include "pagelayoutscene.h"
#include "Framework/document.h"
#include "Tools/tool.h"
#include "Ui/ui.h"
#include "Framework/toolevent.h"
#include "Graphics/pageportal.h"
#include "Framework/History/historymanager.h"
#include "Graphics/page.h"
#include "Graphics/documentsummaryview.h"
#include "mainwindow.h"
#include "Framework/History/historymanagerviewer.h"
#include "Graphics/documentsummaryframe.h"
#include "Tools/image.h"

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QClipboard>
#include <QScrollBar>


DocumentInteractionView::DocumentInteractionView(Document* doc)
{
    this->doc = doc;
    page_layout_scene = new PageLayoutScene(doc, this);
    page_layout_scene->setLayoutType(PageLayoutScene::FitToView);
    setScene(page_layout_scene);

    doc->getUI()->setActiveLayout(page_layout_scene);

    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(AnchorUnderMouse);

    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);

    resetGL();

    centerOn(0,0);

    setTabletTracking(true);
    setMouseTracking(true);

    setBackgroundBrush(QBrush(PageLayoutScene::BACKGROUND));
    doc->updateEndlessLength();
    page_layout_scene->updatePageLayout();

    setResizeAnchor(ViewportAnchor::AnchorViewCenter);
    connect(verticalScrollBar(), &QAbstractSlider::valueChanged, this, &DocumentInteractionView::scrollPositionChanged);
}


DocumentInteractionView::~DocumentInteractionView()
{
    delete page_layout_scene;
}


void DocumentInteractionView::setScale(float view_scale)
{
    float new_scale = view_scale / transform().m11();
    scale(new_scale, new_scale);
}


void DocumentInteractionView::resetGL()
{
    QOpenGLWidget* gl = new QOpenGLWidget();
    QSurfaceFormat format;
    format.setSamples(6);
    gl->setFormat(format);
    setViewport(gl);
    //should need to exist. Antialiasing missing when popping in/out subwindow.
}


void DocumentInteractionView::focusDoc()
{
    if(doc->getUI()->getActiveDocument() != doc) doc->focusDoc();
}


void DocumentInteractionView::enterEvent(QEnterEvent *event)
{
    doc->getUI()->setActiveLayout(page_layout_scene);
    view_inverse = viewportTransform().inverted();
    doc->getUI()->getActiveTool()->documentProximityEvent(event);
}


void DocumentInteractionView::leaveEvent(QEvent *event)
{
    doc->getUI()->getActiveTool()->documentProximityEvent(event);
}


void DocumentInteractionView::tabletEvent(QTabletEvent *event){
    event->accept();

    if(event->type() == QEvent::TabletPress){
        foreach(PagePortal* portal, page_layout_scene->getPortals()){
            if(portal->isUnderMouse()){
                page_layout_scene->setFocusedPortal(portal);
                doc->getUI()->setActivePage(portal->getPage());
                doc->getUI()->setActivePortal(portal);
                page_inverse = portal->getPageOffset() - portal->scenePos();
            }
        }
        doc->getUI()->getActiveTool()->drawPressEvent(ToolEvent(event, this));
        focusDoc();
    } else if(event->type() == QEvent::TabletMove){
        doc->getUI()->getActiveTool()->drawMoveEvent(ToolEvent(event, this));
    } else if(event->type() == QEvent::TabletRelease){
        doc->getUI()->getActiveTool()->drawReleaseEvent(ToolEvent(event, this));
    }
}


void DocumentInteractionView::mousePressEvent(QMouseEvent *event)
{
    if(event->deviceType() == QInputDevice::DeviceType::Mouse){//prevents artificial mouse events from stylus
        foreach(PagePortal* portal, page_layout_scene->getPortals()){
            if(portal->isUnderMouse()){
                page_layout_scene->setFocusedPortal(portal);
                doc->getUI()->setActivePage(portal->getPage());
                doc->getUI()->setActivePortal(portal);
                page_inverse = portal->getPageOffset() - portal->scenePos();
            }
        }
        doc->getUI()->getActiveTool()->drawPressEvent(ToolEvent(event, this));
    }
    focusDoc();
}


void DocumentInteractionView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->deviceType() == QInputDevice::DeviceType::Mouse){//prevents artificial mouse events from stylus
        doc->getUI()->getActiveTool()->drawMoveEvent(ToolEvent(event, this));
    }
    QGraphicsView::mouseMoveEvent(event);//for zoom
}


void DocumentInteractionView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->deviceType() == QInputDevice::DeviceType::Mouse){//prevents artificial mouse events from stylus
        doc->getUI()->getActiveTool()->drawReleaseEvent(ToolEvent(event, this));
    }
}


void DocumentInteractionView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->deviceType() == QInputDevice::DeviceType::Mouse){//prevents artificial mouse events from stylus
        doc->getUI()->getActiveTool()->drawDoubleClickEvent(ToolEvent(event, this));
    }
}



void DocumentInteractionView::wheelEvent(QWheelEvent *event){
    if (event->modifiers() & Qt::ControlModifier){
        if(event->modifiers() & Qt::ShiftModifier){//rotate
            if (event->angleDelta().y() > 0)
                rotate(5);
            else
                rotate(-5);
        } else {//zoom
            if (event->angleDelta().y() > 0 and view_scale < 5){
                scale(1.1,1.1);
                QTransform t = transform();
                view_scale = sqrt(t.m11() * t.m11() + t.m12() * t.m12());
            } else if(event->angleDelta().y() < 0 and view_scale > 0.3){
                scale(1/1.1,1/1.1);
                QTransform t = transform();
                view_scale = sqrt(t.m11() * t.m11() + t.m12() * t.m12());
            }
        }
        event->accept();
        doc->updateAllLayouts();
        doc->updateEndlessLength();
    } else {
        QGraphicsView::wheelEvent(event);
    }
    view_inverse = viewportTransform().inverted();
}


void DocumentInteractionView::keyPressEvent(QKeyEvent *event)
{
    doc->getUI()->getHistoryManagerViewer()->keyPressEvent(event);
    doc->getUI()->getHistoryManagerViewer()->setHistoryManager(doc->getHistoryManager());

    if(event->key() == Qt::Key_V && event->modifiers() == Qt::ControlModifier){
        QClipboard *clip = QGuiApplication::clipboard();
        QImage image = clip->image(QClipboard::Clipboard);
        if(not image.isNull()){
            Image *new_image = new Image(doc->getUI());
            new_image->setImage(image);
        }
    }
    /*
    } else if(event->key() == Qt::Key_Delete){
        foreach(QGraphicsItem *item, doc->selectedItems()){
            doc->removeItem(item);
            delete item;
        }
    } else if(event->key() == Qt::Key_D){
        foreach(QGraphicsItem *item, doc->selectedItems()){

            Stroke* stroke = static_cast<Stroke*>(item);
            if(stroke){
                stroke->setSelected(false);
                Stroke* new_stroke = new Stroke(stroke);
                new_stroke->moveBy(20,20);
                doc->addItem(new_stroke);
                new_stroke->setSelected(true);
            }

        }
        doc->getUI()->setActiveTool(doc->getUI()->getActiveTool());
    }
    */
}


void DocumentInteractionView::scrollPositionChanged()
{
    doc->updateEndlessLength();
}
