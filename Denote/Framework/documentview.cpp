#include "Framework/documentview.h"
#include "Framework/documentgraphics.h"


DocumentView::DocumentView(QMainWindow *parent, Document *doc) : SubWindow(parent){
    graphics = new DocumentGraphics(this, doc);
    setWidget(graphics);
    docs.append(doc);

    setMinimumSize(150,150);
}


void DocumentView::setScale(float scale)
{
    graphics->setScale(scale);
}