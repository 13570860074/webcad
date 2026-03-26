#include "export.h"
#include "RxExport.h"
#include "EmObject.h"
#include "GeInterval.h"
#include "AcArrayExport.h"

#include "GePoint2d.h"
#include "GePoint2dArray.h"
#include "GeScale2d.h"
#include "GeVector2d.h"
#include "GeVector2dArray.h"
#include "GeMatrix2d.h"
#include "GeMatrix2dArray.h"
#include "GePointEnt2d.h"
#include "GeCurve2d.h"
#include "GeCurve2dPtrArray.h"
#include "GeLinearEnt2d.h"
#include "GeLine2d.h"
#include "GeLineSeg2d.h"
#include "GeLineSeg2dArray.h"
#include "GeRay2d.h"
#include "GeCircArc2d.h"
#include "GePointOnCurve2d.h"
#include "GeBoundBlock2d.h"
#include "GeQuaternion.h"

#include "GePoint3d.h"
#include "GePoint3dArray.h"
#include "GeScale3d.h"
#include "GeVector3d.h"
#include "GeVector3dArray.h"
#include "GeMatrix3d.h"
#include "GeMatrix3dArray.h"
#include "GePointEnt3d.h"
#include "GeCurve3d.h"
#include "GeCurve3dPtrArray.h"
#include "GeLinearEnt3d.h"
#include "GeLine3d.h"
#include "GeLineSeg3d.h"
#include "GeLineSeg3dArray.h"
#include "GeRay3d.h"
#include "GeCircArc3d.h"
#include "GePointOnCurve3d.h"
#include "GeBoundBlock3d.h"
#include "GePointOnSurface.h"
#include "GeSurface.h"
#include "GeBoundedPlane.h"
#include "GePlane.h"

#if EMSDK

// GeEntity2d
#define GEENTITY2D_EMSDK_EXPORT_METHODS(CLASSNAME)                                                                   \
    .function("isKindOf", &CLASSNAME::isKindOf)                                                                      \
        .function("type", &CLASSNAME::type)                                                                          \
        .function("copy", &CLASSNAME::copy, allow_raw_pointers())                                                    \
        .function("isEqualTo", select_overload<bool(const CLASSNAME &) const>(&CLASSNAME::isEqualTo))                \
        .function("isEqualTo", select_overload<bool(const CLASSNAME &, const GeTol &) const>(&CLASSNAME::isEqualTo)) \
        .function("transformBy", &CLASSNAME::transformBy)                                                            \
        .function("translateBy", &CLASSNAME::translateBy)                                                            \
        .function("rotateBy", select_overload<CLASSNAME &(double)>(&CLASSNAME::rotateBy))                            \
        .function("rotateBy", select_overload<CLASSNAME &(double, const GePoint2d &)>(&CLASSNAME::rotateBy))         \
        .function("scaleBy", select_overload<CLASSNAME &(double)>(&CLASSNAME::scaleBy))                              \
        .function("scaleBy", select_overload<CLASSNAME &(double, const GePoint2d &)>(&CLASSNAME::scaleBy))           \
        .function("mirror", &CLASSNAME::mirror)                                                                      \
        .function("isOn", select_overload<bool(const GePoint2d &) const>(&CLASSNAME::isOn))                          \
        .function("isOn", select_overload<bool(const GePoint2d &, const GeTol &) const>(&CLASSNAME::isOn))

// GePointEnt2d
#define GEPOINTENT2D_EMSDK_EXPORT_METHODS(CLASSNAME) \
    .function("point2d", &CLASSNAME::point2d)

// GeCurve2d
#define GECURVE2D_EMSDK_EXPORT_METHODS(CLASSNAME)                                                                                                                              \
    .function("distanceTo", select_overload<double(const GePoint2d &) const>(&CLASSNAME::distanceTo))                                                                          \
        .function("distanceTo", select_overload<double(const GePoint2d &, const GeTol &) const>(&CLASSNAME::distanceTo))                                                       \
        .function("distanceTo", select_overload<double(const GeLine2d &) const>(&CLASSNAME::distanceTo))                                                                       \
        .function("distanceTo", select_overload<double(const GeLine2d &, const GeTol &) const>(&CLASSNAME::distanceTo))                                                        \
        .function("distanceTo", select_overload<double(const GeLineSeg2d &) const>(&CLASSNAME::distanceTo))                                                                    \
        .function("distanceTo", select_overload<double(const GeLineSeg2d &, const GeTol &) const>(&CLASSNAME::distanceTo))                                                     \
        .function("distanceTo", select_overload<double(const GeRay2d &) const>(&CLASSNAME::distanceTo))                                                                        \
        .function("distanceTo", select_overload<double(const GeRay2d &, const GeTol &) const>(&CLASSNAME::distanceTo))                                                         \
        .function("distanceTo", select_overload<double(const GeCircArc2d &) const>(&CLASSNAME::distanceTo))                                                                    \
        .function("distanceTo", select_overload<double(const GeCircArc2d &, const GeTol &) const>(&CLASSNAME::distanceTo))                                                     \
        .function("closestPointTo", select_overload<GePoint2d(const GePoint2d &) const>(&CLASSNAME::closestPointTo))                                                           \
        .function("closestPointTo", select_overload<GePoint2d(const GePoint2d &, const GeTol &) const>(&CLASSNAME::closestPointTo))                                            \
        .function("closestPointTo", select_overload<GePoint2d(const GeLine2d &, GePoint2d &) const>(&CLASSNAME::closestPointTo))                                               \
        .function("closestPointTo", select_overload<GePoint2d(const GeLine2d &, GePoint2d &, const GeTol &) const>(&CLASSNAME::closestPointTo))                                \
        .function("closestPointTo", select_overload<GePoint2d(const GeLineSeg2d &, GePoint2d &) const>(&CLASSNAME::closestPointTo))                                            \
        .function("closestPointTo", select_overload<GePoint2d(const GeLineSeg2d &, GePoint2d &, const GeTol &) const>(&CLASSNAME::closestPointTo))                             \
        .function("closestPointTo", select_overload<GePoint2d(const GeRay2d &, GePoint2d &) const>(&CLASSNAME::closestPointTo))                                                \
        .function("closestPointTo", select_overload<GePoint2d(const GeRay2d &, GePoint2d &, const GeTol &) const>(&CLASSNAME::closestPointTo))                                 \
        .function("closestPointTo", select_overload<GePoint2d(const GeCircArc2d &, GePoint2d &) const>(&CLASSNAME::closestPointTo))                                            \
        .function("closestPointTo", select_overload<GePoint2d(const GeCircArc2d &, GePoint2d &, const GeTol &) const>(&CLASSNAME::closestPointTo))                             \
        .function("getClosestPointTo", select_overload<void(const GePoint2d &, GePointOnCurve2d &) const>(&CLASSNAME::getClosestPointTo))                                      \
        .function("getClosestPointTo", select_overload<void(const GePoint2d &, GePointOnCurve2d &, const GeTol &) const>(&CLASSNAME::getClosestPointTo))                       \
        .function("getClosestPointTo", select_overload<void(const GeLine2d &, GePointOnCurve2d &, GePointOnCurve2d &) const>(&CLASSNAME::getClosestPointTo))                   \
        .function("getClosestPointTo", select_overload<void(const GeLine2d &, GePointOnCurve2d &, GePointOnCurve2d &, const GeTol &) const>(&CLASSNAME::getClosestPointTo))    \
        .function("getClosestPointTo", select_overload<void(const GeLineSeg2d &, GePointOnCurve2d &, GePointOnCurve2d &) const>(&CLASSNAME::getClosestPointTo))                \
        .function("getClosestPointTo", select_overload<void(const GeLineSeg2d &, GePointOnCurve2d &, GePointOnCurve2d &, const GeTol &) const>(&CLASSNAME::getClosestPointTo)) \
        .function("getClosestPointTo", select_overload<void(const GeRay2d &, GePointOnCurve2d &, GePointOnCurve2d &) const>(&CLASSNAME::getClosestPointTo))                    \
        .function("getClosestPointTo", select_overload<void(const GeRay2d &, GePointOnCurve2d &, GePointOnCurve2d &, const GeTol &) const>(&CLASSNAME::getClosestPointTo))     \
        .function("getClosestPointTo", select_overload<void(const GeCircArc2d &, GePointOnCurve2d &, GePointOnCurve2d &) const>(&CLASSNAME::getClosestPointTo))                \
        .function("getClosestPointTo", select_overload<void(const GeCircArc2d &, GePointOnCurve2d &, GePointOnCurve2d &, const GeTol &) const>(&CLASSNAME::getClosestPointTo)) \
        .function("length", select_overload<double() const>(&CLASSNAME::length))                                                                                               \
        .function("length", select_overload<double(double, double) const>(&CLASSNAME::length))                                                                                 \
        .function("length", select_overload<double(double, double, double) const>(&CLASSNAME::length))                                                                         \
        .function("paramAtLength", select_overload<double(double, double) const>(&CLASSNAME::paramAtLength))                                                                   \
        .function("paramAtLength", select_overload<double(double, double, double) const>(&CLASSNAME::paramAtLength))                                                           \
        .function("area", select_overload<double() const>(&CLASSNAME::area))                                                                                                   \
        .function("area", select_overload<double(const GeTol &) const>(&CLASSNAME::area))                                                                                      \
        .function("paramOf", select_overload<double(const GePoint2d &) const>(&CLASSNAME::paramOf))                                                                            \
        .function("paramOf", select_overload<double(const GePoint2d &, const GeTol &) const>(&CLASSNAME::paramOf))                                                             \
        .function("isClosed", select_overload<bool() const>(&CLASSNAME::isClosed))                                                                                             \
        .function("isClosed", select_overload<bool(const GeTol &) const>(&CLASSNAME::isClosed))                                                                                \
        .function("boundBlock", select_overload<GeBoundBlock2d() const>(&CLASSNAME::boundBlock))                                                                               \
        .function("boundBlock", select_overload<GeBoundBlock2d(const GeInterval &) const>(&CLASSNAME::boundBlock))                                                             \
        .function("orthoBoundBlock", select_overload<GeBoundBlock2d() const>(&CLASSNAME::orthoBoundBlock))                                                                     \
        .function("orthoBoundBlock", select_overload<GeBoundBlock2d(const GeInterval &) const>(&CLASSNAME::orthoBoundBlock))                                                   \
        .function("hasStartPoint", &CLASSNAME::hasStartPoint)                                                                                                                  \
        .function("hasEndPoint", &CLASSNAME::hasEndPoint)                                                                                                                      \
        .function("evalPoint", &CLASSNAME::evalPoint)

// GeLinearEnt2d
#define GELINEARENT2D_EMSDK_EXPORT_METHODS(CLASSNAME)                                                                                    \
    .function("intersectWith", select_overload<bool(const GeLine2d &, GePoint2d &) const>(&CLASSNAME::intersectWith))                    \
        .function("intersectWith", select_overload<bool(const GeLine2d &, GePoint2d &, const GeTol &) const>(&CLASSNAME::intersectWith)) \
        .function("intersectWith", select_overload<bool(const GeRay2d &, GePoint2d &) const>(&CLASSNAME::intersectWith))                 \
        .function("intersectWith", select_overload<bool(const GeRay2d &, GePoint2d &, const GeTol &) const>(&CLASSNAME::intersectWith))  \
        .function("intersectWith", select_overload<bool(const GeLineSeg2d &, GePoint2d &) const>(&CLASSNAME::intersectWith))             \
        .function("intersectWith", select_overload<bool(const GeLineSeg2d &, GePoint2d &, const GeTol &) const>(&CLASSNAME::intersectWith))

// GeEntity3d
#define GEENTITY3D_EMSDK_EXPORT_METHODS(CLASSNAME)                                                                               \
    .function("isKindOf", &CLASSNAME::isKindOf)                                                                                  \
        .function("type", &CLASSNAME::type)                                                                                      \
        .function("copy", &CLASSNAME::copy, allow_raw_pointers())                                                                \
        .function("isEqualTo", select_overload<bool(const CLASSNAME &) const>(&CLASSNAME::isEqualTo))                            \
        .function("isEqualTo", select_overload<bool(const CLASSNAME &, const GeTol &) const>(&CLASSNAME::isEqualTo))             \
        .function("transformBy", &CLASSNAME::transformBy)                                                                        \
        .function("translateBy", &CLASSNAME::translateBy)                                                                        \
        .function("rotateBy", select_overload<CLASSNAME &(double, const GeVector3d &)>(&CLASSNAME::rotateBy))                    \
        .function("rotateBy", select_overload<CLASSNAME &(double, const GeVector3d &, const GePoint3d &)>(&CLASSNAME::rotateBy)) \
        .function("scaleBy", select_overload<CLASSNAME &(double)>(&CLASSNAME::scaleBy))                                          \
        .function("scaleBy", select_overload<CLASSNAME &(double, const GePoint3d &)>(&CLASSNAME::scaleBy))                       \
        .function("mirror", &CLASSNAME::mirror)                                                                                  \
        .function("isOn", select_overload<bool(const GePoint3d &) const>(&CLASSNAME::isOn))                                      \
        .function("isOn", select_overload<bool(const GePoint3d &, const GeTol &) const>(&CLASSNAME::isOn))

// GePointEnt3d
#define GEPOINTENT3D_EMSDK_EXPORT_METHODS(CLASSNAME) \
    .function("point3d", &CLASSNAME::point3d)

