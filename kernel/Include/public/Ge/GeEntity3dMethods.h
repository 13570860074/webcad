#ifndef GEENTITY3DMETHODS_H
#define GEENTITY3DMETHODS_H 

class GeLine3d;
class GeRay3d;
class GeLineSeg3d;
class GeCircArc3d;
class GePointOnCurve3d;

#define GEENTITY3D_METHODS(CLASSNAME)\
public:\
    virtual bool isKindOf(Ge::EntityId entType) const;\
	virtual Ge::EntityId type() const;\
	virtual CLASSNAME* copy() const;\
	virtual bool operator == (const CLASSNAME& entity) const;\
	virtual bool operator != (const CLASSNAME& entity) const;\
	virtual bool isEqualTo(const CLASSNAME& entity) const;\
	virtual bool isEqualTo(const CLASSNAME& entity, const GeTol& tol) const;\
	virtual CLASSNAME& transformBy(const GeMatrix3d& xfm);\
	virtual CLASSNAME& translateBy(const GeVector3d& translateVec);\
	virtual CLASSNAME& rotateBy(double angle, const GeVector3d& vec);\
	virtual CLASSNAME& rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint);\
	virtual CLASSNAME& mirror(const GePlane& plane);\
	virtual CLASSNAME& scaleBy(double scaleFactor);\
	virtual CLASSNAME& scaleBy(double scaleFactor, const GePoint3d& wrtPoint);\
	virtual bool isOn(const GePoint3d& pnt) const;\
	virtual bool isOn(const GePoint3d& pnt, const GeTol& tol) const;\


#define GEPOINTENT3D_METHODS(CLASSNAME)\
    virtual GePoint3d point3d() const;\
	virtual operator GePoint3d () const;\


