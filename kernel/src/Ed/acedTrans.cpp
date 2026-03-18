#include "acedTrans.h"
#include "DbLayout.h"
#include "DbViewport.h"
#include "DbExtents2d.h"
#include "DbObjectManager.h"
#include "DbBlockTableRecord.h"
#include "DbViewportTableRecord.h"
#include "EdUserInputManager.h"
#include "GePlane.h"
#include "GeRay3d.h"
#include "GeLine3d.h"
#include "kernel.h"
#include "aced.h"
#include "DbImpl.h"


GePoint2d trans_client_to_ncs(unsigned int clientX, unsigned int clientY) {

	double x = clientX;
	double y = clientY;

	x = (x - 0) / ::kernel()->acedUserInputManager()->rangWidth();
	y = (y - 0) / ::kernel()->acedUserInputManager()->rangHeight();

	y = 1.0 - y;
	return GePoint2d(x, y);
}


/**轴角转矩阵*/
GeMatrix3d angleOnAxisToMatrix(const GeVector3d &axis, double angle)
{
	GeMatrix3d mat;

	// Z轴对齐
	GeVector3d refXAxis = GeVector3d::kXAxis;
	GeVector3d refYAxis = GeVector3d::kYAxis;
	if (GeVector3d::kZAxis.isEqualTo(axis) == false)
	{
		// Z轴和输入轴叉乘作为参照x轴
		refXAxis = axis.crossProduct(GeVector3d::kZAxis);

		// 参照x轴和输入轴叉乘作为参照y轴
		refYAxis = axis.crossProduct(refXAxis);

		mat.setToAlignCoordSys(
			GePoint3d::kOrigin, GeVector3d::kXAxis, GeVector3d::kYAxis, GeVector3d::kZAxis,
			GePoint3d::kOrigin, refXAxis, refYAxis, axis);
	}

	// 计算参照x轴的旋转矩阵
	GeMatrix3d tempMat;
	tempMat.setToRotation(angle, axis);
	mat *= tempMat;

	return mat;
}

/**矩阵转轴角*/
void matrixToAngleOnAxis(const GeMatrix3d &mat, GeVector3d &axis, double &angle)
{
	// 获得参照z轴
	GeVector3d refZAxis = GeVector3d::kZAxis;
	refZAxis.transformBy(mat);
	axis = refZAxis;

	// 获得参数x轴
	GeVector3d refXAxis = GeVector3d::kXAxis;
	if (refZAxis.isEqualTo(GeVector3d::kZAxis) == false)
	{
		refXAxis = refZAxis.crossProduct(GeVector3d::kZAxis);
	}

	// 获得参照x轴
	GeVector3d xAxis = GeVector3d::kXAxis;
	xAxis.transformBy(mat);

	// 获得角度
	angle = refXAxis.angleToCCW(xAxis, refZAxis);
}

/** 获得视口的ucs矩阵
 * 1.wcs坐标乘矩阵为ucs坐标
 */
GeMatrix3d trans_ucs_mat(DbObjectId _viewportId)
{

	// 获得当前视口
	if (_viewportId == NULL)
	{
		_viewportId = acedActiveViewportId();
	}

	// 打开视口对象
	DbObject *viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport == NULL)
	{
		return GeMatrix3d::kIdentity;
	}

	// 获得矩阵
	GeMatrix3d mat;
	DbViewportTableRecord *viewportTableRecord = DbViewportTableRecord::cast(viewport);
	if (viewportTableRecord != NULL)
	{
		// 获得ucs
		GePoint3d origin;
		GeVector3d xAxis;
		GeVector3d yAxis;
		viewportTableRecord->getUcs(origin, xAxis, yAxis);

		// 获得视口矩阵
		mat.setToAlignCoordSys(
			GePoint3d::kOrigin, GeVector3d::kXAxis, GeVector3d::kYAxis, GeVector3d::kZAxis,
			origin, xAxis, yAxis, xAxis.crossProduct(yAxis));
	}
	DbViewport *pViewport = DbViewport::cast(viewport);
	if (pViewport != NULL)
	{
		// 获得ucs
		GePoint3d origin;
		GeVector3d xAxis;
		GeVector3d yAxis;
		pViewport->getUcs(origin, xAxis, yAxis);

		// 获得视口矩阵
		mat.setToAlignCoordSys(
			GePoint3d::kOrigin, GeVector3d::kXAxis, GeVector3d::kYAxis, GeVector3d::kZAxis,
			origin, xAxis, yAxis, xAxis.crossProduct(yAxis));
	}

	return mat;
}

