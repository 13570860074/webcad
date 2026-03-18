#include "DbJigStringDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"

DbJigStringDimension::DbJigStringDimension()
{
	this->m_pImpl = new DbJigStringDimensionImpl();
}
DbJigStringDimension::~DbJigStringDimension()
{
}
const AcString DbJigStringDimension::value() const
{
	return ((DbJigStringDimensionImpl *)this->m_pImpl)->value;
}
void DbJigStringDimension::setValue(const AcString&v)
{
	((DbJigStringDimensionImpl *)this->m_pImpl)->value = v;
}
#if EMSDK

const emstring DbJigStringDimension::emsdk_value() const
{
	return ((DbJigStringDimensionImpl*)this->m_pImpl)->value.constPtr();
}
void DbJigStringDimension::emsdk_setValue(const emstring& v)
{
	((DbJigStringDimensionImpl*)this->m_pImpl)->value = v;
}

#endif

Acad::ErrorStatus DbJigStringDimension::dwgInFields(DbDwgFiler *pFiler)
{

	Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbJigStringDimension::dwgOutFields(DbDwgFiler *pFiler) const
{

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}
bool DbJigStringDimension::subWorldDraw(GiWorldDraw *pWd) const
{
	return DbDimension::subWorldDraw(pWd);
}


Acad::ErrorStatus DbJigStringDimension::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbJigStringDimension::subTransformBy(const GeMatrix3d &xform)
{

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbJigStringDimension::subGetGripPoints(
	DbGripDataPtrArray &grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d &curViewDir,
	const int bitflags) const
{

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbJigStringDimension::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d &pickPoint,
	const GePoint3d &lastPoint,
	const GeMatrix3d &viewXform,
	GePoint3dArray &snapPoints,
	DbIntArray &geomIds) const
{

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbJigStringDimension::subMoveGripPointsAt(const DbIntArray &indices, const GeVector3d &offset)
{

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbJigStringDimension::subMoveGripPointsAt(const DbVoidPtrArray &gripAppData, const GeVector3d &offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbJigStringDimension::subIntersectWith(
	const DbEntity *pEnt,
	Db::Intersect intType,
	GePoint3dArray &points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const
{
	return Acad::ErrorStatus::eOk;
}