// GeCurve3d
#define GECURVE3D_EMSDK_EXPORT_METHODS(CLASSNAME)                                                                                                                                                          \
    .function("distanceTo", select_overload<double(const GePoint3d &) const>(&CLASSNAME::distanceTo))                                                                                                      \
        .function("distanceTo", select_overload<double(const GePoint3d &, const GeTol &) const>(&CLASSNAME::distanceTo))                                                                                   \
        .function("distanceTo", select_overload<double(const GeLine3d &) const>(&CLASSNAME::distanceTo))                                                                                                   \
        .function("distanceTo", select_overload<double(const GeLine3d &, const GeTol &) const>(&CLASSNAME::distanceTo))                                                                                    \
        .function("distanceTo", select_overload<double(const GeLineSeg3d &) const>(&CLASSNAME::distanceTo))                                                                                                \
        .function("distanceTo", select_overload<double(const GeLineSeg3d &, const GeTol &) const>(&CLASSNAME::distanceTo))                                                                                 \
        .function("distanceTo", select_overload<double(const GeRay3d &) const>(&CLASSNAME::distanceTo))                                                                                                    \
        .function("distanceTo", select_overload<double(const GeRay3d &, const GeTol &) const>(&CLASSNAME::distanceTo))                                                                                     \
        .function("distanceTo", select_overload<double(const GeCircArc3d &) const>(&CLASSNAME::distanceTo))                                                                                                \
        .function("distanceTo", select_overload<double(const GeCircArc3d &, const GeTol &) const>(&CLASSNAME::distanceTo))                                                                                 \
        .function("closestPointTo", select_overload<GePoint3d(const GePoint3d &) const>(&CLASSNAME::closestPointTo))                                                                                       \
        .function("closestPointTo", select_overload<GePoint3d(const GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointTo))                                                                        \
        .function("closestPointTo", select_overload<GePoint3d(const GeLine3d &, GePoint3d &) const>(&CLASSNAME::closestPointTo))                                                                           \
        .function("closestPointTo", select_overload<GePoint3d(const GeLine3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointTo))                                                            \
        .function("closestPointTo", select_overload<GePoint3d(const GeLineSeg3d &, GePoint3d &) const>(&CLASSNAME::closestPointTo))                                                                        \
        .function("closestPointTo", select_overload<GePoint3d(const GeLineSeg3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointTo))                                                         \
        .function("closestPointTo", select_overload<GePoint3d(const GeRay3d &, GePoint3d &) const>(&CLASSNAME::closestPointTo))                                                                            \
        .function("closestPointTo", select_overload<GePoint3d(const GeRay3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointTo))                                                             \
        .function("closestPointTo", select_overload<GePoint3d(const GeCircArc3d &, GePoint3d &) const>(&CLASSNAME::closestPointTo))                                                                        \
        .function("closestPointTo", select_overload<GePoint3d(const GeCircArc3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointTo))                                                         \
        .function("getClosestPointTo", select_overload<void(const GePoint3d &, GePointOnCurve3d &) const>(&CLASSNAME::getClosestPointTo))                                                                  \
        .function("getClosestPointTo", select_overload<void(const GePoint3d &, GePointOnCurve3d &, const GeTol &) const>(&CLASSNAME::getClosestPointTo))                                                   \
        .function("getClosestPointTo", select_overload<void(const GeLine3d &, GePointOnCurve3d &, GePointOnCurve3d &) const>(&CLASSNAME::getClosestPointTo))                                               \
        .function("getClosestPointTo", select_overload<void(const GeLine3d &, GePointOnCurve3d &, GePointOnCurve3d &, const GeTol &) const>(&CLASSNAME::getClosestPointTo))                                \
        .function("getClosestPointTo", select_overload<void(const GeLineSeg3d &, GePointOnCurve3d &, GePointOnCurve3d &) const>(&CLASSNAME::getClosestPointTo))                                            \
        .function("getClosestPointTo", select_overload<void(const GeLineSeg3d &, GePointOnCurve3d &, GePointOnCurve3d &, const GeTol &) const>(&CLASSNAME::getClosestPointTo))                             \
        .function("getClosestPointTo", select_overload<void(const GeRay3d &, GePointOnCurve3d &, GePointOnCurve3d &) const>(&CLASSNAME::getClosestPointTo))                                                \
        .function("getClosestPointTo", select_overload<void(const GeRay3d &, GePointOnCurve3d &, GePointOnCurve3d &, const GeTol &) const>(&CLASSNAME::getClosestPointTo))                                 \
        .function("getClosestPointTo", select_overload<void(const GeCircArc3d &, GePointOnCurve3d &, GePointOnCurve3d &) const>(&CLASSNAME::getClosestPointTo))                                            \
        .function("getClosestPointTo", select_overload<void(const GeCircArc3d &, GePointOnCurve3d &, GePointOnCurve3d &, const GeTol &) const>(&CLASSNAME::getClosestPointTo))                             \
        .function("projClosestPointTo", select_overload<GePoint3d(const GePoint3d &, const GeVector3d &) const>(&CLASSNAME::projClosestPointTo))                                                           \
        .function("projClosestPointTo", select_overload<GePoint3d(const GePoint3d &, const GeVector3d &, const GeTol &tol) const>(&CLASSNAME::projClosestPointTo))                                         \
        .function("projClosestPointTo", select_overload<GePoint3d(const GeLine3d &, const GeVector3d &, GePoint3d &) const>(&CLASSNAME::projClosestPointTo))                                               \
        .function("projClosestPointTo", select_overload<GePoint3d(const GeLine3d &, const GeVector3d &, GePoint3d &, const GeTol &tol) const>(&CLASSNAME::projClosestPointTo))                             \
        .function("projClosestPointTo", select_overload<GePoint3d(const GeLineSeg3d &, const GeVector3d &, GePoint3d &) const>(&CLASSNAME::projClosestPointTo))                                            \
        .function("projClosestPointTo", select_overload<GePoint3d(const GeLineSeg3d &, const GeVector3d &, GePoint3d &, const GeTol &tol) const>(&CLASSNAME::projClosestPointTo))                          \
        .function("projClosestPointTo", select_overload<GePoint3d(const GeRay3d &, const GeVector3d &, GePoint3d &) const>(&CLASSNAME::projClosestPointTo))                                                \
        .function("projClosestPointTo", select_overload<GePoint3d(const GeRay3d &, const GeVector3d &, GePoint3d &, const GeTol &tol) const>(&CLASSNAME::projClosestPointTo))                              \
        .function("projClosestPointTo", select_overload<GePoint3d(const GeCircArc3d &, const GeVector3d &, GePoint3d &) const>(&CLASSNAME::projClosestPointTo))                                            \
        .function("projClosestPointTo", select_overload<GePoint3d(const GeCircArc3d &, const GeVector3d &, GePoint3d &, const GeTol &tol) const>(&CLASSNAME::projClosestPointTo))                          \
        .function("getProjClosestPointTo", select_overload<void(const GePoint3d &, const GeVector3d &, GePointOnCurve3d &) const>(&CLASSNAME::getProjClosestPointTo))                                      \
        .function("getProjClosestPointTo", select_overload<void(const GePoint3d &, const GeVector3d &, GePointOnCurve3d &, const GeTol &tol) const>(&CLASSNAME::getProjClosestPointTo))                    \
        .function("getProjClosestPointTo", select_overload<void(const GeLine3d &, const GeVector3d &, GePointOnCurve3d &, GePointOnCurve3d &) const>(&CLASSNAME::getProjClosestPointTo))                   \
        .function("getProjClosestPointTo", select_overload<void(const GeLine3d &, const GeVector3d &, GePointOnCurve3d &, GePointOnCurve3d &, const GeTol &) const>(&CLASSNAME::getProjClosestPointTo))    \
        .function("getProjClosestPointTo", select_overload<void(const GeLineSeg3d &, const GeVector3d &, GePointOnCurve3d &, GePointOnCurve3d &) const>(&CLASSNAME::getProjClosestPointTo))                \
        .function("getProjClosestPointTo", select_overload<void(const GeLineSeg3d &, const GeVector3d &, GePointOnCurve3d &, GePointOnCurve3d &, const GeTol &) const>(&CLASSNAME::getProjClosestPointTo)) \
        .function("getProjClosestPointTo", select_overload<void(const GeRay3d &, const GeVector3d &, GePointOnCurve3d &, GePointOnCurve3d &) const>(&CLASSNAME::getProjClosestPointTo))                    \
        .function("getProjClosestPointTo", select_overload<void(const GeRay3d &, const GeVector3d &, GePointOnCurve3d &, GePointOnCurve3d &, const GeTol &) const>(&CLASSNAME::getProjClosestPointTo))     \
        .function("getProjClosestPointTo", select_overload<void(const GeCircArc3d &, const GeVector3d &, GePointOnCurve3d &, GePointOnCurve3d &) const>(&CLASSNAME::getProjClosestPointTo))                \
        .function("getProjClosestPointTo", select_overload<void(const GeCircArc3d &, const GeVector3d &, GePointOnCurve3d &, GePointOnCurve3d &, const GeTol &) const>(&CLASSNAME::getProjClosestPointTo)) \
        .function("getNormalPoint", select_overload<bool(const GePoint3d &, GePointOnCurve3d &) const>(&CLASSNAME::getNormalPoint))                                                                        \
        .function("getNormalPoint", select_overload<bool(const GePoint3d &, GePointOnCurve3d &, const GeTol &) const>(&CLASSNAME::getNormalPoint))                                                         \
        .function("boundBlock", select_overload<GeBoundBlock3d() const>(&CLASSNAME::boundBlock))                                                                                                           \
        .function("boundBlock", select_overload<GeBoundBlock3d(const GeInterval &) const>(&CLASSNAME::boundBlock))                                                                                         \
        .function("orthoBoundBlock", select_overload<GeBoundBlock3d() const>(&CLASSNAME::orthoBoundBlock))                                                                                                 \
        .function("orthoBoundBlock", select_overload<GeBoundBlock3d(const GeInterval &) const>(&CLASSNAME::orthoBoundBlock))                                                                               \
        .function("project", select_overload<CLASSNAME *(const GePlane &, const GeVector3d &) const>(&CLASSNAME::project), allow_raw_pointers())                                                           \
        .function("project", select_overload<CLASSNAME *(const GePlane &, const GeVector3d &, const GeTol &tol) const>(&CLASSNAME::project), allow_raw_pointers())                                         \
        .function("orthoProject", select_overload<CLASSNAME *(const GePlane &) const>(&CLASSNAME::orthoProject), allow_raw_pointers())                                                                     \
        .function("orthoProject", select_overload<CLASSNAME *(const GePlane &, const GeTol &) const>(&CLASSNAME::orthoProject), allow_raw_pointers())                                                      \
        .function("isOn", select_overload<bool(double) const>(&CLASSNAME::isOn))                                                                                                                           \
        .function("isOn", select_overload<bool(double, const GeTol &) const>(&CLASSNAME::isOn))                                                                                                            \
        .function("paramOf", select_overload<double(const GePoint3d &) const>(&CLASSNAME::paramOf))                                                                                                        \
        .function("paramOf", select_overload<double(const GePoint3d &, const GeTol &) const>(&CLASSNAME::paramOf))                                                                                         \
        .function("isClosed", select_overload<bool() const>(&CLASSNAME::isClosed))                                                                                                                         \
        .function("isClosed", select_overload<bool(const GeTol &) const>(&CLASSNAME::isClosed))                                                                                                            \
        .function("isPlanar", select_overload<bool(GePlane &) const>(&CLASSNAME::isPlanar))                                                                                                                \
        .function("isPlanar", select_overload<bool(GePlane &, const GeTol &) const>(&CLASSNAME::isPlanar))                                                                                                 \
        .function("isLinear", select_overload<bool(GeLine3d &) const>(&CLASSNAME::isLinear))                                                                                                               \
        .function("isLinear", select_overload<bool(GeLine3d &, const GeTol &) const>(&CLASSNAME::isLinear))                                                                                                \
        .function("isCoplanarWith", select_overload<bool(const GeLine3d &, GePlane &) const>(&CLASSNAME::isCoplanarWith))                                                                                  \
        .function("isCoplanarWith", select_overload<bool(const GeLine3d &, GePlane &, const GeTol &) const>(&CLASSNAME::isCoplanarWith))                                                                   \
        .function("isCoplanarWith", select_overload<bool(const GeLineSeg3d &, GePlane &) const>(&CLASSNAME::isCoplanarWith))                                                                               \
        .function("isCoplanarWith", select_overload<bool(const GeLineSeg3d &, GePlane &, const GeTol &) const>(&CLASSNAME::isCoplanarWith))                                                                \
        .function("isCoplanarWith", select_overload<bool(const GeRay3d &, GePlane &) const>(&CLASSNAME::isCoplanarWith))                                                                                   \
        .function("isCoplanarWith", select_overload<bool(const GeRay3d &, GePlane &, const GeTol &) const>(&CLASSNAME::isCoplanarWith))                                                                    \
        .function("isCoplanarWith", select_overload<bool(const GeCircArc3d &, GePlane &) const>(&CLASSNAME::isCoplanarWith))                                                                               \
        .function("isCoplanarWith", select_overload<bool(const GeCircArc3d &, GePlane &, const GeTol &) const>(&CLASSNAME::isCoplanarWith))                                                                \
        .function("length", select_overload<double() const>(&CLASSNAME::length))                                                                                                                           \
        .function("length", select_overload<double(double, double) const>(&CLASSNAME::length))                                                                                                             \
        .function("length", select_overload<double(double, double, double) const>(&CLASSNAME::length))                                                                                                     \
        .function("paramAtLength", select_overload<double(double, double) const>(&CLASSNAME::paramAtLength))                                                                                               \
        .function("paramAtLength", select_overload<double(double, double, double) const>(&CLASSNAME::paramAtLength))                                                                                       \
        .function("area", select_overload<double() const>(&CLASSNAME::area))                                                                                                                               \
        .function("area", select_overload<double(const GeTol &) const>(&CLASSNAME::area))                                                                                                                  \
        .function("hasStartPoint", &CLASSNAME::hasStartPoint)                                                                                                                                              \
        .function("hasEndPoint", &CLASSNAME::hasEndPoint)                                                                                                                                                  \
        .function("evalPoint", &CLASSNAME::evalPoint)                                                                                                                                                      \
        .function("getSamplePoints", select_overload<void(double, double, double, GePoint3dArray &, GeDoubleArray &) const>(&CLASSNAME::getSamplePoints))                                                  \
        .function("getSamplePoints", select_overload<void(double, double, double, GePoint3dArray &, GeDoubleArray &, bool) const>(&CLASSNAME::getSamplePoints))                                            \
        .function("getSamplePoints", select_overload<void(int, GePoint3dArray &) const>(&CLASSNAME::getSamplePoints))                                                                                      \
        .function("getSamplePoints", select_overload<void(int, GePoint3dArray &, GeDoubleArray &) const>(&CLASSNAME::getSamplePoints))

// GeLinearEnt3d
#define GELINEARENT3D_EMSDK_EXPORT_METHODS(CLASSNAME)                                                                                    \
    .function("intersectWith", select_overload<bool(const GeLine3d &, GePoint3d &) const>(&CLASSNAME::intersectWith))                    \
        .function("intersectWith", select_overload<bool(const GeLine3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::intersectWith)) \
        .function("intersectWith", select_overload<bool(const GeRay3d &, GePoint3d &) const>(&CLASSNAME::intersectWith))                 \
        .function("intersectWith", select_overload<bool(const GeRay3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::intersectWith))  \
        .function("intersectWith", select_overload<bool(const GeLineSeg3d &, GePoint3d &) const>(&CLASSNAME::intersectWith))             \
        .function("intersectWith", select_overload<bool(const GeLineSeg3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::intersectWith))

// GeSurface
#define GESURFACE_EMSDK_EXPORT_METHODS(CLASSNAME)                                                                                                        \
    .function("paramOf", select_overload<GePoint2d(const GePoint3d &) const>(&CLASSNAME::paramOf))                                                       \
        .function("paramOf", select_overload<GePoint2d(const GePoint3d &, const GeTol &) const>(&CLASSNAME::paramOf))                                    \
        .function("isOn", select_overload<bool(const GePoint3d &, GePoint2d &) const>(&CLASSNAME::isOn))                                                 \
        .function("isOn", select_overload<bool(const GePoint3d &, GePoint2d &, const GeTol &) const>(&CLASSNAME::isOn))                                  \
        .function("closestPointTo", select_overload<GePoint3d(const GePoint3d &) const>(&CLASSNAME::closestPointTo))                                     \
        .function("closestPointTo", select_overload<GePoint3d(const GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointTo))                      \
        .function("getClosestPointTo", select_overload<void(const GePoint3d &, GePointOnSurface &) const>(&CLASSNAME::getClosestPointTo))                \
        .function("getClosestPointTo", select_overload<void(const GePoint3d &, GePointOnSurface &, const GeTol &) const>(&CLASSNAME::getClosestPointTo)) \
        .function("distanceTo", select_overload<double(const GePoint3d &) const>(&CLASSNAME::distanceTo))                                                \
        .function("distanceTo", select_overload<double(const GePoint3d &, const GeTol &) const>(&CLASSNAME::distanceTo))                                 \
        .function("isNormalReversed", &CLASSNAME::isNormalReversed)                                                                                      \
        .function("reverseNormal", &CLASSNAME::reverseNormal)                                                                                            \
        .function("evalPoint", &CLASSNAME::evalPoint)

