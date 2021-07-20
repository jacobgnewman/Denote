#include "ui.h"
#include "Tools/tool.h"
#include "Framework/toolmenuviewer.h"
#include "Framework/toolmenu.h"
#include "Framework/toollibrary.h"
#include "Framework/History/historymanager.h"

#include "Tools/pen.h"
#include "Tools/fill.h"
#include "Tools/eraser.h"
#include "Tools/selectionbox.h"
#include "Tools/circleselect.h"
#include "Tools/lassoselect.h"


UI::UI(MainWindow* main_window)
{
    this->main_window = main_window;

    tool_library = new ToolLibrary(main_window);
    tool_menu_viewer = new ToolMenuViewer(main_window);
    history_manager = new HistoryManager(main_window);

    main_window->addSubWindow(tool_library, Qt::TopDockWidgetArea);
    main_window->addSubWindow(tool_menu_viewer, Qt::BottomDockWidgetArea);
    main_window->addSubWindow(history_manager, Qt::BottomDockWidgetArea);

    for(int i = 0; i < 5; i ++){
        Pen *pen = new Pen(this);
        QColor color;
        color.setHsv(i*36,255,255);
        pen->setColor(color);
        pen->setWidth(i*2+4);
        addTool(pen);
    }
    for(int i = 0; i < 4; i ++){
        Fill *fill = new Fill(this);
        QColor color;
        color.setHsv(i*36,255,255);
        fill->setColor(color);
        addTool(fill);
    }
    addTool(new Eraser(this));
    SelectionBox* box = new SelectionBox(this);
    addTool(box);
    addTool(new CircleSelect(this, box));
    addTool(new LassoSelect(this, box));
}


void UI::addTool(Tool *tool)
{
    tools.append(tool);
    tool_library->addTool(tool);
    setActiveTool(tool);
}


void UI::setActiveTool(Tool *tool){
    if(active_tool != nullptr){
        active_tool->deactivate();
    }
    active_tool = tool;
    tool_library->setActiveTool(tool);
    tool_menu_viewer->setWidget(tool->getToolMenu());
    tool->activate();
}


ToolMenu *UI::getToolMenu()
{
    return tool_menu_viewer->getMenu();
}







