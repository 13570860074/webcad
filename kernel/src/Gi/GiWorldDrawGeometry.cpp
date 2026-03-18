#include "GiWorldDrawGeometry.h"
#include "arxHeaders.h"
#include "GiEntityManager.h"
#include "GiMaterial.h"
#include "GiMaterialManager.h"
#include "GiLineSegment.h"
#include "GiEntityManager.h"
#include "ApDocManager.h"
#include "ApDocument.h"
#include "GiWorldDrawManager.h"
#include "GiWorldDraw.h"
#include "GiSubEntityTraits.h"
#include "GiEntityTraits.h"
#include "GiPointGeometry.h"
#include "GiLineSegmentGeometry.h"
#include "GiTriangleMeshGeometry.h"
#include "GiEntityIterator.h"
#include "DbObjectManager.h"
#include "kernel.h"
#include "GiTextStyle.h"
#include "GiMesh.h"
#include "GiImpl.h"
#include "DbImpl.h"
#include "AcDebug.h"

GiWorldDrawGeometry::GiWorldDrawGeometry()
{
	this->m_pImpl = new GiWorldDrawGeometryImpl();
}
GiWorldDrawGeometry::~GiWorldDrawGeometry()
{
}
GiSubEntityTraits* GiWorldDrawGeometry::getEntityTraits() const
{
	return GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->pEntityTraits;
}

void GiWorldDrawGeometry::getModelToWorldTransform(GeMatrix3d&) const
{
}
void GiWorldDrawGeometry::getWorldToModelTransform(GeMatrix3d&) const
{
}

bool GiWorldDrawGeometry::pushModelTransform(const GeVector3d& vNormal)
{
	return false;
}
bool GiWorldDrawGeometry::pushModelTransform(const GeMatrix3d& xMat)
{
	return false;
}
bool GiWorldDrawGeometry::popModelTransform()
{
	return false;
}
GeMatrix3d GiWorldDrawGeometry::pushPositionTransform(GiPositionTransformBehavior behavior, const GePoint3d& offset)
{
	return GeMatrix3d();
}
GeMatrix3d GiWorldDrawGeometry::pushPositionTransform(GiPositionTransformBehavior behavior, const GePoint2d& offset)
{
	return GeMatrix3d();
}
GeMatrix3d GiWorldDrawGeometry::pushScaleTransform(GiScaleTransformBehavior behavior, const GePoint3d& extents)
{
	return GeMatrix3d();
}
GeMatrix3d GiWorldDrawGeometry::pushScaleTransform(GiScaleTransformBehavior behavior, const GePoint2d& extents)
{
	return GeMatrix3d();
}
GeMatrix3d GiWorldDrawGeometry::pushOrientationTransform(GiOrientationTransformBehavior behavior)
{
	return GeMatrix3d();
}


void GiWorldDrawGeometry::worldGeometryStakes(AcArray<GiWorldGeometryStake*>& g) {
	g = *&GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->worldGeometryStakes;
}

void GiWorldDrawGeometry::removeWorldGeometryStakes() {
	realloc_arry_all(GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->worldGeometryStakes);
}

void GiWorldDrawGeometry::drawPoint(const GiMaterial* pMaterial, const GiEntityTraits* entityTraits, const GePoint3d& point) const
{
	GiPointGeometry* pPointGeometry = new GiPointGeometry();
	if (entityTraits->isOpenMatrix3d() == true)
	{
		GePoint3d position = point;
		position.transformBy(entityTraits->matrix3d());
		pPointGeometry->setPosition(position);
	}
	else
	{
		pPointGeometry->setPosition(point);
	}

	pPointGeometry->setStake(entityTraits->stake());

	GiWorldGeometryStake* stake = new GiWorldGeometryStake();
	stake->setGeometry(pPointGeometry);
	stake->setMaterial((GiMaterial*)pMaterial);
	GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->worldGeometryStakes.append(stake);

	DB_IMP_ENTITY(entityTraits->rootEntity()->m_pImpl)->entitys.append(pPointGeometry);
}
void GiWorldDrawGeometry::drawLine(const GiMaterial* pMaterial, const GiEntityTraits* entityTraits, const GePoint3d& _startPoint, const GePoint3d& _endPoint) const
{
	GiLineSegmentGeometry* pLineSegmentGeometry = new GiLineSegmentGeometry();
	if (entityTraits->isOpenMatrix3d() == true)
	{
		GePoint3d p1 = _startPoint;
		GePoint3d p2 = _endPoint;
		p1.transformBy(entityTraits->matrix3d());
		p2.transformBy(entityTraits->matrix3d());
		pLineSegmentGeometry->setStartPoint(p1);
		pLineSegmentGeometry->setEndPoint(p2);
	}
	else
	{
		pLineSegmentGeometry->setStartPoint(_startPoint);
		pLineSegmentGeometry->setEndPoint(_endPoint);
	}

	pLineSegmentGeometry->setStake(entityTraits->stake());

	GiWorldGeometryStake* stake = new GiWorldGeometryStake();
	stake->setGeometry(pLineSegmentGeometry);
	stake->setMaterial((GiMaterial*)pMaterial);
	GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->worldGeometryStakes.append(stake);

	DB_IMP_ENTITY(entityTraits->rootEntity()->m_pImpl)->entitys.append(pLineSegmentGeometry);
}
void GiWorldDrawGeometry::drawFace(const GiMaterial* pMaterial, const GiEntityTraits* entityTraits, const GePoint3dArray& verts) const
{

	for (int i = 2; i < verts.length(); i++)
	{
		GiTriangleMeshGeometry* pTriangleMeshGeometry = new GiTriangleMeshGeometry();
		if (entityTraits->isOpenMatrix3d() == true)
		{
			GePoint3d p1 = verts.at(i - 2);
			GePoint3d p2 = verts.at(i - 1);
			GePoint3d p3 = verts.at(i);
			p1.transformBy(entityTraits->matrix3d());
			p2.transformBy(entityTraits->matrix3d());
			p3.transformBy(entityTraits->matrix3d());
			pTriangleMeshGeometry->setVertex(0, p1);
			pTriangleMeshGeometry->setVertex(1, p2);
			pTriangleMeshGeometry->setVertex(2, p3);
		}
		else
		{
			pTriangleMeshGeometry->setVertex(0, verts.at(i - 2));
			pTriangleMeshGeometry->setVertex(1, verts.at(i - 1));
			pTriangleMeshGeometry->setVertex(2, verts.at(i));
		}

		pTriangleMeshGeometry->setStake(entityTraits->stake());

		GiWorldGeometryStake* stake = new GiWorldGeometryStake();
		stake->setGeometry(pTriangleMeshGeometry);
		stake->setMaterial((GiMaterial*)pMaterial);
		GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->worldGeometryStakes.append(stake);

		DB_IMP_ENTITY(entityTraits->rootEntity()->m_pImpl)->entitys.append(pTriangleMeshGeometry);
	}
}

