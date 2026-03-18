#ifndef DBDIMENSION_H
#define DBDIMENSION_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbMText.h"


class DbDimension : public DbEntity
{
protected:
    DbDimension();

public:
    virtual ~DbDimension();
    DB_ENTITY_DECLARE_MEMBERS(DbDimension);

         /// <summary> Get dimension text defined width and defined height from MText in dimension block
    /// <param name = "width"> text defined width to return </param>
    /// <param name = "height"> text defined height to return </param>
    /// <returns> void </returns>
    /// </summary>
    void      textDefinedSize(double& width, double& height) const;
#if EMSDK
    double textDefinedWidth()const;
    double textDefinedHeight()const;
#endif

    /// <summary> Set text defined width and defined height to MText in dimension block
    /// <param name = "width"> text defined width </param>
    /// <param name = "height"> text defined height </param>
    /// <returns> void </returns>
    /// </summary>
    void      setTextDefinedSize(double width, double height);

    /// <summary> Reset dimension text defined width and defined height to MText in dimension block.
    /// <param name = "width"> text defined width </param>
    /// <param name = "height"> text defined height </param>
    /// <returns> void </returns>
    /// </summary>
    void      resetTextDefinedSize();

    GePoint3d         textPosition() const;
    Acad::ErrorStatus   setTextPosition(const GePoint3d&);

    Adesk::Boolean      isUsingDefaultTextPosition() const;
    Acad::ErrorStatus   useSetTextPosition();
    Acad::ErrorStatus   useDefaultTextPosition();
    Acad::ErrorStatus   setUsingDefaultTextPosition(bool);

    GeVector3d        normal() const;
    Acad::ErrorStatus   setNormal(const GeVector3d&);

    Adesk::Boolean    isPlanar() const  override { return true; }
    Acad::ErrorStatus getPlane(GePlane&, Db::Planarity&) const override;
#if EMSDK
    GePlane emsdk_getPlane()const;
#endif

    double              elevation() const;
    Acad::ErrorStatus   setElevation(double);

    /// <summary>
    /// This function retrieves the text associated with this dimension
    /// </summary>
    /// <param name="sDimText">Receives this dimension's text string
    /// </param>
    /// <returns>
    /// Returns Acad::eOk if successful, else an error code
    /// </returns>
    /// <remarks>
    /// The overload which returns ACHAR * is deprecated and may be removed in a future release
    /// </remarks>
    Acad::ErrorStatus   dimensionText(AcString& sDimText) const;
    ACHAR* dimensionText() const;  // deprecated
    Acad::ErrorStatus   setDimensionText(const ACHAR*);
#if EMSDK
    std::string emsdk_dimensionText()const;
    Acad::ErrorStatus emsdk_setDimensionText(const std::string text);
#endif

    double              textRotation() const;
    Acad::ErrorStatus   setTextRotation(double);

    DbObjectId        dimensionStyle() const;
    Acad::ErrorStatus   setDimensionStyle(DbObjectId);

    DbMText::AttachmentPoint textAttachment() const;
    Acad::ErrorStatus   setTextAttachment(DbMText::AttachmentPoint eAtt);

    Db::LineSpacingStyle textLineSpacingStyle() const;
    Acad::ErrorStatus   setTextLineSpacingStyle(Db::LineSpacingStyle eStyle);

    double              textLineSpacingFactor() const;
    Acad::ErrorStatus   setTextLineSpacingFactor(double dFactor);

    Acad::ErrorStatus   getDimstyleData(DbDimStyleTableRecord*& pRecord) const;
    Acad::ErrorStatus   setDimstyleData(DbDimStyleTableRecord* pNewData);
    Acad::ErrorStatus   setDimstyleData(DbObjectId newDataId);

    // Note: this erased() method doesn't actually get called!  The
    // dbObj arg is not const, so it is not really overriding the
    // reactor's base class method.
    //
    virtual void        erased(DbObject* dbObj, Adesk::Boolean bErasing);
    void                objectClosed(const DbObjectId dbObj) override;
    void                dragStatus(const Db::DragStat status) override;
    virtual void close();

