#ifndef GISUBENTITYTRAITS_H
#define GISUBENTITYTRAITS_H

#include "Db.h"
#include "acgidefs.h"
#include "RxObject.h"
#include "DbObjectId.h"
#include "CmEntityColor.h"
#include "CmTransparency.h"


class GiFill;
class GiSubEntityTraitsImpl;
class GiSubEntityTraits : public RxObject
    //
    // This class applies attributes to subsequently drawn Gi geometry
    // primitives.
    //
{
public:
    RX_DECLARE_MEMBERS_VIRTUAL(GiSubEntityTraits);

    /// <summary>
    /// This is a new enumeration of valid bitmasks for calls to the
    /// GiSubEntityTraits::setDrawFlags() method.
    /// </summary>
    enum DrawFlags
    {
        /// <summary>
        /// The default value for the draw flags.This indicates no draw flags are active.
        /// </summary>
        kNoDrawFlags = 0,

        /// <summary>
        /// Indicates that shells or meshes that are part of the entity or sub - entity should
        /// always draw back - facing triangles.This is useful for 3D surfaces or solids that
        /// are not completely closed.Using this flag on closed solids is inefficient, but
        /// will still result in correct visual output.
        /// </summary>

        kDrawBackfaces = 1,

        /// <summary>
        /// Indicates that shells or meshes that are part of the entity or subentity should always draw hatches. 
        /// </summary>
        kDrawHatchGroup = 2,

        /// <summary>
        /// Draw front faces only (i.e. no back faces). Mutually exclusive with kDrawBackfaces flag. 
        /// Specifying them together will give the same behavior as kDrawBackfaces. 
        /// </summary>
        kDrawFrontfacesOnly = 4,

        /// <summary>
        /// Draws with a gradient fill 
        /// </summary>
        kDrawGradientFill = 8,

        /// <summary>
        /// Draws with a solid fill 
        /// </summary>
        kDrawSolidFill = 16,

        /// <summary>
        /// Draw with no line weight. Its enum value is 32.
        /// </summary>
        kDrawNoLineWeight = 32,

        /// <summary>
        /// </summary>
        kDrawNoOptimization = 64,

        /// <summary>
        /// For Autodesk Internal Use Only.
        /// </summary>
        kDrawUseGiEntityForDgnLineType = 128,  // For Autodesk Internal Use Only

        /// <summary>
        /// </summary>
        kDrawFillTextBoundaryStart = 256,

        /// <summary>
        /// </summary>
        kDrawFillTextBoundaryEnd = 512,

        /// <summary>
        /// </summary>
        kDrawFillSelectionWindow = 1024,

        /// <summary>
        /// This item controls whether the newly set sub-entity traits, including color, transparency, line type, line weight and plot style, 
        /// will be force convert to ?ByLayer?, when the entity is in an external referenced drawing and the XREFOVERRIDE sysvar in the host drawing is set to 1.
        /// If this flag is set, newly set traits will not be force set to "ByLayer".
        /// If the flag is not set, newly set traits will be force set to "ByLayer".
        /// </summary>
        kDrawNoForceByLayer = 2048
    };

    enum ShadowFlags
    {
        kShadowsCastAndReceive = 0x00,
        kShadowsDoesNotCast = 0x01,
        kShadowsDoesNotReceive = 0x02,
        kShadowsIgnore = kShadowsDoesNotCast | kShadowsDoesNotReceive,
    };

    enum SelectionFlags
    {
        kNoSelectionFlags = 0x00,
        kSelectionIgnore = 0x01
    };

    // Set properties of drawn objects.
    //
    virtual void              setColor(const UInt16 color) = 0;
    virtual void              setTrueColor(const CmEntityColor& color) = 0;
    virtual void              setLayer(const DbObjectId layerId) = 0;
    virtual void              setLineType(const DbObjectId linetypeId) = 0;
    virtual void              setSelectionMarker(void *markerId) = 0;
    virtual void              setFillType(const GiFillType _type) = 0;
    virtual void              setLineWeight(const Db::LineWeight lw) = 0;
    virtual void              setLineTypeScale(double dScale = 1.0) = 0;
    virtual void              setThickness(double dThickness) = 0;
    virtual void              setVisualStyle(const DbObjectId visualStyleId);
    virtual void              setPlotStyleName(Db::PlotStyleNameType type,const DbObjectId & id = DbObjectId::kNull) { }
    virtual void              setMaterial(const DbObjectId materialId);
    virtual void              setSectionable(bool bSectionable);
    virtual Acad::ErrorStatus setDrawFlags(UInt32 flags);
    virtual Acad::ErrorStatus setShadowFlags(ShadowFlags flags);
    virtual void              setSelectionGeom(bool bSelectionflag);
    virtual void              setTransparency(const CmTransparency& transparency);
    virtual void              setFill(const GiFill* pFill);

    // Return current settings.
    //
    virtual UInt16           color(void) const = 0;
    virtual CmEntityColor         trueColor(void) const = 0;
    virtual DbObjectId            layerId(void) const = 0;
    virtual DbObjectId            lineTypeId(void) const = 0;
    virtual GiFillType            fillType(void) const = 0;
    virtual Db::LineWeight        lineWeight(void) const = 0;
    virtual double                  lineTypeScale(void) const = 0;
    virtual double                  thickness(void) const = 0;
    virtual DbObjectId            visualStyle(void) const;
    virtual Db::PlotStyleNameType getPlotStyleNameId(DbObjectId& /*idResult*/) const
    {
        return Db::kPlotStyleNameByLayer;
    }
    virtual DbObjectId            materialId(void) const;
    virtual bool                    sectionable(void) const;
    virtual UInt32           drawFlags(void) const;
    virtual ShadowFlags             shadowFlags(void) const;
    virtual bool                    selectionGeom(void) const;
    virtual CmTransparency        transparency(void) const;
    virtual const GiFill* fill(void) const;

    virtual void pushMarkerOverride(bool flag, const void* markerId) { }
    virtual void popMarkerOverride(void) { }
    virtual void clearMarkerOverride(void) { }
};



#endif