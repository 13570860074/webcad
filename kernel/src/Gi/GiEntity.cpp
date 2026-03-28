#include "GiEntity.h"
#include "kernel.h"
#include "GiEntityReactor.h"
#include "GiEntityManager.h"
#include "GiLineSegmentGeometry.h"
#include "GiTriangleMeshGeometry.h"
#include "GiPointGeometry.h"
#include "GiGeometryPool.h"
#include "GiImpl.h"

GiEntity::GiEntity()
{
}
GiEntity::~GiEntity()
{
}

Gi::EntityType GiEntity::type() const
{
	return Gi::EntityType::kUnknownEntity;
}
Gi::ElementType GiEntity::element() const
{
	return GI_IMP_ENTITY(this->m_pImpl)->element;
}
GiMaterial* GiEntity::material() const {
	return GI_IMP_ENTITY(this->m_pImpl)->material;
}
void GiEntity::setMaterial(const GiMaterial* material) {
	GI_IMP_ENTITY(this->m_pImpl)->material = (GiMaterial*)material;
}
GePoint3d GiEntity::position() const
{
	return GePoint3d(
		GI_IMP_ENTITY(this->m_pImpl)->matrix.entry[0][3],
		GI_IMP_ENTITY(this->m_pImpl)->matrix.entry[1][3],
		GI_IMP_ENTITY(this->m_pImpl)->matrix.entry[2][3]);
}
bool GiEntity::visibility() const
{
	return GI_IMP_ENTITY(this->m_pImpl)->visibility;
}
void GiEntity::clear()
{
	int count = GI_IMP_ENTITY(this->m_pImpl)->geometrys.length();
	for (int i = 0; i < count; i++)
	{
		GI_IMP_ENTITYGEOMETRY(GI_IMP_ENTITY(this->m_pImpl)->geometrys[i]->m_pImpl)->isErased = true;
	}
	if (count > 0) {
		GI_IMP_ENTITY(this->m_pImpl)->bUpdateVertex = true;
	}
}
void GiEntity::update()
{
	GiGeometryPool* pool = ::kernel()->acgiEntityManager()->pool();
	AcArray<GiEntityGeometry*>& geoms = GI_IMP_ENTITY(this->m_pImpl)->geometrys;
	int writeIdx = 0;
	for (int i = 0; i < geoms.length(); i++) {
		GiEntityGeometry* g = geoms[i];
		if (g->isErased()) {
			Gi::EntityGeometryType t = g->type();
			if (t == Gi::EntityGeometryType::kLineSegmentGeometry)
				pool->releaseLineSegment((GiLineSegmentGeometry*)g);
			else if (t == Gi::EntityGeometryType::kTriangleMeshGeometry)
				pool->releaseTriangleMesh((GiTriangleMeshGeometry*)g);
			else if (t == Gi::EntityGeometryType::kPointGeometry)
				pool->releasePoint((GiPointGeometry*)g);
			else
				delete g;
		} else {
			geoms[writeIdx++] = g;
		}
	}
	// 移除尾部已释放的槽位
	while (geoms.length() > writeIdx)
		geoms.removeAt(geoms.length() - 1);
}
unsigned int GiEntity::numGeometrys() const
{
	return GI_IMP_ENTITY(this->m_pImpl)->geometrys.length();
}
const AcArray<GiEntityGeometry*>& GiEntity::geometrys()
{
	return GI_IMP_ENTITY(this->m_pImpl)->geometrys;
}
unsigned int GiEntity::vertexLength() const
{
	return GI_IMP_ENTITY(this->m_pImpl)->vertexLength;
}
#if EMSDK
emscripten::val GiEntity::vertexs()
{
	return val(typed_memory_view(0, GI_IMP_ENTITY(this->m_pImpl)->vertexs));
}
#else
double* GiEntity::vertexs()
{
	return GI_IMP_ENTITY(this->m_pImpl)->vertexs;
}
#endif
void GiEntity::setElement(const Gi::ElementType _style)
{
	GI_IMP_ENTITY(this->m_pImpl)->element = _style;
}
void GiEntity::setVisibility(const bool _isVisibility)
{
	GI_IMP_ENTITY(this->m_pImpl)->visibility = _isVisibility;
}
GeMatrix3d GiEntity::matrix() const
{
	return GI_IMP_ENTITY(this->m_pImpl)->matrix;
}
void GiEntity::setMatrix(const GeMatrix3d& _matrix)
{
	GI_IMP_ENTITY(this->m_pImpl)->matrix = _matrix;
}
GeMatrix3d& GiEntity::applyMatrix(const GeMatrix3d& _matrix) const
{
	GI_IMP_ENTITY(this->m_pImpl)->bTransformation = true;
	GI_IMP_ENTITY(this->m_pImpl)->matrix *= _matrix;
	return GI_IMP_ENTITY(this->m_pImpl)->matrix;
}
void GiEntity::setPosition(const GePoint3d& point)
{
	GI_IMP_ENTITY(this->m_pImpl)->bTransformation = true;
	GI_IMP_ENTITY(this->m_pImpl)->matrix.entry[0][3] = point.x;
	GI_IMP_ENTITY(this->m_pImpl)->matrix.entry[1][3] = point.y;
	GI_IMP_ENTITY(this->m_pImpl)->matrix.entry[2][3] = point.z;
}
GeQuaternion GiEntity::quaternion() const
{
	GeQuaternion val;
	val.setMatrix(GI_IMP_ENTITY(this->m_pImpl)->matrix);
	return val;
}