/** 获得视口的dcs矩阵
 * 1.wcs坐标乘矩阵为dcs矩阵
 */
GeMatrix3d trans_dcs_mat(DbObjectId _viewportId)
{

#if 1
	// 获得当前视口
	if (_viewportId == NULL)
	{
		_viewportId = acedActiveViewportId();
	}

	// 打开视口对象
	DbObject* viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport == NULL)
	{
		return GeMatrix3d::kIdentity;
	}

	// 获得矩阵
	GeMatrix3d mat;
	DbViewportTableRecord* viewportTableRecord = DbViewportTableRecord::cast(viewport);
	if (viewportTableRecord != NULL)
	{
		mat = DB_IMP_VIEWPORTTABLERECORD(viewportTableRecord->m_pImpl)->displayMatrix;


	}
	DbViewport* pViewport = DbViewport::cast(viewport);
	if (pViewport != NULL)
	{
		mat = DB_IMP_VIEWPORT(pViewport->m_pImpl)->displayMatrix;
	}

	return mat;
#endif


#if 0
	// 获得当前视口
	if (_viewportId == NULL)
	{
		_viewportId = acedActiveViewportId();
	}

	// 打开视口对象
	DbObject *viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport == NULL)
	{
		return GeMatrix3d::kIdentity;
	}

	// 获得矩阵
	GeMatrix3d mat;
	DbViewportTableRecord *viewportTableRecord = DbViewportTableRecord::cast(viewport);
	if (viewportTableRecord != NULL)
	{
		mat = DB_IMP_VIEWPORTTABLERECORD(viewportTableRecord->m_pImpl)->displayMatrix;
	}
	DbViewport *pViewport = DbViewport::cast(viewport);
	if (pViewport != NULL)
	{
		mat = DB_IMP_VIEWPORT(pViewport->m_pImpl)->displayMatrix;
	}

	return mat;
#endif
}

void trans_set_dcs_mat(const GeMatrix3d &_mat, DbObjectId _viewportId)
{
	// 获得当前视口
	if (_viewportId == NULL)
	{
		_viewportId = acedActiveViewportId();
	}

	// 打开视口对象
	DbObject *viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport == NULL)
	{
		return;
	}

	// 获得矩阵
	GeMatrix3d mat;
	DbViewportTableRecord *viewportTableRecord = DbViewportTableRecord::cast(viewport);
	if (viewportTableRecord != NULL)
	{
		GeVector3d viewDirection = GeVector3d::kZAxis;
		viewDirection.transformBy(_mat);
		viewportTableRecord->setViewDirection(viewDirection);
		DB_IMP_VIEWPORTTABLERECORD(viewportTableRecord->m_pImpl)->displayMatrix = _mat;
	}
	DbViewport *pViewport = DbViewport::cast(viewport);
	if (pViewport != NULL)
	{
		GeVector3d viewDirection = GeVector3d::kZAxis;
		viewDirection.transformBy(_mat);
		pViewport->setViewDirection(viewDirection);
		DB_IMP_VIEWPORT(pViewport->m_pImpl)->displayMatrix = _mat;
	}
}

/** 获得视口的vcs矩阵 */
GeMatrix3d trans_vcs_mat(DbObjectId _viewportId)
{
	// 获得当前视口
	if (_viewportId == NULL)
	{
		_viewportId = acedActiveViewportId();
	}

	// 获得视口矩阵
	GeMatrix3d viewMat = trans_dcs_mat(_viewportId);

	// 打开视口对象
	DbObject *viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport == NULL)
	{
		return GeMatrix3d::kIdentity;
	}

	// 获得矩阵
	GeMatrix3d mat = viewMat;
	DbViewportTableRecord *viewportTableRecord = DbViewportTableRecord::cast(viewport);
	if (viewportTableRecord != NULL)
	{
		double width = viewportTableRecord->width();
		double height = viewportTableRecord->height();

		GeMatrix3d tempMat;
		tempMat.setToTranslation(GeVector3d(0 - viewportTableRecord->centerPoint().x, 0 - viewportTableRecord->centerPoint().y, 0));
		mat = mat * tempMat;

		tempMat.setToScaling(GeScale3d(1.0 / width, 1.0 / height, 1));
		mat = mat * viewMat;

		tempMat.setToTranslation(GeVector3d(0.5, 0.5, 0));
		mat = mat * viewMat;
	}
	DbViewport *pViewport = DbViewport::cast(viewport);
	if (pViewport != NULL)
	{
	}

	return mat;
}

