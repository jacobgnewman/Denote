#ifndef UNDOOBJECT_H
#define UNDOOBJECT_H

class HistoryManager;

class UndoObject
{
public:
    UndoObject(HistoryManager* manager);
    virtual ~UndoObject();

public:
    virtual void undo() = 0;
    virtual void redo() = 0;

protected:
    HistoryManager* manager;//not sure if needed
};

#endif // UNDOOBJECT_H