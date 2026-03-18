#include "DbJigRealDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"

DbJigRealDimension::DbJigRealDimension()
{
	this->m_pImpl = new DbJigRealDimensionImpl();
}
DbJigRealDimension::~DbJigRealDimension()
{
}
double DbJigRealDimension::value() const
{
	return ((DbJigRealDimensionImpl *)this->m_pImpl)->value;
}
void DbJigRealDimension::setValue(const double &v)
{
	((DbJigRealDimensionImpl *)this->m_pImpl)->value = v;
}

Acad::ErrorStatus DbJigRealDimension::dwgInFields(DbDwgFiler *pFiler)
{

	Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbJigRealDimension::dwgOutFields(DbDwgFiler *pFiler) const
{

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}
bool DbJigRealDimension::subWorldDraw(GiWorldDraw *pWd) const
{
	return DbDimension::subWorldDraw(pWd);
}


Acad::ErrorStatus DbJigRealDimension::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbJigRealDimension::subTransformBy(const GeMatrix3d &xform)
{

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbJigRealDimension::subGetGripPoints(
	DbGripDataPtrArray &grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d &curViewDir,
	const int bitflags) const
{

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbJigRealDimension::subGetOsnapPoints(
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

Acad::ErrorStatus DbJigRealDimension::subMoveGripPointsAt(const DbIntArray &indices, const GeVector3d &offset)
{

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbJigRealDimension::subMoveGripPointsAt(const DbVoidPtrArray &gripAppData, const GeVector3d &offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbJigRealDimension::subIntersectWith(
	const DbEntity *pEnt,
	Db::Intersect intType,
	GePoint3dArray &points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const
{
	return Acad::ErrorStatus::eOk;
}