/** 获得视口的ncs矩阵 */
GeMatrix3d trans_ncs_mat(DbObjectId _viewportId)
{
	// 获得vcs矩阵
	GeMatrix3d mat = trans_vcs_mat(_viewportId);

	return mat;
}

GePoint3d wcs_to_viewport_projection(const GePoint3d &pt, DbObjectId _viewportId)
{

	// 获得当前视口
	if (_viewportId == NULL)
	{
		_viewportId = acedActiveViewportId();
	}

	// 获得视口矩阵
	GeMatrix3d viewMat = trans_dcs_mat(_viewportId);

	// 打开视口对象
	DbObject *viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport == NULL)
	{
		return GePoint3d(pt.x, pt.y, pt.z);
	}

	GePoint3d pos = pt;
	DbViewportTableRecord *viewportTableRecord = DbViewportTableRecord::cast(viewport);
	if (viewportTableRecord != NULL)
	{
		// 获得坐标系z轴
		GePoint3d origin;
		GeVector3d xAxis;
		GeVector3d yAxis;
		viewportTableRecord->getUcs(origin, xAxis, yAxis);
		GeVector3d zAxis = xAxis.crossProduct(yAxis);

		// 获得显示坐标系Z轴
		GeVector3d viewZAxis = GeVector3d::kZAxis;
		viewZAxis.transformBy(::trans_dcs_mat(viewportTableRecord->objectId()));
		pos = pos.project(GePlane(GePoint3d::kOrigin, zAxis), viewZAxis);
	}
	DbViewport *pViewport = DbViewport::cast(viewport);
	if (pViewport != NULL)
	{
		// 获得坐标系z轴
		GePoint3d origin;
		GeVector3d xAxis;
		GeVector3d yAxis;
		pViewport->getUcs(origin, xAxis, yAxis);
		GeVector3d zAxis = xAxis.crossProduct(yAxis);

		// 获得显示坐标系Z轴
		GeVector3d viewZAxis = GeVector3d::kZAxis;
		viewZAxis.transformBy(::trans_dcs_mat(pViewport->objectId()));
		pos = pos.project(GePlane(GePoint3d::kOrigin, zAxis), viewZAxis);
	}

	return pos;
}

DbExtents2d viewport_on_space_extents(DbObjectId _viewportId)
{
	DbExtents2d extents;
	extents.set(GePoint2d(0, 0), GePoint2d(1, 1));

	do
	{

		// 获得当前视口
		if (_viewportId == NULL)
		{
			_viewportId = acedActiveViewportId();
		}

		// 打开视口对象
		DbViewport *viewport = DbViewport::cast(::kernel()->acdbObjectManager()->openDbObject(_viewportId));
		if (viewport == NULL)
		{
			break;
		}

		// 判断当前是否是图纸空间视口
		if (viewport->number() == 1)
		{
			break;
		}

		// 获得视口所在的块表记录
		DbObjectId blockId = viewport->blockId();

		// 打开块表记录
		DbBlockTableRecord *pBlockTableRecord = NULL;
		if (::acdbOpenObject(pBlockTableRecord, blockId) != 0)
		{
			break;
		}

		// 获得布局id
		DbObjectId layoutId = pBlockTableRecord->getLayoutId();

		// 关闭块表记录
		pBlockTableRecord->close();

		// 获得布局
		DbLayout *pLayout = (DbLayout*)::kernel()->acdbObjectManager()->openDbObject(layoutId);
		if (pLayout == NULL)
		{
			break;
		}

		// 获得布局的第一个视口
		DbObjectId fistViewportId = NULL;
		for (int i = 0; i < pLayout->getViewportArray().length(); i++) {
			DbViewport *viewport = (DbViewport*)::kernel()->acdbObjectManager()->openDbObject(pLayout->getViewportArray()[i]);
			if (viewport != NULL) {
				if (viewport->number() == 1) {
					fistViewportId = viewport->objectId();
					break;
				}
			}
		}

		// 判空
		if (fistViewportId == NULL || fistViewportId == _viewportId)
		{
			break;
		}

		// 获得视口在左下和右上角
		GePoint2d lowerLeftCorner = GePoint2d(viewport->centerPoint().x - viewport->width() / 2, viewport->centerPoint().y - viewport->height() / 2);
		GePoint2d upperRightCorner = GePoint2d(viewport->centerPoint().x + viewport->width() / 2, viewport->centerPoint().y + viewport->height() / 2);
		lowerLeftCorner = trans_wcs_to_ncs(GePoint3d(lowerLeftCorner.x, lowerLeftCorner.y, 0), fistViewportId);
		upperRightCorner = trans_wcs_to_ncs(GePoint3d(upperRightCorner.x, upperRightCorner.y, 0), fistViewportId);

		extents.set(lowerLeftCorner, upperRightCorner);

	} while (false);

	return extents;
}