bool GiWorldDrawGeometry::line(const GePoint3d& startPoint, const GePoint3d& endPoint) const
{
	// 获得实体属性
	GiEntityTraits* entityTraits = (GiEntityTraits*)this->getEntityTraits();

	// 判断是否被隐藏
	if (entityTraits->entity()->visibility() == Db::Visibility::kInvisible)
	{
		return true;
	}

	// 获得材质
	GiMaterial* pMaterial = ::acgiMaterialManager()->lineSegments(entityTraits->color(), entityTraits->transparency().alpha(), entityTraits->lineWeight() / 10.0);

	// 创建实体
	this->drawLine(pMaterial, entityTraits, startPoint, endPoint);

	return true;
}
bool GiWorldDrawGeometry::circle(const GePoint3d& center, const double radius, const GeVector3d& normal) const
{
	do
	{

		if (radius < 0)
		{
			break;
		}

		// 获得实体属性
		GiEntityTraits* entityTraits = (GiEntityTraits*)this->getEntityTraits();

		// 判断是否被隐藏
		if (entityTraits->entity()->visibility() == Db::Visibility::kInvisible)
		{
			break;
		}

		// 获得材质
		GiMaterial* pMaterial = ::acgiMaterialManager()->lineSegments(entityTraits->color(), 255, entityTraits->lineWeight() / 10.0);

		// 计算圆的分段数量
		unsigned int segment = 50;

		// 创建几何对象
		GePoint3dArray vertexs = GeCircArc3d::toLineSegment(center, radius, normal, segment);
		for (int i = 0; i < vertexs.length() - 1; i++)
		{
			this->drawLine(pMaterial, entityTraits, vertexs[i], vertexs[i + 1]);
		}

	} while (false);

	return true;
}
bool GiWorldDrawGeometry::circle(const GePoint3d& start, const GePoint3d& point, const GePoint3d& end) const
{
	GeCircArc3d circArc;
	circArc.set(start, point, end);
	return this->circle(circArc.center(), circArc.radius(), circArc.normal());
}

bool GiWorldDrawGeometry::circularArc(const GePoint3d& center, const double radius, const GeVector3d& normal, const GeVector3d& startVector, const double sweepAngle, const GiArcType arcType) const
{

	do
	{

		if (radius < 0)
		{
			break;
		}

		// 获得实体属性
		GiEntityTraits* entityTraits = (GiEntityTraits*)this->getEntityTraits();

		// 判断是否被隐藏
		if (entityTraits->entity()->visibility() == Db::Visibility::kInvisible)
		{
			break;
		}

		// 获得材质
		GiMaterial* pMaterial = ::acgiMaterialManager()->lineSegments(entityTraits->color(), 255, entityTraits->lineWeight() / 10.0);

		// 计算圆的分段数量
		int segment = 50;
		segment = int(abs(sweepAngle) / (PI * 2) * segment + 0.5);
		if (segment < 10)
		{
			segment = 10;
		}

		// 创建几何对象
		GePoint3dArray vertexs = GeCircArc3d::toLineSegment(center, radius, normal, startVector, sweepAngle, segment);
		for (int i = 0; i < vertexs.length() - 1; i++)
		{
			this->drawLine(pMaterial, entityTraits, vertexs[i], vertexs[i + 1]);
		}

	} while (false);

	return true;
}
bool GiWorldDrawGeometry::circularArc(const GePoint3d& start, const GePoint3d& point, const GePoint3d& end, const GiArcType arcType) const
{
	GeCircArc3d circArc;
	circArc.set(start, point, end);

	// 获得起始和结束弧度
	double startAng = circArc.startAng();
	double endAng = circArc.endAng();
	if (startAng > endAng)
	{
		endAng += PI * 2;
	}

	this->circularArc(circArc.center(), circArc.radius(), circArc.normal(), start - circArc.center(), endAng - startAng);
	return false;
}

