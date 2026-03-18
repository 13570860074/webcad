#include "EditorReactor.h"
#include "DbDatabase.h"
#include "EdImpl.h"

enum EditorReactorEvent
{
	kUnknownCommand,
	kCommandWillStart,
	kCommandEnded,
	kCommandCancelled,
	kCommandFailed,

	kLispWillStart,
	kLispEnded,
	kLispCancelled,

	kBeginDwgOpen,
	kEndDwgOpen,

	kUndoSubcommandAuto,
	kUndoSubcommandControl,
	kUndoSubcommandBegin,
	kUndoSubcommandEnd,
	kUndoSubcommandMark,
	kUndoSubcommandBack,
	kUndoSubcommandNumber,

	kPickfirstModified,
	kLayoutSwitched,
	kLayoutToBeSwitched,
	kDwgViewResized,

	kSysVarChanged,
	kSysVarWillChange,

	kDocFrameMovedOrResized,
	kMainFrameMovedOrResized,

	kBeginDoubleClick,
	kBeginRightClick,

	kToolbarBitmapSizeWillChange,
	kToolbarBitmapSizeChanged,

	kBeginDocClose,
	kDocCloseAborted,

	kDocCloseWillStart,

	kBeginClose,

	kBeginCloseAll,
	kBeginQuit,
	kQuitAborted,
	kQuitWillStart,

	kViewChanged,
};

void EditorReactor::unknownCommand(const ACHAR *cmdStr, DbVoidPtrArray *al)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kUnknownCommand, cmdStr, al);
}
void EditorReactor::commandWillStart(const ACHAR *cmdStr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kCommandWillStart, cmdStr);
}
void EditorReactor::commandEnded(const ACHAR *cmdStr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kCommandEnded, cmdStr);
}
void EditorReactor::commandCancelled(const ACHAR *cmdStr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kCommandCancelled, cmdStr);
}
void EditorReactor::commandFailed(const ACHAR *cmdStr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kCommandFailed, cmdStr);
}

void EditorReactor::lispWillStart(const ACHAR *firstLine)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kLispWillStart, firstLine);
}
void EditorReactor::lispEnded()
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kLispEnded);
}
void EditorReactor::lispCancelled()
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kLispCancelled);
}

void EditorReactor::beginDwgOpen(const ACHAR *filename)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kBeginDwgOpen, filename);
}
void EditorReactor::endDwgOpen(const ACHAR *filename, DbDatabase *pDb)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kEndDwgOpen, filename, pDb);
}

void EditorReactor::sysVarChanged(const ACHAR *varName, Adesk::Boolean success)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kSysVarChanged, varName, success);
}
void EditorReactor::sysVarWillChange(const ACHAR *varName)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kSysVarWillChange, varName);
}

void EditorReactor::undoSubcommandAuto(int activity, Adesk::Boolean state)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kUndoSubcommandAuto, activity, state);
}
void EditorReactor::undoSubcommandControl(int activity, int option)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kUndoSubcommandControl, activity, option);
}
void EditorReactor::undoSubcommandBegin(int activity)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kUndoSubcommandBegin, activity);
}
void EditorReactor::undoSubcommandEnd(int activity)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kUndoSubcommandEnd, activity);
}
void EditorReactor::undoSubcommandMark(int activity)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kUndoSubcommandMark, activity);
}
void EditorReactor::undoSubcommandBack(int activity)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kUndoSubcommandBack, activity);
}
void EditorReactor::undoSubcommandNumber(int activity, int num)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kUndoSubcommandNumber, activity, num);
}

void EditorReactor::pickfirstModified()
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kPickfirstModified);
}
void EditorReactor::layoutSwitched(const ACHAR *newLayoutName)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kLayoutSwitched, newLayoutName);
}

void EditorReactor::layoutToBeSwitched(const ACHAR *oldLayoutName, const ACHAR *newLayoutName)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kLayoutToBeSwitched, oldLayoutName, newLayoutName);
}
void EditorReactor::dwgViewResized(Adesk::LongPtr hwndDwgView)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kDwgViewResized, hwndDwgView);
}

void EditorReactor::docFrameMovedOrResized(Adesk::LongPtr hwndDocFrame, bool bMoved)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kDocFrameMovedOrResized, hwndDocFrame, bMoved);
}
void EditorReactor::mainFrameMovedOrResized(Adesk::LongPtr hwndMainFrame, bool bMoved)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kMainFrameMovedOrResized, hwndMainFrame, bMoved);
}

void EditorReactor::beginDoubleClick(const GePoint3d &clickPoint)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kBeginDoubleClick, clickPoint);
}
void EditorReactor::beginRightClick(const GePoint3d &clickPoint)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kBeginRightClick, clickPoint);
}