/** wcs转ucs
 * 1.获得视口中的ucs矩阵
 * 2.矩阵变换
 */
GePoint3d trans_wcs_to_ucs(const GePoint3d &pt, DbObjectId _viewportId)
{
	// 获得当前视口
	if (_viewportId == NULL){
		_viewportId = acedActiveViewportId();
	}

	// 获得视口数据
	GePoint3d origin;
	GeVector3d xAxis;
	GeVector3d yAxis;
	GeVector3d zAxis;
	DbObject *viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport->isA() == DbViewportTableRecord::desc()) {
		DbViewportTableRecord* viewportTableRecord = (DbViewportTableRecord*)viewport;
		viewportTableRecord->getUcs(origin, xAxis, yAxis);
	}
	else if (viewport->isA() == DbViewport::desc()) {
		DbViewport* pViewport = (DbViewport*)viewport;
		pViewport->getUcs(origin, xAxis, yAxis);
	}
	zAxis = xAxis.crossProduct(yAxis);

	// 矩阵变换
	GePoint3d pos = pt;
	GePoint3d vertical = GeLine3d::vertical(pt, GeLine3d(origin, xAxis));
	pos.x = (vertical - origin).length();
	if (GeRay3d(origin, xAxis).isOn(vertical) == false) {
		pos.x = 0.0 - pos.x;
	}
	vertical = GeLine3d::vertical(pt, GeLine3d(origin, yAxis));
	pos.y = (vertical - origin).length();
	if (GeRay3d(origin, yAxis).isOn(vertical) == false) {
		pos.y = 0.0 - pos.y;
	}
	vertical = GeLine3d::vertical(pt, GeLine3d(origin, zAxis));
	pos.z = (vertical - origin).length();
	if (GeRay3d(origin, zAxis).isOn(vertical) == false) {
		pos.z = 0.0 - pos.z;
	}

	return pos;
}

/** wcs转dcs
 * 1.获得视口的矩阵
 * 3.矩阵变换
 */
GePoint2d trans_wcs_to_dcs(const GePoint3d &pt, DbObjectId _viewportId)
{
	// 获得dcs矩阵
	GeMatrix3d mat = trans_dcs_mat(_viewportId);

	mat = mat.invert();

	// 矩阵变换
	GePoint3d pos = GePoint3d(pt[0], pt[1], pt[2]);
	pos.transformBy(mat);

	return GePoint2d(pos.x, pos.y);
}

/** wcs转vcs
 * 1.先wcs坐标转dcs坐标
 * 2.再dcs坐标转vcs坐标
 */
GePoint2d trans_wcs_to_vcs(const GePoint3d &pt, DbObjectId _viewportId)
{
	// wcs转dcs
	GePoint2d dcs = trans_wcs_to_dcs(pt, _viewportId);

	// dcs转vcs
	GePoint2d vcs = trans_dcs_to_vcs(dcs, _viewportId);

	return vcs;
}

/** wcs转ncs
 * 1.wcs转vcs
 * 2.vcs转ncs
 */
GePoint2d trans_wcs_to_ncs(const GePoint3d &pt, DbObjectId _viewportId)
{
	// wcs转vcs
	GePoint2d vcs = trans_wcs_to_vcs(pt, _viewportId);

	// vcs转ncs
	GePoint2d ncs = trans_vcs_to_ncs(vcs, _viewportId);

	return ncs;
}

/** ucs转wcs
 * 1.获得ucs矩阵
 * 2.矩阵求逆
 * 3.矩阵变换
 */
