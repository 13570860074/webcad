
#include "GsElementManager.h"
#include "EvEventManager.h"
#include "arxHeaders.h"
#include "kernel.h"
#include "GsImpl.h"

GsElementManager::GsElementManager() {
	this->m_pImpl = new GsElementManagerImpl();
}
GsElementManager::~GsElementManager() {

}
void GsElementManager::onBeginAnimationFrame(GsElementManager* pointer) {
	GsCursor* cursor = pointer->cursor();
	if (cursor != NULL) {
		cursor->update();
	}

	GsCoordSystem* coordSystem = pointer->coordSystem();
	if (coordSystem != NULL) {
		coordSystem->update();
	}
}
void GsElementManager::onDocumentCreated(GsElementManager* pointer, ApDocument* pDocCreating)
{
	GsCursor* pCursor = new GsCursor();
	GS_IMP_ELEMENT(pCursor->m_pImpl)->document = pDocCreating;
	pointer->addElement(pCursor);
	if (GS_IMP_ELEMENTMANAGER(pointer->m_pImpl)->curCursor == NULL) {
		GS_IMP_ELEMENTMANAGER(pointer->m_pImpl)->curCursor = pCursor;
	}

	GsCoordSystem* pCoordSystem = new GsCoordSystem();
	GS_IMP_ELEMENT(pCoordSystem->m_pImpl)->document = pDocCreating;
	pointer->addElement(pCoordSystem);
	if (GS_IMP_ELEMENTMANAGER(pointer->m_pImpl)->coordSystem == NULL) {
		GS_IMP_ELEMENTMANAGER(pointer->m_pImpl)->coordSystem = pCoordSystem;
	}
}
void GsElementManager::onBeginDrag(GsElementManager* pointer) {

	resbuf res;
	acedGetVar("CORSORSTYLE", &res);
	if (res.resval.rint == EdJig::CursorType::kNoSpecialCursor) {
		if (pointer->cursor() != NULL) {
			pointer->cursor()->setCursorStyle(GsCursor::ECursorStyle::kCrossCursor);
		}
	}
	else if (res.resval.rint == EdJig::CursorType::kCrosshair) {
		if (pointer->cursor() != NULL) {
			pointer->cursor()->setCursorStyle(GsCursor::ECursorStyle::kCrossCursor);
		}
	}
	else if (res.resval.rint == EdJig::CursorType::kRubberBand ||
		res.resval.rint == EdJig::CursorType::kNotRotated ||
		res.resval.rint == EdJig::CursorType::kCrosshairDashed) {
		if (pointer->cursor() != NULL) {
			pointer->cursor()->setCursorStyle(GsCursor::ECursorStyle::kCrossCursor);
		}
	}
	else if (res.resval.rint == EdJig::CursorType::kPkfirstOrGrips) {
		if (pointer->cursor() != NULL) {
			pointer->cursor()->setCursorStyle(GsCursor::ECursorStyle(GsCursor::ECursorStyle::kCrossCursor | GsCursor::ECursorStyle::kTarget));
		}
	}
	else if (res.resval.rint == EdJig::CursorType::kEntitySelect ||
		res.resval.rint == EdJig::CursorType::kTargetBox ||
		res.resval.rint == EdJig::CursorType::kEntitySelectNoPersp) {
		if (pointer->cursor() != NULL) {
			pointer->cursor()->setCursorStyle(GsCursor::ECursorStyle::kTarget);
		}
	}
	else if (res.resval.rint == EdJig::CursorType::kInvisible ||
		res.resval.rint == EdJig::CursorType::kCrosshairDashed) {
		if (pointer->cursor() != NULL) {
			pointer->cursor()->setCursorStyle(GsCursor::ECursorStyle::kNone);
		}
	}
	else if (res.resval.rint == EdJig::CursorType::kSelectionSet) {
		if (pointer->cursor() != NULL) {
			pointer->cursor()->setCursorStyle(GsCursor::ECursorStyle(
				GsCursor::ECursorStyle::kCrossCursor |
				GsCursor::ECursorStyle::kSelectionSet |
				GsCursor::ECursorStyle::kCorner
			));
		}
	}

}
void GsElementManager::onEndDrag(GsElementManager* pointer) {
	if (pointer->cursor() != NULL) {
		pointer->cursor()->setCursorStyle(GsCursor::ECursorStyle(GsCursor::ECursorStyle::kCrossCursor | GsCursor::ECursorStyle::kTarget));
	}
}
void GsElementManager::onSysVarChanged(GsElementManager* pointer, const ACHAR* varName, bool success) {

}
void GsElementManager::onMousedown(GsElementManager* pointer, const EdMouseEvent& mouseEvent) {
	if (pointer->cursor() != NULL) {
		pointer->cursor()->setEnable(true);
	}
}
void GsElementManager::onTouchstart(GsElementManager* pointer, const EdTouchEvent& touchEvent) {
	if (pointer->cursor() != NULL) {
		pointer->cursor()->setEnable(false);
	}
}


void GsElementManager::addElement(GsElement* ele) {
	GS_IMP_ELEMENTMANAGER(this->m_pImpl)->elements.append(ele);
	ele->onInit();
}

void GsElementManager::onInit() {

	/** 监听渲染循环开始事件 */
	::kernel()->acevEventManager()->on(Ev::RendererReactor::kBeginAnimationFrame, (void*)GsElementManager::onBeginAnimationFrame, this);

	/** 监听创建文档事件 */
	::kernel()->acevEventManager()->on(Ev::DocManagerReactor::kDocumentCreated, (void*)GsElementManager::onDocumentCreated, this);

	/** 监听系统变量变化事件 */
	::kernel()->acevEventManager()->on(Ev::EditorReactor::kSysVarChanged, (void*)GsElementManager::onSysVarChanged, this);

	/** 监听拖拽开始事件 */
	::kernel()->acevEventManager()->on(Ev::Drag::kBeginDrag, (void*)GsElementManager::onBeginDrag, this);

	/** 监听拖拽完成事件 */
	::kernel()->acevEventManager()->on(Ev::Drag::kEndDrag, (void*)GsElementManager::onEndDrag, this);

	/** 监听鼠标按下事件 */
	::kernel()->acevEventManager()->on(Ev::UserInput::kMousedown, (void*)GsElementManager::onMousedown, this);

	/** 监听触摸事件 */
	::kernel()->acevEventManager()->on(Ev::UserInput::kTouchstart, (void*)GsElementManager::onTouchstart, this);
}


GsCursor* GsElementManager::cursor() const {
	return (GsCursor*)GS_IMP_ELEMENTMANAGER(this->m_pImpl)->curCursor;
}
GsCoordSystem* GsElementManager::coordSystem() const {
	return (GsCoordSystem*)GS_IMP_ELEMENTMANAGER(this->m_pImpl)->coordSystem;
}
