#ifndef DBMTEXT_H
#define DBMTEXT_H


#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbEntity.h"


class GePlane;
class DbMText : public DbEntity
{
public:
    DbMText();
    virtual ~DbMText();
    DB_ENTITY_DECLARE_MEMBERS(DbMText);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;


    //  Insertion point
    //
    GePoint3d     location() const;
    Acad::ErrorStatus   setLocation(const GePoint3d&);

    //  Normal vector
    // 
    GeVector3d    normal() const;
    Acad::ErrorStatus   setNormal(const GeVector3d&);

    // DbEntity override
    Adesk::Boolean      isPlanar() const override { return Adesk::kTrue; }
    Acad::ErrorStatus getPlane(GePlane& plane, Db::Planarity& type) const override;

    //  Direction vector
    //
    GeVector3d    direction() const;
    Acad::ErrorStatus   setDirection(const GeVector3d&);

    //  Rotation angle (radians)
    // 
    double      rotation() const;
    Acad::ErrorStatus   setRotation(double);

    //  Entity width (constraining value)
    // 
    double      width() const;
    Acad::ErrorStatus   setWidth(double);

    //  Descent/Ascent
    // 
    double      ascent() const;
    double      descent() const;

    //  Text Style (sets initial font)
    //
    DbObjectId        textStyle() const;
    Acad::ErrorStatus   setTextStyle(DbObjectId);

    //  Initial text height (caps height)
    //
    double      textHeight() const;
    Acad::ErrorStatus   setTextHeight(double);

    enum AttachmentPoint {
        kTopLeft = 1,
        kTopCenter = 2,
        kTopRight = 3,
        kMiddleLeft = 4,
        kMiddleCenter = 5,
        kMiddleRight = 6,
        kBottomLeft = 7,
        kBottomCenter = 8,
        kBottomRight = 9,
        kBaseLeft = 10,   // Reserved for future use
        kBaseCenter = 11,   // Reserved for future use
        kBaseRight = 12,   // Reserved for future use
        kBaseAlign = 13,   // Reserved for future use
        kBottomAlign = 14,   // Reserved for future use
        kMiddleAlign = 15,   // Reserved for future use
        kTopAlign = 16,   // Reserved for future use
        kBaseFit = 17,   // Reserved for future use
        kBottomFit = 18,   // Reserved for future use
        kMiddleFit = 19,   // Reserved for future use
        kTopFit = 20,   // Reserved for future use
        kBaseMid = 21,   // Reserved for future use
        kBottomMid = 22,   // Reserved for future use
        kMiddleMid = 23,   // Reserved for future use
        kTopMid = 24,   // Reserved for future use
    };
    AttachmentPoint attachment() const;
    Acad::ErrorStatus   setAttachment(AttachmentPoint);
    // This latter method will implicitly setLocation based on the
    // relationship of current and new attachment values in order
    // to keep the extents of the MText object constant.

    //  Text flow direction
    //
    enum FlowDirection {
        kLtoR = 1,
        kRtoL = 2,    //  not currently supported
        kTtoB = 3,
        kBtoT = 4,    //  not currently supported
        kByStyle = 5
    };
    FlowDirection       flowDirection() const;
    Acad::ErrorStatus   setFlowDirection(FlowDirection);

    //  Text contents
    //
    Acad::ErrorStatus contents(AcString& sContents) const;
    ACHAR* contents() const;  // deprecated
    int              setContents(const ACHAR*);
    /// <summary> This method returns an RTF encoded string that is a
    /// translated version of the contents of the DbMText object. MText
    /// codes are translated to RTF codes. Fonts, colors, tabs, and indents
    /// are preserved.
    /// The conversion uses 24 as the default point size. That is, text with
    /// no height override will have a point size of 24 and text with a height
    /// override will have a point size of the height override multiplied by 24.
    /// The caller is responsible for freeing the memory used by the
    /// returned string (preferrably by using acutDelString()).
    /// </summary>
    Acad::ErrorStatus contentsRTF(AcString& sContentsRTF) const;
    ACHAR* contentsRTF() const; // deprecated
    /// <summary> This method sets the contents of the DbMText object from
    /// the RTF-encoded text string pointed to by RTFString. RTF codes are
    /// translated to MText codes. Fonts, colors, tabs, and indents are
    /// preserved. Bullets and numbered lists are mapped to hanging paragraphs
    /// using the implied setting for tabs and indents.
    /// The conversion uses 24 as the default point size. That is, 24 point
    /// text (RTF \fs48) will have no height override and text with any other
    /// point size will have a height override of that point size divided by 24.
    /// </summary>
    /// <param name="RTFString">Input RTF-encoded string.</param>
    /// <returns>Returns 0 if successful.</returns>
    int setContentsRTF(const ACHAR* RTFString);