    double              horizontalRotation() const;
    Acad::ErrorStatus   setHorizontalRotation(double newVal);

    // Functions for accessing the Dimension's defining block
    //
    DbObjectId        dimBlockId() const;
    Acad::ErrorStatus   setDimBlockId(const DbObjectId&);
    GePoint3d         dimBlockPosition() const;
    Acad::ErrorStatus   setDimBlockPosition(const GePoint3d&);
    Acad::ErrorStatus   recomputeDimBlock(bool forceUpdate = true);
    Acad::ErrorStatus   generateLayout();
    Acad::ErrorStatus   measurement(double& val);
    /// <summary>
    /// This function retrieves the text associated with this dimension
    /// </summary>
    /// <param name="sMTextContent">Receives the text string for this measurement
    /// <param name="measurement">Input measurement to be used by this dimension
    /// <param name="dimensionText">Input replacement text or template. Can be null
    /// </param>
    /// <returns>
    /// Returns Acad::eOk if successful, else an error code
    /// </returns>
    /// <remarks>
    /// The overload which takes ACHAR * and size_t args is deprecated and may
    /// be removed in a future release
    /// </remarks>
    Acad::ErrorStatus   formatMeasurement(AcString& sMTextContent,
            double measurement,
            const ACHAR* dimensionText = nullptr);
    Acad::ErrorStatus   formatMeasurement(ACHAR* MTextContentBuffer,
        size_t contentBufferLen,
        double measurement,
        ACHAR* dimensionText); // deprecated

    // Functions for Dynamic Dimensions control
    //
    bool                isDynamicDimension() const;
    Acad::ErrorStatus   setDynamicDimension(bool newVal);

    //new getOsnapPoints API
    Acad::ErrorStatus   getOsnapPointsAtSubentPath(Db::OsnapMode osnapMode,
        const DbFullSubentPath& subentId,
        const GePoint3d& pickPoint,
        const GePoint3d& lastPoint,
        const GeMatrix3d& viewXform,
        GePoint3dArray& snapPoints,
        DbIntArray& geomIds) const;


    DbObjectId dimLineLinetype() const;
    Acad::ErrorStatus setDimLineLinetype(const DbObjectId linetype);
    DbObjectId dimExt1Linetype() const;
    Acad::ErrorStatus setDimExt1Linetype(const DbObjectId linetype);
    DbObjectId dimExt2Linetype() const;
    Acad::ErrorStatus setDimExt2Linetype(const DbObjectId linetype);

    Acad::ErrorStatus   removeTextField();
    Acad::ErrorStatus   fieldToMText(DbMText* pDimMText);
    Acad::ErrorStatus   fieldFromMText(DbMText* pDimMText);

    // Dimension variable api:
    //
#undef DBDIMVAR_H                   // force the file to get read again
#include "dbdimvar.h"
// DEPRECATED METHODS!
// These are supported but will be removed in future releases:
//
    int dimfit() const;
    int dimunit() const;

    Acad::ErrorStatus setDimfit(int);
    Acad::ErrorStatus setDimunit(int);
    //
    // end DEPRECATED METHODS!

    // Functions for internal use only
    //
    bool                isHorizontalRefTextRotation() const;
    Acad::ErrorStatus   setHorizontalRefTextRotation(bool newVal);

    // Dimension Flip Arrow Support
    bool getArrowFirstIsFlipped() const;
    bool getArrowSecondIsFlipped() const;
    Acad::ErrorStatus setArrowFirstIsFlipped(bool bIsFlipped);
    Acad::ErrorStatus setArrowSecondIsFlipped(bool bIsFlipped);

    GeMatrix3d blockTransform() const;

    // Spago inspection dimension enhancement
    enum DimInspect {
        kShapeRemove = 0,     // Not displayed
        kShapeRound = 1,     // Rounded end (default)
        kShapeAngular = 2,     // Angular end
        kShapeNone = 4,     // No bounding shape
        kShapeLabel = 0x10,  // Label separator and alpha field displayed
        kShapeRate = 0x20,  // Shape separator and alpha field displayed
    };
    bool inspection() const;
    Acad::ErrorStatus setInspection(bool val);

