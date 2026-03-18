#ifndef DBVIEWPORT_H
#define DBVIEWPORT_H

#include "Db.h"
#include "DbEntity.h"
#include "DbObjectIdArray.h"


class DbViewport : public  DbEntity
{
public:
    DbViewport();
    virtual ~DbViewport();
    DB_ENTITY_DECLARE_MEMBERS(DbViewport);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;


    double height() const;
    Acad::ErrorStatus setHeight(double);

    double width() const;
    Acad::ErrorStatus setWidth(double);

    GePoint3d centerPoint() const;
    Acad::ErrorStatus setCenterPoint(const GePoint3d&);

   Int16 number() const;

    bool isOn() const;
    Acad::ErrorStatus setOn();
    Acad::ErrorStatus setOff();
    Acad::ErrorStatus setIsOn(bool bOn);

    GePoint3d viewTarget() const;
    Acad::ErrorStatus setViewTarget(const GePoint3d&);

    GeVector3d viewDirection() const;
    Acad::ErrorStatus setViewDirection(const GeVector3d&);

    // Model Space height, and center in Display coordinates
    //
    double viewHeight() const;
    Acad::ErrorStatus setViewHeight(double ht);

    GePoint2d viewCenter() const;
    Acad::ErrorStatus setViewCenter(const GePoint2d& pt);

    double twistAngle() const;
    Acad::ErrorStatus setTwistAngle(double);

    double lensLength() const;
    Acad::ErrorStatus setLensLength(double);

    bool isFrontClipOn() const;
    Acad::ErrorStatus setFrontClipOn();
    Acad::ErrorStatus setFrontClipOff();
    Acad::ErrorStatus setFrontClipOn(bool bOn);

    bool isBackClipOn() const;
    Acad::ErrorStatus setBackClipOn();
    Acad::ErrorStatus setBackClipOff();
    Acad::ErrorStatus setBackClipOn(bool bOn);

    bool isFrontClipAtEyeOn() const;
    Acad::ErrorStatus setFrontClipAtEyeOn();
    Acad::ErrorStatus setFrontClipAtEyeOff();
    Acad::ErrorStatus setFrontClipAtEyeOn(bool bOn);

    // FrontZ 
    double frontClipDistance() const;
    Acad::ErrorStatus setFrontClipDistance(double newVal);

    // BackZ 
    double backClipDistance() const;
    Acad::ErrorStatus setBackClipDistance(double newVal);

    bool isPerspectiveOn() const;
    Acad::ErrorStatus setPerspectiveOn();
    Acad::ErrorStatus setPerspectiveOff();
    Acad::ErrorStatus setPerspectiveOn(bool bOn);

    bool isUcsFollowModeOn() const;
    Acad::ErrorStatus setUcsFollowModeOn();
    Acad::ErrorStatus setUcsFollowModeOff();
    Acad::ErrorStatus setUcsFollowModeOn(bool bOn);

    bool isUcsIconVisible() const;
    Acad::ErrorStatus setUcsIconVisible();
    Acad::ErrorStatus setUcsIconInvisible();
    Acad::ErrorStatus setUcsIconVisible(bool);

    bool isUcsIconAtOrigin() const;
    Acad::ErrorStatus setUcsIconAtOrigin();
    Acad::ErrorStatus setUcsIconAtCorner();
    Acad::ErrorStatus setUcsIconAtOrigin(bool);

    bool isFastZoomOn() const { return true; }
    Acad::ErrorStatus setFastZoomOn() { return Acad::eOk; }
    Acad::ErrorStatus setFastZoomOff() { return Acad::eOk; }
    Acad::ErrorStatus setFastZoomOn(bool);

    UInt16 circleSides() const;
    Acad::ErrorStatus setCircleSides(UInt16);

    bool isSnapOn() const;
    Acad::ErrorStatus setSnapOn();
    Acad::ErrorStatus setSnapOff();
    Acad::ErrorStatus setSnapOn(bool);

    bool isSnapIsometric() const;
    Acad::ErrorStatus setSnapIsometric();
    Acad::ErrorStatus setSnapStandard();
    Acad::ErrorStatus setSnapIsometric(bool);

    double snapAngle() const;
    Acad::ErrorStatus setSnapAngle(double);

    GePoint2d snapBasePoint() const;
    Acad::ErrorStatus setSnapBasePoint(const GePoint2d&);

    GeVector2d snapIncrement() const;
    Acad::ErrorStatus setSnapIncrement(const GeVector2d&);

    UInt16 snapIsoPair() const;
    Acad::ErrorStatus setSnapIsoPair(UInt16);

    bool isGridOn() const;
    Acad::ErrorStatus setGridOn();
    Acad::ErrorStatus setGridOff();
    Acad::ErrorStatus setGridOn(bool);

    bool isGridBoundToLimits() const;
    Acad::ErrorStatus setGridBoundToLimits(bool bNewVal);

    bool isGridAdaptive() const;
    Acad::ErrorStatus setGridAdaptive(bool bNewVal);

    bool isGridSubdivisionRestricted() const;
    Acad::ErrorStatus setGridSubdivisionRestricted(bool bNewVal);

    bool isGridFollow() const;
    Acad::ErrorStatus setGridFollow(bool bNewVal);

    UInt16 gridMajor() const;
    Acad::ErrorStatus setGridMajor(UInt16);

    GeVector2d gridIncrement() const;
    Acad::ErrorStatus setGridIncrement(const GeVector2d&);

    Acad::ErrorStatus freezeLayersInViewport(const DbObjectIdArray&);
    Acad::ErrorStatus thawLayersInViewport(const DbObjectIdArray&);
    Acad::ErrorStatus thawAllLayersInViewport();
    bool isLayerFrozenInViewport(const DbObjectId& layerId) const;
    Acad::ErrorStatus getFrozenLayerList(DbObjectIdArray& ids) const;

    Acad::ErrorStatus updateDisplay() const;

    bool isLocked() const;
    Acad::ErrorStatus setLocked();
    Acad::ErrorStatus setUnlocked();
    Acad::ErrorStatus setLocked(bool);

    void close();

    // UCS query methods.
    //
    Acad::ErrorStatus getUcs(GePoint3d& origin, GeVector3d& xAxis, GeVector3d& yAxis) const;
    bool isUcsOrthographic(Db::OrthographicView& view) const;
    DbObjectId ucsName() const;
    double elevation() const;

    // UCS set methods.
    //
    Acad::ErrorStatus setUcs(const GePoint3d& origin, const GeVector3d& xAxis, const GeVector3d& yAxis);
    Acad::ErrorStatus setUcs(Db::OrthographicView view);
    Acad::ErrorStatus setUcs(const DbObjectId& ucsId);
    Acad::ErrorStatus setUcsToWorld();
    Acad::ErrorStatus setElevation(double elev);

    // Orthographic view methods.
    //
    bool isViewOrthographic(Db::OrthographicView& view) const;
    Acad::ErrorStatus setViewDirection(Db::OrthographicView view);

    // Methods to get/set UCSVP for viewport.
    //
    bool isUcsSavedWithViewport() const;
    void setUcsPerViewport(bool ucsvp);

    // ShadePlot enum and get/set methods
    enum ShadePlotType {
        kAsDisplayed = 0,
        kWireframe = 1,
        kHidden = 2,
        kRendered = 3,
        kVisualStyle = 4,
        kRenderPreset = 5,
    };
};


#endif