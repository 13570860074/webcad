#ifndef EDITORREACTOR_H
#define EDITORREACTOR_H

#include "RxEventReactor.h"
#include "DbVoidPtrArray.h"
#include "GePoint3d.h"

class DbDatabase;
class EditorReactor : public RxEventReactor
{ 
public:
    RX_DECLARE_MEMBERS(EditorReactor);

    // Command Events
    //
    virtual void unknownCommand(const ACHAR* cmdStr, DbVoidPtrArray* al);
    virtual void commandWillStart(const ACHAR* cmdStr);
    virtual void commandEnded(const ACHAR* cmdStr);
    virtual void commandCancelled(const ACHAR* cmdStr);
    virtual void commandFailed(const ACHAR* cmdStr);

    // Lisp Events
    //
    virtual void lispWillStart(const ACHAR* firstLine);
    virtual void lispEnded();
    virtual void lispCancelled();

    // DWG Open Events.
    //
    virtual void beginDwgOpen(ACHAR*) final {}; //use new const overload instead
    virtual void beginDwgOpen(const ACHAR* filename);
    virtual void endDwgOpen(const ACHAR* filename, DbDatabase* pDb);

    // Sys Var Events.
    //
    virtual void sysVarChanged        (const ACHAR* varName, Adesk::Boolean success);
    // The final overloads is only to catch mis-declared overrides and are not actually used
    virtual void sysVarChanged        (const ACHAR*, int) final { };
    virtual void sysVarWillChange     (const ACHAR* varName);

    // UNDO Events 
    //
    virtual void undoSubcommandAuto(int activity, Adesk::Boolean state);
    virtual void undoSubcommandAuto(int, int) final { };
    virtual void undoSubcommandControl(int activity, int option);
    virtual void undoSubcommandBegin(int activity);
    virtual void undoSubcommandEnd(int activity);
    virtual void undoSubcommandMark(int activity);
    virtual void undoSubcommandBack(int activity);
    virtual void undoSubcommandNumber(int activity, int num);

    virtual void pickfirstModified();
    virtual void layoutSwitched(const ACHAR* newLayoutName);

    // Layout switch is about to occur
    virtual void layoutToBeSwitched(const ACHAR* /*oldLayoutName*/, const ACHAR* /*newLayoutName*/);
    // Drawing area has moved or resized
    virtual void dwgViewResized(Adesk::LongPtr /*hwndDwgView*/);

    //window messages
    virtual void docFrameMovedOrResized(Adesk::LongPtr hwndDocFrame, bool bMoved);
    virtual void mainFrameMovedOrResized(Adesk::LongPtr hwndMainFrame, bool bMoved);

    //Mouse events
    virtual void beginDoubleClick(const GePoint3d& clickPoint);
    virtual void beginRightClick(const GePoint3d& clickPoint);

    // Toolbar Size changes
    virtual void toolbarBitmapSizeWillChange(Adesk::Boolean bLarge);
    virtual void toolbarBitmapSizeChanged(Adesk::Boolean bLarge);
    virtual void toolbarBitmapSizeWillChange(int) final { };
    virtual void toolbarBitmapSizeChanged(int) final { };

    // Close and Quit Events.
    //
    // One can call veto() during this notifcation to stop
    // AutoCAD from shutting down the document
    // The DbDatabase * arg may be null if the doc hasn't been fully opened yet
    virtual void beginDocClose(DbDatabase* /*pDwg*/);

    // docCloseAborted is fired to all reactors that received
    // the beginDocClose event when the last reactor vetos the close
    virtual void docCloseAborted(DbDatabase* /*pDwg*/);

    // This notification is fired when the legacy beginClose event used to be fired,
    // after the beginDocClose() notifications
    // The DbDatabase * arg may be null if the doc hasn't been fully opened yet
    virtual void docCloseWillStart(DbDatabase* /*pDwg*/);

    // This method is no longer called and will be removed in a future release.
    // Please use docCloseWillStart() instead
    virtual void beginClose(DbDatabase* /*pDwg*/) final {}

    virtual void beginCloseAll();
    virtual void beginQuit();
    virtual void quitAborted();
    virtual void quitWillStart();

    virtual void viewChanged();


    //emsdk
#if EMSDK
public:
    EditorReactor();
    virtual ~EditorReactor();

    void onUnknownCommand(int ptr);
    void onCommandWillStart(int ptr);
    void onCommandEnded(int ptr);
    void onCommandCancelled(int ptr);
    void onCommandFailed(int ptr);

    void onLispWillStart(int ptr);
    void onLispEnded(int ptr);
    void onLispCancelled(int ptr);

    void onBeginDwgOpen(int ptr);
    void onEndDwgOpen(int ptr);

    void onSysVarChanged(int ptr);
    void onSysVarWillChange(int ptr);

    void onUndoSubcommandAuto(int ptr);
    void onUndoSubcommandControl(int ptr);
    void onUndoSubcommandBegin(int ptr);
    void onUndoSubcommandEnd(int ptr);
    void onUndoSubcommandMark(int ptr);
    void onUndoSubcommandBack(int ptr);
    void onUndoSubcommandNumber(int ptr);

    void onPickfirstModified(int ptr);
    void onLayoutSwitched(int ptr);
    void onLayoutToBeSwitched(int ptr);
    void onDwgViewResized(int ptr);

    void onDocFrameMovedOrResized(int ptr);
    void onMainFrameMovedOrResized(int ptr);

    void onBeginDoubleClick(int ptr);
    void onBeginRightClick(int ptr);

    void onToolbarBitmapSizeWillChange(int ptr);
    void onToolbarBitmapSizeChanged(int ptr);

    void onBeginDocClose(int ptr);
    void onDocCloseAborted(int ptr);

    void onDocCloseWillStart(int ptr);

    void onBeginClose(int ptr);

    void onBeginCloseAll(int ptr);
    void onBeginQuit(int ptr);
    void onQuitAborted(int ptr);
    void onQuitWillStart(int ptr);

    void onViewChanged(int ptr);

#endif


protected:
    Acad::ErrorStatus veto();
};



#endif