#ifndef ACEDTRANS_H
#define ACEDTRANS_H

#include "DbObjectId.h"
#include "GeMatrix3d.h"
#include "DbExtents2d.h"




/**
 * wcs:全局坐标系
 * ucs:用户坐标系,用户输入的基准坐标系
 * dcs:显示坐标系,原点和wcs的原点相同,相对于wcs,x轴永远和屏幕x轴平行,y轴永远和屏幕y轴平行,z轴永远朝向屏幕外
 * vcs:相对于视口的坐标系,视口的最左下角为(0,0),视口的最右上角为(1,1)
 * ncs:相对于绘图区的坐标系,绘图区的最左下角为(0,0),绘图区的最右上角为(1,1)
 */

 /**
 * 屏幕坐标转ncs
 * 1.用户坐标是相对于绘图区左上角的坐标,实际的物理像素尺寸
 */
GePoint2d trans_client_to_ncs(unsigned int clientX, unsigned int clientY);

/**轴角转矩阵*/
GeMatrix3d angleOnAxisToMatrix(const GeVector3d& axis, double angle);

/**矩阵转轴角*/
void matrixToAngleOnAxis(const GeMatrix3d& mat, GeVector3d& axis, double& angle);

/** 获得视口的ucs矩阵
 * 1.wcs坐标乘矩阵为ucs坐标
 */
GeMatrix3d trans_ucs_mat(DbObjectId _viewportId = NULL);

/** 获得视口的dcs矩阵
 * 1.wcs坐标乘矩阵为dcs矩阵
 */
GeMatrix3d trans_dcs_mat(DbObjectId _viewportId = NULL);

/** 设置视口的dcs矩阵
 * 1.wcs坐标乘矩阵为dcs矩阵
 */
void trans_set_dcs_mat(const GeMatrix3d& _mat, DbObjectId _viewportId = NULL);

/** 获得视口的vcs矩阵 */
GeMatrix3d trans_vcs_mat(DbObjectId _viewportId = NULL);

/** 获得视口的ncs矩阵 */
GeMatrix3d trans_ncs_mat(DbObjectId _viewportId = NULL);

/** wcs在视口上的投影*/
GePoint3d wcs_to_viewport_projection(const GePoint3d& pt, DbObjectId _viewportId = NULL);

/** 获得视口在图纸中间中的范围*/
DbExtents2d viewport_on_space_extents(DbObjectId _viewportId = NULL);

/** wcs转ucs */
GePoint3d trans_wcs_to_ucs(const GePoint3d& pt, DbObjectId _viewportId = NULL);

/** wcs转dcs */
GePoint2d trans_wcs_to_dcs(const GePoint3d& pt, DbObjectId _viewportId = NULL);

/** wcs转vcs */
GePoint2d trans_wcs_to_vcs(const GePoint3d& pt, DbObjectId _viewportId = NULL);

/** wcs转ncs */
GePoint2d trans_wcs_to_ncs(const GePoint3d& pt, DbObjectId _viewportId = NULL);

/** ucs转wcs */
GePoint3d trans_ucs_to_wcs(const GePoint3d& pt, DbObjectId _viewportId = NULL);

/** ucs转dcs */
GePoint2d trans_ucs_to_dcs(const GePoint3d& pt, DbObjectId _viewportId = NULL);

/** ucs转vcs */
GePoint2d trans_ucs_to_vcs(const GePoint3d& pt, DbObjectId _viewportId = NULL);

/** ucs转ncs */
GePoint2d trans_ucs_to_ncs(const GePoint3d& pt, DbObjectId _viewportId = NULL);

/** dcs转wcs */
GePoint3d trans_dcs_to_wcs(const GePoint2d& pt, const GeMatrix3d& mat);
GePoint3d trans_dcs_to_wcs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** dcs转ucs */
GePoint3d trans_dcs_to_ucs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** dcs转vcs */
GePoint2d trans_dcs_to_vcs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** dcs转ncs */
GePoint2d trans_dcs_to_ncs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** vcs转wcs */
GePoint3d trans_vcs_to_wcs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** vcs转ucs */
GePoint3d trans_vcs_to_ucs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** vcs转dcs */
GePoint2d trans_vcs_to_dcs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** vcs转ncs */
GePoint2d trans_vcs_to_ncs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** ncs转wcs */
GePoint3d trans_ncs_to_wcs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** ncs转ucs */
GePoint3d trans_ncs_to_ucs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** ncs转dcs */
GePoint2d trans_ncs_to_dcs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** ncs转vcs */
GePoint2d trans_ncs_to_vcs(const GePoint2d& pt, DbObjectId _viewportId = NULL);

/** ncs转屏幕像素 */
GePoint2d trans_ncs_to_pixel(const GePoint2d& pt);

/** 像素转ncs */
GePoint2d trans_pixel_to_ncs(const GePoint2d& pt);

/** 像素在dcs下的尺寸 */
double trans_pixel_in_dcs_size(double pixel, DbObjectId _viewportId = NULL);

/** 光标尺寸转dcs尺寸 */
double trans_cursor_in_dcs_size(const double size, DbObjectId _viewportId = NULL);


#endif
