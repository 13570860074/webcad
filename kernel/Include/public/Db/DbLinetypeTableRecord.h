#ifndef DBLINETYPETABLERECORD_H
#define DBLINETYPETABLERECORD_H

#include "Db.h"
#include "DbSymbolTableRecord.h"


class GeVector2d;
class DbLinetypeTableRecord : public  DbSymbolTableRecord
{
public:
    DbLinetypeTableRecord();
    virtual ~DbLinetypeTableRecord();
    DB_DECLARE_MEMBERS(DbLinetypeTableRecord);

    typedef DbLinetypeTable TableType;

    Acad::ErrorStatus comments(ACHAR*& pString) const;  // deprecated
    Acad::ErrorStatus comments(const ACHAR*& pString) const;
    Acad::ErrorStatus comments(AcString& sComments) const;
    Acad::ErrorStatus setComments(const ACHAR* pString);
#if EMSDK
    const emstring emsdk_comments()const;
    Acad::ErrorStatus emsdk_setComments(const emstring& pString);
#endif

    // These methods are deprecated. Please use the comments() methods
    Acad::ErrorStatus asciiDescription(ACHAR*& pString) const
    {
        return comments(pString);
    }
    Acad::ErrorStatus asciiDescription(const ACHAR*& pString) const
    {
        return comments(pString);
    }
    Acad::ErrorStatus setAsciiDescription(const ACHAR* pString)
    {
        return setComments(pString);
    }
#if EMSDK
    const emstring emsdk_asciiDescription()const;
    Acad::ErrorStatus emsdk_setAsciiDescription(const emstring& pString);
#endif

    double                            patternLength() const;
    Acad::ErrorStatus setPatternLength(double patternLength);

    int                               numDashes() const;
    Acad::ErrorStatus setNumDashes(int count);

    double                            dashLengthAt(int index) const;
    Acad::ErrorStatus setDashLengthAt(int index, double value);

    DbObjectId                      shapeStyleAt(int index) const;
    Acad::ErrorStatus setShapeStyleAt(int index, DbObjectId id);

    int                               shapeNumberAt(int index) const;
    Acad::ErrorStatus setShapeNumberAt(int index, int shapeNumber);

    GeVector2d                      shapeOffsetAt(int index) const;
    Acad::ErrorStatus setShapeOffsetAt(int index, const GeVector2d& offset);

    double                            shapeScaleAt(int index) const;
    Acad::ErrorStatus setShapeScaleAt(int index, double scale);

    bool              isScaledToFit() const;
    void              setIsScaledToFit(bool scaledToFit);

    bool                              shapeIsUcsOrientedAt(int index) const;
    Acad::ErrorStatus setShapeIsUcsOrientedAt(int index, bool isUcsOriented);

    bool                              shapeIsUprightAt(int index) const;
    Acad::ErrorStatus setShapeIsUprightAt(int index, bool isUpright);

    double                            shapeRotationAt(int index) const;
    Acad::ErrorStatus setShapeRotationAt(int index, double rotation);

    Acad::ErrorStatus textAt(int index, AcString& sText) const;
    Acad::ErrorStatus textAt(int index, ACHAR*& text) const;    // deprecated
    Acad::ErrorStatus textAt(int index, const ACHAR*& text) const;
    Acad::ErrorStatus setTextAt(int index, const ACHAR* text);
#if EMSDK
    const emstring emsdk_textAt(int index)const;
    Acad::ErrorStatus emsdk_setTextAt(int index, const emstring& text);
#endif
};


#endif