GePoint3d trans_ucs_to_wcs(const GePoint3d &pt, DbObjectId _viewportId)
{
	if (_viewportId == NULL) {
		_viewportId = acedActiveViewportId();
	}

	// 获得视口数据
	GePoint3d origin;
	GeVector3d xAxis;
	GeVector3d yAxis;
	GeVector3d zAxis;
	DbObject* viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport->isA() == DbViewportTableRecord::desc()) {
		DbViewportTableRecord* viewportTableRecord = (DbViewportTableRecord*)viewport;
		viewportTableRecord->getUcs(origin, xAxis, yAxis);
	}
	else if (viewport->isA() == DbViewport::desc()) {
		DbViewport* pViewport = (DbViewport*)viewport;
		pViewport->getUcs(origin, xAxis, yAxis);
	}
	zAxis = xAxis.crossProduct(yAxis);

	GePoint3d pos = origin;
	pos += xAxis * pt.x;
	pos += yAxis * pt.y;
	pos += zAxis * pt.z;
	return pos;
}

/** ucs转dcs
 * 1.ucs转wcs
 * 2.wcs转dcs
 */
GePoint2d trans_ucs_to_dcs(const GePoint3d &pt, DbObjectId _viewportId)
{
	// ucs转wcs
	GePoint3d wcs = trans_ucs_to_wcs(pt, _viewportId);

	// wcs转dcs
	GePoint2d dcs = trans_wcs_to_dcs(wcs, _viewportId);

	return dcs;
}

/** ucs转vcs
 * 1.ucs转wcs
 * 2.wcs转vcs
 */
GePoint2d trans_ucs_to_vcs(const GePoint3d &pt, DbObjectId _viewportId)
{
	// ucs转wcs
	GePoint3d wcs = trans_ucs_to_wcs(pt, _viewportId);

	// wcs转vcs
	GePoint2d vcs = trans_wcs_to_vcs(wcs, _viewportId);

	return vcs;
}

/** ucs转ncs
 * 1.ucs转wcs
 * 2.wcs转ncs
 */
GePoint2d trans_ucs_to_ncs(const GePoint3d &pt, DbObjectId _viewportId)
{
	// ucs转wcs
	GePoint3d wcs = trans_ucs_to_wcs(pt, _viewportId);

	// wcs转ncs
	GePoint2d ncs = trans_wcs_to_ncs(wcs, _viewportId);

	return ncs;
}

/** dcs转wcs
 * 1.获得dcs矩阵
 * 2.矩阵求逆
 * 3.矩阵变换
 */
GePoint3d trans_dcs_to_wcs(const GePoint2d &pt, const GeMatrix3d &mat){
	GePoint3d pos(pt.x, pt.y, 0.0);
	pos.transformBy(mat);
	return pos;
}
GePoint3d trans_dcs_to_wcs(const GePoint2d &pt, DbObjectId _viewportId)
{
	// 获得ucs矩阵
	GeMatrix3d mat = trans_dcs_mat(_viewportId);

	// 矩阵求逆
	//mat = mat.inverse();

	// 矩阵相乘
	GePoint3d pos(pt.x, pt.y, 0.0);
	pos.transformBy(mat);

	return pos;
}

/** dcs转ucs
 * 1.dcs转wcs
 * 2.wcs转ucs
 */
GePoint3d trans_dcs_to_ucs(const GePoint2d &pt, DbObjectId _viewportId)
{
	// dcs转wcs
	GePoint3d wcs = trans_dcs_to_wcs(pt, _viewportId);

	// wcs转ucs
	GePoint3d ucs = trans_wcs_to_ucs(wcs, _viewportId);

	return ucs;
}

/** dcs转vcs
 * 1.获得视口高度和宽度
 * 2.获得dcs最左下角坐标minPoint
 * 3.x = (dcs.x - minPoint.x) / viewWidth
 * 	 y = (dcs.y - minPoint.y) / viewHeight
 */
