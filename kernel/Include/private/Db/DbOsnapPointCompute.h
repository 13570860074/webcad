#ifndef DBOSNAPPOINTCOMPUTE_H
#define DBOSNAPPOINTCOMPUTE_H

#include "Db.h"
#include "adesk.h"
#include "GePoint3d.h"
#include "GeMatrix3d.h"
#include "GePoint3dArray.h"

class DbLine;
class DbRay;
class DbXline;
class DbCircle;
class DbArc;
class DbPolyline;
class DbBlockReference;

/* 捕捉最近点 */
extern GePoint3dArray osnap_near_to_lintseg(const GePoint3d &pickPoint, const DbLine *pLine, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_near_to_ray(const GePoint3d &pickPoint, const DbRay *pRay, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_near_to_line(const GePoint3d &pickPoint, const DbXline *pXline, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_near_to_circle(const GePoint3d &pickPoint, const DbCircle *pCircle, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_near_to_arc(const GePoint3d &pickPoint, const DbArc *pArc, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_near_to_polyline(const GePoint3d &pickPoint, const DbPolyline *pPolyline, const GeMatrix3d &viewXform);

/* 捕捉中点 */
extern GePoint3dArray osnap_mid_to_lintseg(const GePoint3d &pickPoint, const DbLine *pLine, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_mid_to_ray(const GePoint3d &pickPoint, const DbRay *pRay, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_mid_to_line(const GePoint3d &pickPoint, const DbXline *pXline, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_mid_to_circle(const GePoint3d &pickPoint, const DbCircle *pCircle, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_mid_to_arc(const GePoint3d &pickPoint, const DbArc *pArc, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_mid_to_polyline(const GePoint3d &pickPoint, const DbPolyline *pPolyline, const GeMatrix3d &viewXform);

/* 捕捉圆心 */
extern GePoint3dArray osnap_center_to_circle(const GePoint3d &pickPoint, const DbCircle *pCircle, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_center_to_arc(const GePoint3d &pickPoint, const DbArc *pArc, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_center_to_polyline(const GePoint3d &pickPoint, const DbPolyline *pPolyline, const GeMatrix3d &viewXform);

/* 捕捉象限点 */
extern GePoint3dArray osnap_quad_to_circle(const GePoint3d &pickPoint, const DbCircle *pCircle, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_quad_to_arc(const GePoint3d &pickPoint, const DbArc *pArc, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_quad_to_polyline(const GePoint3d &pickPoint, const DbPolyline *pPolyline, const GeMatrix3d &viewXform);

/* 捕捉端点 */
extern GePoint3dArray osnap_end_to_lintseg(const GePoint3d &pickPoint, const DbLine *pLine, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_end_to_ray(const GePoint3d &pickPoint, const DbRay *pRay, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_end_to_line(const GePoint3d &pickPoint, const DbXline *pXline, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_end_to_circle(const GePoint3d &pickPoint, const DbCircle *pCircle, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_end_to_arc(const GePoint3d &pickPoint, const DbArc *pArc, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_end_to_polyline(const GePoint3d &pickPoint, const DbPolyline *pPolyline, const GeMatrix3d &viewXform);

/* 捕捉垂点 */
extern GePoint3dArray osnap_perp_to_lintseg(const GePoint3d &pickPoint, const GePoint3d &lastPoint, const DbLine *pLine, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_perp_to_ray(const GePoint3d &pickPoint, const GePoint3d &lastPoint, const DbRay *pRay, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_perp_to_line(const GePoint3d &pickPoint, const GePoint3d &lastPoint, const DbXline *pXline, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_perp_to_circle(const GePoint3d &pickPoint, const GePoint3d &lastPoint, const DbCircle *pCircle, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_perp_to_arc(const GePoint3d &pickPoint, const GePoint3d &lastPoint, const DbArc *pArc, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_perp_to_polyline(const GePoint3d &pickPoint, const GePoint3d &lastPoint, const DbPolyline *pPolyline, const GeMatrix3d &viewXform);

/* 捕捉切点*/
extern GePoint3dArray osnap_tan_to_circle(const GePoint3d &pickPoint, const GePoint3d &lastPoint, const DbCircle *pCircle, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_tan_to_arc(const GePoint3d &pickPoint, const GePoint3d &lastPoint, const DbArc *pArc, const GeMatrix3d &viewXform);
extern GePoint3dArray osnap_tan_to_polyline(const GePoint3d &pickPoint, const GePoint3d &lastPoint, const DbPolyline *pPolyline, const GeMatrix3d &viewXform);

/* 求交点 */
extern GePoint3dArray osnap_inters_lintseg_to_lintseg(const DbLine *pLine1, const DbLine *pLine2, Db::Intersect intType);
extern GePoint3dArray osnap_inters_lintseg_to_ray(const DbLine *pLine, const DbRay *pRay, Db::Intersect intType);
extern GePoint3dArray osnap_inters_lintseg_to_line(const DbLine *pLine, const DbXline *pXline, Db::Intersect intType);
extern GePoint3dArray osnap_inters_lintseg_to_circle(const DbLine *pLine, const DbCircle *pCircle, Db::Intersect intType);
extern GePoint3dArray osnap_inters_lintseg_to_arc(const DbLine *pLine, const DbArc *pArc, Db::Intersect intType);
extern GePoint3dArray osnap_inters_lintseg_to_polyline(const DbLine *pLine, const DbPolyline *pPolyline, Db::Intersect intType);

extern GePoint3dArray osnap_inters_ray_to_lintseg(const DbRay *pRay, const DbLine *pLine, Db::Intersect intType);
extern GePoint3dArray osnap_inters_ray_to_ray(const DbRay *pRay1, const DbRay *pRay2, Db::Intersect intType);
extern GePoint3dArray osnap_inters_ray_to_line(const DbRay *pRay, const DbXline *pXline, Db::Intersect intType);
extern GePoint3dArray osnap_inters_ray_to_circle(const DbRay *pRay, const DbCircle *pCircle, Db::Intersect intType);
extern GePoint3dArray osnap_inters_ray_to_arc(const DbRay *pRay, const DbArc *pArc, Db::Intersect intType);
extern GePoint3dArray osnap_inters_ray_to_polyline(const DbRay *pRay, const DbPolyline *pPolyline, Db::Intersect intType);

extern GePoint3dArray osnap_inters_line_to_lintseg(const DbXline *pXline, const DbLine *pLine, Db::Intersect intType);
extern GePoint3dArray osnap_inters_line_to_ray(const DbXline *pXline, const DbRay *pRay2, Db::Intersect intType);
extern GePoint3dArray osnap_inters_line_to_line(const DbXline *pXline1, const DbXline *pXline2, Db::Intersect intType);
extern GePoint3dArray osnap_inters_line_to_circle(const DbXline *pXline, const DbCircle *pCircle, Db::Intersect intType);
extern GePoint3dArray osnap_inters_line_to_arc(const DbXline *pXline, const DbArc *pArc, Db::Intersect intType);
extern GePoint3dArray osnap_inters_line_to_polyline(const DbXline *pXline, const DbPolyline *pPolyline, Db::Intersect intType);

extern GePoint3dArray osnap_inters_circle_to_lintseg(const DbCircle *pCircle, const DbLine *pLine, Db::Intersect intType);
extern GePoint3dArray osnap_inters_circle_to_ray(const DbCircle *pCircle, const DbRay *pRay, Db::Intersect intType);
extern GePoint3dArray osnap_inters_circle_to_line(const DbCircle *pCircle, const DbXline *pXline, Db::Intersect intType);
extern GePoint3dArray osnap_inters_circle_to_circle(const DbCircle *pCircle1, const DbCircle *pCircle2, Db::Intersect intType);
extern GePoint3dArray osnap_inters_circle_to_arc(const DbCircle *pCircle, const DbArc *pArc, Db::Intersect intType);
extern GePoint3dArray osnap_inters_circle_to_polyline(const DbCircle *pCircle, const DbPolyline *pPolyline, Db::Intersect intType);

extern GePoint3dArray osnap_inters_arc_to_lintseg(const DbArc *pArc, const DbLine *pLine, Db::Intersect intType);
extern GePoint3dArray osnap_inters_arc_to_ray(const DbArc *pArc, const DbRay *pRay, Db::Intersect intType);
extern GePoint3dArray osnap_inters_arc_to_line(const DbArc *pArc, const DbXline *pXline, Db::Intersect intType);
extern GePoint3dArray osnap_inters_arc_to_circle(const DbArc *pArc, const DbCircle *pCircle, Db::Intersect intType);
extern GePoint3dArray osnap_inters_arc_to_arc(const DbArc *pArc1, const DbArc *pArc2, Db::Intersect intType);
extern GePoint3dArray osnap_inters_arc_to_polyline(const DbArc *pArc, const DbPolyline *pPolyline, Db::Intersect intType);

extern GePoint3dArray osnap_inters_polyline_to_lintseg(const DbPolyline *pPolyline, const DbLine *pLine, Db::Intersect intType);
extern GePoint3dArray osnap_inters_polyline_to_ray(const DbPolyline *pPolyline, const DbRay *pRay, Db::Intersect intType);
extern GePoint3dArray osnap_inters_polyline_to_line(const DbPolyline *pPolyline, const DbXline *pXline, Db::Intersect intType);
extern GePoint3dArray osnap_inters_polyline_to_circle(const DbPolyline *pPolyline, const DbCircle *pCircle, Db::Intersect intType);
extern GePoint3dArray osnap_inters_polyline_to_arc(const DbPolyline *pPolyline, const DbArc *pArc2, Db::Intersect intType);
extern GePoint3dArray osnap_inters_polyline_to_polyline(const DbPolyline *pPolyline1, const DbPolyline *pPolyline2, Db::Intersect intType);

#endif