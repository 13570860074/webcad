#ifndef GITRIANGLEMESHGEOMETRY_H
#define GITRIANGLEMESHGEOMETRY_H

#include "GePoint3d.h"
#include "GiEntityGeometry.h"

class GiTriangleMeshGeometry : public GiEntityGeometry
{
public:
	GiTriangleMeshGeometry();
	virtual ~GiTriangleMeshGeometry();
	RX_DECLARE_MEMBERS(GiTriangleMeshGeometry);

public:
	/* 类型 */
	virtual Gi::EntityGeometryType type();

	/* clone */
	virtual GiTriangleMeshGeometry* clone() const;

	/* 获得顶点 */
	GePoint3d vertex(int _index) const;

	/* 设置顶点 */
	void setVertex(int _index, const GePoint3d &_vertex);

	GePoint2d displayCoord(unsigned char index) const;
	void setDisplayCoord(unsigned char index, const GePoint2d &pt) const;
};

#endif