#ifndef PAGEITEM_H
#define PAGEITEM_H

#include <QGraphicsItem>
#include "Framework/History/undocreation.h"

const int UserType = QGraphicsItem::UserType;

enum ItemType {
    TypePage = UserType+1,
    TypePenStroke = UserType+2,
    TypeFillStroke = UserType+3,
    TypeEraser = UserType+4,
    TypeSelectionBox = UserType+5,
    TypeCircleSelect = UserType+6,
    TypeLassoSelect = UserType+7,
    TypeImage = UserType+8
};

class PageItem : public QGraphicsItem
{
public:
    PageItem();

public:
    bool isPresent(){return present;}
    void removeItem();
    void recoverItem();

private:
    bool present = true;
};

#endif // PAGEITEM_H