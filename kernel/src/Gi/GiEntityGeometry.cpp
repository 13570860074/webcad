#include "GiEntityGeometry.h"
#include "GiEntityManager.h"
#include "GiEntity.h"
#include "kernel.h"
#include "GiImpl.h"

GiEntityGeometry::GiEntityGeometry()
{
}
GiEntityGeometry::~GiEntityGeometry()
{
}
void GiEntityGeometry::clone(GiEntityGeometry *&p) const
{
}
GiEntityGeometry *GiEntityGeometry::clone() const
{
	return NULL;
}

DbEntityStake* GiEntityGeometry::stake() const {
	return  GI_IMP_ENTITYGEOMETRY(this->m_pImpl)->stake;
}
void GiEntityGeometry::setStake(DbEntityStake* stakep) {
	 GI_IMP_ENTITYGEOMETRY(this->m_pImpl)->stake = stakep;
}
bool GiEntityGeometry::visibility() const
{
	return GI_IMP_ENTITYGEOMETRY(this->m_pImpl)->visibility;
}
void GiEntityGeometry::setVisibility(const bool _isVisibility)
{
	if (this->giEntity() != NULL)
	{
		GI_IMP_ENTITY(this->giEntity()->m_pImpl)->bUpdateVertex = true;
	}
	GI_IMP_ENTITYGEOMETRY(this->m_pImpl)->visibility = _isVisibility;
}
bool GiEntityGeometry::isErased()
{
	return GI_IMP_ENTITYGEOMETRY(this->m_pImpl)->isErased;
}
void GiEntityGeometry::erased()
{
	if (this->giEntity() != NULL)
	{
		GI_IMP_ENTITY(this->giEntity()->m_pImpl)->bUpdateVertex = true;
	}

	/* 标记删除 */
	GI_IMP_ENTITYGEOMETRY(this->m_pImpl)->isErased = true;
}
GiEntity *GiEntityGeometry::giEntity()
{
	return GI_IMP_ENTITYGEOMETRY(this->m_pImpl)->giEntity;
}
GePoint2d GiEntityGeometry::displayCoord(unsigned char index) const
{
	return GePoint2d(0, 0);
}
void GiEntityGeometry::setDisplayCoord(unsigned char index, const GePoint2d &pt) const
{
}