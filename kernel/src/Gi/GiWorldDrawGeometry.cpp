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
#include "GeEllipArc3d.h"
#include "GiDrawable.h"

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

void GiWorldDrawGeometry::getModelToWorldTransform(GeMatrix3d& xMat) const
{
	int len = GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.length();
	if (len > 0) {
		xMat = GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.at(len - 1);
	}
	else {
		xMat = GeMatrix3d::kIdentity;
	}
}
void GiWorldDrawGeometry::getWorldToModelTransform(GeMatrix3d& xMat) const
{
	GeMatrix3d modelToWorld;
	this->getModelToWorldTransform(modelToWorld);
	xMat = modelToWorld.inverse();
}

bool GiWorldDrawGeometry::pushModelTransform(const GeVector3d& vNormal)
{
	GeMatrix3d xMat = GeMatrix3d::planeToWorld(vNormal);
	return this->pushModelTransform(xMat);
}
bool GiWorldDrawGeometry::pushModelTransform(const GeMatrix3d& xMat)
{
	int len = GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.length();
	if (len > 0) {
		// 累积变换: 新变换 = 当前变换 * 传入变换
		GeMatrix3d composite = GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.at(len - 1) * xMat;
		GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.append(composite);
	}
	else {
		GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.append(xMat);
	}
	return true;
}
bool GiWorldDrawGeometry::popModelTransform()
{
	int len = GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.length();
	if (len > 0) {
		GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.removeAt(len - 1);
		return true;
	}
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
	GePoint3d position = point;

	// 应用模型变换
	int stackLen = GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.length();
	if (stackLen > 0) {
		position.transformBy(GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.at(stackLen - 1));
	}

	// 应用实体变换
	if (entityTraits->isOpenMatrix3d() == true) {
		position.transformBy(entityTraits->matrix3d());
	}

	pPointGeometry->setPosition(position);
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
	GePoint3d p1 = _startPoint;
	GePoint3d p2 = _endPoint;

	// 应用模型变换
	int stackLen = GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.length();
	if (stackLen > 0) {
		const GeMatrix3d& modelXform = GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.at(stackLen - 1);
		p1.transformBy(modelXform);
		p2.transformBy(modelXform);
	}

	// 应用实体变换
	if (entityTraits->isOpenMatrix3d() == true) {
		p1.transformBy(entityTraits->matrix3d());
		p2.transformBy(entityTraits->matrix3d());
	}

	pLineSegmentGeometry->setStartPoint(p1);
	pLineSegmentGeometry->setEndPoint(p2);
	pLineSegmentGeometry->setStake(entityTraits->stake());

	GiWorldGeometryStake* stake = new GiWorldGeometryStake();
	stake->setGeometry(pLineSegmentGeometry);
	stake->setMaterial((GiMaterial*)pMaterial);
	GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->worldGeometryStakes.append(stake);

	DB_IMP_ENTITY(entityTraits->rootEntity()->m_pImpl)->entitys.append(pLineSegmentGeometry);
}
void GiWorldDrawGeometry::drawFace(const GiMaterial* pMaterial, const GiEntityTraits* entityTraits, const GePoint3dArray& verts) const
{

	// 获取模型变换
	int stackLen = GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.length();
	bool hasModelXform = stackLen > 0;
	bool hasEntityXform = entityTraits->isOpenMatrix3d();

	for (int i = 2; i < verts.length(); i++)
	{
		GiTriangleMeshGeometry* pTriangleMeshGeometry = new GiTriangleMeshGeometry();
		GePoint3d p1 = verts.at(i - 2);
		GePoint3d p2 = verts.at(i - 1);
		GePoint3d p3 = verts.at(i);

		// 应用模型变换
		if (hasModelXform) {
			const GeMatrix3d& modelXform = GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->modelTransformStack.at(stackLen - 1);
			p1.transformBy(modelXform);
			p2.transformBy(modelXform);
			p3.transformBy(modelXform);
		}

		// 应用实体变换
		if (hasEntityXform) {
			p1.transformBy(entityTraits->matrix3d());
			p2.transformBy(entityTraits->matrix3d());
			p3.transformBy(entityTraits->matrix3d());
		}

		pTriangleMeshGeometry->setVertex(0, p1);
		pTriangleMeshGeometry->setVertex(1, p2);
		pTriangleMeshGeometry->setVertex(2, p3);

		pTriangleMeshGeometry->setStake(entityTraits->stake());

		GiWorldGeometryStake* stake = new GiWorldGeometryStake();
		stake->setGeometry(pTriangleMeshGeometry);
		stake->setMaterial((GiMaterial*)pMaterial);
		GI_IMP_WORLDDRAWGEOMETRY(this->m_pImpl)->worldGeometryStakes.append(stake);

		DB_IMP_ENTITY(entityTraits->rootEntity()->m_pImpl)->entitys.append(pTriangleMeshGeometry);
	}
}