GePoint2d trans_dcs_to_vcs(const GePoint2d &pt, DbObjectId _viewportId)
{

	// 获得当前视口
	if (_viewportId == NULL)
	{
		_viewportId = acedActiveViewportId();
	}

	// 打开视口对象
	DbObject *viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport == NULL)
	{
		return GePoint2d(pt.x, pt.y);
	}

	// 获得视口中心
	GePoint2d vcsPoint;
	DbViewportTableRecord *viewportTableRecord = DbViewportTableRecord::cast(viewport);
	if (viewportTableRecord != NULL)
	{
		// 获得视口高度和宽度
		double viewHeight = viewportTableRecord->height();
		double viewWidth = viewportTableRecord->height() * ::kernel()->acedUserInputManager()->screenAspect();
		viewHeight *= (viewportTableRecord->upperRightCorner().y - viewportTableRecord->lowerLeftCorner().y);
		viewWidth *= (viewportTableRecord->upperRightCorner().x - viewportTableRecord->lowerLeftCorner().x);

		// 获得左下角坐标
		GePoint2d minPoint = GePoint2d(viewportTableRecord->centerPoint().x - viewWidth / 2, viewportTableRecord->centerPoint().y - viewHeight / 2);

		// 计算视口屏幕坐标
		vcsPoint.x = pt.x - minPoint.x;
		vcsPoint.y = pt.y - minPoint.y;
		vcsPoint.x = vcsPoint.x / viewWidth;
		vcsPoint.y = vcsPoint.y / viewHeight;
	}
	DbViewport *pViewport = DbViewport::cast(viewport);
	if (pViewport != NULL)
	{
		// 获得视口范围
		DbExtents2d extents = viewport_on_space_extents(_viewportId);

		// 获得视口高度和宽度
		double viewHeight = pViewport->viewHeight();
		double viewWidth = pViewport->viewHeight() * ::kernel()->acedUserInputManager()->screenAspect();
		viewWidth = viewWidth * ((extents.maxPoint().x - extents.minPoint().x) / (extents.maxPoint().y - extents.minPoint().y));

		// 获得左下角坐标
		GePoint2d minPoint = GePoint2d(pViewport->viewCenter().x - viewWidth / 2, pViewport->viewCenter().y - viewHeight / 2);

		// 计算视口屏幕坐标
		vcsPoint.x = pt.x - minPoint.x;
		vcsPoint.y = pt.y - minPoint.y;
		vcsPoint.x = vcsPoint.x / viewWidth;
		vcsPoint.y = vcsPoint.y / viewHeight;
	}

	return vcsPoint;
}

/** dcs转ncs
 * 1.dcs转vcs
 * 2.vcs转ncs
 */
GePoint2d trans_dcs_to_ncs(const GePoint2d &pt, DbObjectId _viewportId)
{
	// dcs转vcs
	GePoint2d vcs = trans_dcs_to_vcs(pt, _viewportId);

	// vcs转ncs
	GePoint2d ncs = trans_vcs_to_ncs(vcs, _viewportId);

	return ncs;
}

/** vcs转wcs
 * 1.vcs转dcs
 * 2.dcs转vcs
 */
GePoint3d trans_vcs_to_wcs(const GePoint2d &pt, DbObjectId _viewportId)
{
	// vcs转dcs
	GePoint2d dcs = trans_vcs_to_dcs(pt, _viewportId);

	// dcs转vcs
	GePoint3d wcs = trans_dcs_to_wcs(dcs, _viewportId);

	return wcs;
}

/** vcs转ucs
 * 1.vcs转wcs
 * 2.wcs转ucs
 */
GePoint3d trans_vcs_to_ucs(const GePoint2d &pt, DbObjectId _viewportId)
{
	// vcs转wcs
	GePoint3d wcs = trans_vcs_to_wcs(pt, _viewportId);

	// wcs转ucs
	GePoint3d ucs = trans_wcs_to_ucs(wcs, _viewportId);

	return ucs;
}

/** vcs转dcs
 * 1.获得视口中心和视口宽度(width),高度(height)
 * 2.获得视口最左下角(minPoint)和右上角(maxPoint)
 * 3.dcs.x = minPoint.x + vcs.x * width;
 *	dcs.y = minPoint.y + vcs.y * height;
 */
