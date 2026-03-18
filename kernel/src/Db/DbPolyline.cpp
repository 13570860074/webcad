#include "DbPolyline.h"
#include "GePlane.h"
#include "GeMatrix3d.h"
#include "GeLineSeg2d.h"
#include "GeLineSeg3d.h"
#include "GeCircArc2d.h"
#include "GeCircArc3d.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "DbOsnapPointCompute.h"
#include "DbImpl.h"

// 多段线顶点和各参数的关系
/*
	1. 段数:定点数减1,如果是闭合的,则段数等于定点数
	2. 顶点数:作为基数
	3. 凸度:和段数一致,一段的凸度是bulge[index]
	4. 起始宽度:和段数一致,一段的起始宽度是startWidths[index - 1]
	5. 结束宽度:和段数一致,一段的终止宽度是endWidths[index - 1]
*/


DbPolyline::DbPolyline()
{
	this->m_pImpl = new DbPolylineImpl();
}
DbPolyline::~DbPolyline()
{
}

Acad::ErrorStatus DbPolyline::dwgInFields(DbDwgFiler* pFiler)
{

	Acad::ErrorStatus es = DbCurve::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	// 删除顶点和段数据
	DB_IMP_POLYLINE(this->m_pImpl)->vertexs.removeAll();
	realloc_arry_all(DB_IMP_POLYLINE(this->m_pImpl)->segments);

	// 读取顶点
	unsigned int numVert = 0;
	pFiler->readUInt32(&numVert);
	for (int i = 0; i < numVert; i++) {
		GePoint2d vert;
		pFiler->readPoint2d(&vert);
		DB_IMP_POLYLINE(this->m_pImpl)->vertexs.append(vert);
	}

	// 读取段
	unsigned int numSegments = this->numVerts();
	pFiler->readUInt32(&numSegments);
	for (int i = 0; i < numSegments; i++)
	{
		double bulge = 0.0;
		pFiler->readDouble(&bulge);

		double startWidth = 0.0;
		pFiler->readDouble(&startWidth);

		double endWidth = 0.0;
		pFiler->readDouble(&endWidth);

		int vertexIdentifier = 0;
		pFiler->readInt32(&vertexIdentifier);

		DbPolylineSegImpl* seg = new DbPolylineSegImpl();
		seg->bulge = bulge;
		seg->startWidth = startWidth;
		seg->endWidth = endWidth;
		seg->vertexIdentifier = vertexIdentifier;
		DB_IMP_POLYLINE(this->m_pImpl)->segments.append(seg);
	}
	for (int i = numSegments; i < numVert; i++) {
		DbPolylineSegImpl* seg = new DbPolylineSegImpl();
		seg->bulge = 0.0;
		seg->startWidth = 0.0;
		seg->endWidth = 0.0;
		seg->vertexIdentifier = 0;
		DB_IMP_POLYLINE(this->m_pImpl)->segments.append(seg);
	}

	double elevation;
	pFiler->readDouble(&elevation);
	this->setElevation(elevation);

	double thickness;
	pFiler->readDouble(&thickness);
	this->setThickness(thickness);

	bool isClosed;
	pFiler->readBool(&isClosed);
	this->setClosed(isClosed);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::dwgOutFields(DbDwgFiler* pFiler) const
{

	Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	// 写入顶点
	pFiler->writeUInt32(this->numVerts());
	for (int i = 0; i < this->numVerts(); i++) {
		pFiler->writePoint2d(DB_IMP_POLYLINE(this->m_pImpl)->vertexs[i]);
	}

	// 写入段
	pFiler->writeUInt32(DB_IMP_POLYLINE(this->m_pImpl)->numSegments());
	for (int i = 0; i < DB_IMP_POLYLINE(this->m_pImpl)->numSegments(); i++) {
		pFiler->writeDouble(DB_IMP_POLYLINE(this->m_pImpl)->segments[i]->bulge);
		pFiler->writeDouble(DB_IMP_POLYLINE(this->m_pImpl)->segments[i]->startWidth);
		pFiler->writeDouble(DB_IMP_POLYLINE(this->m_pImpl)->segments[i]->endWidth);
		pFiler->writeInt32(DB_IMP_POLYLINE(this->m_pImpl)->segments[i]->vertexIdentifier);
	}

	pFiler->writeDouble(this->elevation());
	pFiler->writeDouble(this->thickness());
	pFiler->writeBool(this->isClosed());

	return Acad::ErrorStatus::eOk;
}
bool DbPolyline::subWorldDraw(GiWorldDraw* pWd) const
{
	if (this->numVerts() < 2)
	{
		return false;
	}

	GeMatrix3d mat;
	mat.setToPlaneToWorld(this->normal());

	AcArray<DbPolylineSegImpl*> segments = DB_IMP_POLYLINE(this->m_pImpl)->segments;

	for (int i = 0; i < this->numVerts(); i++) {

		// 判断最后一段是否关闭,如果关闭则跳过
		if (this->isClosed() == false && i == this->numVerts() - 1) {
			continue;
		}

		// 获得顶点,宽度,凸度数据
		GePoint2d vertex1;
		GePoint2d vertex2;
		this->getPointAt(i, vertex1);
		if (this->isClosed() == true && i == this->numVerts() - 1) {
			this->getPointAt(0, vertex2);
		}
		else {
			this->getPointAt(i + 1, vertex2);
		}
		double startWidth = 0;
		double endWidth = 0;
		this->getWidthsAt(i, startWidth, endWidth);
		double bulge = 0.0;
		this->getBulgeAt(i, bulge);

		// 获得三维起始点,端点,方向
		GePoint3d startPoint;
		startPoint = GePoint3d(vertex1.x, vertex1.y, 0.0);
		startPoint.transformBy(mat);
		startPoint += (this->normal() * this->elevation());
		GePoint3d endPoint;
		endPoint = GePoint3d(vertex2.x, vertex2.y, 0.0);
		endPoint.transformBy(mat);
		endPoint += (this->normal() * this->elevation());
		GeVector3d direction = endPoint - startPoint;
		direction.rotateBy(PI / 2.0, this->normal());
		direction.normalize();

		if (startWidth < 0.0002) {
			startWidth = 0.0;
		}
		if (endWidth < 0.0002) {
			endWidth = 0.0;
		}

		DbPolyline::SegType segType = this->segType(i);
		if (segType == DbPolyline::SegType::kLine) {
			if (abs(startWidth) < 0.0002 && abs(endWidth) < 0.0002)
			{
				pWd->geometry().line(startPoint, endPoint);
			}
			else if (startWidth > 0.0002 && endWidth > 0.0002)
			{

				Adesk::UInt32 nbVertex = 4;
				GePoint3d* pVertexList = new GePoint3d[nbVertex];
				Adesk::UInt32 faceListSize = 8;
				Adesk::Int32* pFaceList = new Adesk::Int32[faceListSize];

				GePoint3d pt = startPoint;
				pt += (direction * startWidth / 2.0);
				pVertexList[0] = pt;

				pt = startPoint;
				pt += (direction * (0.0 - startWidth) / 2.0);
				pVertexList[1] = pt;

				pt = endPoint;
				pt += (direction * endWidth / 2.0);
				pVertexList[2] = pt;

				pt = endPoint;
				pt += (direction * (0.0 - endWidth) / 2.0);
				pVertexList[3] = pt;

				pFaceList[0] = 3;
				pFaceList[1] = 0;
				pFaceList[2] = 2;
				pFaceList[3] = 3;
				pFaceList[4] = 3;
				pFaceList[5] = 3;
				pFaceList[6] = 1;
				pFaceList[7] = 0;
				pWd->geometry().shell(nbVertex, pVertexList, faceListSize, pFaceList);
			}
			else if (abs(startWidth) < 0.0002 && endWidth > 0.0002)
			{

				Adesk::UInt32 nbVertex = 3;
				GePoint3d* pVertexList = new GePoint3d[nbVertex];
				Adesk::UInt32 faceListSize = 4;
				Adesk::Int32* pFaceList = new Adesk::Int32[faceListSize];

				GePoint3d pt = startPoint;
				pVertexList[0] = pt;

				pt = endPoint;
				pt += (direction * endWidth / 2.0);
				pVertexList[1] = pt;

				pt = endPoint;
				pt += (direction * (0.0 - endWidth) / 2.0);
				pVertexList[2] = pt;

				pFaceList[0] = 3;
				pFaceList[1] = 0;
				pFaceList[2] = 1;
				pFaceList[3] = 2;
				pWd->geometry().shell(nbVertex, pVertexList, faceListSize, pFaceList);
			}
			else if (startWidth > 0.0002 && abs(endWidth) < 0.0002)
			{

				Adesk::UInt32 nbVertex = 3;
				GePoint3d* pVertexList = new GePoint3d[nbVertex];
				Adesk::UInt32 faceListSize = 4;
				Adesk::Int32* pFaceList = new Adesk::Int32[faceListSize];

				GePoint3d pt = startPoint;
				pt += (direction * startWidth / 2.0);
				pVertexList[0] = pt;

				pt = startPoint;
				pt += (direction * (0.0 - startWidth) / 2.0);
				pVertexList[1] = pt;

				pt = endPoint;
				pVertexList[2] = pt;

				pFaceList[0] = 3;
				pFaceList[1] = 0;
				pFaceList[2] = 1;
				pFaceList[3] = 2;
				pWd->geometry().shell(nbVertex, pVertexList, faceListSize, pFaceList);
			}
		}
		else if (segType == DbPolyline::SegType::kArc) {
			if (abs(startWidth) < 0.0002 && abs(endWidth) < 0.0002) {
				GePoint3d midPoint = startPoint + (endPoint - startPoint) / 2;
				double dist = startPoint.distanceTo(midPoint);
				double angle = atan(bulge) * 4;
				double adjacent = dist / tan(angle / 2.0);

				// 获得参照
				GeVector3d refVec = startPoint - midPoint;
				refVec.normalize();
				refVec.rotateBy(0 - PI / 2.0, this->normal());

				// 获得圆心
				GePoint3d center = midPoint;
				center += (refVec * adjacent);

				// 获得半径
				double radius = center.distanceTo(startPoint);

				GeVector3d startVector;
				startVector = startPoint - center;
				startVector.normalize();
				pWd->geometry().circularArc(center, radius, this->normal(), startVector, angle);
			}
		}
		else {

		}
	}

	return true;
}
Acad::ErrorStatus DbPolyline::getPointAt(unsigned int index, GePoint3d& pt) const {
	if (index >= this->numVerts()) {
		return Acad::ErrorStatus::eFail;
	}
	this->getPointAtParam(double(index), pt);
	return Acad::ErrorStatus::eOk;
}

DbPolyline::SegType DbPolyline::segType(unsigned int index) const
{
	if (index >= DB_IMP_POLYLINE(this->m_pImpl)->numSegments()) {
		return DbPolyline::SegType::kEmpty;
	}

	if (DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index].isEqualTo(DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index + 1]) == true) {
		return DbPolyline::SegType::kCoincident;
	}

	if (abs(DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->bulge) < 0.0002) {
		return DbPolyline::SegType::kLine;
	}
	return DbPolyline::SegType::kArc;
}
Acad::ErrorStatus DbPolyline::getLineSegAt(unsigned int index, GeLineSeg2d& ln) const
{
	if (this->segType(index) == DbPolyline::SegType::kLine) {
		if (index == this->numVerts() - 1) {
			ln.set(DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index], DB_IMP_POLYLINE(this->m_pImpl)->vertexs[0]);
		}
		else {
			ln.set(DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index], DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index + 1]);
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::getLineSegAt(unsigned int index, GeLineSeg3d& ln) const
{
	GeLineSeg2d line2d;
	if (this->getLineSegAt(index, line2d) != Acad::ErrorStatus::eOk) {
		return Acad::ErrorStatus::eFail;
	}

	GePoint3d p1 = GePoint3d(line2d.startPoint().x, line2d.startPoint().y, 0.0);
	GePoint3d p2 = GePoint3d(line2d.endPoint().x, line2d.endPoint().y, 0.0);
	GeMatrix3d mat;
	this->getEcs(mat);
	p1.transformBy(mat);
	p2.transformBy(mat);
	ln.set(p1, p2);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getArcSegAt(unsigned int index, GeCircArc2d& arc) const
{
	if (this->segType(index) == DbPolyline::SegType::kArc) {

		if (index == this->numVerts() - 1) {
			arc.set(DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index], DB_IMP_POLYLINE(this->m_pImpl)->vertexs[0], DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->bulge);
		}
		else {
			arc.set(DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index], DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index + 1], DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->bulge);
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::getArcSegAt(unsigned int index, GeCircArc3d& arc) const
{
	if (this->segType(index) == DbPolyline::SegType::kArc) {

		GePoint3d p1;
		GePoint3d p2;
		this->getPointAt(index, p1);
		this->getPointAt(index + 1, p2);

		arc.set(p1, p2, DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->bulge);
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

void DbPolyline::setClosed(Adesk::Boolean _isClosed)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_POLYLINE(this->m_pImpl)->isClosed = _isClosed;
	}
}
void DbPolyline::setElevation(double _elevation)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_POLYLINE(this->m_pImpl)->elevation = _elevation;
	}
}
Acad::ErrorStatus DbPolyline::setThickness(double _thickness)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_POLYLINE(this->m_pImpl)->thickness = _thickness;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::setConstantWidth(double _width)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		for (int i = 0; i < DB_IMP_POLYLINE(this->m_pImpl)->segments.length(); i++) {
			DB_IMP_POLYLINE(this->m_pImpl)->segments[i]->startWidth = _width;
			DB_IMP_POLYLINE(this->m_pImpl)->segments[i]->endWidth = _width;
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::setNormal(const GeVector3d& _normal)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_POLYLINE(this->m_pImpl)->normal = _normal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Adesk::Boolean DbPolyline::isOnlyLines() const
{
	Adesk::Boolean onlyLines = true;

	for (int i = 0; i < DB_IMP_POLYLINE(this->m_pImpl)->segments.length(); i++) {
		if (abs(DB_IMP_POLYLINE(this->m_pImpl)->segments[i]->bulge) > 0.0002) {
			onlyLines = false;
			break;
		}
	}
	if (onlyLines == false) {
		return onlyLines;
	}

	return onlyLines;
}
double DbPolyline::elevation() const
{
	return DB_IMP_POLYLINE(this->m_pImpl)->elevation;
}
double DbPolyline::thickness() const
{
	return DB_IMP_POLYLINE(this->m_pImpl)->thickness;
}
Acad::ErrorStatus DbPolyline::getConstantWidth(double& _width) const
{

	if (this->numVerts() < 1)
	{
		return Acad::ErrorStatus::eFail;
	}

	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	for (int i = 0; i < DB_IMP_POLYLINE(this->m_pImpl)->numSegments(); i++)
	{
		if (i == 0) {
			_width = DB_IMP_POLYLINE(this->m_pImpl)->segments[i]->startWidth;
		}
		if (abs(_width - DB_IMP_POLYLINE(this->m_pImpl)->segments[i]->startWidth) > 0.002) {
			es = Acad::ErrorStatus::eFail;
			break;
		}
		if (abs(_width - DB_IMP_POLYLINE(this->m_pImpl)->segments[i]->endWidth) > 0.002) {
			es = Acad::ErrorStatus::eFail;
			break;
		}
	}

	return es;
}
GeVector3d DbPolyline::normal() const
{
	return DB_IMP_POLYLINE(this->m_pImpl)->normal;
}

Acad::ErrorStatus DbPolyline::addVertexAt(unsigned int index, const GePoint2d& pt, double bulge, double startWidth, double endWidth, Adesk::Int32 vertexIdentifier)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		if (startWidth < 0.0) {
			startWidth = 0.0;
		}
		if (endWidth < 0.0) {
			endWidth = 0.0;
		}

		// 添加顶点
		if (index >= DB_IMP_POLYLINE(this->m_pImpl)->vertexs.length() - 1) {
			DB_IMP_POLYLINE(this->m_pImpl)->vertexs.append(pt);
		}
		else {
			DB_IMP_POLYLINE(this->m_pImpl)->vertexs.insert(index, pt);
		}

		if (index >= DB_IMP_POLYLINE(this->m_pImpl)->vertexs.length() - 1) {
			DbPolylineSegImpl* seg = new DbPolylineSegImpl();
			seg->bulge = bulge;
			seg->startWidth = startWidth;
			seg->endWidth = endWidth;
			seg->vertexIdentifier = vertexIdentifier;
			DB_IMP_POLYLINE(this->m_pImpl)->segments.append(seg);
		}
		else {
			DbPolylineSegImpl* seg = new DbPolylineSegImpl();
			seg->bulge = bulge;
			seg->startWidth = startWidth;
			seg->endWidth = endWidth;
			seg->vertexIdentifier = vertexIdentifier;
			DB_IMP_POLYLINE(this->m_pImpl)->segments.insert(index, seg);
		}

		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbPolyline::removeVertexAt(unsigned int index)
{
	if (index >= DB_IMP_POLYLINE(this->m_pImpl)->vertexs.length()) {
		return Acad::ErrorStatus::eFail;
	}

	// 移除顶点
	DB_IMP_POLYLINE(this->m_pImpl)->vertexs.removeAt(index);
	realloc_arry_at(DB_IMP_POLYLINE(this->m_pImpl)->segments, index);

	return Acad::ErrorStatus::eOk;
}

unsigned int DbPolyline::numVerts() const
{
	return DB_IMP_POLYLINE(this->m_pImpl)->vertexs.length();
}

Acad::ErrorStatus DbPolyline::getPointAt(unsigned int index, GePoint2d& pt) const
{
	if (index >= this->numVerts()) {
		return Acad::ErrorStatus::eFail;
	}

	pt = DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index];
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getBulgeAt(unsigned int index, double& bulge) const
{
	if (index >= DB_IMP_POLYLINE(this->m_pImpl)->segments.length()) {
		return Acad::ErrorStatus::eFail;
	}

	bulge = DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->bulge;

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getVertexIdentifierAt(unsigned int index, Adesk::Int32& vertexIdentifier) const
{
	if (index >= DB_IMP_POLYLINE(this->m_pImpl)->segments.length()) {
		return Acad::ErrorStatus::eFail;
	}

	vertexIdentifier = DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->vertexIdentifier;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getWidthsAt(unsigned int index, double& startWidth, double& endWidth) const
{
	if (index >= DB_IMP_POLYLINE(this->m_pImpl)->segments.length()) {
		return Acad::ErrorStatus::eFail;
	}

	startWidth = DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->startWidth;
	endWidth = DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->endWidth;
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbPolyline::setPointAt(unsigned int index, const GePoint2d& pt)
{
	if (index > this->numVerts() - 1) {
		return Acad::ErrorStatus::eFail;
	}

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index].set(pt.x, pt.y);
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::setBulgeAt(unsigned int index, double bulge)
{
	if (index >= DB_IMP_POLYLINE(this->m_pImpl)->segments.length()) {
		return Acad::ErrorStatus::eFail;
	}

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->bulge = bulge;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::setVertexIdentifierAt(unsigned int index, Adesk::Int32 suggestedValue)
{
	if (index >= DB_IMP_POLYLINE(this->m_pImpl)->segments.length()) {
		return Acad::ErrorStatus::eFail;
	}

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->vertexIdentifier = suggestedValue;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::setWidthsAt(unsigned int index, double startWidth, double endWidth)
{
	if (index >= DB_IMP_POLYLINE(this->m_pImpl)->segments.length()) {
		return Acad::ErrorStatus::eFail;
	}

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {

		DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->startWidth = startWidth;
		DB_IMP_POLYLINE(this->m_pImpl)->segments[index]->endWidth = endWidth;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

void DbPolyline::getEcs(GeMatrix3d& retVal) const
{
	retVal.setToPlaneToWorld(this->normal());
}

Acad::ErrorStatus DbPolyline::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::subTransformBy(const GeMatrix3d& xform)
{

	GeMatrix3d workMat;
	this->getEcs(workMat);

	// 获得当前三维顶点
	GePoint3dArray verts;
	for (int i = 0; i < this->numVerts(); i++)
	{
		GePoint2d pt;
		this->getPointAt(i, pt);

		GePoint3d vert(pt.x, pt.y, 0.0);
		vert.transformBy(workMat);
		verts.append(vert);
	}

	// 得到变幻后的顶点
	GePoint3dArray transformVerts;
	for (int i = 0; i < verts.length(); i++)
	{
		GePoint3d pt = verts[i];
		pt.transformBy(xform);
		transformVerts.append(pt);
	}

	// 得到变换后的法向量
	GeVector3d normal = this->normal();
	GePoint3dArray normalVerts;
	for (int i = 0; i < transformVerts.length(); i++)
	{
		if (i == 0)
		{
			normalVerts.append(transformVerts[i]);
			continue;
		}

		bool isFind = false;
		for (int u = 0; u < normalVerts.length(); u++)
		{
			if (normalVerts[u].isEqualTo(transformVerts[i]) == true)
			{
				isFind = true;
				break;
			}
		}
		if (isFind == false)
		{
			normalVerts.append(transformVerts[i]);
		}
		if (normalVerts.length() >= 3)
		{
			break;
		}
	}
	if (transformVerts.length() >= 3)
	{
		GeVector3d vec1 = normalVerts[0] - normalVerts[1];
		GeVector3d vec2 = normalVerts[2] - normalVerts[1];
		normal = vec1.crossProduct(vec2);
		normal.normalize();
	}

	// 获得平面到世界坐标系的矩阵(用于将三维转二维)
	GeMatrix3d mat;
	mat.setToWorldToPlane(normal);

	// 重新设置顶点和法线
	for (int i = 0; i < transformVerts.length(); i++)
	{
		GePoint3d pos = transformVerts[i];
		pos.transformBy(mat);
		this->setPointAt(i, GePoint2d(pos.x, pos.y));
		this->setElevation(pos.z);
	}
	this->setNormal(normal);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const
{
	DB_IMP_POLYLINE(this->m_pImpl)->isInitPoint = false;
	for (int i = 0; i < this->numVerts(); i++)
	{
		GePoint3d vert;
		this->getPointAt(i, vert);

		DbGripData* gripData = new DbGripData();
		gripData->setGripPoint(vert);
		grips.append(gripData);
	}
	for (int i = 1; i < this->numVerts(); i++)
	{

		if (this->segType(i - 1) == DbPolyline::SegType::kLine)
		{
			GePoint3d vert1, vert2;
			this->getPointAt(i - 1, vert1);
			this->getPointAt(i, vert2);

			GePoint3d midPoint = vert1 + (vert2 - vert1) / 2.0;
			DbGripData* gripData = new DbGripData();
			gripData->setGripMode(DbGripData::DbGripMode::kGripModePolylineMid);
			gripData->setGripPoint(midPoint);
			gripData->setDirection((vert2 - vert1).normal());
			grips.append(gripData);
		}
		if (this->segType(i - 1) == DbPolyline::SegType::kArc)
		{

			GeCircArc3d circArc;
			this->getArcSegAt(i - 1, circArc);
			GePoint3d ps = circArc.evalPoint(circArc.startAng() + (circArc.endAng() - circArc.startAng()) / 2.0);

			GePoint3d vert1, vert2;
			this->getPointAt(i - 1, vert1);
			this->getPointAt(i, vert2);

			DbGripData* gripData = new DbGripData();
			gripData->setGripMode(DbGripData::DbGripMode::kGripModePolylineMid);
			gripData->setGripPoint(GePoint3d(ps.x, ps.y, 0));
			gripData->setDirection((vert2 - vert1).normal());
			grips.append(gripData);
		}
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbPolyline::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const
{

	if (osnapMode == Db::OsnapMode::kOsModeNear)
	{
		GePoint3dArray points = ::osnap_near_to_polyline(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeMid)
	{
		GePoint3dArray points = ::osnap_mid_to_polyline(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeCen)
	{
		GePoint3dArray points = ::osnap_center_to_polyline(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeQuad)
	{
		GePoint3dArray points = ::osnap_quad_to_polyline(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeEnd)
	{
		GePoint3dArray points = ::osnap_end_to_polyline(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModePerp)
	{
		GePoint3dArray points = ::osnap_perp_to_polyline(pickPoint, lastPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeTan)
	{
		GePoint3dArray points = ::osnap_tan_to_polyline(pickPoint, lastPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbPolyline::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset)
{
	GeMatrix3d mat;
	mat.setToPlaneToWorld(this->normal());

	for (int i = 0; i < indices.length(); i++)
	{
		if (indices.at(i) < this->numVerts())
		{
			GePoint3d vertex;
			this->getPointAt(indices.at(i), vertex);
			vertex += offset;
			vertex.transformBy(mat);
			this->setPointAt(indices.at(i), GePoint2d(vertex.x, vertex.y));
		}
		else
		{
			int index = indices.at(i) - this->numVerts();
			if (this->segType(index) == DbPolyline::SegType::kLine)
			{
				GePoint3d vert1, vert2;
				this->getPointAt(index, vert1);
				this->getPointAt(index + 1, vert2);

				vert1 += offset;
				vert1.transformBy(mat);
				this->setPointAt(index, GePoint2d(vert1.x, vert1.y));

				vert2 += offset;
				vert2.transformBy(mat);
				this->setPointAt(index + 1, GePoint2d(vert2.x, vert2.y));
			}
			else if (this->segType(index) == DbPolyline::SegType::kArc)
			{

				GePoint3d vert1, vert2;
				this->getPointAt(index, vert1);
				this->getPointAt(index + 1, vert2);
				double bulge = 0.0;
				this->getBulgeAt(index, bulge);

				GePoint3d midPoint = vert1 + (vert2 - vert1) / 2.0;
				double dist = vert1.distanceTo(midPoint);
				double angle = atan(bulge) * 4;
				double adjacent = dist / tan(angle / 2.0);
				GeVector3d refVec = vert1 - midPoint;
				refVec.normalize();
				refVec.rotateBy(0 - PI / 2.0, this->normal());
				GePoint3d center = midPoint;
				center += (refVec * adjacent);
				if (bulge < 0)
				{
					midPoint = center + (refVec * center.distanceTo(vert1));
				}
				else
				{
					midPoint = center + (refVec * (0 - center.distanceTo(vert2)));
				}

				if (DB_IMP_POLYLINE(this->m_pImpl)->isInitPoint == false)
				{
					DB_IMP_POLYLINE(this->m_pImpl)->initPoint = midPoint;
					DB_IMP_POLYLINE(this->m_pImpl)->isInitPoint = true;
				}
				DB_IMP_POLYLINE(this->m_pImpl)->initPoint += offset;
				midPoint = DB_IMP_POLYLINE(this->m_pImpl)->initPoint;

				refVec = vert1 - center;
				refVec.normalize();
				angle = refVec.angleToCCW((vert2 - center).normal(), this->normal());

				bulge = tan(angle / 4.0);

				// GeCircArc3d::pointToBulge(vert1, midPoint, vert2, this->normal(), bulge);
				this->setBulgeAt(index, bulge);
			}
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const
{

	if (pEnt == NULL)
	{
		return Acad::ErrorStatus::eFail;
	}

	if (DB_IMP_ENTITY(pEnt->m_pImpl)->isLine())
	{
		GePoint3dArray ps = ::osnap_inters_polyline_to_lintseg(this, (DbLine*)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isXline())
	{
		GePoint3dArray ps = ::osnap_inters_polyline_to_line(this, (DbXline*)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isRay())
	{
		GePoint3dArray ps = ::osnap_inters_polyline_to_ray(this, (DbRay*)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isCircle())
	{
		GePoint3dArray ps = ::osnap_inters_polyline_to_circle(this, (DbCircle*)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isArc())
	{
		GePoint3dArray ps = ::osnap_inters_polyline_to_arc(this, (DbArc*)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isPolyline())
	{
		GePoint3dArray ps = ::osnap_inters_polyline_to_polyline(this, (DbPolyline*)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isBlockReference())
	{

	}

	return Acad::ErrorStatus::eOk;
}

bool DbPolyline::isClosed() const
{
	return DB_IMP_POLYLINE(this->m_pImpl)->isClosed;
}
bool DbPolyline::isPeriodic() const
{
	return false;
}
bool DbPolyline::isPlanar() const
{
	return true;
}
Acad::ErrorStatus DbPolyline::getPlane(GePlane& plane, Db::Planarity& planarity) const
{
	GePoint3d startPoint;
	this->getStartPoint(startPoint);
	plane = GePlane(startPoint, this->normal());
	planarity = Db::Planarity::kPlanar;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getStartParam(double& _v) const
{
	_v = 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getEndParam(double& _v) const
{
	_v = double(this->numVerts());
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getStartPoint(GePoint3d& _v) const
{
	double startParam = 0.0;
	this->getStartParam(startParam);
	this->getPointAtParam(startParam, _v);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getEndPoint(GePoint3d& _v) const
{
	double endParam = 0.0;
	this->getEndParam(endParam);
	this->getPointAtParam(endParam, _v);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getPointAtParam(double param, GePoint3d& pt) const
{
	double startParam = 0.0;
	double endParam = 0.0;
	this->getStartParam(startParam);
	this->getEndParam(endParam);
	if (param < startParam || param > endParam)
	{
		return Acad::ErrorStatus::eFail;
	}

	GeMatrix3d mat;
	mat.setToPlaneToWorld(this->normal());

	// 参数是整数的情况
	if (abs(int(param) - param) < 0.00001)
	{
		int index = int(param);
		pt.set(DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index].x, DB_IMP_POLYLINE(this->m_pImpl)->vertexs[index].y, 0);
		pt.transformBy(mat);
		pt += (this->normal() * this->elevation());
		return Acad::ErrorStatus::eOk;
	}

	int index = int(param);
	GePoint2d vertex1, vertex2;
	this->getPointAt(index, vertex1);
	this->getPointAt(index + 1, vertex2);
	if (this->segType(index) == DbPolyline::SegType::kLine)
	{
		GeVector2d direction = vertex2 - vertex1;
		GePoint2d vertex = vertex1 + direction * (param - int(param));

		pt = GePoint3d(vertex.x, vertex.y, 0.0);
		pt.transformBy(mat);
		pt += (this->normal() * this->elevation());
	}
	else if (this->segType(index) == DbPolyline::SegType::kArc)
	{

		double bulge = 0.0;
		this->getBulgeAt(index, bulge);
		GeCircArc2d circArc;
		circArc.set(vertex1, vertex2, bulge);
		// circArc.evalPoint();
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getParamAtPoint(const GePoint3d& point, double& param) const
{

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getDistAtParam(double param, double& dist) const
{
	dist = abs(param);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getParamAtDist(double dist, double& param) const
{
	param = dist;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getDistAtPoint(const GePoint3d& point, double& dist) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getPointAtDist(double dist, GePoint3d& point) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getFirstDeriv(double, GeVector3d&) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::getFirstDeriv(const GePoint3d&, GeVector3d&) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::getSecondDeriv(double, GeVector3d&) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::getSecondDeriv(const GePoint3d&, GeVector3d&) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::getClosestPointTo(const GePoint3d& point, GePoint3d& closest, bool) const
{
	return this->getClosestPointTo(point, this->normal(), closest, false);
}
Acad::ErrorStatus DbPolyline::getClosestPointTo(const GePoint3d& point, const GeVector3d& projectDirection, GePoint3d& closest, bool) const
{
	// 将输入点转换成二维点
	GePoint2d inputPoint;
	GeMatrix3d mat;
	mat.setToWorldToPlane(projectDirection);
	GePoint3d pt = point;
	pt.transformBy(mat);
	inputPoint.x = pt.x;
	inputPoint.y = pt.y;

	// 求最近点
	bool isClosest = false;
	GePoint2d closestPoint;
	for (int i = 0; i < this->numVerts(); i++)
	{
		DbPolyline::SegType segType = this->segType(i);
		if (segType == DbPolyline::SegType::kLine)
		{
			GeLineSeg2d lineSeg;
			this->getLineSegAt(i, lineSeg);
			GePoint2d temp = lineSeg.closestPointTo(inputPoint);
			if (isClosest == false)
			{
				closestPoint = temp;
				isClosest = true;
			}
			else if (temp.distanceTo(inputPoint) < closestPoint.distanceTo(inputPoint))
			{
				closestPoint = temp;
			}
		}
		else if (segType == DbPolyline::SegType::kPoint)
		{
			GeCircArc2d circArc;
			this->getArcSegAt(i, circArc);
			GePoint2d temp = circArc.closestPointTo(inputPoint);
			if (isClosest == false)
			{
				closestPoint = temp;
				isClosest = true;
			}
			else if (temp.distanceTo(inputPoint) < closestPoint.distanceTo(inputPoint))
			{
				closestPoint = temp;
			}
		}
		else if (segType == DbPolyline::kPoint)
		{
			GePoint2d temp;
			this->getPointAt(i, temp);
			if (isClosest == false)
			{
				closestPoint = temp;
				isClosest = true;
			}
			else if (temp.distanceTo(inputPoint) < closestPoint.distanceTo(inputPoint))
			{
				closestPoint = temp;
			}
		}
	}

	// 将最近点转换成三维点
	mat.setToPlaneToWorld(projectDirection);
	closest = GePoint3d(closestPoint.x, closestPoint.y, 0.0);
	closest.transformBy(mat);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getOrthoProjectedCurve(const GePlane& plane, DbCurve*& curve) const
{
	return this->getProjectedCurve(plane, plane.normal(), curve);
}
Acad::ErrorStatus DbPolyline::getProjectedCurve(const GePlane& plane, const GeVector3d& normal, DbCurve*& curve) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::getOffsetCurves(double v, DbVoidPtrArray& curves) const
{

	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::getSpline(DbSpline*&) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::getSplitCurves(const GeDoubleArray&, DbVoidPtrArray& curveSegments) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::getSplitCurves(const GePoint3dArray&, DbVoidPtrArray&) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::extend(double)
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::extend(bool, const GePoint3d&)
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::getArea(double& area) const
{
	area = 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::reverseCurve()
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbPolyline::getGeCurve(GeCurve3d*& pGeCurve, const GeTol& tol) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPolyline::setFromGeCurve(const GeCurve3d& geCurve, GeVector3d* normal, const GeTol& tol)
{
	return Acad::ErrorStatus::eFail;
}