// 根据半径和弧度计算自适应分段数
static int computeSegments(double radius, double sweepAngle)
{
	// 最大弦高偏差 (世界坐标单位)
	double maxDeviation = 0.5;

	int segments;
	if (radius <= 0 || radius < maxDeviation) {
		segments = 8;
	}
	else {
		// 由弦高公式: d = r * (1 - cos(theta/2))
		// 解得每段角度: theta = 2 * acos(1 - d/r)
		double chordAngle = 2.0 * acos(1.0 - maxDeviation / radius);
		if (chordAngle < 0.001) {
			chordAngle = 0.001;
		}
		segments = (int)(abs(sweepAngle) / chordAngle + 0.5);
	}

	// 限制范围
	if (segments < 8) segments = 8;
	if (segments > 1024) segments = 1024;

	return segments;
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

		// 计算圆的分段数量 (自适应)
		unsigned int segment = computeSegments(radius, PI * 2);

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

		// 计算圆弧的分段数量 (自适应)
		int segment = computeSegments(radius, sweepAngle);

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

Adesk::Boolean GiWorldDrawGeometry::polygon(const Adesk::UInt32 nbPoints, const GePoint3d* pVertexList) const
{
	do
	{
		if (nbPoints < 3) {
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
		GiMaterial* pMaterial = ::acgiMaterialManager()->meshBasic(entityTraits->color(), entityTraits->transparency().alpha());

		// 构造面列表: 一个 n 边形面
		GePoint3dArray faceVertexs;
		for (Adesk::UInt32 i = 0; i < nbPoints; i++)
		{
			faceVertexs.append(pVertexList[i]);
		}

		// 使用三角扇进行绘制
		this->drawFace(pMaterial, entityTraits, faceVertexs);

	} while (false);

	return true;
}

bool GiWorldDrawGeometry::ellipArc(const GeEllipArc3d& ellipArc, const GiArcType arcType) const
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
		GiMaterial* pMaterial = ::acgiMaterialManager()->lineSegments(entityTraits->color(), 255, entityTraits->lineWeight() / 10.0);

		// 计算自适应分段数
		double majorRadius = ellipArc.majorRadius();
		double startAng = ellipArc.startAng();
		double endAng = ellipArc.endAng();
		double sweepAngle = endAng - startAng;
		if (sweepAngle <= 0) {
			sweepAngle += PI * 2;
		}

		int segment = computeSegments(majorRadius, sweepAngle);

		// 生成椭圆弧的线段
		GeVector3d majorAxis = ellipArc.majorAxis() * majorRadius;
		GeVector3d minorAxis = ellipArc.minorAxis() * ellipArc.minorRadius();
		GePoint3dArray vertexs = GeEllipArc3d::toLineSegment(ellipArc.center(), majorAxis, minorAxis, startAng, endAng, segment);

		// 绘制线段
		for (int i = 0; i < vertexs.length() - 1; i++)
		{
			this->drawLine(pMaterial, entityTraits, vertexs[i], vertexs[i + 1]);
		}

	} while (false);

	return true;
}

Adesk::Boolean GiWorldDrawGeometry::mesh(const Adesk::UInt32 rows, const Adesk::UInt32 columns,
	const GePoint3d* pVertexList,
	const GiEdgeData* pEdgeData,
	const GiFaceData* pFaceData,
	const GiVertexData* pVertexData) const
{
	do
	{
		if (rows < 2 || columns < 2) {
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
		GiMaterial* pMaterial = ::acgiMaterialManager()->meshBasic(entityTraits->color(), entityTraits->transparency().alpha());

		// 将 rows x columns 网格转换为三角面片
		for (Adesk::UInt32 r = 0; r < rows - 1; r++)
		{
			for (Adesk::UInt32 c = 0; c < columns - 1; c++)
			{
				// 四边形的4个顶点索引
				Adesk::UInt32 i0 = r * columns + c;
				Adesk::UInt32 i1 = r * columns + c + 1;
				Adesk::UInt32 i2 = (r + 1) * columns + c + 1;
				Adesk::UInt32 i3 = (r + 1) * columns + c;

				// 拆分为2个三角形绘制
				GePoint3dArray quad;
				quad.append(pVertexList[i0]);
				quad.append(pVertexList[i1]);
				quad.append(pVertexList[i2]);
				quad.append(pVertexList[i3]);
				this->drawFace(pMaterial, entityTraits, quad);
			}
		}

	} while (false);

	return true;
}

void GiWorldDrawGeometry::draw(const GiDrawable* pDrawable) const
{
	if (pDrawable == NULL) {
		return;
	}

	// 获取当前绘制上下文的GiWorldDraw
	GiEntityTraits* entityTraits = (GiEntityTraits*)this->getEntityTraits();
	if (entityTraits == NULL || entityTraits->document() == NULL) {
		return;
	}

	// 获取绘图器
	GiWorldDraw* pWorldDraw = ::kernel()->acgiWorldDrawManager()->worldDraw(entityTraits->document());
	if (pWorldDraw == NULL) {
		return;
	}

	// 委托给drawable的worldDraw
	const_cast<GiDrawable*>(pDrawable)->worldDraw(pWorldDraw);
}