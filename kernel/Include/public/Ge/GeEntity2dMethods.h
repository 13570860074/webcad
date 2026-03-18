#ifndef GEENTITY2DMETHODS_H
#define GEENTITY2DMETHODS_H 

class GeLine2d;
class GeRay2d;
class GeLineSeg2d;
class GeCircArc2d;
class GePointOnCurve2d;

#define GEENTITY2D_METHODS(CLASSNAME)\
    virtual bool isKindOf(Ge::EntityId entType) const;\
	virtual Ge::EntityId type() const;\
	virtual CLASSNAME* copy() const;\
	virtual bool operator == (const CLASSNAME& entity) const;\
	virtual bool operator != (const CLASSNAME& entity) const;\
	virtual bool isEqualTo(const CLASSNAME& entity) const;\
	virtual bool isEqualTo(const CLASSNAME& entity, const GeTol& tol) const;\
	virtual CLASSNAME& transformBy(const GeMatrix2d& xfm);\
	virtual CLASSNAME& translateBy(const GeVector2d& translateVec);\
	virtual CLASSNAME& rotateBy(double angle);\
	virtual CLASSNAME& rotateBy(double angle, const GePoint2d& wrtPoint);\
	virtual CLASSNAME& mirror(const GeLine2d& line);\
	virtual CLASSNAME& scaleBy(double scaleFactor);\
	virtual CLASSNAME& scaleBy(double scaleFactor, const GePoint2d& wrtPoint);\
	virtual bool isOn(const GePoint2d& pnt) const;\
	virtual bool isOn(const GePoint2d& pnt, const GeTol& tol) const;\


#define GEPOINTENT2D_METHODS(CLASSNAME)\
    virtual GePoint2d point2d() const;\
	virtual operator GePoint2d () const;\


