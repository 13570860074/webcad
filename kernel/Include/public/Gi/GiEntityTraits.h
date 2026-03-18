#ifndef GIENTITYTRAITS_H
#define GIENTITYTRAITS_H

#include "GeMatrix3d.h"
#include "GiSubEntityTraits.h"
#include "DbBlockReference.h"

class GiFill;
class ApDocument;
class DbEntityStake;
class GiEntityTraits : public GiSubEntityTraits
{
public:
	GiEntityTraits();
	virtual ~GiEntityTraits();
	RX_DECLARE_MEMBERS(GiEntityTraits);

	virtual void init();

	void setEntity(DbEntity* pEntity);
	DbEntity* entity() const;
	void setDocument(ApDocument* _document);
	ApDocument* document();
	void setRootEntity(DbEntity* entity);
	DbEntity* rootEntity()const;
	void openMatrix3d(const bool isMatrix);
	bool isOpenMatrix3d()const;
	void setMatrix3d(const GeMatrix3d& mat);
	const GeMatrix3d& matrix3d()const;
	void setStake(DbEntityStake* stake);
	DbEntityStake* stake()const;

	virtual void              setColor(const UInt16 color);
	virtual void              setTrueColor(const CmEntityColor& color);
	virtual void              setLayer(const DbObjectId layerId);
	virtual void              setLineType(const DbObjectId linetypeId);
	virtual void              setSelectionMarker(void* markerId);
	virtual void              setFillType(const GiFillType _type);
	virtual void              setLineWeight(const Db::LineWeight lw);
	virtual void              setLineTypeScale(double dScale = 1.0);
	virtual void              setThickness(double dThickness);
	virtual void              setVisualStyle(const DbObjectId visualStyleId);
	virtual void              setPlotStyleName(Db::PlotStyleNameType type, const DbObjectId& id = DbObjectId::kNull);
	virtual void              setMaterial(const DbObjectId materialId);
	virtual void              setSectionable(bool bSectionable);
	virtual Acad::ErrorStatus setDrawFlags(UInt32 flags);
	virtual Acad::ErrorStatus setShadowFlags(ShadowFlags flags);
	virtual void              setSelectionGeom(bool bSelectionflag);
	virtual void              setTransparency(const CmTransparency& transparency);
	virtual void              setFill(const GiFill* pFill);
	virtual void              setVisibility(bool _isVisibility);

	// Return current settings.
	//
	virtual UInt16           color(void) const;
	virtual CmEntityColor         trueColor(void) const;
	virtual DbObjectId            layerId(void) const;
	virtual DbObjectId            lineTypeId(void) const;
	virtual GiFillType            fillType(void) const;
	virtual Db::LineWeight        lineWeight(void) const;
	virtual double                  lineTypeScale(void) const;
	virtual double                  thickness(void) const;
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
	virtual bool isVisibility()const;

	virtual void pushMarkerOverride(bool flag, const void* markerId);
	virtual void popMarkerOverride(void);
	virtual void clearMarkerOverride(void);
};



#endif