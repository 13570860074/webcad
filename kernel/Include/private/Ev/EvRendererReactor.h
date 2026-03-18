#ifndef EVRENDERERREACTOR_H
#define EVRENDERERREACTOR_H


#include "ReRendererReactor.h"


class ReCamera;
class DbObject;
class EvRendererReactor :public ReRendererReactor {
public:
    EvRendererReactor();
    virtual ~EvRendererReactor();

    virtual void beginAnimationFrame();
    virtual void endAnimationFrame();

};




#endif




