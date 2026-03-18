#ifndef REIMPL_H
#define REIMPL_H

#include "RxImpl.h"
#include "AcArray.h"
#include "AcEvent.h"
#include "AcIterator.h"
#include "AcEmsdkEvent.h"
#include "GeMatrix3d.h"
#include "ReCamera.h"
#include "ReRenderer.h"
#include "ReRendererReactor.h"
#include "ReOrthogonalCamera.h"
#include "ReRendererManager.h"
#include "DbObjectId.h"



#define RE_IMP_RENDERERMANAGER(x) ((ReRendererManagerImpl*)x)

#define RE_IMP_USERINPUT(x) ((ReUserInputImpl*)x)

#define RE_IMP_CONTROLLER(x) ((ReControllerImpl*)x)
#define RE_IMP_ACADCONTROLLER(x) ((ReAcadControllerImpl*)x)

#define RE_IMP_RENDERERBEGIN(x) ((ReRendererBeginImpl*)x)

#define RE_IMP_RENDERER(x) ((ReRendererIpml*)x)

#define RE_IMP_RERENDERERREACTOR(x) ((ReRendererReactorImpl*)x)

#define RE_IMP_CAMERA(x) ((ReCameraImpl*)x)
#define RE_IMP_ORTHOGONALCAMERA(x) ((ReOrthogonalCameraImpl*)x)

#define RE_IMP_VIEWPORT(x) ((ReViewportImpl*)x)

#define RE_IMP_CURSOR(x) ((ReCursorImpl*)x)

#define RE_IMP_TARGETMAPPING(x) ((ReTargetMappingImpl*)x)



class ReRendererManagerImpl :public RxObjectImpl {
public:
	ReRendererManagerImpl() {
		this->isAsyncAwait = false;
		this->rendererMode = ReRendererManager::ERendererMode::kAllViewport;
	}
	virtual ~ReRendererManagerImpl() {
		realloc_arry_all(this->reactors);
	}

	bool isAsyncAwait;
	ReRendererManager::ERendererMode rendererMode;
	AcArray<ReRendererReactor*> reactors;
	AcArray<ReRenderer*> renderers;

	DbObjectId lastActiveViewportId;
};


class ReUserInputImpl :public RxObjectImpl {
public:
	ReUserInputImpl() {

		this->pEvent = new AcEvent();

		this->rangWidth = 0;
		this->rangHeight = 0;
		this->offsetTop = 0;
		this->offsetLeft = 0;
		this->mouse[0] = 0.0;
		this->mouse[1] = 0.0;
		this->isDownMouseLeftButton = false;
		this->isDownMouseRightButton = false;
		this->isDownMouseMidButton = false;
	}
	virtual ~ReUserInputImpl() {
		delete this->pEvent;
	}

	AcEvent* pEvent;
	double rangWidth;
	double rangHeight;
	double offsetTop;
	double offsetLeft;
	double mouse[2];
	bool isDownMouseLeftButton;
	bool isDownMouseRightButton;
	bool isDownMouseMidButton;
};


class ReControllerImpl :public RxObjectImpl {
public:
	ReControllerImpl() {

	}
	virtual ~ReControllerImpl() {

	}
};
class ReAcadControllerImpl :public ReControllerImpl {
public:
	ReAcadControllerImpl() {
		this->isDrag = false;
		this->isRotation = false;
	}
	virtual ~ReAcadControllerImpl() {

	}

	bool isDrag;
	GePoint2d dragStartPoint;
	GePoint2d dragEndPoint;

	bool isRotation;
	GePoint2d rotationStartPoint;
	GePoint2d rotationEndPoint;

	bool isSelectBox;
	GePoint2d selectBoxStartPoint;
	GePoint2d selectBoxEndPoint;
};


class ReRendererBeginImpl :public RxObjectImpl {
public:
	ReRendererBeginImpl() {
		this->isViewportChange = false;
		this->isElementChange = false;
		this->isCoordSystemChange = false;
		this->isGripChange = false;
	}
	virtual ~ReRendererBeginImpl() {

	}

	bool isViewportChange;
	bool isElementChange;
	bool isCoordSystemChange;
	bool isGripChange;
	GePoint2d cursorPosition;
};


class ReRendererReactorImpl :public RxObjectReactorImpl {
public:
	ReRendererReactorImpl() {

	}
	virtual ~ReRendererReactorImpl() {

	}
};


class ReTargetMappingImpl :public RxObjectImpl {
public:
	ReTargetMappingImpl() {
		this->size = 16;
		this->pxiLen = 0;
		this->pixels = NULL;
	}
	virtual ~ReTargetMappingImpl() {

	}

	double size;
	GePoint2d screen;
	unsigned int pxiLen;
	unsigned int* pixels;
};


class ReCameraImpl :public RxObjectImpl {
public:
	ReCameraImpl() {
		this->id = 0;
	}
	virtual ~ReCameraImpl() {

	}

	unsigned int id;
	GeMatrix3d matrix;
};
class ReOrthogonalCameraImpl :public ReCameraImpl {
public:
	ReOrthogonalCameraImpl() {
		this->top = 1;
		this->bottom = 1;
		this->left = 1;
		this->right = 1;
	}
	virtual ~ReOrthogonalCameraImpl() {

	}

	double top;
	double bottom;
	double left;
	double right;
	GePoint2d lowerLeftCorner;
	GePoint2d upperRightCorner;
};
class ReRendererIpml :public RxObjectImpl {
public:
	ReRendererIpml() {
		this->viewport = NULL;
		this->isWorkArea = false;
		this->camera = NULL;
		this->entityIds = NULL;
		this->numEntity = 0;
	}
	virtual ~ReRendererIpml() {
		if (this->camera != NULL) {
			delete this->camera;
			this->camera = NULL;
		}
	}

	ReCamera* camera;
	bool isWorkArea;
	DbObjectId viewport;
	unsigned int* entityIds;
	unsigned int numEntity;
};



class ReViewportImpl :public RxObjectImpl {
public:
	ReViewportImpl() {
		this->camera = NULL;
		this->lowerLeftCorner = GePoint2d(0.0, 0.0);
		this->upperRightCorner = GePoint2d(1.0, 1.0);
	}
	virtual ~ReViewportImpl() {

	}

	ReCamera *camera;
	GePoint2d lowerLeftCorner;
	GePoint2d upperRightCorner;
};



class ReCursorImpl :public RxObjectImpl {
public:
	ReCursorImpl() {

	}
	virtual ~ReCursorImpl() {

	}
};



#endif