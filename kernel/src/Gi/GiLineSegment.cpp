#include "GiLineSegment.h"
#include "GiLineSegmentGeometry.h"
#include "GiEntityManager.h"
#include "GeCircArc3d.h"
#include "GeLine3d.h"
#include "GeRay3d.h"
#include "Db.h"
#include "kernel.h"
#include "GiImpl.h"

GiLineSegment::GiLineSegment()
{
	this->m_pImpl = new GiLineSegmentImpl();
}
GiLineSegment::~GiLineSegment()
{
}
Gi::EntityType GiLineSegment::type() const
{
	return Gi::EntityType::kLineSegment;
}
void GiLineSegment::update() {

	GiEntity::update();

	/** 分配内存 */
	int sizeVertexData = GI_IMP_LINESEGMENT(this->m_pImpl)->geometrys.length() * 6;
	if (GI_IMP_LINESEGMENT(this->m_pImpl)->vertexs == NULL) {
		GI_IMP_LINESEGMENT(this->m_pImpl)->sizeVertexData = sizeVertexData;
		GI_IMP_LINESEGMENT(this->m_pImpl)->vertexs = new double[GI_IMP_LINESEGMENT(this->m_pImpl)->sizeVertexData];
	}
	else if (sizeVertexData > GI_IMP_LINESEGMENT(this->m_pImpl)->sizeVertexData) {
		delete[] GI_IMP_LINESEGMENT(this->m_pImpl)->vertexs;
		GI_IMP_LINESEGMENT(this->m_pImpl)->sizeVertexData = sizeVertexData;
		GI_IMP_LINESEGMENT(this->m_pImpl)->vertexs = new double[GI_IMP_LINESEGMENT(this->m_pImpl)->sizeVertexData];
	}

	/** 设置顶点 */
	GI_IMP_LINESEGMENT(this->m_pImpl)->vertexLength = 0;
	for (int i = 0; i < GI_IMP_LINESEGMENT(this->m_pImpl)->geometrys.length(); i++) {
		GiLineSegmentGeometry* pLineSegmentGeometry = (GiLineSegmentGeometry*)GI_IMP_LINESEGMENT(this->m_pImpl)->geometrys[i];
		if (pLineSegmentGeometry->isErased() == true) {
			continue;
		}
		if (pLineSegmentGeometry->visibility() == false) {
			continue;
		}
		GI_IMP_LINESEGMENT(this->m_pImpl)->vertexs[GI_IMP_LINESEGMENT(this->m_pImpl)->vertexLength++] = pLineSegmentGeometry->startPoint().x;
		GI_IMP_LINESEGMENT(this->m_pImpl)->vertexs[GI_IMP_LINESEGMENT(this->m_pImpl)->vertexLength++] = pLineSegmentGeometry->startPoint().y;
		GI_IMP_LINESEGMENT(this->m_pImpl)->vertexs[GI_IMP_LINESEGMENT(this->m_pImpl)->vertexLength++] = pLineSegmentGeometry->startPoint().z;

		GI_IMP_LINESEGMENT(this->m_pImpl)->vertexs[GI_IMP_LINESEGMENT(this->m_pImpl)->vertexLength++] = pLineSegmentGeometry->endPoint().x;
		GI_IMP_LINESEGMENT(this->m_pImpl)->vertexs[GI_IMP_LINESEGMENT(this->m_pImpl)->vertexLength++] = pLineSegmentGeometry->endPoint().y;
		GI_IMP_LINESEGMENT(this->m_pImpl)->vertexs[GI_IMP_LINESEGMENT(this->m_pImpl)->vertexLength++] = pLineSegmentGeometry->endPoint().z;
	}
}

#if EMSDK
emscripten::val GiLineSegment::vertexs()
{
	return val(typed_memory_view(GI_IMP_LINESEGMENT(this->m_pImpl)->vertexLength, GI_IMP_ENTITY(this->m_pImpl)->vertexs));
}
#else
double* GiLineSegment::vertexs()
{
	return GI_IMP_LINESEGMENT(this->m_pImpl)->vertexs;
}
#endif

GiLineSegmentGeometry* GiLineSegment::appendLineSegment(const GePoint3d& _startPoint, const GePoint3d& _endPoint)
{
	GiLineSegmentGeometry* pLineSegmentGeometry = new GiLineSegmentGeometry();
	pLineSegmentGeometry->setStartPoint(_startPoint);
	pLineSegmentGeometry->setEndPoint(_endPoint);

	this->appendLineSegment(pLineSegmentGeometry);

	return pLineSegmentGeometry;
}
void GiLineSegment::appendLineSegment(GiLineSegmentGeometry* line)
{
	GI_IMP_ENTITYGEOMETRY(line->m_pImpl)->giEntity = this;
	GI_IMP_LINESEGMENT(this->m_pImpl)->geometrys.append(line);
	GI_IMP_LINESEGMENT(this->m_pImpl)->bUpdateVertex = true;
}
GiLineSegmentGeometry* GiLineSegment::appendLine(const GePoint3d& basePoint, const GeVector3d& direction)
{
	GePoint3d startPoint = basePoint - direction.normal() * ACDB_VIEWPORT_EXTMAXSIZE / 2.0;
	GePoint3d endPoint = basePoint + direction.normal() * ACDB_VIEWPORT_EXTMAXSIZE / 2.0;
	return this->appendLineSegment(startPoint, endPoint);
}
GiLineSegmentGeometry* GiLineSegment::appendRay(const GePoint3d& basePoint, const GeVector3d& direction)
{
	GePoint3d startPoint = basePoint;
	GePoint3d endPoint = basePoint + direction.normal() * ACDB_VIEWPORT_EXTMAXSIZE / 2.0;
	return this->appendLineSegment(startPoint, endPoint);
}
void GiLineSegment::appendCircle(int numSegments, const GePoint3d& _center, const double radius, const GeVector3d& _normal)
{

	GePoint3dArray points = GeCircArc3d::toLineSegment(_center, radius, _normal, numSegments);
	for (int i = 1; i < points.length(); i++)
	{
		// 创建几何线段
		GiLineSegmentGeometry* pLineSegmentGeometry = new GiLineSegmentGeometry();
		pLineSegmentGeometry->setStartPoint(points[i - 1]);
		pLineSegmentGeometry->setEndPoint(points[i]);

		// 添加到实体列表
		this->appendLineSegment(pLineSegmentGeometry);
	}

	// 设置有顶点更新
	GI_IMP_LINESEGMENT(this->m_pImpl)->bUpdateVertex = true;
}
void GiLineSegment::appendArc(int numSegments, const GePoint3d& _center, const double radius, const double startAng, const double endAng, const GeVector3d& _normal)
{
	GePoint3dArray points = GeCircArc3d::toLineSegment(_center, radius, startAng, endAng, _normal, numSegments);
	for (int i = 1; i < points.length(); i++)
	{
		// 创建几何线段
		GiLineSegmentGeometry* pLineSegmentGeometry = new GiLineSegmentGeometry();
		pLineSegmentGeometry->setStartPoint(points[i - 1]);
		pLineSegmentGeometry->setEndPoint(points[i]);

		// 添加到实体列表
		this->appendLineSegment(pLineSegmentGeometry);
	}
}