#define GECURVE3D_METHODS(CLASSNAME)\
    virtual double distanceTo(const GePoint3d& pnt) const;\
	virtual double distanceTo(const GePoint3d& pnt, const GeTol& tol) const;\
	virtual double distanceTo(const GeLine3d& entity) const;\
	virtual double distanceTo(const GeLine3d&, const GeTol& tol) const;\
	virtual double distanceTo(const GeLineSeg3d& entity) const;\
	virtual double distanceTo(const GeLineSeg3d&, const GeTol& tol) const;\
	virtual double distanceTo(const GeRay3d& entity) const;\
	virtual double distanceTo(const GeRay3d&, const GeTol& tol) const;\
	virtual double distanceTo(const GeCircArc3d& entity) const;\
	virtual double distanceTo(const GeCircArc3d&, const GeTol& tol) const;\
    virtual GePoint3d closestPointTo(const GePoint3d& pnt) const;\
	virtual GePoint3d closestPointTo(const GePoint3d& pnt, const GeTol& tol) const;\
    virtual GePoint3d closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv) const;\
	virtual GePoint3d closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const;\
    virtual GePoint3d closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv) const;\
	virtual GePoint3d closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual GePoint3d closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv) const;\
	virtual GePoint3d closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual GePoint3d closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv) const;\
	virtual GePoint3d closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const;\
    virtual void getClosestPointTo(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const;\
	virtual void getClosestPointTo(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const;\
	virtual void getClosestPointTo(const GeLine3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const;\
	virtual void getClosestPointTo(const GeLine3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual void getClosestPointTo(const GeLineSeg3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const;\
	virtual void getClosestPointTo(const GeLineSeg3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual void getClosestPointTo(const GeRay3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const;\
	virtual void getClosestPointTo(const GeRay3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual void getClosestPointTo(const GeCircArc3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const;\
	virtual void getClosestPointTo(const GeCircArc3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual GePoint3d projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection) const;\
	virtual GePoint3d projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, const GeTol& tol) const;\
	virtual GePoint3d projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const;\
	virtual GePoint3d projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual GePoint3d projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const;\
	virtual GePoint3d projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual GePoint3d projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const;\
	virtual GePoint3d projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual GePoint3d projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const;\
	virtual GePoint3d projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual void getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv) const;\
	virtual void getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const;\
	virtual void getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const;\
	virtual void getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual void getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const;\
	virtual void getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual void getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const;\
	virtual void getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const;\
	virtual void getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const;\
	virtual void getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const;\
    virtual bool getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const;\
	virtual bool getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const;\
    virtual GeBoundBlock3d boundBlock() const;\
	virtual GeBoundBlock3d boundBlock(const GeInterval& range) const;\
    virtual GeBoundBlock3d orthoBoundBlock() const;\
	virtual GeBoundBlock3d orthoBoundBlock(const GeInterval& range) const;\
    virtual CLASSNAME* project(const GePlane& projectionPlane, const GeVector3d& projectDirection) const;\
	virtual CLASSNAME* project(const GePlane& projectionPlane, const GeVector3d& projectDirection, const GeTol& tol) const;\
	virtual CLASSNAME* orthoProject(const GePlane& projectionPlane) const;\
	virtual CLASSNAME* orthoProject(const GePlane& projectionPlane, const GeTol& tol) const;\
	virtual bool isOn(const GePoint3d& pnt, double& param) const;\
	virtual bool isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const;\
	virtual bool isOn(double param) const;\
	virtual bool isOn(double param, const GeTol& tol) const;\
    virtual double paramOf(const GePoint3d& pnt) const;\
	virtual double paramOf(const GePoint3d& pnt, const GeTol& tol) const;\
    virtual void getTrimmedOffset(double distance,const GeVector3d& planeNormal,GeVoidPointerArray& offsetCurveList) const;\
	virtual void getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const;\
	virtual void getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const;\
    virtual bool isClosed() const;\
	virtual bool isClosed(const GeTol& tol) const;\
    virtual bool isPlanar(GePlane& plane) const;\
	virtual bool isPlanar(GePlane& plane, const GeTol& tol) const;\
	virtual bool isLinear(GeLine3d& line) const;\
	virtual bool isLinear(GeLine3d& line, const GeTol& tol) const;\
	virtual bool isCoplanarWith(const GeLine3d& curve3d, GePlane& plane) const;\
	virtual bool isCoplanarWith(const GeLine3d& curve3d, GePlane& plane, const GeTol& tol) const;\
	virtual bool isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane) const;\
	virtual bool isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane, const GeTol& tol) const;\
	virtual bool isCoplanarWith(const GeRay3d& curve3d, GePlane& plane) const;\
	virtual bool isCoplanarWith(const GeRay3d& curve3d, GePlane& plane, const GeTol& tol) const;\
	virtual bool isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane) const;\
	virtual bool isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane, const GeTol& tol) const;\
	virtual double length() const;\
	virtual double length(double fromParam, double toParam)const;\
	virtual double length(double fromParam, double toParam, double tol)const;\
    virtual double paramAtLength(double datumParam, double length) const;\
	virtual double paramAtLength(double datumParam, double length, double tol) const;\
    virtual double area() const;\
	virtual double area(const GeTol& tol) const;\
	virtual void getSplitCurves(double param, GeCurve3d* piece1, GeCurve3d* piece2) const;\
	virtual bool explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const;\
    virtual bool hasStartPoint(GePoint3d& startPoint) const;\
	virtual bool hasEndPoint(GePoint3d& endPoint) const;\
	virtual GePoint3d evalPoint(double param) const;\
    virtual void getSamplePoints(double fromParam,double toParam,double approxEps,GePoint3dArray& pointArray,GeDoubleArray& paramArray) const;\
	virtual void getSamplePoints(double fromParam,double toParam,double approxEps,GePoint3dArray& pointArray,GeDoubleArray& paramArray,bool forceResampling) const;\
	virtual void getSamplePoints(int numSample, GePoint3dArray& pointArray) const;\
	virtual void getSamplePoints(int numSample, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const;\


#define GELINEARENT3D_METHODS(CLASSNAME)\
	virtual bool intersectWith(const GeLine3d& line, GePoint3d& intPnt) const;\
	virtual bool intersectWith(const GeLine3d& line, GePoint3d& intPnt, const GeTol& tol) const;\
	virtual bool intersectWith(const GeRay3d& line, GePoint3d& intPnt) const;\
	virtual bool intersectWith(const GeRay3d& line, GePoint3d& intPnt, const GeTol& tol) const;\
	virtual bool intersectWith(const GeLineSeg3d& line, GePoint3d& intPnt) const;\
	virtual bool intersectWith(const GeLineSeg3d& line, GePoint3d& intPnt, const GeTol& tol) const;\


#define GESURFACE_METHODS(CLASSNAME)\
    virtual GePoint2d paramOf(const GePoint3d& pnt) const;\
	virtual GePoint2d paramOf(const GePoint3d& pnt, const GeTol& tol) const;\
    virtual bool isOn(const GePoint3d& pnt, GePoint2d& paramPoint) const;\
	virtual bool isOn(const GePoint3d& pnt, GePoint2d& paramPoint, const GeTol& tol) const;\
    virtual GePoint3d closestPointTo(const GePoint3d& pnt) const;\
	virtual GePoint3d closestPointTo(const GePoint3d& pnt, const GeTol& tol) const;\
    virtual void getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result) const;\
	virtual void getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result, const GeTol& tol) const;\
    virtual double distanceTo(const GePoint3d& pnt) const;\
	virtual double distanceTo(const GePoint3d& pnt, const GeTol& tol) const;\
    virtual bool isNormalReversed() const;\
	virtual GeSurface& reverseNormal();\
    virtual GePoint3d evalPoint(const GePoint2d& param) const;\



#define GEPLANARENT_METHODS(CLASSNAME)\
	virtual bool intersectWith(const GeLine3d& line, GePoint3d& pnt) const;\
	virtual bool intersectWith(const GeLine3d& line, GePoint3d& pnt, const GeTol& tol) const;\
	virtual bool intersectWith(const GeLineSeg3d& line, GePoint3d& pnt) const;\
	virtual bool intersectWith(const GeLineSeg3d& line, GePoint3d& pnt, const GeTol& tol) const;\
	virtual bool intersectWith(const GeRay3d& line, GePoint3d& pnt) const;\
	virtual bool intersectWith(const GeRay3d& line, GePoint3d& pnt, const GeTol& tol) const;\
	virtual GePoint3d closestPointToLinearEnt(const GeLine3d& line, GePoint3d& pointOnLine) const;\
    virtual GePoint3d closestPointToLinearEnt(const GeLine3d& line, GePoint3d& pointOnLine, const GeTol& tol) const;\
	virtual GePoint3d closestPointToLinearEnt(const GeLineSeg3d& line, GePoint3d& pointOnLine) const;\
    virtual GePoint3d closestPointToLinearEnt(const GeLineSeg3d& line, GePoint3d& pointOnLine, const GeTol& tol) const;\
	virtual GePoint3d closestPointToLinearEnt(const GeRay3d& line, GePoint3d& pointOnLine) const;\
    virtual GePoint3d closestPointToLinearEnt(const GeRay3d& line, GePoint3d& pointOnLine, const GeTol& tol) const;\
	virtual GePoint3d closestPointToPlanarEnt(const GePlane& otherPln, GePoint3d& pointOnOtherPln) const;\
	virtual GePoint3d closestPointToPlanarEnt(const GePlane& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const;\
	virtual GePoint3d closestPointToPlanarEnt(const GeBoundedPlane& otherPln, GePoint3d& pointOnOtherPln) const;\
	virtual GePoint3d closestPointToPlanarEnt(const GeBoundedPlane& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const;\
	virtual bool isParallelTo(const GeLine3d& linEnt) const;\
	virtual bool isParallelTo(const GeLine3d& linEnt, const GeTol& tol) const;\
	virtual bool isParallelTo(const GeLineSeg3d& linEnt) const;\
	virtual bool isParallelTo(const GeLineSeg3d& linEnt, const GeTol& tol) const;\
	virtual bool isParallelTo(const GeRay3d& linEnt) const;\
	virtual bool isParallelTo(const GeRay3d& linEnt, const GeTol& tol) const;\
	virtual bool isParallelTo(const GePlane& otherPlnEnt) const;\
	virtual bool isParallelTo(const GePlane& otherPlnEnt, const GeTol& tol) const;\
	virtual bool isParallelTo(const GeBoundedPlane& otherPlnEnt) const;\
	virtual bool isParallelTo(const GeBoundedPlane& otherPlnEnt, const GeTol& tol) const;\
	virtual bool isPerpendicularTo(const GeLine3d& linEnt) const; \
	virtual bool isPerpendicularTo(const GeLine3d& linEnt, const GeTol& tol) const; \
	virtual bool isPerpendicularTo(const GeLineSeg3d& linEnt) const; \
	virtual bool isPerpendicularTo(const GeLineSeg3d& linEnt, const GeTol& tol) const; \
	virtual bool isPerpendicularTo(const GeRay3d& linEnt) const; \
	virtual bool isPerpendicularTo(const GeRay3d& linEnt, const GeTol& tol) const; \
	virtual bool isPerpendicularTo(const GePlane& otherPlnEnt) const; \
	virtual bool isPerpendicularTo(const GePlane& otherPlnEnt, const GeTol& tol) const; \
	virtual bool isPerpendicularTo(const GeBoundedPlane& otherPlnEnt) const; \
	virtual bool isPerpendicularTo(const GeBoundedPlane& otherPlnEnt, const GeTol& tol) const; \
	virtual bool isCoplanarTo(const GePlane& otherPlnEnt) const; \
	virtual bool isCoplanarTo(const GePlane& otherPlnEnt, const GeTol& tol) const; \
	virtual bool isCoplanarTo(const GeBoundedPlane& otherPlnEnt) const; \
	virtual bool isCoplanarTo(const GeBoundedPlane& otherPlnEnt, const GeTol& tol) const; \



#define GECURVE3D_METHODS_GETCLOSESTPOINTTO(CLASSNAME)\
void CLASSNAME::getClosestPointTo(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const {\
	this->getClosestPointTo(pnt, pntOnCrv, GeContext::gTol);\
}\
void CLASSNAME::getClosestPointTo(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const {\
	GePoint3d closest = this->closestPointTo(pnt, tol);\
	pntOnCrv.setCurve(*this);\
	pntOnCrv.setParameter(this->paramOf(closest));\
}\
void CLASSNAME::getClosestPointTo(const GeLine3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {\
	this->getClosestPointTo(curve3d, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);\
}\
void CLASSNAME::getClosestPointTo(const GeLine3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {\
	GePoint3d pntOnOther;\
	GePoint3d pntOnThis = this->closestPointTo(curve3d, pntOnOther, tol);\
	pntOnThisCrv.setCurve(*this);\
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis));\
	pntOnOtherCrv.setCurve(curve3d);\
	pntOnOtherCrv.setParameter(this->paramOf(pntOnOther));\
}\
void CLASSNAME::getClosestPointTo(const GeLineSeg3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {\
	this->getClosestPointTo(curve3d, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);\
}\
void CLASSNAME::getClosestPointTo(const GeLineSeg3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {\
	GePoint3d pntOnOther;\
	GePoint3d pntOnThis = this->closestPointTo(curve3d, pntOnOther, tol);\
	pntOnThisCrv.setCurve(*this);\
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis));\
	pntOnOtherCrv.setCurve(curve3d);\
	pntOnOtherCrv.setParameter(this->paramOf(pntOnOther));\
}\
void CLASSNAME::getClosestPointTo(const GeRay3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {\
	this->getClosestPointTo(curve3d, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);\
}\
void CLASSNAME::getClosestPointTo(const GeRay3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {\
	GePoint3d pntOnOther;\
	GePoint3d pntOnThis = this->closestPointTo(curve3d, pntOnOther, tol);\
	pntOnThisCrv.setCurve(*this);\
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis));\
	pntOnOtherCrv.setCurve(curve3d);\
	pntOnOtherCrv.setParameter(this->paramOf(pntOnOther));\
}\
void CLASSNAME::getClosestPointTo(const GeCircArc3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {\
	this->getClosestPointTo(curve3d, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);\
}\
void CLASSNAME::getClosestPointTo(const GeCircArc3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {\
	GePoint3d pntOnOther;\
	GePoint3d pntOnThis = this->closestPointTo(curve3d, pntOnOther, tol);\
	pntOnThisCrv.setCurve(*this);\
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis));\
	pntOnOtherCrv.setCurve(curve3d);\
	pntOnOtherCrv.setParameter(this->paramOf(pntOnOther));\
}\


#define GECURVE3D_METHODS_DISTANCETO(CLASSNAME)\
double CLASSNAME::distanceTo(const GePoint3d& point) const{\
	return this->distanceTo(point, GeContext::gTol);\
}\
double CLASSNAME::distanceTo(const GePoint3d& point, const GeTol& tol) const{\
	GePoint3d closest = this->closestPointTo(point, tol);\
	return closest.distanceTo(point);\
}\
double CLASSNAME::distanceTo(const GeLine3d& entity) const{\
	return this->distanceTo(entity, GeContext::gTol);\
}\
double CLASSNAME::distanceTo(const GeLine3d& line, const GeTol& tol) const{\
	GePoint3d closest;\
	GePoint3d itself = this->closestPointTo(line, closest, tol);\
	return closest.distanceTo(itself);\
}\
double CLASSNAME::distanceTo(const GeLineSeg3d& entity) const{\
	return this->distanceTo(entity, GeContext::gTol);\
}\
double CLASSNAME::distanceTo(const GeLineSeg3d& line, const GeTol& tol) const{\
	GePoint3d closest;\
	GePoint3d itself = this->closestPointTo(line, closest, tol);\
	return closest.distanceTo(itself);\
}\
double CLASSNAME::distanceTo(const GeRay3d& entity) const{\
	return this->distanceTo(entity, GeContext::gTol);\
}\
double CLASSNAME::distanceTo(const GeRay3d& line, const GeTol& tol) const{\
	GePoint3d closest;\
	GePoint3d itself = this->closestPointTo(line, closest, tol);\
	return closest.distanceTo(itself);\
}\
double CLASSNAME::distanceTo(const GeCircArc3d& entity) const {\
	return this->distanceTo(entity, GeContext::gTol);\
}\
double CLASSNAME::distanceTo(const GeCircArc3d& arc, const GeTol& tol) const{\
	GePoint3d closest; \
	GePoint3d itself = this->closestPointTo(arc, closest, tol); \
	return closest.distanceTo(itself); \
}\

#endif

