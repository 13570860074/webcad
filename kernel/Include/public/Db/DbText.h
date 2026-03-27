#ifndef DBTEXT_H
#define DBTEXT_H


#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbEntity.h"


class DbText : public DbEntity
{
public:
    DbText();
    DbText(
        const GePoint3d& position,
        const ACHAR* text,
        DbObjectId        style = DbObjectId::kNull,
        double              height = 0,
        double              rotation = 0);
    virtual ~DbText();
    DB_ENTITY_DECLARE_MEMBERS(DbText);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;


    GePoint3d         position() const;
    Acad::ErrorStatus   setPosition(const GePoint3d&);

    GePoint3d         alignmentPoint() const;
    Acad::ErrorStatus   setAlignmentPoint(const GePoint3d&);
    Adesk::Boolean      isDefaultAlignment() const;

    GeVector3d        normal() const;
    Acad::ErrorStatus   setNormal(const GeVector3d&);

    Adesk::Boolean      isPlanar() const override { return Adesk::kTrue; }
    Acad::ErrorStatus   getPlane(GePlane&, Db::Planarity&) const override;

    double              thickness() const;
    Acad::ErrorStatus   setThickness(double);

    double              oblique() const;
    Acad::ErrorStatus   setOblique(double);

    double              rotation() const;
    Acad::ErrorStatus   setRotation(double);

    double              height() const;
    Acad::ErrorStatus   setHeight(double);

    double              widthFactor() const;
    Acad::ErrorStatus   setWidthFactor(double);

    
    Acad::ErrorStatus   textString(AcString& sText) const;
    ACHAR* textString() const;     // deprecated
    const ACHAR* textStringConst() const;
    Acad::ErrorStatus   setTextString(const ACHAR*);

    DbObjectId        textStyle() const;
    Acad::ErrorStatus   setTextStyle(DbObjectId);

    Adesk::Boolean      isMirroredInX() const;
    Acad::ErrorStatus   mirrorInX(Adesk::Boolean);

    Adesk::Boolean      isMirroredInY() const;
    Acad::ErrorStatus   mirrorInY(Adesk::Boolean);

    Db::TextHorzMode  horizontalMode() const;
    Acad::ErrorStatus   setHorizontalMode(Db::TextHorzMode);

    Db::TextVertMode  verticalMode() const;
    Acad::ErrorStatus   setVerticalMode(Db::TextVertMode);

    virtual Acad::ErrorStatus   adjustAlignment(
        const DbDatabase* pDb = nullptr);


    enum AcTextAlignment
    {
        kTextAlignmentLeft = 0,
        kTextAlignmentCenter = (kTextAlignmentLeft + 1),
        kTextAlignmentRight = (kTextAlignmentCenter + 1),
        kTextAlignmentAligned = (kTextAlignmentRight + 1),
        kTextAlignmentMiddle = (kTextAlignmentAligned + 1),
        kTextAlignmentFit = (kTextAlignmentMiddle + 1),
        kTextAlignmentTopLeft = (kTextAlignmentFit + 1),
        kTextAlignmentTopCenter = (kTextAlignmentTopLeft + 1),
        kTextAlignmentTopRight = (kTextAlignmentTopCenter + 1),
        kTextAlignmentMiddleLeft = (kTextAlignmentTopRight + 1),
        kTextAlignmentMiddleCenter = (kTextAlignmentMiddleLeft + 1),
        kTextAlignmentMiddleRight = (kTextAlignmentMiddleCenter + 1),
        kTextAlignmentBottomLeft = (kTextAlignmentMiddleRight + 1),
        kTextAlignmentBottomCenter = (kTextAlignmentBottomLeft + 1),
        kTextAlignmentBottomRight = (kTextAlignmentBottomCenter + 1)
    };

     AcTextAlignment              justification() const;
     Acad::ErrorStatus            setJustification(AcTextAlignment);
};



#endif