#define GECURVE2D_METHODS(CLASSNAME)\
    virtual double distanceTo(const GePoint2d& pnt) const;\
	virtual double distanceTo(const GePoint2d& pnt, const GeTol& tol) const;\
	virtual double distanceTo(const GeLine2d& entity) const;\
	virtual double distanceTo(const GeLine2d&, const GeTol& tol) const;\
	virtual double distanceTo(const GeLineSeg2d& entity) const;\
	virtual double distanceTo(const GeLineSeg2d&, const GeTol& tol) const;\
	virtual double distanceTo(const GeRay2d& entity) const;\
	virtual double distanceTo(const GeRay2d&, const GeTol& tol) const;\
	virtual double distanceTo(const GeCircArc2d& entity) const;\
	virtual double distanceTo(const GeCircArc2d&, const GeTol& tol) const;\
    virtual GePoint2d closestPointTo(const GePoint2d& pnt) const;\
	virtual GePoint2d closestPointTo(const GePoint2d& pnt, const GeTol& tol) const;\
    virtual GePoint2d closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv) const;\
	virtual GePoint2d closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const;\
    virtual GePoint2d closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv) const;\
	virtual GePoint2d closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual GePoint2d closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv) const;\
	virtual GePoint2d closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual GePoint2d closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv) const;\
	virtual GePoint2d closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const;\
    virtual void getClosestPointTo(const GePoint2d& pnt, GePointOnCurve2d& pntOnCrv) const;\
	virtual void getClosestPointTo(const GePoint2d& pnt, GePointOnCurve2d& pntOnCrv, const GeTol& tol) const;\
	virtual void getClosestPointTo(const GeLine2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv) const;\
	virtual void getClosestPointTo(const GeLine2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual void getClosestPointTo(const GeLineSeg2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv) const;\
	virtual void getClosestPointTo(const GeLineSeg2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual void getClosestPointTo(const GeRay2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv) const;\
	virtual void getClosestPointTo(const GeRay2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual void getClosestPointTo(const GeCircArc2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv) const;\
	virtual void getClosestPointTo(const GeCircArc2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv, const GeTol& tol) const;\
    virtual double length() const;\
	virtual double length(double fromParam, double toParam)const;\
	virtual double length(double fromParam, double toParam, double tol)const;\
    virtual double paramAtLength(double datumParam, double length) const;\
	virtual double paramAtLength(double datumParam, double length, double tol) const;\
    virtual double area() const;\
	virtual double area(const GeTol& tol) const;\
	virtual bool explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const;\
    virtual double paramOf(const GePoint2d& pnt) const;\
	virtual double paramOf(const GePoint2d& pnt, const GeTol& tol) const;\
    virtual void getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList) const;\
	virtual void getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const;\
	virtual void getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const;\
    virtual bool isClosed() const;\
	virtual bool isClosed(const GeTol& tol) const;\
	virtual void getSplitCurves(double param, GeCurve2d*& piece1, GeCurve2d*& piece2) const;\
    virtual GeBoundBlock2d boundBlock() const;\
	virtual GeBoundBlock2d boundBlock(const GeInterval& range) const;\
    virtual GeBoundBlock2d orthoBoundBlock() const;\
	virtual GeBoundBlock2d orthoBoundBlock(const GeInterval& range) const;\
    virtual bool hasStartPoint(GePoint2d& startPoint) const;\
	virtual bool hasEndPoint(GePoint2d& endPoint) const;\


#define GELINEARENT2D_METHODS(CLASSNAME)\
	virtual bool intersectWith(const GeLine2d& line, GePoint2d& intPnt) const;\
	virtual bool intersectWith(const GeLine2d& line, GePoint2d& intPnt, const GeTol& tol) const;\
	virtual bool intersectWith(const GeRay2d& line, GePoint2d& intPnt) const;\
	virtual bool intersectWith(const GeRay2d& line, GePoint2d& intPnt, const GeTol& tol) const;\
	virtual bool intersectWith(const GeLineSeg2d& line, GePoint2d& intPnt) const;\
	virtual bool intersectWith(const GeLineSeg2d& line, GePoint2d& intPnt, const GeTol& tol) const;\




#define GECURVE2D_METHODS_GETCLOSESTPOINTTO(CLASSNAME)\
void CLASSNAME::getClosestPointTo(const GePoint2d& pnt, GePointOnCurve2d& pntOnCrv) const {\
	this->getClosestPointTo(pnt, pntOnCrv, GeContext::gTol);\
}\
void CLASSNAME::getClosestPointTo(const GePoint2d& pnt, GePointOnCurve2d& pntOnCrv, const GeTol& tol) const {\
	GePoint2d closest = this->closestPointTo(pnt, tol);\
	pntOnCrv.setCurve(*this);\
	pntOnCrv.setParameter(this->paramOf(closest));\
}\
void CLASSNAME::getClosestPointTo(const GeLine2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv) const {\
	this->getClosestPointTo(curve2d, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);\
}\
void CLASSNAME::getClosestPointTo(const GeLine2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv, const GeTol& tol) const {\
	GePoint2d pntOnOther;\
	GePoint2d pntOnThis = this->closestPointTo(curve2d, pntOnOther, tol);\
	pntOnThisCrv.setCurve(*this);\
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis));\
	pntOnOtherCrv.setCurve(curve2d);\
	pntOnOtherCrv.setParameter(curve2d.paramOf(pntOnOther));\
}\
void CLASSNAME::getClosestPointTo(const GeLineSeg2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv) const {\
	this->getClosestPointTo(curve2d, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);\
}\
void CLASSNAME::getClosestPointTo(const GeLineSeg2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv, const GeTol& tol) const {\
	GePoint2d pntOnOther;\
	GePoint2d pntOnThis = this->closestPointTo(curve2d, pntOnOther, tol);\
	pntOnThisCrv.setCurve(*this);\
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis));\
	pntOnOtherCrv.setCurve(curve2d);\
	pntOnOtherCrv.setParameter(curve2d.paramOf(pntOnOther));\
}\
void CLASSNAME::getClosestPointTo(const GeRay2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv) const {\
	this->getClosestPointTo(curve2d, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);\
}\
void CLASSNAME::getClosestPointTo(const GeRay2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv, const GeTol& tol) const {\
	GePoint2d pntOnOther;\
	GePoint2d pntOnThis = this->closestPointTo(curve2d, pntOnOther, tol);\
	pntOnThisCrv.setCurve(*this);\
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis));\
	pntOnOtherCrv.setCurve(curve2d);\
	pntOnOtherCrv.setParameter(curve2d.paramOf(pntOnOther));\
}\
void CLASSNAME::getClosestPointTo(const GeCircArc2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv) const {\
	this->getClosestPointTo(curve2d, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);\
}\
void CLASSNAME::getClosestPointTo(const GeCircArc2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv, const GeTol& tol) const {\
	GePoint2d pntOnOther;\
	GePoint2d pntOnThis = this->closestPointTo(curve2d, pntOnOther, tol);\
	pntOnThisCrv.setCurve(*this);\
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis));\
	pntOnOtherCrv.setCurve(curve2d);\
	pntOnOtherCrv.setParameter(curve2d.paramOf(pntOnOther));\
}\



#endif

