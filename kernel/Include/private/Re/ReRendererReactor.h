#ifndef RERENDERERREACTOR_H
#define RERENDERERREACTOR_H


#include "RxObject.h"


class DbObject;
class ReRenderer;
class ReRendererReactor : public RxObject {
public:
	RX_DECLARE_MEMBERS(ReRendererReactor);

	virtual void beginAnimationFrame();
	virtual void endAnimationFrame();
	virtual void beginRenderer();
	virtual void endRenderer();
	virtual void clearCanvas();
	virtual void clearWorkCanvas();
	virtual void beginRendererViewport(DbObject* _pViewport);
	virtual void endRendererViewport(DbObject* _pViewport);
	virtual void rendererViewport(ReRenderer* _pRenderer);
	

#if EMSDK
public:
	ReRendererReactor();
	virtual ~ReRendererReactor();

	void onBeginAnimationFrame(int ptr);
	void onEndAnimationFrame(int ptr);
	void onBeginRenderer(int ptr);
	void onEndRenderer(int ptr);
	void onClearCanvas(int ptr);
	void onClearWorkCanvas(int ptr);
	void onBeginRendererViewport(int ptr);
	void onEndRendererViewport(int ptr);
	void onRendererViewport(int ptr);
#endif

};



#endif