GePoint2d trans_vcs_to_dcs(const GePoint2d &pt, DbObjectId _viewportId)
{

	// 获得当前视口
	if (_viewportId == NULL)
	{
		_viewportId = acedActiveViewportId();
	}

	// 打开视口对象
	DbObject *viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport == NULL)
	{
		return pt;
	}

	// 获得视口数据
	double width = 0.0;
	double height = 0.0;
	GePoint2d centerPoint;
	if (DB_IMP_OBJECT(viewport->m_pImpl)->isViewportTableRecord() == true)
	{
		DbViewportTableRecord* viewportTableRecord = (DbViewportTableRecord*)viewport;
		height = viewportTableRecord->height();
		width = height * ::kernel()->acedUserInputManager()->screenAspect();
		width *= (viewportTableRecord->upperRightCorner().x - viewportTableRecord->lowerLeftCorner().x);
		height *= (viewportTableRecord->upperRightCorner().y - viewportTableRecord->lowerLeftCorner().y);
		centerPoint = viewportTableRecord->centerPoint();
	}
	else if (DB_IMP_OBJECT(viewport->m_pImpl)->isViewport() == true)
	{
		DbViewport* pViewport = (DbViewport*)viewport;

		DbExtents2d extents = viewport_on_space_extents(_viewportId);
		height = pViewport->viewHeight();
		width = height * ::kernel()->acedUserInputManager()->screenAspect();
		width = width * ((extents.maxPoint().x - extents.minPoint().x) / (extents.maxPoint().y - extents.minPoint().y));
		centerPoint = pViewport->viewCenter();
	}

	// 获得左下角
	GePoint2d minPoint(centerPoint.x - width / 2.0, centerPoint.y - height / 2.0);

	// 获得dcs
	GePoint2d dcs;
	dcs.x = minPoint.x + width * pt.x;
	dcs.y = minPoint.y + height * pt.y;

	return dcs;
}

/** vcs转ncs
 * 1.获得视口在ncs的左下角(minPoint)和右上角坐标(maxPoint)
 * 2.ncs.x = (maxPoint.x - minPoint.x) * vcs.x + minPoint.x
 *	ncs.y = (maxPoint.y - minPoint.y) * vcs.y + minPoint.y
 */
GePoint2d trans_vcs_to_ncs(const GePoint2d &pt, DbObjectId _viewportId)
{

	// 获得当前视口
	if (_viewportId == NULL)
	{
		_viewportId = acedActiveViewportId();
	}

	// 打开视口对象
	DbObject *viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport == NULL)
	{
		return pt;
	}

	GePoint2d ncs;
	DbViewportTableRecord *viewportTableRecord = DbViewportTableRecord::cast(viewport);
	if (viewportTableRecord != NULL)
	{
		ncs.x = (viewportTableRecord->upperRightCorner().x - viewportTableRecord->lowerLeftCorner().x) * pt.x + viewportTableRecord->lowerLeftCorner().x;
		ncs.y = (viewportTableRecord->upperRightCorner().y - viewportTableRecord->lowerLeftCorner().y) * pt.y + viewportTableRecord->lowerLeftCorner().y;
	}
	DbViewport *pViewport = DbViewport::cast(viewport);
	if (pViewport != NULL)
	{
		DbExtents2d extents = viewport_on_space_extents(_viewportId);

		ncs.x = (extents.maxPoint().x - extents.minPoint().x) * pt.x + extents.minPoint().x;
		ncs.y = (extents.maxPoint().y - extents.minPoint().y) * pt.y + extents.minPoint().y;
	}

	return ncs;
}

/** ncs转wcs
 * 1.ncs转dcs
 * 2.dcs转wcs
 */
GePoint3d trans_ncs_to_wcs(const GePoint2d &pt, DbObjectId _viewportId)
{

	// ncs转dcs
	GePoint2d dcs = trans_ncs_to_dcs(pt, _viewportId);

	// dcs转wcs
	GePoint3d wcs = trans_dcs_to_wcs(dcs, _viewportId);

	return wcs;
}

/** ncs转ucs
 * 1.ncs转wcs
 * 2.wcs转ucs
 */
GePoint3d trans_ncs_to_ucs(const GePoint2d &pt, DbObjectId _viewportId)
{
	// ncs转wcs
	GePoint3d wcs = trans_ncs_to_wcs(pt, _viewportId);

	// wcs转ucs
	GePoint3d ucs = trans_wcs_to_ucs(wcs, _viewportId);

	return ucs;
}

/** ncs转dcs
 * 1.ncs转vcs
 * 2.vcs转dcs
 */
GePoint2d trans_ncs_to_dcs(const GePoint2d &pt, DbObjectId _viewportId)
{
	// ncs转vcs
	GePoint2d vcs = trans_ncs_to_vcs(pt, _viewportId);

	// vcs转dcs
	GePoint2d dcs = trans_vcs_to_dcs(vcs, _viewportId);

	return dcs;
}