    int inspectionFrame() const;
    Acad::ErrorStatus setInspectionFrame(int frame);

    const ACHAR* inspectionLabel() const;
    Acad::ErrorStatus setInspectionLabel(const ACHAR* label);

    const ACHAR* inspectionRate() const;
    Acad::ErrorStatus setInspectionRate(const ACHAR* label);

    // Dimensional constraint support from dimension objects.
    bool  isConstraintObject() const;
    bool  isConstraintObject(bool& hasExpression,
        bool& isReferenceConstraint) const;
    bool  isConstraintDynamic(void) const;
    Acad::ErrorStatus  setConstraintDynamic(bool bDynamic);

    bool  shouldParticipateInOPM(void) const;
    void  setShouldParticipateInOPM(bool bShouldParticipate);
    double                centerMarkSize() const;
    Acad::ErrorStatus     prefix(AcString& sPrefix) const;
    Acad::ErrorStatus     setPrefix(const AcString&);
    Acad::ErrorStatus     suffix(AcString&) const;
    Acad::ErrorStatus     setSuffix(const AcString&);

    Acad::ErrorStatus     alternateSuffix(AcString&) const;
    Acad::ErrorStatus     setAlternateSuffix(const AcString&);
    Acad::ErrorStatus     alternatePrefix(AcString&) const;
    Acad::ErrorStatus     setAlternatePrefix(const AcString&);

    bool                  suppressAngularLeadingZeros() const;
    Acad::ErrorStatus     setSuppressAngularLeadingZeros(bool);

    bool                  suppressAngularTrailingZeros() const;
    Acad::ErrorStatus     setSuppressAngularTrailingZeros(bool);

    bool                  altSuppressZeroInches() const;
    Acad::ErrorStatus     setAltSuppressZeroInches(bool);
    bool                  altSuppressZeroFeet() const;
    Acad::ErrorStatus     setAltSuppressZeroFeet(bool);
    bool                  altSuppressTrailingZeros() const;
    Acad::ErrorStatus     setAltSuppressTrailingZeros(bool);

    bool                  altToleranceSuppressLeadingZeros() const;
    Acad::ErrorStatus     setAltToleranceSuppressLeadingZeros(bool);
    bool                  altToleranceSuppressZeroInches() const;
    Acad::ErrorStatus     setAltToleranceSuppressZeroInches(bool);

    bool                  altToleranceSuppressZeroFeet() const;
    Acad::ErrorStatus     setAltToleranceSuppressZeroFeet(bool);
    bool                  altToleranceSuppressTrailingZeros() const;
    Acad::ErrorStatus     setAltToleranceSuppressTrailingZeros(bool);

    bool                  suppressZeroFeet() const;
    Acad::ErrorStatus     setSuppressZeroFeet(bool);
    bool                  suppressTrailingZeros() const;
    Acad::ErrorStatus     setSuppressTrailingZeros(bool);
    bool                  suppressLeadingZeros() const;
    Acad::ErrorStatus     setSuppressLeadingZeros(bool);
    bool                  suppressZeroInches() const;
    Acad::ErrorStatus     setSuppressZeroInches(bool);

    bool                  altSuppressLeadingZeros() const;
    Acad::ErrorStatus     setAltSuppressLeadingZeros(bool);
    bool                  toleranceSuppressZeroFeet() const;
    Acad::ErrorStatus     setToleranceSuppressZeroFeet(bool);

    bool                  toleranceSuppressTrailingZeros() const;
    Acad::ErrorStatus     setToleranceSuppressTrailingZeros(bool);
    bool                  toleranceSuppressLeadingZeros() const;
    Acad::ErrorStatus     setToleranceSuppressLeadingZeros(bool);

    bool                  toleranceSuppressZeroInches() const;
    Acad::ErrorStatus     setToleranceSuppressZeroInches(bool);

    enum CenterMarkType {
        kMark = 0,
        kLine = 1,
        kNone = 2
    };

    DbDimension::CenterMarkType centerMarkType() const;
};

#endif
