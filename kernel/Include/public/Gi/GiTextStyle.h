#ifndef GITEXTSTYLE_H
#define GITEXTSTYLE_H


#include "GiParameter.h"
#include "PAL/api/charset.h"


class FontUtils
{
public:
    enum FontPitch
    {
        kDefault,
        kFixed,
        kVariable
    };
    enum FontFamily
    {
        kDoNotCare = 0,
        kRoman = 16,
        kSwiss = 32,
        kModern = 48,
        kScript = 64,
        kDecorative = 80
    };
};


class GePoint2d;
class DbDatabase;
class GiWorldDraw;
class GiTextStyle : public GiParameter
    // 
    // This class contains methods to create and modify a text
    // style object
    //
{
public:
    ~GiTextStyle();
    GiTextStyle(DbDatabase* pDb = NULL);

    GiTextStyle(
        const ACHAR* fontName,
        const ACHAR* bigFontName,
        const double textSize,
        const double xScale,
        const double obliqueAngle,
        const double trPercent,

        const Adesk::Boolean isBackward,
        const Adesk::Boolean isUpsideDown,
        const Adesk::Boolean isVertical,

        const Adesk::Boolean isOverlined,
        const Adesk::Boolean isUnderlined,
        const Adesk::Boolean isStrikethrough = false,

        const ACHAR* styleName = NULL);
    RX_DECLARE_MEMBERS(GiTextStyle);

    // Unicode: change from char to int, since in the implementation, the returned
    // result is actually bitmask indicating some status result.
    virtual int loadStyleRec(DbDatabase* pDb = NULL) const;
    virtual void setTextSize(const double size);
    virtual void setXScale(const double xScale);
    virtual void setObliquingAngle(const double obliquingAngle);

    virtual void setTrackingPercent(const double trPercent);

    virtual void setBackward(const Adesk::Boolean isBackward);
    virtual void setUpsideDown(const Adesk::Boolean isUpsideDown);
    virtual void setVertical(const Adesk::Boolean isVertical);

    virtual void setUnderlined(const Adesk::Boolean isUnderlined);
    virtual void setOverlined(const Adesk::Boolean isOverlined);
    virtual void setStrikethrough(const Adesk::Boolean isStrikethrough);

    virtual void setFileName(const ACHAR* fontName);
    virtual void setBigFontFileName(const ACHAR* bigFontFileName);

    virtual double textSize() const;
    virtual double xScale() const;
    virtual double obliquingAngle() const;

    virtual double trackingPercent() const;

    virtual Adesk::Boolean isBackward() const;
    virtual Adesk::Boolean isUpsideDown() const;
    virtual Adesk::Boolean isVertical() const;

    virtual Adesk::Boolean isUnderlined() const;
    virtual Adesk::Boolean isOverlined() const;
    virtual Adesk::Boolean isStrikethrough() const;

    virtual const ACHAR* fileName() const;
    virtual const ACHAR* bigFontFileName() const;

    virtual GePoint2d extents(const ACHAR* pStr,
        const Adesk::Boolean penups,
        const int len,
        const Adesk::Boolean raw,
        GiWorldDraw* ctxt = NULL) const;

    virtual const ACHAR* styleName() const;
    virtual Acad::ErrorStatus setStyleName(const ACHAR*);

    virtual Acad::ErrorStatus setFont(const ACHAR* pTypeface,
        Adesk::Boolean bold,
        Adesk::Boolean italic,
        Charset charset,
        FontUtils::FontPitch pitch,
        FontUtils::FontFamily family);

    virtual Acad::ErrorStatus font(AcString& sTypeface,
        bool& bold, bool& italic, Charset& charset,
        FontUtils::FontPitch& pitch,
        FontUtils::FontFamily& family) const;

    // deprecated. To be removed. Calls the above overload
    virtual Acad::ErrorStatus font(ACHAR*& pTypeface,
        bool& bold, bool& italic, Charset& charset,
        FontUtils::FontPitch& pitch,
        FontUtils::FontFamily& family) const final;

    virtual Acad::ErrorStatus  extentsBox(const ACHAR* pStr,
        const Adesk::Boolean penups,
        const int len,
        const Adesk::Boolean raw,
        GePoint2d& extMin,
        GePoint2d& extMax,
        GiWorldDraw* ctxt = NULL) const;

    virtual void setTrackKerning(double trackPercent) const;

    virtual bool preLoaded() const;
    virtual void setPreLoaded(bool);
};



#endif