/** ncs转vcs */
GePoint2d trans_ncs_to_vcs(const GePoint2d &pt, DbObjectId _viewportId)
{

	// 获得当前视口
	if (_viewportId == NULL)
	{
		_viewportId = acedActiveViewportId();
	}

	// 打开视口对象
	DbObject *viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport == NULL)
	{
		return pt;
	}

	GePoint2d vcs;
	if (DB_IMP_OBJECT(viewport->m_pImpl)->isViewportTableRecord() == true)
	{
		DbViewportTableRecord* viewportTableRecord = (DbViewportTableRecord*)viewport;
		vcs.x = (pt.x - viewportTableRecord->lowerLeftCorner().x) / (viewportTableRecord->upperRightCorner().x - viewportTableRecord->lowerLeftCorner().x);
		vcs.y = (pt.y - viewportTableRecord->lowerLeftCorner().y) / (viewportTableRecord->upperRightCorner().y - viewportTableRecord->lowerLeftCorner().y);
	}
	else if (DB_IMP_OBJECT(viewport->m_pImpl)->isViewport() == true)
	{
		DbViewport* pViewport = DbViewport::cast(viewport);

		DbExtents2d extents;
		if (DbViewport::cast(::kernel()->acdbObjectManager()->openDbObject(_viewportId))->number() == 1)
		{
			extents.set(GePoint2d(0, 0), GePoint2d(1, 1));
		}
		else
		{
			extents = viewport_on_space_extents(_viewportId);
		}

		vcs.x = (pt.x - extents.minPoint().x) / (extents.maxPoint().x - extents.minPoint().x);
		vcs.y = (pt.y - extents.minPoint().y) / (extents.maxPoint().y - extents.minPoint().y);
	}

	return vcs;
}

GePoint2d trans_ncs_to_pixel(const GePoint2d &pt)
{

	double rangWidth = ::kernel()->acedUserInputManager()->rangWidth();
	double rangHeight = ::kernel()->acedUserInputManager()->rangHeight();

	GePoint2d pixel;
	pixel.x = pt.x * rangWidth;
	pixel.y = (1.0 - pt.y) * rangHeight;
	return pixel;
}

GePoint2d trans_pixel_to_ncs(const GePoint2d &pt)
{
	double rangWidth = ::kernel()->acedUserInputManager()->rangWidth();
	double rangHeight = ::kernel()->acedUserInputManager()->rangHeight();

	GePoint2d ncs;
	ncs.x = pt.x / rangWidth;
	ncs.y = 1.0 - pt.y / rangHeight;
	return ncs;
}

double trans_pixel_in_dcs_size(double pixel, DbObjectId _viewportId)
{

	// 获得当前视口
	if (_viewportId == NULL)
	{
		_viewportId = acedActiveViewportId();
	}

	// 打开视口对象
	DbObject *viewport = ::kernel()->acdbObjectManager()->openDbObject(_viewportId);
	if (viewport == NULL)
	{
		return 1;
	}

	// 获得视口高度
	double viewHeight = 1;
	if (DB_IMP_OBJECT(viewport->m_pImpl)->isViewportTableRecord() == true)
	{
		DbViewportTableRecord* viewportTableRecord = (DbViewportTableRecord*)viewport;
		viewHeight = viewportTableRecord->height();
	}
	else if (DB_IMP_OBJECT(viewport->m_pImpl)->isViewport() == true)
	{
		DbViewport* pViewport = (DbViewport*)viewport;

		viewHeight = pViewport->viewHeight();
		DbExtents2d extents = ::viewport_on_space_extents(pViewport->objectId());
		viewHeight = viewHeight / (extents.maxPoint().y - extents.minPoint().y);
	}

	// 当前是以2560x1440为基准
	//double val = 2560.0 / ::kernel()->acedUserInputManager()->screenHeight();

	viewHeight = viewHeight * (::kernel()->acedUserInputManager()->screenHeight() / ::kernel()->acedUserInputManager()->rangHeight());
	return viewHeight / ::kernel()->acedUserInputManager()->screenHeight() * pixel;
}
double trans_cursor_in_dcs_size(const double size, DbObjectId _viewportId) {
	return ::trans_pixel_in_dcs_size(size, _viewportId) * 30.0;
}
