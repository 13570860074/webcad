#ifndef GEIMPL_H
#define GEIMPL_H

#include "Ge.h"
#include "GePoint2d.h"
#include "GePoint3d.h"
#include "GeMatrix2d.h"
#include "GeCurve2d.h"
#include "GeCurve3d.h"
#include "GeSurface.h"
#include <typeinfo>

#define GE_IMP_POINTONCURVE2D(x) ((GePointOnCurve2dImpl *)x)
#define GE_IMP_POINTONCURVE3D(x) ((GePointOnCurve3dImpl *)x)
#define GE_IMP_POINTONSURFACE(x) ((GePointOnSurfaceImpl *)x)

#define GE_IMP_BOUNDBLOCK2D(x) ((GeBoundBlock2dImpl *)x)
#define GE_IMP_BOUNDBLOCK3D(x) ((GeBoundBlock3dImpl *)x)

#define GE_IMP_CIRCARC2D(x) ((GeCircArc2dImpl *)x)
#define GE_IMP_CIRCARC3D(x) ((GeCircArc3dImpl *)x)
#define GE_IMP_ELLIPARC2D(x) ((GeEllipArc2dImpl *)x)
#define GE_IMP_ELLIPARC3D(x) ((GeEllipArc3dImpl *)x)
#define GE_IMP_CURVE2D(x) ((GeCurve2dImpl *)x)
#define GE_IMP_CURVE3D(x) ((GeCurve3dImpl *)x)
#define GE_IMP_LINE2D(x) ((GeLine2dImpl *)x)
#define GE_IMP_LINE3D(x) ((GeLine3dImpl *)x)
#define GE_IMP_LINEARENT2D(x) ((GeLinearEnt2dImpl *)x)
#define GE_IMP_LINEARENT3D(x) ((GeLinearEnt3dImpl *)x)
#define GE_IMP_LINESEG2D(x) ((GeLineSeg2dImpl *)x)
#define GE_IMP_LINESEG3D(x) ((GeLineSeg3dImpl *)x)
#define GE_IMP_RAY2D(x) ((GeRay2dImpl *)x)
#define GE_IMP_RAY3D(x) ((GeRay3dImpl *)x)

#define GE_IMP_SURFACE(x) ((GeSurfaceImpl *)x)
#define GE_IMP_PLANARENT(x) ((GePlanarEntImpl *)x)
#define GE_IMP_PLANE(x) ((GePlaneImpl *)x)
#define GE_IMP_BOUNDEDPLANE(x) ((GeBoundedPlaneImpl *)x)
#define GE_IMP_SPHERE(x) ((GeSphereImpl *)x)


#define GE_IMP_MEMORY_ENTITY(x) \
if (typeid(this) == typeid(x*)) {\
	this->m_pImpl = new x##Impl();\
}\
else {\
	x##Impl impl;\
	this->m_pImpl = &impl;\
}\


class GeImpEntity2d
{
protected:
	GeImpEntity2d() {}
};
class GeImpEntity3d
{
protected:
	GeImpEntity3d() {}
};

class GePointEnt2dImpl : public GeImpEntity2d
{
protected:
	GePointEnt2dImpl() {}
	~GePointEnt2dImpl() {}
};
class GePointEnt3dImpl : public GeImpEntity3d
{
protected:
	GePointEnt3dImpl() {}
	~GePointEnt3dImpl() {}
};
class GePointOnCurve2dImpl : public GePointEnt2dImpl
{
public:
	GePointOnCurve2dImpl()
	{
		this->pCurve = NULL;
		this->param = 0.0;
	}
	~GePointOnCurve2dImpl() {}

public:
	const GeCurve2d *pCurve;
	double param;
};
class GePointOnCurve3dImpl : public GePointEnt3dImpl
{
public:
	GePointOnCurve3dImpl()
	{
		this->pCurve = NULL;
		this->param = 0.0;
	}
	~GePointOnCurve3dImpl() {}

public:
	const GeCurve3d *pCurve;
	double param;
};
class GePointOnSurfaceImpl : public GePointEnt3dImpl
{
public:
	GePointOnSurfaceImpl()
	{
		this->surface = NULL;
		this->param = GePoint2d(0, 0);
	}
	~GePointOnSurfaceImpl() {}

public:
	const GeSurface *surface;
	GePoint2d param;
};

class GeBoundBlock2dImpl : public GeImpEntity2d
{
public:
	GeBoundBlock2dImpl()
	{
		this->isBox = false;
		this->basePoint = GePoint2d::kOrigin;
		this->dir1 = GeVector2d(0, 0);
		this->dir2 = GeVector2d(0, 0);
	}
	~GeBoundBlock2dImpl() {}

	bool isBox;
	GePoint2d basePoint;
	GeVector2d dir1;
	GeVector2d dir2;
};
class GeBoundBlock3dImpl : public GeImpEntity3d
{
public:
	GeBoundBlock3dImpl()
	{
		this->isBox = false;
		this->basePoint = GePoint3d::kOrigin;
		this->dir1 = GeVector3d(0, 0, 0);
		this->dir2 = GeVector3d(0, 0, 0);
		this->dir3 = GeVector3d(0, 0, 0);
	}
	~GeBoundBlock3dImpl() {}

	bool isBox;
	GePoint3d basePoint;
	GeVector3d dir1;
	GeVector3d dir2;
	GeVector3d dir3;
};