void EditorReactor::toolbarBitmapSizeWillChange(Adesk::Boolean bLarge)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kToolbarBitmapSizeWillChange, bLarge);
}
void EditorReactor::toolbarBitmapSizeChanged(Adesk::Boolean bLarge)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kToolbarBitmapSizeChanged, bLarge);
}

void EditorReactor::beginDocClose(DbDatabase *pDwg)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kBeginDocClose, pDwg);
}
void EditorReactor::docCloseAborted(DbDatabase *pDwg)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kDocCloseAborted, pDwg);
}
void EditorReactor::docCloseWillStart(DbDatabase *pDwg)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kDocCloseWillStart, pDwg);
}

void EditorReactor::beginCloseAll()
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kBeginCloseAll);
}
void EditorReactor::beginQuit()
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kBeginQuit);
}
void EditorReactor::quitAborted()
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kQuitAborted);
}
void EditorReactor::quitWillStart()
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kQuitWillStart);
}

void EditorReactor::viewChanged()
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EditorReactorEvent::kViewChanged);
}

Acad::ErrorStatus veto()
{
	return Acad::ErrorStatus::eOk;
}

#if EMSDK
EditorReactor::EditorReactor()
{
	this->m_pImpl = new EditorReactorImpl();
}
EditorReactor::~EditorReactor()
{
}
void EditorReactor::onUnknownCommand(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kUnknownCommand, ptr);
}
void EditorReactor::onCommandWillStart(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kCommandWillStart, ptr);
}
void EditorReactor::onCommandEnded(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kCommandEnded, ptr);
}
void EditorReactor::onCommandCancelled(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kCommandCancelled, ptr);
}
void EditorReactor::onCommandFailed(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kCommandFailed, ptr);
}

void EditorReactor::onLispWillStart(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kLispWillStart, ptr);
}
void EditorReactor::onLispEnded(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kLispEnded, ptr);
}
void EditorReactor::onLispCancelled(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kLispCancelled, ptr);
}

void EditorReactor::onBeginDwgOpen(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kBeginDwgOpen, ptr);
}
void EditorReactor::onEndDwgOpen(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kEndDwgOpen, ptr);
}
void EditorReactor::onSysVarChanged(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kSysVarChanged, ptr);
}
void EditorReactor::onSysVarWillChange(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kSysVarWillChange, ptr);
}
void EditorReactor::onUndoSubcommandAuto(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kUndoSubcommandAuto, ptr);
}
void EditorReactor::onUndoSubcommandControl(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kUndoSubcommandControl, ptr);
}
void EditorReactor::onUndoSubcommandBegin(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kUndoSubcommandBegin, ptr);
}
void EditorReactor::onUndoSubcommandEnd(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kUndoSubcommandEnd, ptr);
}
void EditorReactor::onUndoSubcommandMark(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kUndoSubcommandMark, ptr);
}
void EditorReactor::onUndoSubcommandBack(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kUndoSubcommandBack, ptr);
}
void EditorReactor::onUndoSubcommandNumber(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kUndoSubcommandNumber, ptr);
}

void EditorReactor::onPickfirstModified(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kPickfirstModified, ptr);
}
void EditorReactor::onLayoutSwitched(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kLayoutSwitched, ptr);
}
void EditorReactor::onLayoutToBeSwitched(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kLayoutToBeSwitched, ptr);
}
void EditorReactor::onDwgViewResized(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kDwgViewResized, ptr);
}

void EditorReactor::onDocFrameMovedOrResized(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kDocFrameMovedOrResized, ptr);
}
void EditorReactor::onMainFrameMovedOrResized(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kMainFrameMovedOrResized, ptr);
}

void EditorReactor::onBeginDoubleClick(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kBeginDoubleClick, ptr);
}
void EditorReactor::onBeginRightClick(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kBeginRightClick, ptr);
}

void EditorReactor::onToolbarBitmapSizeWillChange(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kToolbarBitmapSizeWillChange, ptr);
}
void EditorReactor::onToolbarBitmapSizeChanged(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kToolbarBitmapSizeChanged, ptr);
}

void EditorReactor::onBeginDocClose(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kBeginDocClose, ptr);
}
void EditorReactor::onDocCloseAborted(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kDocCloseAborted, ptr);
}

void EditorReactor::onDocCloseWillStart(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kDocCloseWillStart, ptr);
}

void EditorReactor::onBeginClose(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kBeginClose, ptr);
}

void EditorReactor::onBeginCloseAll(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kBeginCloseAll, ptr);
}
void EditorReactor::onBeginQuit(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kBeginQuit, ptr);
}
void EditorReactor::onQuitAborted(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kQuitAborted, ptr);
}
void EditorReactor::onQuitWillStart(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kQuitWillStart, ptr);
}

void EditorReactor::onViewChanged(int ptr)
{
	ED_IMP_EDITTORREACTOR(this->m_pImpl)->pEmsdkEvent->on(EditorReactorEvent::kViewChanged, ptr);
}

#endif