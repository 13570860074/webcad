#ifndef DBABSTRACTVIEWTABLERECORD_H
#define DBABSTRACTVIEWTABLERECORD_H


#include "Db.h"
#include "CmColor.h"
#include "GePoint2d.h"
#include "GePoint3d.h"
#include "DbSymbolTableRecord.h"


class DbAbstractViewTableRecord : public  DbSymbolTableRecord
{
public:
    DbAbstractViewTableRecord();
    virtual ~DbAbstractViewTableRecord();
    DB_DECLARE_MEMBERS(DbAbstractViewTableRecord);

public:
    GePoint2d centerPoint() const;
    void setCenterPoint(const GePoint2d&);

    double height() const;
    void setHeight(double height);

    double width() const;
    void setWidth(double width);

    GePoint3d target() const;
    void setTarget(const GePoint3d& target);

    GeVector3d viewDirection() const;
    void setViewDirection(const GeVector3d& viewDirection);

    double viewTwist() const;
    void setViewTwist(double angle);

    double lensLength() const;
    void setLensLength(double length);

    double frontClipDistance() const;
    void setFrontClipDistance(double distance);

    double backClipDistance() const;
    void setBackClipDistance(double distance);

    bool perspectiveEnabled() const;
    void setPerspectiveEnabled(bool enabled);

    bool frontClipEnabled() const;
    void setFrontClipEnabled(bool enabled);

    bool backClipEnabled() const;
    void setBackClipEnabled(bool enabled);

    bool frontClipAtEye() const;
    void setFrontClipAtEye(bool atEye);

    // Background
    const DbObjectId& background() const;
    DbObjectId& background();
    Acad::ErrorStatus setBackground(const DbObjectId& backgroundId);

    // VisualStyle
    DbObjectId visualStyle() const;
    Acad::ErrorStatus setVisualStyle(const DbObjectId& visualStyleId);

    // Brightness controls the relative intensity of lights.
    double brightness() const;
    Acad::ErrorStatus setBrightness(double);
    // Contrast controls intensity of ambient light, relative to other lights.
    double contrast() const;
    Acad::ErrorStatus setContrast(double);

    CmColor ambientLightColor() const;
    Acad::ErrorStatus setAmbientLightColor(const CmColor& clr);

    // A single sun (distant light) can be associated with each viewport.
    DbObjectId sunId() const;
    Acad::ErrorStatus setSun(DbObjectId& retId, DbObject* pSun);
    Acad::ErrorStatus setSun(DbObjectId& retId, DbObject* pSun, bool eraseOldSun);

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
};


#endif