    Acad::ErrorStatus text(AcString& sText) const;
    ACHAR* text() const;   // deprecated

    //  Return points marking text box
    //
    void        getBoundingPoints(GePoint3dArray&) const;


    //  These strings can be used instead of sprinkling your code 
    //  with string constants.
    static const ACHAR* const nonBreakSpace();   //  \~
    static const ACHAR* const overlineOn();  //  \O
    static const ACHAR* const overlineOff(); //  \o
    static const ACHAR* const underlineOn(); //  \L
    static const ACHAR* const underlineOff();    //  \l
    static const ACHAR* const colorChange(); //  \C
    static const ACHAR* const fontChange();  //  \F
    static const ACHAR* const heightChange();    //  \H
    static const ACHAR* const widthChange(); //  \W
    static const ACHAR* const obliqueChange();   //  \Q
    static const ACHAR* const trackChange(); //  \T
    static const ACHAR* const lineBreak();   //  \p
    static const ACHAR* const paragraphBreak();  //  \P
    static const ACHAR* const stackStart();  //  \S
    static const ACHAR* const alignChange(); //  \A
    static const ACHAR* const blockBegin();  //  {
    static const ACHAR* const blockEnd();    //  }
    static const ACHAR* const strikethroughOn(); //  \K
    static const ACHAR* const strikethroughOff();    //  \k

    Acad::ErrorStatus   setLineSpacingStyle(Db::LineSpacingStyle eStyle);
    Db::LineSpacingStyle lineSpacingStyle() const;

    // 1.0 = single spaced (default), 2.0 = double-spaced, etc.
    Acad::ErrorStatus   setLineSpacingFactor(double dFactor);
    double              lineSpacingFactor() const;

    /// <summary> Get the flag controlling mtext border's visibility.
    /// <returns> bool </returns>
    /// </summary>
    bool      showBorders() const;

    /// <summary> Set the flag controlling mtext border's visibility.
    /// <param name = "enable"> true means borders will be visible
    ///                         false means borders will be invisible </param>
    /// <returns> Acad::ErrorStatus </returns>
    /// </summary>
    Acad::ErrorStatus setShowBorders(bool enable);

    bool backgroundFillOn() const;
    Acad::ErrorStatus setBackgroundFill(bool enable);

    Acad::ErrorStatus getBackgroundFillColor(CmColor& color) const;
    Acad::ErrorStatus setBackgroundFillColor(const CmColor& color);

    Acad::ErrorStatus getBackgroundScaleFactor(double& scale) const;
    Acad::ErrorStatus setBackgroundScaleFactor(const double scale);

    Acad::ErrorStatus getBackgroundTransparency(CmTransparency& transp) const;
    Acad::ErrorStatus setBackgroundTransparency(const CmTransparency& transp);

    bool useBackgroundColorOn() const;
    Acad::ErrorStatus setUseBackgroundColor(bool enable);

    enum ColumnType {
        kNoColumns,
        kStaticColumns,
        kDynamicColumns,
    };

    ColumnType columnType() const;
    bool columnAutoHeight() const;
    int columnCount() const;
    double columnWidth() const;
    double columnGutterWidth() const;
    bool columnFlowReversed() const;
    double columnHeight(int) const;
    Acad::ErrorStatus getColumnType(ColumnType&) const;
    Acad::ErrorStatus setColumnType(ColumnType);
    Acad::ErrorStatus getColumnAutoHeight(bool&) const;
    Acad::ErrorStatus setColumnAutoHeight(bool);
    Acad::ErrorStatus getColumnCount(int&) const;
    Acad::ErrorStatus setColumnCount(int);
    Acad::ErrorStatus getColumnWidth(double&) const;
    Acad::ErrorStatus setColumnWidth(double);
    Acad::ErrorStatus getColumnGutterWidth(double&) const;
    Acad::ErrorStatus setColumnGutterWidth(double);
    Acad::ErrorStatus getColumnFlowReversed(bool&) const;
    Acad::ErrorStatus setColumnFlowReversed(bool);
    Acad::ErrorStatus getColumnHeight(int, double&) const;
    Acad::ErrorStatus setColumnHeight(int, double);

    // FOR INTERNAL USE ONLY
    //  Entity height (constraining value)
    // 
    double      height() const;
    Acad::ErrorStatus   setHeight(double);

};



#endif