// GePlanarEnt
#define GEPLANARENT_EMSDK_EXPORT_METHODS(CLASSNAME)                                                                                                                    \
    .function("intersectWith", select_overload<bool(const GeLinearEnt3d &, GePoint3d &) const>(&CLASSNAME::intersectWith))                                             \
        .function("intersectWith", select_overload<bool(const GeLinearEnt3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::intersectWith))                          \
        .function("closestPointToLinearEnt", select_overload<GePoint3d(const GeLinearEnt3d &, GePoint3d &) const>(&CLASSNAME::closestPointToLinearEnt))                \
        .function("closestPointToLinearEnt", select_overload<GePoint3d(const GeLinearEnt3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointToLinearEnt)) \
        .function("closestPointToPlanarEnt", select_overload<GePoint3d(const GePlanarEnt &, GePoint3d &) const>(&CLASSNAME::closestPointToPlanarEnt))                  \
        .function("closestPointToPlanarEnt", select_overload<GePoint3d(const GePlanarEnt &, GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointToPlanarEnt))  \
        .function("isParallelTo", select_overload<bool(const GeLinearEnt3d &) const>(&CLASSNAME::isParallelTo))                                                        \
        .function("isParallelTo", select_overload<bool(const GeLinearEnt3d &, const GeTol &) const>(&CLASSNAME::isParallelTo))                                         \
        .function("isParallelTo", select_overload<bool(const GePlanarEnt &) const>(&CLASSNAME::isParallelTo))                                                          \
        .function("isParallelTo", select_overload<bool(const GePlanarEnt &, const GeTol &) const>(&CLASSNAME::isParallelTo))                                           \
        .function("isPerpendicularTo", select_overload<bool(const GeLinearEnt3d &) const>(&CLASSNAME::isPerpendicularTo))                                              \
        .function("isPerpendicularTo", select_overload<bool(const GeLinearEnt3d &, const GeTol &) const>(&CLASSNAME::isPerpendicularTo))                               \
        .function("isPerpendicularTo", select_overload<bool(const GePlanarEnt &) const>(&CLASSNAME::isPerpendicularTo))                                                \
        .function("isPerpendicularTo", select_overload<bool(const GePlanarEnt &, const GeTol &) const>(&CLASSNAME::isPerpendicularTo))                                 \
        .function("isCoplanarTo", select_overload<bool(const GePlanarEnt &) const>(&CLASSNAME::isCoplanarTo))                                                          \
        .function("isCoplanarTo", select_overload<bool(const GePlanarEnt &, const GeTol &) const>(&CLASSNAME::isCoplanarTo))

// GePlane / GeBoundedPlane (uses concrete types from GEPLANARENT_METHODS)
#define GEPLANE_EMSDK_EXPORT_METHODS(CLASSNAME)                                                                                                                               \
    .function("intersectWith", select_overload<bool(const GeLine3d &, GePoint3d &) const>(&CLASSNAME::intersectWith))                                                         \
        .function("intersectWith", select_overload<bool(const GeLine3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::intersectWith))                                      \
        .function("intersectWith", select_overload<bool(const GeLineSeg3d &, GePoint3d &) const>(&CLASSNAME::intersectWith))                                                  \
        .function("intersectWith", select_overload<bool(const GeLineSeg3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::intersectWith))                                   \
        .function("intersectWith", select_overload<bool(const GeRay3d &, GePoint3d &) const>(&CLASSNAME::intersectWith))                                                      \
        .function("intersectWith", select_overload<bool(const GeRay3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::intersectWith))                                       \
        .function("closestPointToLinearEnt", select_overload<GePoint3d(const GeLine3d &, GePoint3d &) const>(&CLASSNAME::closestPointToLinearEnt))                            \
        .function("closestPointToLinearEnt", select_overload<GePoint3d(const GeLine3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointToLinearEnt))             \
        .function("closestPointToLinearEnt", select_overload<GePoint3d(const GeLineSeg3d &, GePoint3d &) const>(&CLASSNAME::closestPointToLinearEnt))                         \
        .function("closestPointToLinearEnt", select_overload<GePoint3d(const GeLineSeg3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointToLinearEnt))          \
        .function("closestPointToLinearEnt", select_overload<GePoint3d(const GeRay3d &, GePoint3d &) const>(&CLASSNAME::closestPointToLinearEnt))                             \
        .function("closestPointToLinearEnt", select_overload<GePoint3d(const GeRay3d &, GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointToLinearEnt))              \
        .function("closestPointToPlanarEnt", select_overload<GePoint3d(const GePlane &, GePoint3d &) const>(&CLASSNAME::closestPointToPlanarEnt))                             \
        .function("closestPointToPlanarEnt", select_overload<GePoint3d(const GePlane &, GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointToPlanarEnt))              \
        .function("closestPointToPlanarEnt", select_overload<GePoint3d(const GeBoundedPlane &, GePoint3d &) const>(&CLASSNAME::closestPointToPlanarEnt))                      \
        .function("closestPointToPlanarEnt", select_overload<GePoint3d(const GeBoundedPlane &, GePoint3d &, const GeTol &) const>(&CLASSNAME::closestPointToPlanarEnt))       \
        .function("isParallelTo", select_overload<bool(const GeLine3d &) const>(&CLASSNAME::isParallelTo))                                                                    \
        .function("isParallelTo", select_overload<bool(const GeLine3d &, const GeTol &) const>(&CLASSNAME::isParallelTo))                                                     \
        .function("isParallelTo", select_overload<bool(const GeLineSeg3d &) const>(&CLASSNAME::isParallelTo))                                                                 \
        .function("isParallelTo", select_overload<bool(const GeLineSeg3d &, const GeTol &) const>(&CLASSNAME::isParallelTo))                                                  \
        .function("isParallelTo", select_overload<bool(const GeRay3d &) const>(&CLASSNAME::isParallelTo))                                                                     \
        .function("isParallelTo", select_overload<bool(const GeRay3d &, const GeTol &) const>(&CLASSNAME::isParallelTo))                                                      \
        .function("isParallelTo", select_overload<bool(const GePlane &) const>(&CLASSNAME::isParallelTo))                                                                     \
        .function("isParallelTo", select_overload<bool(const GePlane &, const GeTol &) const>(&CLASSNAME::isParallelTo))                                                      \
        .function("isParallelTo", select_overload<bool(const GeBoundedPlane &) const>(&CLASSNAME::isParallelTo))                                                              \
        .function("isParallelTo", select_overload<bool(const GeBoundedPlane &, const GeTol &) const>(&CLASSNAME::isParallelTo))                                               \
        .function("isPerpendicularTo", select_overload<bool(const GeLine3d &) const>(&CLASSNAME::isPerpendicularTo))                                                          \
        .function("isPerpendicularTo", select_overload<bool(const GeLine3d &, const GeTol &) const>(&CLASSNAME::isPerpendicularTo))                                           \
        .function("isPerpendicularTo", select_overload<bool(const GeLineSeg3d &) const>(&CLASSNAME::isPerpendicularTo))                                                       \
        .function("isPerpendicularTo", select_overload<bool(const GeLineSeg3d &, const GeTol &) const>(&CLASSNAME::isPerpendicularTo))                                        \
        .function("isPerpendicularTo", select_overload<bool(const GeRay3d &) const>(&CLASSNAME::isPerpendicularTo))                                                           \
        .function("isPerpendicularTo", select_overload<bool(const GeRay3d &, const GeTol &) const>(&CLASSNAME::isPerpendicularTo))                                            \
        .function("isPerpendicularTo", select_overload<bool(const GePlane &) const>(&CLASSNAME::isPerpendicularTo))                                                           \
        .function("isPerpendicularTo", select_overload<bool(const GePlane &, const GeTol &) const>(&CLASSNAME::isPerpendicularTo))                                            \
        .function("isPerpendicularTo", select_overload<bool(const GeBoundedPlane &) const>(&CLASSNAME::isPerpendicularTo))                                                    \
        .function("isPerpendicularTo", select_overload<bool(const GeBoundedPlane &, const GeTol &) const>(&CLASSNAME::isPerpendicularTo))                                     \
        .function("isCoplanarTo", select_overload<bool(const GePlane &) const>(&CLASSNAME::isCoplanarTo))                                                                     \
        .function("isCoplanarTo", select_overload<bool(const GePlane &, const GeTol &) const>(&CLASSNAME::isCoplanarTo))                                                      \
        .function("isCoplanarTo", select_overload<bool(const GeBoundedPlane &) const>(&CLASSNAME::isCoplanarTo))                                                              \
        .function("isCoplanarTo", select_overload<bool(const GeBoundedPlane &, const GeTol &) const>(&CLASSNAME::isCoplanarTo))