class GeCurve2dImpl : public GeImpEntity2d
{
protected:
	GeCurve2dImpl()
	{
	}
};
class GeCurve3dImpl : public GeImpEntity3d
{
protected:
	GeCurve3dImpl()
	{
	}
};

class GeLinearEnt2dImpl : public GeCurve2dImpl
{
protected:
	GeLinearEnt2dImpl()
	{
	}

public:
	GePoint2d origin;
	GeVector2d vector;
};
class GeLinearEnt3dImpl : public GeCurve3dImpl
{
protected:
	GeLinearEnt3dImpl()
	{
	}

public:
	GePoint3d origin;
	GeVector3d vector;
};

class GeLineSeg2dImpl : public GeLinearEnt2dImpl
{
public:
	GeLineSeg2dImpl()
	{
	}
};
class GeLineSeg3dImpl : public GeLinearEnt3dImpl
{
public:
	GeLineSeg3dImpl()
	{
	}
};

class GeLine2dImpl : public GeLinearEnt2dImpl
{
public:
	GeLine2dImpl()
	{
	}
};
class GeLine3dImpl : public GeLinearEnt3dImpl
{
public:
	GeLine3dImpl()
	{
	}
};

class GeRay2dImpl : public GeLinearEnt2dImpl
{
public:
	GeRay2dImpl()
	{
	}
};
class GeRay3dImpl : public GeLinearEnt3dImpl
{
public:
	GeRay3dImpl()
	{
	}
};

class GeCircArc2dImpl : public GeCurve2dImpl
{
public:
	GeCircArc2dImpl()
	{
		this->radius = 0;
		this->startAngle = 0;
		this->endAngle = PI * 2;
		this->refVec = GeVector2d::kXAxis;
		this->isClockWise = false;
	}

public:
	GePoint2d center;
	double radius;
	double startAngle;
	double endAngle;
	GeVector2d refVec;
	bool isClockWise;
};
class GeCircArc3dImpl : public GeCurve3dImpl
{
public:
	GeCircArc3dImpl()
	{
		this->radius = 0;
		this->startAngle = 0;
		this->endAngle = PI * 2;
		this->refVec = GeVector3d::kXAxis;
		this->normal = GeVector3d::kZAxis;
	}

public:
	GePoint3d center;
	double radius;
	double startAngle;
	double endAngle;
	GeVector3d refVec;
	GeVector3d normal;
};

class GeEllipArc2dImpl : public GeCurve2dImpl
{
public:
	GeEllipArc2dImpl()
	{
		this->center = GePoint2d::kOrigin;
		this->majorAxis = GeVector2d::kXAxis;
		this->ratio = 1;
		this->startAngle = 0;
		this->endAngle = PI * 2;
	}
	~GeEllipArc2dImpl()
	{
	}

	GePoint2d center;
	GeVector2d majorAxis;
	;
	double ratio;
	double startAngle;
	double endAngle;
};
class GeEllipArc3dImpl : public GeCurve3dImpl
{
public:
	GeEllipArc3dImpl()
	{
		this->center = GePoint3d::kOrigin;
		this->majorAxis = GeVector3d::kXAxis;
		this->minorAxis = GeVector3d::kYAxis;
		this->startAngle = 0;
		this->endAngle = PI * 2;
	}
	~GeEllipArc3dImpl()
	{
	}

	GePoint3d center;
	GeVector3d majorAxis;
	;
	GeVector3d minorAxis;
	;
	double startAngle;
	double endAngle;
};

class GeSurfaceImpl : public GeImpEntity3d
{
public:
	GeSurfaceImpl()
	{
	}
	~GeSurfaceImpl()
	{
	}
};
class GePlanarEntImpl : public GeSurfaceImpl
{
public:
	GePlanarEntImpl()
	{
		this->isNormalReversed = false;
		this->xAxis = GeVector3d::kZAxis;
		this->yAxis = GeVector3d::kYAxis;
		this->normal = GeVector3d::kZAxis;
	}
	~GePlanarEntImpl()
	{
	}

	GePoint3d origin;
	GeVector3d xAxis;
	GeVector3d yAxis;
	GeVector3d normal;
	bool isNormalReversed;
};
class GePlaneImpl : public GePlanarEntImpl
{
public:
	GePlaneImpl()
	{
	}
	~GePlaneImpl()
	{
	}
};
class GeBoundedPlaneImpl : public GePlanarEntImpl
{
public:
	GeBoundedPlaneImpl()
	{
	}
	~GeBoundedPlaneImpl()
	{
	}
};
class GeSphereImpl : public GeSurfaceImpl
{
public:
	GeSphereImpl()
	{
		this->center = GePoint3d::kOrigin;
		this->radius = 1.0;
		this->northAxis = GeVector3d::kYAxis;
		this->refAxis = GeVector3d::kXAxis;
		this->startAngleU = -PI * 0.5;
		this->endAngleU = PI * 0.5;
		this->startAngleV = -PI;
		this->endAngleV = PI;
		this->isOuterNormal = false;
	}
	virtual ~GeSphereImpl()
	{
	}

	GePoint3d center;
	double radius;
	GeVector3d northAxis;
	GeVector3d refAxis;
	double startAngleU;
	double endAngleU;
	double startAngleV;
	double endAngleV;
	bool isOuterNormal;
};

#endif