Adesk::Boolean GiWorldDrawGeometry::polyline(const Adesk::UInt32 nbPoints, const GePoint3d* pVertexList, const GeVector3d* pNormal, Adesk::LongPtr lBaseSubEntMarker) const
{

	do
	{

		// 获得实体属性
		GiEntityTraits* entityTraits = (GiEntityTraits*)this->getEntityTraits();

		// 判断是否被隐藏
		if (entityTraits->entity()->visibility() == Db::Visibility::kInvisible)
		{
			break;
		}

		// 判断是否是点
		if (nbPoints == 2 && pVertexList[0].isEqualTo(pVertexList[1]) == true)
		{

			// 获得材质
			GiMaterial* pMaterial = ::acgiMaterialManager()->points(entityTraits->color(), 255, 1);

			// 创建几何对象
			this->drawPoint(pMaterial, entityTraits, pVertexList[0]);
		}
		else
		{

			// 获得材质
			GiMaterial* pMaterial = ::acgiMaterialManager()->lineSegments(entityTraits->color(), 255, entityTraits->lineWeight() / 10.0);

			// 创建几何对象
			for (int i = 0; i < nbPoints - 1; i++)
			{
				this->drawLine(pMaterial, entityTraits, pVertexList[i], pVertexList[i + 1]);
			}
		}

	} while (false);

	return true;
}

Adesk::Boolean GiWorldDrawGeometry::shell(const Adesk::UInt32 nbVertex,
	const GePoint3d* pVertexList,
	const Adesk::UInt32 faceListSize,
	const Adesk::Int32* pFaceList,
	const GiEdgeData* pEdgeData,
	const GiFaceData* pFaceData,
	const GiVertexData* pVertexData,
	const struct resbuf* pResBuf,
	const bool bAutoGenerateNormals) const
{

	do
	{
		// 获得实体属性
		GiEntityTraits* entityTraits = (GiEntityTraits*)this->getEntityTraits();

		// 判断是否被隐藏
		if (entityTraits->entity()->visibility() == Db::Visibility::kInvisible)
		{
			break;
		}

		// 获得材质
		GiMaterial* pMaterial = ::acgiMaterialManager()->meshBasic(entityTraits->color(), entityTraits->transparency().alpha());

		for (int i = 0; i < faceListSize; i++)
		{

			// 获得face数量
			int countFace = pFaceList[i];

			// 获得点
			GePoint3dArray faceVertexs;
			for (int u = 0; u < countFace; u++)
			{
				GePoint3d vertex = pVertexList[pFaceList[i + u + 1]];
				faceVertexs.append(vertex);
			}

			// 绘制片体
			this->drawFace(pMaterial, entityTraits, faceVertexs);

			i = i + countFace;
		}

	} while (false);

	return false;
}

Adesk::Boolean GiWorldDrawGeometry::text(const GePoint3d& position,
	const GeVector3d& normal,
	const GeVector3d& direction,
	const double height,
	const double width,
	const double oblique,
	const ACHAR* pMsg) const
{

	GiTextStyle textStyle;
	textStyle.setTextSize(height);
	textStyle.setObliquingAngle(oblique);
	this->text(position, normal, GeVector3d::kXAxis, pMsg, strlen(pMsg), true, textStyle);

	return true;
}

Adesk::Boolean GiWorldDrawGeometry::text(const GePoint3d& position,
	const GeVector3d& normal,
	const GeVector3d& direction,
	const ACHAR* pMsg,
	const Adesk::Int32 length,
	const Adesk::Boolean raw,
	const GiTextStyle& pTextStyle) const
{


	return true;
}

Adesk::Boolean GiWorldDrawGeometry::xline(const GePoint3d& basePoint, const GePoint3d& secondPoint) const
{
	GeVector3d direction = secondPoint - basePoint;
	direction.normalize();
	this->line(basePoint, basePoint - direction * ACDB_VIEWPORT_EXTMAXSIZE);
	this->line(basePoint, basePoint + direction * ACDB_VIEWPORT_EXTMAXSIZE);
	return true;
}
Adesk::Boolean GiWorldDrawGeometry::ray(const GePoint3d& basePoint, const GePoint3d& secondPoint) const
{
	GeVector3d direction = secondPoint - basePoint;
	direction.normalize();
	return this->line(basePoint, basePoint + direction * ACDB_VIEWPORT_EXTMAXSIZE);
}