EMSCRIPTEN_BINDINGS(Ge)
{

    // EntityId
    enum_<Ge::EntityId>("EntityId")
        .value("kEntity2d", Ge::kEntity2d)
        .value("kEntity3d", Ge::kEntity3d)
        .value("kPointEnt2d", Ge::kPointEnt2d)
        .value("kPointEnt3d", Ge::kPointEnt3d)
        .value("kPosition2d", Ge::kPosition2d)
        .value("kPosition3d", Ge::kPosition3d)
        .value("kPointOnCurve2d", Ge::kPointOnCurve2d)
        .value("kPointOnCurve3d", Ge::kPointOnCurve3d)
        .value("kPointOnSurface", Ge::kPointOnSurface)
        .value("kBoundedPlane", Ge::kBoundedPlane)
        .value("kCircArc2d", Ge::kCircArc2d)
        .value("kCircArc3d", Ge::kCircArc3d)
        .value("kConic2d", Ge::kConic2d)
        .value("kConic3d", Ge::kConic3d)
        .value("kCurve2d", Ge::kCurve2d)
        .value("kCurve3d", Ge::kCurve3d)
        .value("kEllipArc2d", Ge::kEllipArc2d)
        .value("kEllipArc3d", Ge::kEllipArc3d)
        .value("kLine2d", Ge::kLine2d)
        .value("kLine3d", Ge::kLine3d)
        .value("kLinearEnt2d", Ge::kLinearEnt2d)
        .value("kLinearEnt3d", Ge::kLinearEnt3d)
        .value("kLineSeg2d", Ge::kLineSeg2d)
        .value("kLineSeg3d", Ge::kLineSeg3d)
        .value("kPlanarEnt", Ge::kPlanarEnt)
        .value("kPlane", Ge::kPlane)
        .value("kRay2d", Ge::kRay2d)
        .value("kRay3d", Ge::kRay3d)
        .value("kSurface", Ge::kSurface)
        .value("kSphere", Ge::kSphere)
        .value("kCylinder", Ge::kCylinder)
        .value("kTorus", Ge::kTorus)
        .value("kCone", Ge::kCone)
        .value("kSplineEnt2d", Ge::kSplineEnt2d)
        .value("kPolyline2d", Ge::kPolyline2d)
        .value("kAugPolyline2d", Ge::kAugPolyline2d)
        .value("kNurbCurve2d", Ge::kNurbCurve2d)
        .value("kDSpline2d", Ge::kDSpline2d)
        .value("kCubicSplineCurve2d", Ge::kCubicSplineCurve2d)
        .value("kSplineEnt3d", Ge::kSplineEnt3d)
        .value("kPolyline3d", Ge::kPolyline3d)
        .value("kAugPolyline3d", Ge::kAugPolyline3d)
        .value("kNurbCurve3d", Ge::kNurbCurve3d)
        .value("kDSpline3d", Ge::kDSpline3d)
        .value("kCubicSplineCurve3d", Ge::kCubicSplineCurve3d)
        .value("kTrimmedCrv2d", Ge::kTrimmedCrv2d)
        .value("kCompositeCrv2d", Ge::kCompositeCrv2d)
        .value("kCompositeCrv3d", Ge::kCompositeCrv3d)
        .value("kExternalSurface", Ge::kExternalSurface)
        .value("kNurbSurface", Ge::kNurbSurface)
        .value("kTrimmedSurface", Ge::kTrimmedSurface)
        .value("kOffsetSurface", Ge::kOffsetSurface)
        .value("kEnvelope2d", Ge::kEnvelope2d)
        .value("kCurveBoundedSurface", Ge::kCurveBoundedSurface)
        .value("kExternalCurve3d", Ge::kExternalCurve3d)
        .value("kExternalCurve2d", Ge::kExternalCurve2d)
        .value("kSurfaceCurve2dTo3d", Ge::kSurfaceCurve2dTo3d)
        .value("kSurfaceCurve3dTo2d", Ge::kSurfaceCurve3dTo2d)
        .value("kExternalBoundedSurface", Ge::kExternalBoundedSurface)
        .value("kCurveCurveInt2d", Ge::kCurveCurveInt2d)
        .value("kCurveCurveInt3d", Ge::kCurveCurveInt3d)
        .value("kBoundBlock2d", Ge::kBoundBlock2d)
        .value("kBoundBlock3d", Ge::kBoundBlock3d)
        .value("kOffsetCurve2d", Ge::kOffsetCurve2d)
        .value("kOffsetCurve3d", Ge::kOffsetCurve3d)
        .value("kPolynomCurve3d", Ge::kPolynomCurve3d)
        .value("kBezierCurve3d", Ge::kBezierCurve3d)
        .value("kObject", Ge::kObject)
        .value("kFitData3d", Ge::kFitData3d)
        .value("kHatch", Ge::kHatch)
        .value("kTrimmedCurve2d", Ge::kTrimmedCurve2d)
        .value("kTrimmedCurve3d", Ge::kTrimmedCurve3d)
        .value("kCurveSampleData", Ge::kCurveSampleData)
        .value("kEllipCone", Ge::kEllipCone)
        .value("kEllipCylinder", Ge::kEllipCylinder)
        .value("kIntervalBoundBlock", Ge::kIntervalBoundBlock)
        .value("kClipBoundary2d", Ge::kClipBoundary2d)
        .value("kExternalObject", Ge::kExternalObject)
        .value("kCurveSurfaceInt", Ge::kCurveSurfaceInt)
        .value("kSurfaceSurfaceInt", Ge::kSurfaceSurfaceInt)
        .value("kHelix", Ge::kHelix)
        .value("kSpunSurface", Ge::kSpunSurface)
        .value("kSegmentChain2d", Ge::kSegmentChain2d)
        .value("kRevolvedSurface", Ge::kRevolvedSurface)
        .value("kDummy1", Ge::kDummy1)
        .value("kDummy2", Ge::kDummy2)
        .value("kDummy3", Ge::kDummy3)
        .value("kDummy4", Ge::kDummy4)
        .value("kDummy5", Ge::kDummy5)
        .value("kDummy6", Ge::kDummy6)
        .value("kDummy7", Ge::kDummy7)
        .value("kDummy8", Ge::kDummy8)
        .value("kDummy9", Ge::kDummy9);

    // ExternalEntityKind
    enum_<Ge::ExternalEntityKind>("ExternalEntityKind")
        .value("kAcisEntity", Ge::kAcisEntity)
        .value("kGe3dCurveEntity", Ge::kGe3dCurveEntity)
        .value("kGe3dSurfaceEntity", Ge::kGe3dSurfaceEntity)
        .value("kExternalEntityUndefined", Ge::kExternalEntityUndefined)
        .value("kBimEntity", Ge::kBimEntity);

    // PointContainment
    enum_<Ge::PointContainment>("PointContainment")
        .value("kInside", Ge::kInside)
        .value("kOutside", Ge::kOutside)
        .value("kOnBoundary", Ge::kOnBoundary);

    // OffsetCrvExtType
    enum_<Ge::OffsetCrvExtType>("OffsetCrvExtType")
        .value("kFillet", Ge::kFillet)
        .value("kChamfer", Ge::kChamfer)
        .value("kOnBoundary", Ge::kExtend);

    // GeXConfig
    enum_<Ge::GeXConfig>("GeXConfig")
        .value("kNotDefined", Ge::kNotDefined)
        .value("kUnknown", Ge::kUnknown)
        .value("kLeftRight", Ge::kLeftRight)
        .value("kRightLeft", Ge::kRightLeft)
        .value("kRightRight", Ge::kRightRight)
        .value("kPointLeft", Ge::kPointLeft)
        .value("kPointRight", Ge::kPointRight)
        .value("kLeftOverlap", Ge::kLeftOverlap)
        .value("kOverlapLeft", Ge::kOverlapLeft)
        .value("kRightOverlap", Ge::kRightOverlap)
        .value("kOverlapRight", Ge::kOverlapRight)
        .value("kOverlapStart", Ge::kOverlapStart)
        .value("kOverlapEnd", Ge::kOverlapEnd)
        .value("kOverlapOverlap", Ge::kOverlapOverlap);

    // BooleanType
    enum_<Ge::BooleanType>("BooleanType")
        .value("kUnion", Ge::kUnion)
        .value("kSubtract", Ge::kSubtract)
        .value("kCommon", Ge::kCommon);

    // ClipError
    enum_<Ge::ClipError>("ClipError")
        .value("eOk", Ge::eOk)
        .value("eInvalidClipBoundary", Ge::eInvalidClipBoundary)
        .value("eNotInitialized", Ge::eNotInitialized);

    // ClipError
    enum_<Ge::ClipCondition>("ClipCondition")
        .value("kInvalid", Ge::kInvalid)
        .value("kAllSegmentsInside", Ge::kAllSegmentsInside)
        .value("kSegmentsIntersect", Ge::kSegmentsIntersect)
        .value("kAllSegmentsOutsideZeroWinds", Ge::kAllSegmentsOutsideZeroWinds)
        .value("kAllSegmentsOutsideOddWinds", Ge::kAllSegmentsOutsideOddWinds)
        .value("kAllSegmentsOutsideEvenWinds", Ge::kAllSegmentsOutsideEvenWinds);

    // ErrorCondition
    enum_<Ge::ErrorCondition>("ErrorCondition")
        .value("kOk", Ge::kOk)
        .value("k0This", Ge::k0This)
        .value("k0Arg1", Ge::k0Arg1)
        .value("k0Arg2", Ge::k0Arg2)
        .value("kPerpendicularArg1Arg2", Ge::kPerpendicularArg1Arg2)
        .value("kEqualArg1Arg2", Ge::kEqualArg1Arg2)
        .value("kEqualArg1Arg3", Ge::kEqualArg1Arg3)
        .value("kEqualArg2Arg3", Ge::kEqualArg2Arg3)
        .value("kLinearlyDependentArg1Arg2Arg3", Ge::kLinearlyDependentArg1Arg2Arg3)
        .value("kArg1TooBig", Ge::kArg1TooBig)
        .value("kArg1OnThis", Ge::kArg1OnThis)
        .value("kArg1InsideThis", Ge::kArg1InsideThis)
        .value("kNonCoplanarGeometry", Ge::kNonCoplanarGeometry)
        .value("kDegenerateGeometry", Ge::kDegenerateGeometry)
        .value("kSingularPoint", Ge::kSingularPoint);

    // csiConfig
    enum_<Ge::csiConfig>("csiConfig")
        .value("kXUnknown", Ge::kXUnknown)
        .value("kXOut", Ge::kXOut)
        .value("kXIn", Ge::kXIn)
        .value("kXTanOut", Ge::kXTanOut)
        .value("kXTanIn", Ge::kXTanIn)
        .value("kXCoincident", Ge::kXCoincident)
        .value("kXCoincidentUnbounded", Ge::kXCoincidentUnbounded);

    // ssiType
    enum_<Ge::ssiType>("ssiType")
        .value("kSSITransverse", Ge::kSSITransverse)
        .value("kSSITangent", Ge::kSSITangent)
        .value("kSSIAntiTangent", Ge::kSSIAntiTangent);

    // ssiConfig
    enum_<Ge::ssiConfig>("ssiConfig")
        .value("kSSIUnknown", Ge::kSSIUnknown)
        .value("kSSIOut", Ge::kSSIOut)
        .value("kSSIIn", Ge::kSSIIn)
        .value("kSSICoincident", Ge::kSSICoincident);

    // GeIntersectError
    enum_<Ge::GeIntersectError>("GeIntersectError")
        .value("kXXOk", Ge::kXXOk)
        .value("kXXIndexOutOfRange", Ge::kXXIndexOutOfRange)
        .value("kXXWrongDimensionAtIndex", Ge::kXXWrongDimensionAtIndex)
        .value("kXXUnknown", Ge::kXXUnknown);

    // GeKnotParameterization
    enum_<Ge::GeKnotParameterization>("GeKnotParameterization")
        .value("kChord", Ge::kChord)
        .value("kSqrtChord", Ge::kSqrtChord)
        .value("kUniform", Ge::kUniform)
        .value("kCustomParameterization", Ge::kCustomParameterization)
        .value("kNotDefinedKnotParam", Ge::kNotDefinedKnotParam);

    // GeIntArray
    emscripten::class_<GeIntArray>("GeIntArray")
        .constructor()
            ACARRAY_EMSDK_EXPORT_METHODS(GeIntArray, int);

    // GeDoubleArray
    emscripten::class_<GeDoubleArray>("GeDoubleArray")
        .constructor()
            ACARRAY_EMSDK_EXPORT_METHODS(GeDoubleArray, double);

    // GeVector2dArray
    emscripten::class_<GeVector2dArray>("GeVector2dArray")
        .constructor()
            ACARRAY_EMSDK_EXPORT_METHODS(GeVector2dArray, GeVector2d);

    // GeVector3dArray
    emscripten::class_<GeVector3dArray>("GeVector3dArray")
        .constructor()
            ACARRAY_EMSDK_EXPORT_METHODS(GeVector3dArray, GeVector3d);

    // GePoint2dArray
    emscripten::class_<GePoint2dArray>("GePoint2dArray")
        .constructor()
            ACARRAY_EMSDK_EXPORT_METHODS(GePoint2dArray, GePoint2d);

    // GePoint3dArray
    emscripten::class_<GePoint3dArray>("GePoint3dArray")
        .constructor()
            ACARRAY_EMSDK_EXPORT_METHODS(GePoint3dArray, GePoint3d);

    // GeCurve2dPtrArray
    emscripten::class_<GeCurve2dPtrArray>("GeCurve2dPtrArray")
        .constructor()
            ACARRAY_PTR_EMSDK_EXPORT_METHODS(GeCurve2dPtrArray, GeCurve2d*);

    // GeCurve3dPtrArray
    emscripten::class_<GeCurve3dPtrArray>("GeCurve3dPtrArray")
        .constructor()
            ACARRAY_PTR_EMSDK_EXPORT_METHODS(GeCurve3dPtrArray, GeCurve3d*);

    // GeLineSeg2dArray
    emscripten::class_<GeLineSeg2dArray>("GeLineSeg2dArray")
        .constructor()
            ACARRAY_EMSDK_EXPORT_METHODS(GeLineSeg2dArray, GeLineSeg2d);

    // GeLineSeg3dArray
    emscripten::class_<GeLineSeg3dArray>("GeLineSeg3dArray")
        .constructor()
            ACARRAY_EMSDK_EXPORT_METHODS(GeLineSeg3dArray, GeLineSeg3d);

    // GeMatrix2dArray
    emscripten::class_<GeMatrix2dArray>("GeMatrix2dArray")
        .constructor()
            ACARRAY_EMSDK_EXPORT_METHODS(GeMatrix2dArray, GeMatrix2d);

    // GeMatrix3dArray
    emscripten::class_<GeMatrix3dArray>("GeMatrix3dArray")
        .constructor()
            ACARRAY_EMSDK_EXPORT_METHODS(GeMatrix3dArray, GeMatrix3d);

    // GeTol
    emscripten::class_<GeTol>("GeTol")
        .constructor<double>()
        .constructor<double, double>()
        .function("equalPoint", &GeTol::equalPoint)
        .function("equalVector", &GeTol::equalVector)
        .function("setEqualPoint", &GeTol::setEqualPoint)
        .function("setEqualVector", &GeTol::setEqualVector);

    // GeInterval
    emscripten::class_<GeInterval, base<EmObject>>("GeInterval")
        .constructor()
        .constructor<double>()
        .constructor<double, double>()
        .constructor<double, double, double>()
        .function("lowerBound", &GeInterval::lowerBound)
        .function("upperBound", &GeInterval::upperBound)
        .function("element", &GeInterval::element)
        .function("length", &GeInterval::length)
        .function("tolerance", &GeInterval::tolerance)
        .function("set", select_overload<GeInterval &(double, double)>(&GeInterval::set))
        .function("set", select_overload<GeInterval &(bool, double)>(&GeInterval::set))
        .function("set", select_overload<GeInterval &()>(&GeInterval::set))
        .function("setUpper", &GeInterval::setUpper)
        .function("setLower", &GeInterval::setLower)
        .function("setTolerance", &GeInterval::setTolerance)
        .function("getMerge", &GeInterval::getMerge)
        .function("subtract", &GeInterval::subtract)
        .function("intersectWith", &GeInterval::intersectWith)
        .function("isBounded", &GeInterval::isBounded)
        .function("isBoundedAbove", &GeInterval::isBoundedAbove)
        .function("isBoundedBelow", &GeInterval::isBoundedBelow)
        .function("isUnBounded", &GeInterval::isUnBounded)
        .function("isSingleton", &GeInterval::isSingleton)
        .function("isDisjoint", &GeInterval::isDisjoint)
        .function("contains", select_overload<bool(const GeInterval &) const>(&GeInterval::contains))
        .function("contains", select_overload<bool(double) const>(&GeInterval::contains))
        .function("isEqualAtUpper", select_overload<bool(const GeInterval &) const>(&GeInterval::isEqualAtUpper))
        .function("isEqualAtUpper", select_overload<bool(double) const>(&GeInterval::isEqualAtUpper))
        .function("isEqualAtLower", select_overload<bool(const GeInterval &) const>(&GeInterval::isEqualAtLower))
        .function("isEqualAtLower", select_overload<bool(double) const>(&GeInterval::isEqualAtLower));

    // GeMatrix2d
    emscripten::class_<GeMatrix2d, base<EmObject>>("GeMatrix2d")
        .constructor()
        .constructor<const GeMatrix2d &>()
        .class_property("kIdentity", &GeMatrix2d::kIdentity)
        .function("set", &GeMatrix2d::set)
        .function("setToIdentity", &GeMatrix2d::setToIdentity)
        .function("preMultBy", &GeMatrix2d::preMultBy)
        .function("postMultBy", &GeMatrix2d::postMultBy)
        .function("setToProduct", &GeMatrix2d::setToProduct)
        .function("invert", &GeMatrix2d::invert)
        .function("inverse", &GeMatrix2d::inverse)
        .function("isSingular", select_overload<bool() const>(&GeMatrix2d::isSingular))
        .function("isSingular", select_overload<bool(const GeTol &) const>(&GeMatrix2d::isSingular))
        .function("transposeIt", &GeMatrix2d::transposeIt)
        .function("transpose", &GeMatrix2d::transpose)
        .function("isEqualTo", select_overload<bool(const GeMatrix2d &) const>(&GeMatrix2d::isEqualTo))
        .function("isEqualTo", select_overload<bool(const GeMatrix2d &, const GeTol &) const>(&GeMatrix2d::isEqualTo))
        .function("scale", &GeMatrix2d::scale)
        .function("det", &GeMatrix2d::det)
        .function("setTranslation", &GeMatrix2d::setTranslation)
        .function("translation", select_overload<GeVector2d() const>(&GeMatrix2d::translation))
        .function("setCoordSystem", &GeMatrix2d::setCoordSystem)
        .function("getCoordSystem", &GeMatrix2d::getCoordSystem)
        .function("setToTranslation", &GeMatrix2d::setToTranslation)
        .function("setToRotation", select_overload<GeMatrix2d &(double)>(&GeMatrix2d::setToRotation))
        .function("setToRotation", select_overload<GeMatrix2d &(double, const GePoint2d &)>(&GeMatrix2d::setToRotation))
        .function("setToScaling", select_overload<GeMatrix2d &(double)>(&GeMatrix2d::setToScaling))
        .function("setToScaling", select_overload<GeMatrix2d &(double, const GePoint2d &)>(&GeMatrix2d::setToScaling))
        .function("setToScaling", select_overload<GeMatrix2d &(const GeScale2d &)>(&GeMatrix2d::setToScaling))
        .function("setToScaling", select_overload<GeMatrix2d &(const GeScale2d &, const GePoint2d &)>(&GeMatrix2d::setToScaling))
        .function("setToMirroring", select_overload<GeMatrix2d &(const GePoint2d &)>(&GeMatrix2d::setToMirroring))
        .function("setToMirroring", select_overload<GeMatrix2d &(const GeLine2d &)>(&GeMatrix2d::setToMirroring))
        .function("setToAlignCoordSys", &GeMatrix2d::setToAlignCoordSys)
        .class_function("translation", select_overload<GeMatrix2d(const GeVector2d &)>(&GeMatrix2d::translation))
        .class_function("rotation", select_overload<GeMatrix2d(double)>(&GeMatrix2d::rotation))
        .class_function("rotation", select_overload<GeMatrix2d(double, const GePoint2d &)>(&GeMatrix2d::rotation))
        .class_function("scaling", select_overload<GeMatrix2d(double)>(&GeMatrix2d::scaling))
        .class_function("scaling", select_overload<GeMatrix2d(double, const GePoint2d &)>(&GeMatrix2d::scaling))
        .class_function("scaling", select_overload<GeMatrix2d(const GeScale2d &)>(&GeMatrix2d::scaling))
        .class_function("scaling", select_overload<GeMatrix2d(const GeScale2d &, const GePoint2d &)>(&GeMatrix2d::scaling))
        .class_function("mirroring", select_overload<GeMatrix2d(const GePoint2d &)>(&GeMatrix2d::mirroring))
        .class_function("mirroring", select_overload<GeMatrix2d(const GeLine2d &)>(&GeMatrix2d::mirroring))
        .class_function("alignCoordSys", &GeMatrix2d::alignCoordSys);

    // GeScale2d
    emscripten::class_<GeScale2d, base<EmObject>>("GeScale2d")
        .property("sx", &GeScale2d::sx)
        .property("sy", &GeScale2d::sy)
        .constructor()
        .constructor<const GeScale2d &>()
        .constructor<double, double>()
        .class_property("kIdentity", &GeScale2d::kIdentity)
        .function("preMultBy", &GeScale2d::preMultBy)
        .function("postMultBy", &GeScale2d::postMultBy)
        .function("setToProduct", select_overload<GeScale2d &(const GeScale2d &, double)>(&GeScale2d::setToProduct))
        .function("setToProduct", select_overload<GeScale2d &(const GeScale2d &, const GeScale2d &)>(&GeScale2d::setToProduct))
        .function("inverse", &GeScale2d::inverse)
        .function("invert", &GeScale2d::invert)
        .function("isProportional", select_overload<bool() const>(&GeScale2d::isProportional))
        .function("isProportional", select_overload<bool(const GeTol &) const>(&GeScale2d::isProportional))
        .function("isEqualTo", select_overload<bool(const GeScale2d &) const>(&GeScale2d::isEqualTo))
        .function("isEqualTo", select_overload<bool(const GeScale2d &, const GeTol &) const>(&GeScale2d::isEqualTo))
        .function("set", &GeScale2d::set)
        .function("getMatrix", &GeScale2d::getMatrix)
        .function("extractScale", &GeScale2d::extractScale)
        .function("removeScale", &GeScale2d::removeScale);

    // GeVector2d
    emscripten::class_<GeVector2d, base<EmObject>>("GeVector2d")
        .property("x", &GeVector2d::x)
        .property("y", &GeVector2d::y)
        .constructor()
        .constructor<const GeVector2d &>()
        .constructor<double, double>()
        .class_property("kIdentity", &GeVector2d::kIdentity)
        .class_property("kXAxis", &GeVector2d::kXAxis)
        .class_property("kYAxis", &GeVector2d::kYAxis)
        .function("setToProduct", select_overload<GeVector2d &(const GeMatrix2d &, const GeVector2d &)>(&GeVector2d::setToProduct))
        .function("setToProduct", select_overload<GeVector2d &(const GeVector2d &, double)>(&GeVector2d::setToProduct))
        .function("transformBy", &GeVector2d::transformBy)
        .function("rotateBy", &GeVector2d::rotateBy)
        .function("mirror", &GeVector2d::mirror)
        .function("crossProduct", &GeVector2d::crossProduct)
        .function("setToSum", &GeVector2d::setToSum)
        .function("negate", &GeVector2d::negate)
        .function("perpVector", &GeVector2d::perpVector)
        .function("angle", &GeVector2d::angle)
        .function("angleTo", &GeVector2d::angleTo)
        .function("angleToCCW", &GeVector2d::angleToCCW)
        .function("normal", select_overload<GeVector2d() const>(&GeVector2d::normal))
        .function("normal", select_overload<GeVector2d(const GeTol &) const>(&GeVector2d::normal))
        .function("normalize", select_overload<GeVector2d &()>(&GeVector2d::normalize))
        .function("normalize", select_overload<GeVector2d &(const GeTol &)>(&GeVector2d::normalize))
        .function("length", &GeVector2d::length)
        .function("lengthSqrd", &GeVector2d::lengthSqrd)
        .function("isUnitLength", select_overload<bool() const>(&GeVector2d::isUnitLength))
        .function("isUnitLength", select_overload<bool(const GeTol &) const>(&GeVector2d::isUnitLength))
        .function("isZeroLength", select_overload<bool() const>(&GeVector2d::isZeroLength))
        .function("isZeroLength", select_overload<bool(const GeTol &) const>(&GeVector2d::isZeroLength))
        .function("isParallelTo", select_overload<bool(const GeVector2d &) const>(&GeVector2d::isParallelTo))
        .function("isParallelTo", select_overload<bool(const GeVector2d &, const GeTol &) const>(&GeVector2d::isParallelTo))
        .function("isCodirectionalTo", select_overload<bool(const GeVector2d &) const>(&GeVector2d::isCodirectionalTo))
        .function("isCodirectionalTo", select_overload<bool(const GeVector2d &, const GeTol &) const>(&GeVector2d::isCodirectionalTo))
        .function("isPerpendicularTo", select_overload<bool(const GeVector2d &) const>(&GeVector2d::isPerpendicularTo))
        .function("isPerpendicularTo", select_overload<bool(const GeVector2d &, const GeTol &) const>(&GeVector2d::isPerpendicularTo))
        .function("dotProduct", &GeVector2d::dotProduct)
        .function("isEqualTo", select_overload<bool(const GeVector2d &) const>(&GeVector2d::isEqualTo))
        .function("isEqualTo", select_overload<bool(const GeVector2d &, const GeTol &) const>(&GeVector2d::isEqualTo))
        .function("set", &GeVector2d::set);

    // GePoint2d
    emscripten::class_<GePoint2d, base<EmObject>>("GePoint2d")
        .property("x", &GePoint2d::x)
        .property("y", &GePoint2d::y)
        .constructor()
        .constructor<const GePoint2d &>()
        .constructor<double, double>()
        .class_property("kOrigin", &GePoint2d::kOrigin)
        .function("distanceTo", &GePoint2d::distanceTo)
        .function("setToProduct", &GePoint2d::setToProduct)
        .function("transformBy", &GePoint2d::transformBy)
        .function("translation", &GePoint2d::translation)
        .function("rotateBy", select_overload<GePoint2d &(double)>(&GePoint2d::rotateBy))
        .function("rotateBy", select_overload<GePoint2d &(double, const GePoint2d &)>(&GePoint2d::rotateBy))
        .function("mirror", &GePoint2d::mirror)
        .function("scaleBy", select_overload<GePoint2d &(double)>(&GePoint2d::scaleBy))
        .function("scaleBy", select_overload<GePoint2d &(double, const GePoint2d &)>(&GePoint2d::scaleBy))
        .function("scaleBy", select_overload<GePoint2d &(const GeScale2d &)>(&GePoint2d::scaleBy))
        .function("scaleBy", select_overload<GePoint2d &(const GeScale2d &, const GePoint2d &)>(&GePoint2d::scaleBy))
        .function("setToSum", &GePoint2d::setToSum)
        .function("asVector", &GePoint2d::asVector)
        .function("isEqualTo", select_overload<bool(const GePoint2d &) const>(&GePoint2d::isEqualTo))
        .function("isEqualTo", select_overload<bool(const GePoint2d &, const GeTol &) const>(&GePoint2d::isEqualTo))
        .function("set", &GePoint2d::set);

    // GeEntity2d
    emscripten::class_<GeEntity2d>("GeEntity2d")
        GEENTITY2D_EMSDK_EXPORT_METHODS(GeEntity2d);

    // GeBoundBlock2d
    emscripten::class_<GeBoundBlock2d, base<GeEntity2d>>("GeBoundBlock2d")
        .constructor()
        .constructor<const GePoint2d &, const GePoint2d &>()
        .constructor<const GePoint2d &, const GeVector2d &, const GeVector2d &>()
            GEENTITY2D_EMSDK_EXPORT_METHODS(GeBoundBlock2d)
        .function("getMinMaxPoints", &GeBoundBlock2d::getMinMaxPoints)
        .function("get", &GeBoundBlock2d::get)
        .function("set", select_overload<GeBoundBlock2d &(const GePoint2d &, const GePoint2d &)>(&GeBoundBlock2d::set))
        .function("set", select_overload<GeBoundBlock2d &(const GePoint2d &, const GeVector2d &, const GeVector2d &)>(&GeBoundBlock2d::set))
        .function("extend", &GeBoundBlock2d::extend)
        .function("swell", &GeBoundBlock2d::swell)
        .function("contains", &GeBoundBlock2d::contains)
        .function("isDisjoint", &GeBoundBlock2d::isDisjoint)
        .function("isBox", &GeBoundBlock2d::isBox)
        .function("setToBox", &GeBoundBlock2d::setToBox);

    // GePointEnt2d
    emscripten::class_<GePointEnt2d, base<GeEntity2d>>("GePointEnt2d")
        .function("point2d", &GePointEnt2d::point2d);

    // GePointOnCurve2d
    emscripten::class_<GePointOnCurve2d, base<GePointEnt2d>>("GePointOnCurve2d")
        .constructor()
        .constructor<const GeCurve2d &>()
        .constructor<const GeCurve2d &, double>()
            GEENTITY2D_EMSDK_EXPORT_METHODS(GePointOnCurve2d)
                GEPOINTENT2D_EMSDK_EXPORT_METHODS(GePointOnCurve2d)
        .function("curve", &GePointOnCurve2d::curve, allow_raw_pointers())
        .function("parameter", &GePointOnCurve2d::parameter)
        .function("point", select_overload<GePoint2d() const>(&GePointOnCurve2d::point))
        .function("point", select_overload<GePoint2d(double) const>(&GePointOnCurve2d::point))
        .function("point", select_overload<GePoint2d(const GeCurve2d &, double) const>(&GePointOnCurve2d::point))
        .function("setCurve", &GePointOnCurve2d::setCurve, allow_raw_pointers())
        .function("setParameter", &GePointOnCurve2d::setParameter);

    // GeCurve2d
    emscripten::class_<GeCurve2d, base<GeEntity2d>>("GeCurve2d")
        .function("distanceTo", select_overload<double(const GePoint2d &) const>(&GeCurve2d::distanceTo))
        .function("distanceTo", select_overload<double(const GePoint2d &, const GeTol &) const>(&GeCurve2d::distanceTo))
        .function("distanceTo", select_overload<double(const GeCurve2d &) const>(&GeCurve2d::distanceTo))
        .function("distanceTo", select_overload<double(const GeCurve2d &, const GeTol &) const>(&GeCurve2d::distanceTo));

    // GeLinearEnt2d
    emscripten::class_<GeLinearEnt2d, base<GeCurve2d>>("GeLinearEnt2d")
        .class_function("vertical", select_overload<GePoint2d(const GePoint2d &, const GeLinearEnt2d &)>(&GeLinearEnt2d::vertical))
        .class_function("vertical", select_overload<GePoint2d(const GePoint2d &, const GeLinearEnt2d &, const GeTol &)>(&GeLinearEnt2d::vertical))
        .function("intersectWith", select_overload<bool(const GeLinearEnt2d &, GePoint2d &) const>(&GeLinearEnt2d::intersectWith))
        .function("intersectWith", select_overload<bool(const GeLinearEnt2d &, GePoint2d &, const GeTol &) const>(&GeLinearEnt2d::intersectWith))
        .function("isParallelTo", select_overload<bool(const GeLinearEnt2d &) const>(&GeLinearEnt2d::isParallelTo))
        .function("isParallelTo", select_overload<bool(const GeLinearEnt2d &, const GeTol &) const>(&GeLinearEnt2d::isParallelTo))
        .function("isPerpendicularTo", select_overload<bool(const GeLinearEnt2d &) const>(&GeLinearEnt2d::isPerpendicularTo))
        .function("isPerpendicularTo", select_overload<bool(const GeLinearEnt2d &, const GeTol &) const>(&GeLinearEnt2d::isPerpendicularTo))
        .function("isColinearTo", select_overload<bool(const GeLinearEnt2d &) const>(&GeLinearEnt2d::isColinearTo))
        .function("isColinearTo", select_overload<bool(const GeLinearEnt2d &, const GeTol &) const>(&GeLinearEnt2d::isColinearTo))
        .function("getPerpLine", &GeLinearEnt2d::getPerpLine)
        .function("pointOnLine", &GeLinearEnt2d::pointOnLine)
        .function("direction", &GeLinearEnt2d::direction)
        .function("getLine", &GeLinearEnt2d::getLine);

    // GeRay2d
    emscripten::class_<GeRay2d, base<GeLinearEnt2d>>("GeRay2d")
        .constructor()
        .constructor<const GeRay2d &>()
        .constructor<const GePoint2d &, const GePoint2d &>()
            GEENTITY2D_EMSDK_EXPORT_METHODS(GeRay2d)
                GECURVE2D_EMSDK_EXPORT_METHODS(GeRay2d)
                    GELINEARENT2D_EMSDK_EXPORT_METHODS(GeRay2d)
        .function("set", select_overload<GeRay2d &(const GePoint2d &, const GeVector2d &)>(&GeRay2d::set))
        .function("set", select_overload<GeRay2d &(const GePoint2d &, const GePoint2d &)>(&GeRay2d::set));

    // GeLine2d
    emscripten::class_<GeLine2d, base<GeLinearEnt2d>>("GeLine2d")
        .constructor()
        .constructor<const GeLine2d &>()
        .constructor<const GePoint2d &, const GePoint2d &>()
        .class_property("kXAxis", &GeLine2d::kXAxis)
        .class_property("kYAxis", &GeLine2d::kYAxis)
            GEENTITY2D_EMSDK_EXPORT_METHODS(GeLine2d)
                GECURVE2D_EMSDK_EXPORT_METHODS(GeLine2d)
                    GELINEARENT2D_EMSDK_EXPORT_METHODS(GeLine2d)
        .function("set", select_overload<GeLine2d &(const GePoint2d &, const GeVector2d &)>(&GeLine2d::set))
        .function("set", select_overload<GeLine2d &(const GePoint2d &, const GePoint2d &)>(&GeLine2d::set));

    // GeLineSeg2d
    emscripten::class_<GeLineSeg2d, base<GeLinearEnt2d>>("GeLineSeg2d")
        .constructor()
        .constructor<const GeLineSeg2d &>()
        .constructor<const GePoint2d &, const GePoint2d &>()
            GEENTITY2D_EMSDK_EXPORT_METHODS(GeLineSeg2d)
                GECURVE2D_EMSDK_EXPORT_METHODS(GeLineSeg2d)
                    GELINEARENT2D_EMSDK_EXPORT_METHODS(GeLineSeg2d)
        .function("getBisector", &GeLineSeg2d::getBisector)
        .function("baryComb", &GeLineSeg2d::baryComb)
        .function("startPoint", &GeLineSeg2d::startPoint)
        .function("midPoint", &GeLineSeg2d::midPoint)
        .function("endPoint", &GeLineSeg2d::endPoint)
        .function("set", select_overload<GeLineSeg2d &(const GePoint2d &, const GeVector2d &)>(&GeLineSeg2d::set))
        .function("set", select_overload<GeLineSeg2d &(const GePoint2d &, const GePoint2d &)>(&GeLineSeg2d::set));

    // GeCircArc2d
    emscripten::class_<GeCircArc2d, base<GeCurve2d>>("GeCircArc2d")
        .constructor()
        .constructor<const GeCircArc2d &>()
        .constructor<const GePoint2d &, double>()
        .constructor<const GePoint2d &, double, double, double>()
        .constructor<const GePoint2d &, double, double, double, const GeVector2d &>()
        .constructor<const GePoint2d &, double, double, double, const GeVector2d &, bool>()
        .constructor<const GePoint2d &, const GePoint2d &, const GePoint2d &>()
            GEENTITY2D_EMSDK_EXPORT_METHODS(GeCircArc2d)
                GECURVE2D_EMSDK_EXPORT_METHODS(GeCircArc2d)
        .class_function("pointToCenter", select_overload<bool(const GePoint2d &, const GePoint2d &, const GePoint2d &, GePoint2d &)>(&GeCircArc2d::pointToCenter))
        .class_function("pointToCenter", select_overload<bool(const GePoint2d &, const GePoint2d &, const GePoint2d &, GePoint2d &, double)>(&GeCircArc2d::pointToCenter))
        .class_function("isClockWise", select_overload<bool(const GePoint2d &, const GePoint2d &, const GePoint2d &)>(&GeCircArc2d::isClockWise))
        .function("intersectWith", select_overload<GePoint2dArray(const GeLinearEnt2d &) const>(&GeCircArc2d::intersectWith))
        .function("intersectWith", select_overload<GePoint2dArray(const GeLinearEnt2d &, const GeTol &) const>(&GeCircArc2d::intersectWith))
        .function("intersectWith", select_overload<GePoint2dArray(const GeCircArc2d &) const>(&GeCircArc2d::intersectWith))
        .function("intersectWith", select_overload<GePoint2dArray(const GeCircArc2d &, const GeTol &) const>(&GeCircArc2d::intersectWith))
        .function("tangent", select_overload<bool(const GePoint2d &, GeLine2d &) const>(&GeCircArc2d::tangent))
        .function("tangent", select_overload<bool(const GePoint2d &, GeLine2d &, const GeTol &) const>(&GeCircArc2d::tangent))
        .function("isInside", select_overload<bool(const GePoint2d &) const>(&GeCircArc2d::isInside))
        .function("isInside", select_overload<bool(const GePoint2d &, const GeTol &) const>(&GeCircArc2d::isInside))
        .function("center", &GeCircArc2d::center)
        .function("radius", &GeCircArc2d::radius)
        .function("startAng", &GeCircArc2d::startAng)
        .function("endAng", &GeCircArc2d::endAng)
        .function("refVec", &GeCircArc2d::refVec)
        .function("startPoint", &GeCircArc2d::startPoint)
        .function("endPoint", &GeCircArc2d::endPoint)
        .function("isClockWise", select_overload<bool() const>(&GeCircArc2d::isClockWise))
        .function("setCenter", &GeCircArc2d::setCenter)
        .function("setRadius", &GeCircArc2d::setRadius)
        .function("setAngles", &GeCircArc2d::setAngles)
        .function("set", select_overload<GeCircArc2d &(const GePoint2d &, double)>(&GeCircArc2d::set))
        .function("set", select_overload<GeCircArc2d &(const GePoint2d &, double, double, double)>(&GeCircArc2d::set))
        .function("set", select_overload<GeCircArc2d &(const GePoint2d &, double, double, double, const GeVector2d &)>(&GeCircArc2d::set))
        .function("set", select_overload<GeCircArc2d &(const GePoint2d &, double, double, double, const GeVector2d &, bool)>(&GeCircArc2d::set))
        .function("set", select_overload<GeCircArc2d &(const GePoint2d &, const GePoint2d &, const GePoint2d &)>(&GeCircArc2d::set))
        .function("set", select_overload<GeCircArc2d &(const GePoint2d &, const GePoint2d &, double)>(&GeCircArc2d::set))
        .function("set", select_overload<GeCircArc2d &(const GePoint2d &, const GePoint2d &, double, bool)>(&GeCircArc2d::set));

    // GeQuaternion
    emscripten::class_<GeQuaternion, base<EmObject>>("GeQuaternion")
        .property("x", &GeQuaternion::x)
        .property("y", &GeQuaternion::y)
        .property("z", &GeQuaternion::z)
        .property("w", &GeQuaternion::w)
        .constructor()
        .constructor<const GeMatrix3d &>()
        .constructor<const GeVector3d &, const double>()
        .constructor<const double, const double, const double, const double>()
        .function("isEqual", &GeQuaternion::isEqual)
        .function("setRotation", select_overload<void(const GeVector3d &, const GeVector3d &)>(&GeQuaternion::setRotation))
        .function("setRotation", select_overload<void(const GeVector3d &, const GeVector3d &, const GeVector3d &)>(&GeQuaternion::setRotation))
        .function("setVectorAndAngle", &GeQuaternion::setVectorAndAngle)
        .function("getVectorAndAngle", select_overload<GeVector3d() const>(&GeQuaternion::getVectorAndAngle))
        .function("setMatrix", &GeQuaternion::setMatrix)
        .function("getMatrix", &GeQuaternion::getMatrix)
        .function("set", select_overload<void(const double, const double, const double, const double)>(&GeQuaternion::set))
        .function("set", select_overload<void(const GeQuaternion &)>(&GeQuaternion::set))
        .function("setIdent", &GeQuaternion::setIdent)
        .function("reverse", &GeQuaternion::reverse)
        .function("reversed", &GeQuaternion::reversed)
        .function("invert", &GeQuaternion::invert)
        .function("inverted", &GeQuaternion::inverted)
        .function("squareNorm", &GeQuaternion::squareNorm)
        .function("norm", &GeQuaternion::norm)
        .function("scale", &GeQuaternion::scale)
        .function("scaled", &GeQuaternion::scaled)
        .function("stabilizeLength", &GeQuaternion::stabilizeLength)
        .function("normalize", &GeQuaternion::normalize)
        .function("negated", &GeQuaternion::negated)
        .function("added", &GeQuaternion::added)
        .function("subtracted", &GeQuaternion::subtracted)
        .function("multiplied", &GeQuaternion::multiplied)
        .function("add", &GeQuaternion::add)
        .function("subtract", &GeQuaternion::subtract)
        .function("multiply", select_overload<void(const GeQuaternion &)>(&GeQuaternion::multiply))
        .function("dot", &GeQuaternion::dot)
        .function("getRotationAngle", &GeQuaternion::getRotationAngle)
        .function("multiply", select_overload<GeVector3d(const GeVector3d &) const>(&GeQuaternion::multiply));

    // GeMatrix3d
    emscripten::class_<GeMatrix3d, base<EmObject>>("GeMatrix3d")
        .constructor()
        .constructor<const GeMatrix3d &>()
        .class_property("kIdentity", &GeMatrix3d::kIdentity)
        .function("set", &GeMatrix3d::set)
        .function("get", &GeMatrix3d::get)
        .function("setToIdentity", &GeMatrix3d::setToIdentity)
        .function("preMultBy", &GeMatrix3d::preMultBy)
        .function("postMultBy", &GeMatrix3d::postMultBy)
        .function("setToProduct", &GeMatrix3d::setToProduct)
        .function("invert", &GeMatrix3d::invert)
        .function("inverse", select_overload<GeMatrix3d() const>(&GeMatrix3d::inverse))
        .function("inverse", select_overload<GeMatrix3d(const GeTol &) const>(&GeMatrix3d::inverse))
        .function("inverse", select_overload<bool(GeMatrix3d &, double) const>(&GeMatrix3d::inverse))
        .function("isSingular", select_overload<bool() const>(&GeMatrix3d::isSingular))
        .function("isSingular", select_overload<bool(const GeTol &) const>(&GeMatrix3d::isSingular))
        .function("isEqualTo", select_overload<bool(const GeMatrix3d &) const>(&GeMatrix3d::isEqualTo))
        .function("isEqualTo", select_overload<bool(const GeMatrix3d &, const GeTol &) const>(&GeMatrix3d::isEqualTo))
        .function("det", &GeMatrix3d::det)
        .function("setTranslation", &GeMatrix3d::setTranslation)
        .function("setCoordSystem", &GeMatrix3d::setCoordSystem)
        .function("getCoordSystem", &GeMatrix3d::getCoordSystem)
        .function("getCsOrigin", &GeMatrix3d::getCsOrigin)
        .function("getCsXAxis", &GeMatrix3d::getCsXAxis)
        .function("getCsYAxis", &GeMatrix3d::getCsYAxis)
        .function("getCsZAxis", &GeMatrix3d::getCsZAxis)
        .function("setToTranslation", &GeMatrix3d::setToTranslation)
        .function("setToRotation", select_overload<GeMatrix3d &(double, const GeVector3d &)>(&GeMatrix3d::setToRotation))
        .function("setToRotation", select_overload<GeMatrix3d &(double, const GeVector3d &, const GePoint3d &)>(&GeMatrix3d::setToRotation))
        .function("setToScaling", select_overload<GeMatrix3d &(double)>(&GeMatrix3d::setToScaling))
        .function("setToScaling", select_overload<GeMatrix3d &(double, const GePoint3d &)>(&GeMatrix3d::setToScaling))
        .function("setToScaling", select_overload<GeMatrix3d &(const GeScale3d &)>(&GeMatrix3d::setToScaling))
        .function("setToScaling", select_overload<GeMatrix3d &(const GeScale3d &, const GePoint3d &)>(&GeMatrix3d::setToScaling))
        .function("setToMirroring", select_overload<GeMatrix3d &(const GePlane &)>(&GeMatrix3d::setToMirroring))
        .function("setToMirroring", select_overload<GeMatrix3d &(const GePoint3d &)>(&GeMatrix3d::setToMirroring))
        .function("setToMirroring", select_overload<GeMatrix3d &(const GeLine3d &)>(&GeMatrix3d::setToMirroring))
        .function("setToAlignCoordSys", &GeMatrix3d::setToAlignCoordSys)
        .function("setToWorldToPlane", select_overload<GeMatrix3d &(const GeVector3d &)>(&GeMatrix3d::setToWorldToPlane))
        .function("setToWorldToPlane", select_overload<GeMatrix3d &(const GePlane &)>(&GeMatrix3d::setToWorldToPlane))
        .function("setToPlaneToWorld", select_overload<GeMatrix3d &(const GeVector3d &)>(&GeMatrix3d::setToPlaneToWorld))
        .function("setToPlaneToWorld", select_overload<GeMatrix3d &(const GePlane &)>(&GeMatrix3d::setToPlaneToWorld))
        .function("translation", select_overload<GeVector3d() const>(&GeMatrix3d::translation))
        .class_function("translation", select_overload<GeMatrix3d(const GeVector3d &)>(&GeMatrix3d::translation))
        .class_function("rotation", select_overload<GeMatrix3d(double, const GeVector3d &)>(&GeMatrix3d::rotation))
        .class_function("rotation", select_overload<GeMatrix3d(double, const GeVector3d &, const GePoint3d &)>(&GeMatrix3d::rotation))
        .class_function("scaling", select_overload<GeMatrix3d(double)>(&GeMatrix3d::scaling))
        .class_function("scaling", select_overload<GeMatrix3d(double, const GePoint3d &)>(&GeMatrix3d::scaling))
        .class_function("scaling", select_overload<GeMatrix3d(const GeScale3d &)>(&GeMatrix3d::scaling))
        .class_function("scaling", select_overload<GeMatrix3d(const GeScale3d &, const GePoint3d &)>(&GeMatrix3d::scaling))
        .class_function("mirroring", select_overload<GeMatrix3d(const GePlane &)>(&GeMatrix3d::mirroring))
        .class_function("mirroring", select_overload<GeMatrix3d(const GePoint3d &)>(&GeMatrix3d::mirroring))
        .class_function("mirroring", select_overload<GeMatrix3d(const GeLine3d &)>(&GeMatrix3d::mirroring))
        .class_function("alignCoordSys", &GeMatrix3d::alignCoordSys)
        .class_function("worldToPlane", select_overload<GeMatrix3d(const GeVector3d &)>(&GeMatrix3d::worldToPlane))
        .class_function("worldToPlane", select_overload<GeMatrix3d(const GePlane &)>(&GeMatrix3d::worldToPlane))
        .class_function("planeToWorld", select_overload<GeMatrix3d(const GeVector3d &)>(&GeMatrix3d::planeToWorld))
        .class_function("planeToWorld", select_overload<GeMatrix3d(const GePlane &)>(&GeMatrix3d::planeToWorld))
        .function("scale", &GeMatrix3d::scale)
        .function("norm", &GeMatrix3d::norm);

    // GeScale3d
    emscripten::class_<GeScale3d, base<EmObject>>("GeScale3d")
        .property("sx", &GeScale3d::sx)
        .property("sy", &GeScale3d::sy)
        .property("sz", &GeScale3d::sz)
        .constructor()
        .constructor<const GeScale3d &>()
        .constructor<double, double, double>()
        .class_property("kIdentity", &GeScale3d::kIdentity)
        .function("preMultBy", &GeScale3d::preMultBy)
        .function("postMultBy", &GeScale3d::postMultBy)
        .function("setToProduct", select_overload<GeScale3d &(const GeScale3d &, double)>(&GeScale3d::setToProduct))
        .function("setToProduct", select_overload<GeScale3d &(const GeScale3d &, const GeScale3d &)>(&GeScale3d::setToProduct))
        .function("inverse", &GeScale3d::inverse)
        .function("invert", &GeScale3d::invert)
        .function("isProportional", select_overload<bool() const>(&GeScale3d::isProportional))
        .function("isProportional", select_overload<bool(const GeTol &) const>(&GeScale3d::isProportional))
        .function("isEqualTo", select_overload<bool(const GeScale3d &) const>(&GeScale3d::isEqualTo))
        .function("isEqualTo", select_overload<bool(const GeScale3d &, const GeTol &) const>(&GeScale3d::isEqualTo))
        .function("set", &GeScale3d::set)
        .function("getMatrix", &GeScale3d::getMatrix)
        .function("extractScale", &GeScale3d::extractScale)
        .function("removeScale", &GeScale3d::removeScale);

    // GeVector3d
    emscripten::class_<GeVector3d, base<EmObject>>("GeVector3d")
        .property("x", &GeVector3d::x)
        .property("y", &GeVector3d::y)
        .property("z", &GeVector3d::z)
        .constructor()
        .constructor<const GeVector3d &>()
        .constructor<double, double, double>()
        .constructor<const GePlanarEnt &, const GeVector2d &>()
        .class_property("kIdentity", &GeVector3d::kIdentity)
        .class_property("kXAxis", &GeVector3d::kXAxis)
        .class_property("kYAxis", &GeVector3d::kYAxis)
        .class_property("kZAxis", &GeVector3d::kZAxis)
        .function("setToProduct", select_overload<GeVector3d &(const GeMatrix3d &, const GeVector3d &)>(&GeVector3d::setToProduct))
        .function("setToProduct", select_overload<GeVector3d &(const GeVector3d &, double)>(&GeVector3d::setToProduct))
        .function("transformBy", &GeVector3d::transformBy)
        .function("rotateBy", &GeVector3d::rotateBy)
        .function("mirror", &GeVector3d::mirror)
        .function("convert2d", select_overload<GeVector2d(const GePlanarEnt &) const>(&GeVector3d::convert2d))
        .function("convert2d", select_overload<GeVector2d() const>(&GeVector3d::convert2d))
        .function("asPoint", &GeVector3d::asPoint)
        .function("crossProduct", &GeVector3d::crossProduct)
        .function("setToSum", &GeVector3d::setToSum)
        .function("negate", &GeVector3d::negate)
        .function("perpVector", &GeVector3d::perpVector)
        .function("angle", &GeVector3d::angle)
        .function("angleTo", &GeVector3d::angleTo)
        .function("angleOnPlane", &GeVector3d::angleOnPlane)
        .function("normal", select_overload<GeVector3d() const>(&GeVector3d::normal))
        .function("normal", select_overload<GeVector3d(const GeTol &) const>(&GeVector3d::normal))
        .function("normalize", select_overload<GeVector3d &()>(&GeVector3d::normalize))
        .function("normalize", select_overload<GeVector3d &(const GeTol &)>(&GeVector3d::normalize))
        .function("length", &GeVector3d::length)
        .function("setLength", &GeVector3d::setLength)
        .function("lengthSqrd", &GeVector3d::lengthSqrd)
        .function("isUnitLength", select_overload<bool() const>(&GeVector3d::isUnitLength))
        .function("isUnitLength", select_overload<bool(const GeTol &) const>(&GeVector3d::isUnitLength))
        .function("isZeroLength", select_overload<bool() const>(&GeVector3d::isZeroLength))
        .function("isZeroLength", select_overload<bool(const GeTol &) const>(&GeVector3d::isZeroLength))
        .function("isParallelTo", select_overload<bool(const GeVector3d &) const>(&GeVector3d::isParallelTo))
        .function("isParallelTo", select_overload<bool(const GeVector3d &, const GeTol &) const>(&GeVector3d::isParallelTo))
        .function("isCodirectionalTo", select_overload<bool(const GeVector3d &) const>(&GeVector3d::isCodirectionalTo))
        .function("isCodirectionalTo", select_overload<bool(const GeVector3d &, const GeTol &) const>(&GeVector3d::isCodirectionalTo))
        .function("isPerpendicularTo", select_overload<bool(const GeVector3d &) const>(&GeVector3d::isPerpendicularTo))
        .function("isPerpendicularTo", select_overload<bool(const GeVector3d &, const GeTol &) const>(&GeVector3d::isPerpendicularTo))
        .function("dotProduct", &GeVector3d::dotProduct)
        .function("rotateTo", select_overload<GeMatrix3d(const GeVector3d &) const>(&GeVector3d::rotateTo))
        .function("rotateTo", select_overload<GeMatrix3d(const GeVector3d &, const GeVector3d &) const>(&GeVector3d::rotateTo))
        .function("project", select_overload<GeVector3d(const GeVector3d &, const GeVector3d &) const>(&GeVector3d::project))
        .function("project", select_overload<GeVector3d(const GeVector3d &, const GeVector3d &, const GeTol &) const>(&GeVector3d::project))
        .function("orthoProject", select_overload<GeVector3d(const GeVector3d &) const>(&GeVector3d::orthoProject))
        .function("orthoProject", select_overload<GeVector3d(const GeVector3d &, const GeTol &) const>(&GeVector3d::orthoProject))
        .function("isEqualTo", select_overload<bool(const GeVector3d &) const>(&GeVector3d::isEqualTo))
        .function("isEqualTo", select_overload<bool(const GeVector3d &, const GeTol &) const>(&GeVector3d::isEqualTo))
        .function("set", select_overload<GeVector3d &(double, double, double)>(&GeVector3d::set))
        .function("set", select_overload<GeVector3d &(const GePlanarEnt &, const GeVector2d &)>(&GeVector3d::set));

    // GePoint3d
    emscripten::class_<GePoint3d, base<EmObject>>("GePoint3d")
        .property("x", &GePoint3d::x)
        .property("y", &GePoint3d::y)
        .property("z", &GePoint3d::z)
        .constructor()
        .constructor<const GePoint3d &>()
        .constructor<double, double, double>()
        .class_property("kOrigin", &GePoint3d::kOrigin)
        .function("distanceTo", &GePoint3d::distanceTo)
        .function("setToProduct", &GePoint3d::setToProduct)
        .function("transformBy", select_overload<GePoint3d &(const GeMatrix3d &)>(&GePoint3d::transformBy))
        .function("translation", &GePoint3d::translation)
        .function("rotateBy", select_overload<GePoint3d &(double, const GeVector3d &)>(&GePoint3d::rotateBy))
        .function("rotateBy", select_overload<GePoint3d &(double, const GeVector3d &, const GePoint3d &)>(&GePoint3d::rotateBy))
        .function("mirror", &GePoint3d::mirror)
        .function("scaleBy", select_overload<GePoint3d &(double)>(&GePoint3d::scaleBy))
        .function("scaleBy", select_overload<GePoint3d &(double, const GePoint3d &)>(&GePoint3d::scaleBy))
        .function("scaleBy", select_overload<GePoint3d &(const GeScale3d &)>(&GePoint3d::scaleBy))
        .function("scaleBy", select_overload<GePoint3d &(const GeScale3d &, const GePoint3d &)>(&GePoint3d::scaleBy))
        .function("convert2d", select_overload<GePoint2d(const GePlanarEnt &) const>(&GePoint3d::convert2d))
        .function("convert2d", select_overload<GePoint2d() const>(&GePoint3d::convert2d))
        .function("setToSum", &GePoint3d::setToSum)
        .function("asVector", &GePoint3d::asVector)
        .function("project", &GePoint3d::project)
        .function("orthoProject", &GePoint3d::orthoProject)
        .function("isEqualTo", select_overload<bool(const GePoint3d &) const>(&GePoint3d::isEqualTo))
        .function("isEqualTo", select_overload<bool(const GePoint3d &, const GeTol &) const>(&GePoint3d::isEqualTo))
        .function("set", select_overload<GePoint3d &(double, double, double)>(&GePoint3d::set))
        .function("set", select_overload<GePoint3d &(const GePoint2d &)>(&GePoint3d::set))
        .function("set", select_overload<GePoint3d &(const GePoint2d &, double)>(&GePoint3d::set));

    // GeBoundBlock3d
    emscripten::class_<GeBoundBlock3d, base<GeEntity3d>>("GeBoundBlock3d")
        .constructor()
        .constructor<const GeBoundBlock3d &>()
        .constructor<const GePoint3d &, const GePoint3d &>()
        .constructor<const GePoint3d &, const GeVector3d &, const GeVector3d &, const GeVector3d &>()
            GEENTITY3D_EMSDK_EXPORT_METHODS(GeBoundBlock3d)
        .function("getMinMaxPoints", &GeBoundBlock3d::getMinMaxPoints)
        .function("get", &GeBoundBlock3d::get)
        .function("set", select_overload<GeBoundBlock3d &(const GePoint3d &, const GePoint3d &)>(&GeBoundBlock3d::set))
        .function("set", select_overload<GeBoundBlock3d &(const GePoint3d &, const GeVector3d &, const GeVector3d &, const GeVector3d &)>(&GeBoundBlock3d::set))
        .function("extend", &GeBoundBlock3d::extend)
        .function("swell", &GeBoundBlock3d::swell)
        .function("contains", &GeBoundBlock3d::contains)
        .function("isDisjoint", &GeBoundBlock3d::isDisjoint)
        .function("isBox", &GeBoundBlock3d::isBox)
        .function("setToBox", &GeBoundBlock3d::setToBox);

    // GeEntity3d
    emscripten::class_<GeEntity3d, base<EmObject>>("GeEntity3d")
        GEENTITY3D_EMSDK_EXPORT_METHODS(GeEntity3d);

    // GePointEnt3d
    emscripten::class_<GePointEnt3d, base<GeEntity3d>>("GePointEnt3d")
        .function("point3d", &GePointEnt3d::point3d);

    // GePointOnCurve3d
    emscripten::class_<GePointOnCurve3d, base<GePointEnt3d>>("GePointOnCurve3d")
        .constructor()
        .constructor<const GeCurve3d &>()
        .constructor<const GeCurve3d &, double>()
            GEENTITY3D_EMSDK_EXPORT_METHODS(GePointOnCurve3d)
                GEPOINTENT3D_EMSDK_EXPORT_METHODS(GePointOnCurve3d)
        .function("curve", &GePointOnCurve3d::curve, allow_raw_pointers())
        .function("parameter", &GePointOnCurve3d::parameter)
        .function("point", select_overload<GePoint3d() const>(&GePointOnCurve3d::point))
        .function("point", select_overload<GePoint3d(double) const>(&GePointOnCurve3d::point))
        .function("point", select_overload<GePoint3d(const GeCurve3d &, double) const>(&GePointOnCurve3d::point))
        .function("setCurve", &GePointOnCurve3d::setCurve, allow_raw_pointers())
        .function("setParameter", &GePointOnCurve3d::setParameter);

    // GePointOnSurface
    emscripten::class_<GePointOnSurface, base<GePointEnt3d>>("GePointOnSurface")
        .constructor()
        .constructor<const GePointOnSurface &>()
        .constructor<const GeSurface &, const GePoint2d &>()
            GEENTITY3D_EMSDK_EXPORT_METHODS(GePointOnSurface)
                GEPOINTENT3D_EMSDK_EXPORT_METHODS(GePointOnSurface)
        .function("surface", &GePointOnSurface::surface, allow_raw_pointers())
        .function("parameter", &GePointOnSurface::parameter, allow_raw_pointers())
        .function("point", select_overload<GePoint3d() const>(&GePointOnSurface::point))
        .function("point", select_overload<GePoint3d(const GePoint2d &) const>(&GePointOnSurface::point))
        .function("point", select_overload<GePoint3d(const GeSurface &, const GePoint2d &) const>(&GePointOnSurface::point))
        .function("setSurface", &GePointOnSurface::setSurface, allow_raw_pointers())
        .function("setParameter", &GePointOnSurface::setParameter);

    // GeSurface
    emscripten::class_<GeSurface, base<GeEntity3d>>("GeSurface")
        .function("paramOf", select_overload<GePoint2d(const GePoint3d &) const>(&GeSurface::paramOf))
        .function("paramOf", select_overload<GePoint2d(const GePoint3d &, const GeTol &) const>(&GeSurface::paramOf))
        .function("isOn", select_overload<bool(const GePoint3d &) const>(&GeSurface::isOn))
        .function("isOn", select_overload<bool(const GePoint3d &, const GeTol &) const>(&GeSurface::isOn))
        .function("isOn", select_overload<bool(const GePoint3d &, GePoint2d &) const>(&GeSurface::isOn))
        .function("isOn", select_overload<bool(const GePoint3d &, GePoint2d &, const GeTol &) const>(&GeSurface::isOn))
        .function("closestPointTo", select_overload<GePoint3d(const GePoint3d &) const>(&GeSurface::closestPointTo))
        .function("closestPointTo", select_overload<GePoint3d(const GePoint3d &, const GeTol &) const>(&GeSurface::closestPointTo))
        .function("getClosestPointTo", select_overload<void(const GePoint3d &, GePointOnSurface &) const>(&GeSurface::getClosestPointTo))
        .function("getClosestPointTo", select_overload<void(const GePoint3d &, GePointOnSurface &, const GeTol &) const>(&GeSurface::getClosestPointTo))
        .function("distanceTo", select_overload<double(const GePoint3d &) const>(&GeSurface::distanceTo))
        .function("distanceTo", select_overload<double(const GePoint3d &, const GeTol &) const>(&GeSurface::distanceTo))
        .function("isNormalReversed", &GeSurface::isNormalReversed)
        .function("reverseNormal", &GeSurface::reverseNormal)
        .function("evalPoint", &GeSurface::evalPoint);

    // GePlanarEnt
    emscripten::class_<GePlanarEnt, base<GeSurface>>("GePlanarEnt")
        .function("intersectWith", select_overload<bool(const GeLinearEnt3d &, GePoint3d &) const>(&GePlanarEnt::intersectWith))
        .function("intersectWith", select_overload<bool(const GeLinearEnt3d &, GePoint3d &, const GeTol &) const>(&GePlanarEnt::intersectWith))
        .function("closestPointToLinearEnt", select_overload<GePoint3d(const GeLinearEnt3d &, GePoint3d &) const>(&GePlanarEnt::closestPointToLinearEnt))
        .function("closestPointToLinearEnt", select_overload<GePoint3d(const GeLinearEnt3d &, GePoint3d &, const GeTol &) const>(&GePlanarEnt::closestPointToLinearEnt))
        .function("closestPointToPlanarEnt", select_overload<GePoint3d(const GePlanarEnt &, GePoint3d &) const>(&GePlanarEnt::closestPointToPlanarEnt))
        .function("closestPointToPlanarEnt", select_overload<GePoint3d(const GePlanarEnt &, GePoint3d &, const GeTol &) const>(&GePlanarEnt::closestPointToPlanarEnt))
        .function("isParallelTo", select_overload<bool(const GeLinearEnt3d &) const>(&GePlanarEnt::isParallelTo))
        .function("isParallelTo", select_overload<bool(const GeLinearEnt3d &, const GeTol &) const>(&GePlanarEnt::isParallelTo))
        .function("isParallelTo", select_overload<bool(const GePlanarEnt &) const>(&GePlanarEnt::isParallelTo))
        .function("isParallelTo", select_overload<bool(const GePlanarEnt &, const GeTol &) const>(&GePlanarEnt::isParallelTo))
        .function("isPerpendicularTo", select_overload<bool(const GeLinearEnt3d &) const>(&GePlanarEnt::isPerpendicularTo))
        .function("isPerpendicularTo", select_overload<bool(const GeLinearEnt3d &, const GeTol &) const>(&GePlanarEnt::isPerpendicularTo))
        .function("isPerpendicularTo", select_overload<bool(const GePlanarEnt &) const>(&GePlanarEnt::isPerpendicularTo))
        .function("isPerpendicularTo", select_overload<bool(const GePlanarEnt &, const GeTol &) const>(&GePlanarEnt::isPerpendicularTo))
        .function("isCoplanarTo", select_overload<bool(const GePlanarEnt &) const>(&GePlanarEnt::isCoplanarTo))
        .function("isCoplanarTo", select_overload<bool(const GePlanarEnt &, const GeTol &) const>(&GePlanarEnt::isCoplanarTo))
        .function("get", select_overload<void(GePoint3d &, GeVector3d &, GeVector3d &) const>(&GePlanarEnt::get))
        .function("get", select_overload<void(GePoint3d &, GePoint3d &, GePoint3d &) const>(&GePlanarEnt::get))
        .function("pointOnPlane", &GePlanarEnt::pointOnPlane)
        .function("normal", &GePlanarEnt::normal)
        .function("getCoordSystem", &GePlanarEnt::getCoordSystem);

    // GePlane
    emscripten::class_<GePlane, base<GePlanarEnt>>("GePlane")
        .constructor()
        .constructor<const GePlane &>()
        .constructor<const GePoint3d &, const GeVector3d &>()
        .constructor<const GePoint3d &, const GePoint3d &, const GePoint3d &>()
        .constructor<double, double, double, double>()
            GEENTITY3D_EMSDK_EXPORT_METHODS(GePlane)
                GESURFACE_EMSDK_EXPORT_METHODS(GePlane)
                    GEPLANE_EMSDK_EXPORT_METHODS(GePlane)
        .function("signedDistanceTo", &GePlane::signedDistanceTo)
        .function("intersectWith", select_overload<bool(const GeLinearEnt3d &, GePoint3d &) const>(&GePlane::intersectWith))
        .function("intersectWith", select_overload<bool(const GeLinearEnt3d &, GePoint3d &, const GeTol &) const>(&GePlane::intersectWith))
        .function("intersectWith", select_overload<bool(const GePlane &, GeLine3d &) const>(&GePlane::intersectWith))
        .function("intersectWith", select_overload<bool(const GePlane &, GeLine3d &, const GeTol &) const>(&GePlane::intersectWith))
        .function("intersectWith", select_overload<bool(const GeBoundedPlane &, GeLineSeg3d &) const>(&GePlane::intersectWith))
        .function("intersectWith", select_overload<bool(const GeBoundedPlane &, GeLineSeg3d &, const GeTol &) const>(&GePlane::intersectWith))
        .function("set", select_overload<GePlane &(const GePoint3d &, const GeVector3d &)>(&GePlane::set))
        .function("set", select_overload<GePlane &(const GePoint3d &, const GePoint3d &, const GePoint3d &)>(&GePlane::set))
        .function("set", select_overload<GePlane &(double, double, double, double)>(&GePlane::set))
        .function("set", select_overload<GePlane &(const GePoint3d &, const GeVector3d &, const GeVector3d &)>(&GePlane::set));

    // GeBoundedPlane
    emscripten::class_<GeBoundedPlane, base<GePlanarEnt>>("GeBoundedPlane")
        .constructor()
        .constructor<const GeBoundedPlane &>()
        .constructor<const GePoint3d &, const GePoint3d &, const GePoint3d &>()
            GEENTITY3D_EMSDK_EXPORT_METHODS(GeBoundedPlane)
                GESURFACE_EMSDK_EXPORT_METHODS(GeBoundedPlane)
                    GEPLANE_EMSDK_EXPORT_METHODS(GeBoundedPlane)
        .function("intersectWith", select_overload<bool(const GeLinearEnt3d &, GePoint3d &) const>(&GeBoundedPlane::intersectWith))
        .function("intersectWith", select_overload<bool(const GeLinearEnt3d &, GePoint3d &, const GeTol &) const>(&GeBoundedPlane::intersectWith))
        .function("intersectWith", select_overload<bool(const GePlane &, GeLineSeg3d &) const>(&GeBoundedPlane::intersectWith))
        .function("intersectWith", select_overload<bool(const GePlane &, GeLineSeg3d &, const GeTol &) const>(&GeBoundedPlane::intersectWith))
        .function("intersectWith", select_overload<bool(const GeBoundedPlane &, GeLineSeg3d &) const>(&GeBoundedPlane::intersectWith))
        .function("intersectWith", select_overload<bool(const GeBoundedPlane &, GeLineSeg3d &, const GeTol &) const>(&GeBoundedPlane::intersectWith))
        .function("set", select_overload<GeBoundedPlane &(const GePoint3d &, const GePoint3d &, const GePoint3d &)>(&GeBoundedPlane::set))
        .function("set", select_overload<GeBoundedPlane &(const GePoint3d &, const GeVector3d &, const GeVector3d &)>(&GeBoundedPlane::set));

    // GeCurve3d
    emscripten::class_<GeCurve3d, base<GeEntity3d>>("GeCurve3d")
        .function("distanceTo", select_overload<double(const GePoint3d &) const>(&GeCurve3d::distanceTo))
        .function("distanceTo", select_overload<double(const GePoint3d &, const GeTol &) const>(&GeCurve3d::distanceTo))
        .function("closestPointTo", select_overload<GePoint3d(const GePoint3d &) const>(&GeCurve3d::closestPointTo))
        .function("closestPointTo", select_overload<GePoint3d(const GePoint3d &, const GeTol &) const>(&GeCurve3d::closestPointTo))
        .function("closestPointTo", select_overload<GePoint3d(const GeCurve3d &, GePoint3d &) const>(&GeCurve3d::closestPointTo))
        .function("closestPointTo", select_overload<GePoint3d(const GeCurve3d &, GePoint3d &, const GeTol &tol) const>(&GeCurve3d::closestPointTo))
        .function("getClosestPointTo", select_overload<void(const GePoint3d &, GePointOnCurve3d &) const>(&GeCurve3d::getClosestPointTo))
        .function("getClosestPointTo", select_overload<void(const GePoint3d &, GePointOnCurve3d &, const GeTol &) const>(&GeCurve3d::getClosestPointTo))
        .function("getClosestPointTo", select_overload<void(const GeCurve3d &, GePointOnCurve3d &, GePointOnCurve3d &) const>(&GeCurve3d::getClosestPointTo))
        .function("getClosestPointTo", select_overload<void(const GeCurve3d &, GePointOnCurve3d &, GePointOnCurve3d &, const GeTol &) const>(&GeCurve3d::getClosestPointTo))
        .function("projClosestPointTo", select_overload<GePoint3d(const GePoint3d &, const GeVector3d &) const>(&GeCurve3d::projClosestPointTo))
        .function("projClosestPointTo", select_overload<GePoint3d(const GePoint3d &, const GeVector3d &, const GeTol &) const>(&GeCurve3d::projClosestPointTo))
        .function("projClosestPointTo", select_overload<GePoint3d(const GeCurve3d &, const GeVector3d &, GePoint3d &) const>(&GeCurve3d::projClosestPointTo))
        .function("projClosestPointTo", select_overload<GePoint3d(const GeCurve3d &, const GeVector3d &, GePoint3d &, const GeTol &) const>(&GeCurve3d::projClosestPointTo))
        .function("getProjClosestPointTo", select_overload<void(const GePoint3d &, const GeVector3d &, GePointOnCurve3d &) const>(&GeCurve3d::getProjClosestPointTo))
        .function("getProjClosestPointTo", select_overload<void(const GePoint3d &, const GeVector3d &, GePointOnCurve3d &, const GeTol &) const>(&GeCurve3d::getProjClosestPointTo))
        .function("getProjClosestPointTo", select_overload<void(const GeCurve3d &, const GeVector3d &, GePointOnCurve3d &, GePointOnCurve3d &) const>(&GeCurve3d::getProjClosestPointTo))
        .function("getProjClosestPointTo", select_overload<void(const GeCurve3d &, const GeVector3d &, GePointOnCurve3d &, GePointOnCurve3d &, const GeTol &) const>(&GeCurve3d::getProjClosestPointTo))
        .function("getNormalPoint", select_overload<bool(const GePoint3d &, GePointOnCurve3d &) const>(&GeCurve3d::getNormalPoint))
        .function("getNormalPoint", select_overload<bool(const GePoint3d &, GePointOnCurve3d &, const GeTol &) const>(&GeCurve3d::getNormalPoint))
        .function("boundBlock", select_overload<GeBoundBlock3d() const>(&GeCurve3d::boundBlock))
        .function("boundBlock", select_overload<GeBoundBlock3d(const GeInterval &) const>(&GeCurve3d::boundBlock))
        .function("orthoBoundBlock", select_overload<GeBoundBlock3d() const>(&GeCurve3d::orthoBoundBlock))
        .function("orthoBoundBlock", select_overload<GeBoundBlock3d(const GeInterval &) const>(&GeCurve3d::orthoBoundBlock))
        .function("project", select_overload<GeEntity3d *(const GePlane &, const GeVector3d &) const>(&GeCurve3d::project), allow_raw_pointers())
        .function("project", select_overload<GeEntity3d *(const GePlane &, const GeVector3d &, const GeTol &) const>(&GeCurve3d::project), allow_raw_pointers())
        .function("orthoProject", select_overload<GeEntity3d *(const GePlane &) const>(&GeCurve3d::orthoProject), allow_raw_pointers())
        .function("orthoProject", select_overload<GeEntity3d *(const GePlane &, const GeTol &) const>(&GeCurve3d::orthoProject), allow_raw_pointers())
        .function("isOn", select_overload<bool(const GePoint3d &) const>(&GeCurve3d::isOn))
        .function("isOn", select_overload<bool(const GePoint3d &, const GeTol &) const>(&GeCurve3d::isOn))
        .function("isOn", select_overload<bool(double) const>(&GeCurve3d::isOn))
        .function("isOn", select_overload<bool(double, const GeTol &) const>(&GeCurve3d::isOn))
        .function("paramOf", select_overload<double(const GePoint3d &) const>(&GeCurve3d::paramOf))
        .function("paramOf", select_overload<double(const GePoint3d &, const GeTol &) const>(&GeCurve3d::paramOf))
        .function("isClosed", select_overload<bool() const>(&GeCurve3d::isClosed))
        .function("isClosed", select_overload<bool(const GeTol &) const>(&GeCurve3d::isClosed))
        .function("isPlanar", select_overload<bool(GePlane &) const>(&GeCurve3d::isPlanar))
        .function("isPlanar", select_overload<bool(GePlane &, const GeTol &) const>(&GeCurve3d::isPlanar))
        .function("isLinear", select_overload<bool(GeLine3d &) const>(&GeCurve3d::isLinear))
        .function("isLinear", select_overload<bool(GeLine3d &, const GeTol &) const>(&GeCurve3d::isLinear))
        .function("isCoplanarWith", select_overload<bool(const GeCurve3d &, GePlane &) const>(&GeCurve3d::isCoplanarWith))
        .function("isCoplanarWith", select_overload<bool(const GeCurve3d &, GePlane &, const GeTol &) const>(&GeCurve3d::isCoplanarWith))
        .function("length", select_overload<double() const>(&GeCurve3d::length))
        .function("length", select_overload<double(double, double) const>(&GeCurve3d::length))
        .function("length", select_overload<double(double, double, double) const>(&GeCurve3d::length))
        .function("paramAtLength", select_overload<double(double, double) const>(&GeCurve3d::paramAtLength))
        .function("paramAtLength", select_overload<double(double, double, double) const>(&GeCurve3d::paramAtLength))
        .function("area", select_overload<double() const>(&GeCurve3d::area))
        .function("area", select_overload<double(const GeTol &) const>(&GeCurve3d::area))
        .function("hasStartPoint", &GeCurve3d::hasStartPoint)
        .function("hasEndPoint", &GeCurve3d::hasEndPoint)
        .function("evalPoint", &GeCurve3d::evalPoint)
        .function("getSamplePoints", select_overload<void(double, double, double, GePoint3dArray &, GeDoubleArray &) const>(&GeCurve3d::getSamplePoints))
        .function("getSamplePoints", select_overload<void(double, double, double, GePoint3dArray &, GeDoubleArray &, bool) const>(&GeCurve3d::getSamplePoints))
        .function("getSamplePoints", select_overload<void(int, GePoint3dArray &) const>(&GeCurve3d::getSamplePoints))
        .function("getSamplePoints", select_overload<void(int, GePoint3dArray &, GeDoubleArray &) const>(&GeCurve3d::getSamplePoints));

    // GeLinearEnt3d
    emscripten::class_<GeLinearEnt3d, base<GeCurve3d>>("GeLinearEnt3d")
        .class_function("vertical", select_overload<GePoint3d(const GePoint3d &, const GeLinearEnt3d &)>(&GeLinearEnt3d::vertical))
        .class_function("vertical", select_overload<GePoint3d(const GePoint3d &, const GeLinearEnt3d &, const GeTol &)>(&GeLinearEnt3d::vertical))
        .function("intersectWith", select_overload<bool(const GeLinearEnt3d &, GePoint3d &) const>(&GeLinearEnt3d::intersectWith))
        .function("intersectWith", select_overload<bool(const GeLinearEnt3d &, GePoint3d &, const GeTol &) const>(&GeLinearEnt3d::intersectWith))
        .function("intersectWith", select_overload<bool(const GePlanarEnt &, GePoint3d &) const>(&GeLinearEnt3d::intersectWith))
        .function("intersectWith", select_overload<bool(const GePlanarEnt &, GePoint3d &, const GeTol &) const>(&GeLinearEnt3d::intersectWith))
        .function("projIntersectWith", select_overload<bool(const GeLinearEnt3d &, const GeVector3d &, GePoint3d &, GePoint3d &) const>(&GeLinearEnt3d::projIntersectWith))
        .function("projIntersectWith", select_overload<bool(const GeLinearEnt3d &, const GeVector3d &, GePoint3d &, GePoint3d &, const GeTol &) const>(&GeLinearEnt3d::projIntersectWith))
        .function("isOn", select_overload<bool(const GePoint3d &) const>(&GeLinearEnt3d::isOn))
        .function("isOn", select_overload<bool(const GePoint3d &, const GeTol &) const>(&GeLinearEnt3d::isOn))
        .function("isOn", select_overload<bool(double) const>(&GeLinearEnt3d::isOn))
        .function("isOn", select_overload<bool(double, const GeTol &) const>(&GeLinearEnt3d::isOn))
        .function("isOn", select_overload<bool(const GePlane &) const>(&GeLinearEnt3d::isOn))
        .function("isOn", select_overload<bool(const GePlane &, const GeTol &) const>(&GeLinearEnt3d::isOn))
        .function("isParallelTo", select_overload<bool(const GeLinearEnt3d &) const>(&GeLinearEnt3d::isParallelTo))
        .function("isParallelTo", select_overload<bool(const GeLinearEnt3d &, const GeTol &) const>(&GeLinearEnt3d::isParallelTo))
        .function("isParallelTo", select_overload<bool(const GePlanarEnt &) const>(&GeLinearEnt3d::isParallelTo))
        .function("isParallelTo", select_overload<bool(const GePlanarEnt &, const GeTol &) const>(&GeLinearEnt3d::isParallelTo))
        .function("isPerpendicularTo", select_overload<bool(const GeLinearEnt3d &) const>(&GeLinearEnt3d::isPerpendicularTo))
        .function("isPerpendicularTo", select_overload<bool(const GeLinearEnt3d &, const GeTol &) const>(&GeLinearEnt3d::isPerpendicularTo))
        .function("isPerpendicularTo", select_overload<bool(const GePlanarEnt &) const>(&GeLinearEnt3d::isPerpendicularTo))
        .function("isPerpendicularTo", select_overload<bool(const GePlanarEnt &, const GeTol &) const>(&GeLinearEnt3d::isPerpendicularTo))
        .function("isColinearTo", select_overload<bool(const GeLinearEnt3d &) const>(&GeLinearEnt3d::isColinearTo))
        .function("isColinearTo", select_overload<bool(const GeLinearEnt3d &, const GeTol &) const>(&GeLinearEnt3d::isColinearTo))
        .function("getPerpPlane", &GeLinearEnt3d::getPerpPlane)
        .function("pointOnLine", &GeLinearEnt3d::pointOnLine)
        .function("direction", &GeLinearEnt3d::direction)
        .function("getLine", &GeLinearEnt3d::getLine);

    // GeRay3d
    emscripten::class_<GeRay3d, base<GeLinearEnt3d>>("GeRay3d")
        .constructor()
        .constructor<const GeRay3d &>()
        .constructor<const GePoint3d &, const GePoint3d &>()
            GEENTITY3D_EMSDK_EXPORT_METHODS(GeRay3d)
                GECURVE3D_EMSDK_EXPORT_METHODS(GeRay3d)
                    GELINEARENT3D_EMSDK_EXPORT_METHODS(GeRay3d)
        .function("set", select_overload<GeRay3d &(const GePoint3d &, const GeVector3d &)>(&GeRay3d::set))
        .function("set", select_overload<GeRay3d &(const GePoint3d &, const GePoint3d &)>(&GeRay3d::set));

    // GeLine3d
    emscripten::class_<GeLine3d, base<GeLinearEnt3d>>("GeLine3d")
        .constructor()
        .constructor<const GeLine3d &>()
        .constructor<const GePoint3d &, const GePoint3d &>()
        .class_property("kXAxis", &GeLine3d::kXAxis)
        .class_property("kYAxis", &GeLine3d::kYAxis)
            GEENTITY3D_EMSDK_EXPORT_METHODS(GeLine3d)
                GECURVE3D_EMSDK_EXPORT_METHODS(GeLine3d)
                    GELINEARENT3D_EMSDK_EXPORT_METHODS(GeLine3d)
        .function("set", select_overload<GeLine3d &(const GePoint3d &, const GeVector3d &)>(&GeLine3d::set))
        .function("set", select_overload<GeLine3d &(const GePoint3d &, const GePoint3d &)>(&GeLine3d::set));

    // GeLineSeg3d
    emscripten::class_<GeLineSeg3d, base<GeLinearEnt3d>>("GeLineSeg3d")
        .constructor()
        .constructor<const GeLineSeg3d &>()
        .constructor<const GePoint3d &, const GePoint3d &>()
            GEENTITY3D_EMSDK_EXPORT_METHODS(GeLineSeg3d)
                GECURVE3D_EMSDK_EXPORT_METHODS(GeLineSeg3d)
                    GELINEARENT3D_EMSDK_EXPORT_METHODS(GeLineSeg3d)
        .function("getBisector", &GeLineSeg3d::getBisector)
        .function("baryComb", &GeLineSeg3d::baryComb)
        .function("startPoint", &GeLineSeg3d::startPoint)
        .function("midPoint", &GeLineSeg3d::midPoint)
        .function("endPoint", &GeLineSeg3d::endPoint)
        .function("set", select_overload<GeLineSeg3d &(const GePoint3d &, const GeVector3d &)>(&GeLineSeg3d::set))
        .function("set", select_overload<GeLineSeg3d &(const GePoint3d &, const GePoint3d &)>(&GeLineSeg3d::set));

    // GeCircArc3d
    emscripten::class_<GeCircArc3d, base<GeCurve3d>>("GeCircArc3d")
        .constructor()
        .constructor<const GeCircArc3d &>()
        .constructor<const GePoint3d &, const GeVector3d &, double>()
        .constructor<const GePoint3d &, const GeVector3d &, const GeVector3d &, double, double, double>()
            GEENTITY3D_EMSDK_EXPORT_METHODS(GeCircArc3d)
                GECURVE3D_EMSDK_EXPORT_METHODS(GeCircArc3d)
        .class_function("pointToCenter", select_overload<bool(const GePoint3d &, const GePoint3d &, const GePoint3d &, GePoint3d &)>(&GeCircArc3d::pointToCenter))
        .class_function("pointToCenter", select_overload<bool(const GePoint3d &, const GePoint3d &, const GePoint3d &, GePoint3d &, double)>(&GeCircArc3d::pointToCenter))
        .class_function("isClockWise", select_overload<bool(const GePoint3d &, const GePoint3d &, const GePoint3d &)>(&GeCircArc3d::isClockWise))
        .function("intersectWith", select_overload<GePoint3dArray(const GeLinearEnt3d &) const>(&GeCircArc3d::intersectWith))
        .function("intersectWith", select_overload<GePoint3dArray(const GeLinearEnt3d &, const GeTol &) const>(&GeCircArc3d::intersectWith))
        .function("intersectWith", select_overload<GePoint3dArray(const GeCircArc3d &) const>(&GeCircArc3d::intersectWith))
        .function("intersectWith", select_overload<GePoint3dArray(const GeCircArc3d &, const GeTol &) const>(&GeCircArc3d::intersectWith))
        .function("tangent", select_overload<bool(const GePoint3d &, GeLine3d &) const>(&GeCircArc3d::tangent))
        .function("tangent", select_overload<bool(const GePoint3d &, GeLine3d &, const GeTol &) const>(&GeCircArc3d::tangent))
        .function("isInside", select_overload<bool(const GePoint3d &) const>(&GeCircArc3d::isInside))
        .function("isInside", select_overload<bool(const GePoint3d &, const GeTol &) const>(&GeCircArc3d::isInside))
        .function("center", &GeCircArc3d::center)
        .function("normal", &GeCircArc3d::normal)
        .function("radius", &GeCircArc3d::radius)
        .function("startAng", &GeCircArc3d::startAng)
        .function("endAng", &GeCircArc3d::endAng)
        .function("refVec", &GeCircArc3d::refVec)
        .function("startPoint", &GeCircArc3d::startPoint)
        .function("endPoint", &GeCircArc3d::endPoint)
        .function("setCenter", &GeCircArc3d::setCenter)
        .function("setRadius", &GeCircArc3d::setRadius)
        .function("setAngles", &GeCircArc3d::setAngles)
        .function("set", select_overload<GeCircArc3d &(const GePoint3d &, const GeVector3d &, double)>(&GeCircArc3d::set))
        .function("set", select_overload<GeCircArc3d &(const GePoint3d &, const GeVector3d &, const GeVector3d &, double, double, double)>(&GeCircArc3d::set))
        .function("set", select_overload<GeCircArc3d &(const GePoint3d &, const GePoint3d &, const GePoint3d &)>(&GeCircArc3d::set))
        .function("set", select_overload<GeCircArc3d &(const GePoint3d &, const GePoint3d &, double)>(&GeCircArc3d::set));
}

#endif
