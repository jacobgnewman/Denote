QT       += core gui
QT       += openglwidgets
qtHaveModule(printsupport): QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Framework/document.cpp \
    Framework/documentview.cpp \
    Framework/History/historymanager.cpp \
    Framework/History/historymanagerviewer.cpp \
    Framework/History/undocreation.cpp \
    Framework/History/undodeletion.cpp \
    Framework/History/undoobject.cpp \
    Framework/fileviewer.cpp \
    Framework/icolor.cpp \
    Framework/pageitem.cpp \
    Framework/toolevent.cpp \
    Framework/toolmenu.cpp \
    Framework/toolmenuviewer.cpp \
    Framework/toollibrary.cpp \
    Framework/toolpreset.cpp \
    Graphics/documentinteractionframe.cpp \
    Graphics/documentinteractionview.cpp \
    Graphics/documentsummaryframe.cpp \
    Graphics/documentsummaryview.cpp \
    Graphics/pagelayoutscene.cpp \
    Graphics/pageportal.cpp \
    Tools/circleselect.cpp \
    Tools/eraser.cpp \
    Tools/fill.cpp \
    Tools/fillstroke.cpp \
    Tools/highlighter.cpp \
    Tools/image.cpp \
    Tools/lassoselect.cpp \
    Tools/pressurepoint.cpp \
    Tools/pen.cpp \
    Tools/rectangle.cpp \
    Tools/selectionbox.cpp \
    Tools/stroke.cpp \
    Tools/tool.cpp \
    main.cpp \
    mainwindow.cpp \
    Graphics/page.cpp \
    Ui/ui.cpp \
    tabletapplication.cpp

HEADERS += \
    Framework/document.h \
    Framework/documentview.h \
    Framework/History/historymanager.h \
    Framework/History/historymanagerviewer.h \
    Framework/History/undocreation.h \
    Framework/History/undodeletion.h \
    Framework/History/undoobject.h \
    Framework/fileviewer.h \
    Framework/icolor.h \
    Framework/pageitem.h \
    Framework/serializable.h \
    Framework/toolevent.h \
    Framework/toolmenu.h \
    Framework/toolmenuviewer.h \
    Framework/toollibrary.h \
    Graphics/documentinteractionframe.h \
    Graphics/documentinteractionview.h \
    Graphics/documentsummaryframe.h \
    Graphics/documentsummaryview.h \
    Graphics/pagelayoutscene.h \
    Graphics/pageportal.h \
    Tools/circleselect.h \
    Tools/eraser.h \
    Tools/fill.h \
    Tools/fillstroke.h \
    Tools/highlighter.h \
    Tools/image.h \
    Tools/lassoselect.h \
    Tools/pressurepoint.h \
    Tools/pen.h \
    Tools/rectangle.h \
    Tools/selectionbox.h \
    Tools/stroke.h \
    Tools/tool.h \
    mainwindow.h \
    Graphics/page.h \
    Ui/ui.h \
    tabletapplication.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc
