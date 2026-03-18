#include "GeSphere.h"
#include "GeImpl.h"

GeSphere::GeSphere()
{
    GE_IMP_MEMORY_ENTITY(GeSphere);
}
GeSphere::GeSphere(double radius, const GePoint3d &center)
{
    GE_IMP_MEMORY_ENTITY(GeSphere);

    GE_IMP_SPHERE(this->m_pImpl)->radius = radius;
    GE_IMP_SPHERE(this->m_pImpl)->center = center;
}
GeSphere::GeSphere(double radius, const GePoint3d &center,
                   const GeVector3d &northAxis, const GeVector3d &refAxis,
                   double startAngleU, double endAngleU,
                   double startAngleV, double endAngleV)
{
    GE_IMP_MEMORY_ENTITY(GeSphere);

    GE_IMP_SPHERE(this->m_pImpl)->radius = radius;
    GE_IMP_SPHERE(this->m_pImpl)->center = center;
    GE_IMP_SPHERE(this->m_pImpl)->northAxis = northAxis;
    GE_IMP_SPHERE(this->m_pImpl)->refAxis = refAxis;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleU = startAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleU = endAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleV = startAngleV;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleV = endAngleV;
}
GeSphere::GeSphere(const GeSphere &sphere)
{
    GE_IMP_MEMORY_ENTITY(GeSphere);

    GE_IMP_SPHERE(this->m_pImpl)->radius = GE_IMP_SPHERE(sphere.m_pImpl)->radius;
    GE_IMP_SPHERE(this->m_pImpl)->center = GE_IMP_SPHERE(sphere.m_pImpl)->center;
    GE_IMP_SPHERE(this->m_pImpl)->northAxis = GE_IMP_SPHERE(sphere.m_pImpl)->northAxis;
    GE_IMP_SPHERE(this->m_pImpl)->refAxis = GE_IMP_SPHERE(sphere.m_pImpl)->refAxis;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleU = GE_IMP_SPHERE(sphere.m_pImpl)->startAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleU = GE_IMP_SPHERE(sphere.m_pImpl)->endAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleV = GE_IMP_SPHERE(sphere.m_pImpl)->startAngleV;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleV = GE_IMP_SPHERE(sphere.m_pImpl)->endAngleV;
}

double GeSphere::radius() const
{
    return GE_IMP_SPHERE(this->m_pImpl)->radius;
}
GePoint3d GeSphere::center() const
{
    return GE_IMP_SPHERE(this->m_pImpl)->center;
}
void GeSphere::getAnglesInU(double &start, double &end) const
{
    start = GE_IMP_SPHERE(this->m_pImpl)->startAngleU;
    end = GE_IMP_SPHERE(this->m_pImpl)->endAngleU;
}
void GeSphere::getAnglesInV(double &start, double &end) const
{
    start = GE_IMP_SPHERE(this->m_pImpl)->startAngleV;
    end = GE_IMP_SPHERE(this->m_pImpl)->endAngleV;
}
GeVector3d GeSphere::northAxis() const
{
    return GE_IMP_SPHERE(this->m_pImpl)->northAxis;
}
GeVector3d GeSphere::refAxis() const
{
    return GE_IMP_SPHERE(this->m_pImpl)->refAxis;
}
GePoint3d GeSphere::northPole() const
{
    GePoint3d pos = this->center();
    pos += this->northAxis().normal() * this->radius();
    return pos;
}
GePoint3d GeSphere::southPole() const
{
    GePoint3d pos = this->center();
    pos -= this->northAxis().normal() * this->radius();
    return pos;
}
Adesk::Boolean GeSphere::isOuterNormal() const
{
    return false;
}
Adesk::Boolean GeSphere::isClosed(const GeTol &tol) const
{
    if (abs(GE_IMP_SPHERE(this->m_pImpl)->endAngleU - GE_IMP_SPHERE(this->m_pImpl)->startAngleU) < tol.equalPoint() &&
        abs(GE_IMP_SPHERE(this->m_pImpl)->endAngleV - GE_IMP_SPHERE(this->m_pImpl)->startAngleV) < tol.equalPoint())
    {
        return true;
    }
    return false;
}

GeSphere &GeSphere::setRadius(double _radius)
{
    GE_IMP_SPHERE(this->m_pImpl)->radius = _radius;
    return *this;
}
GeSphere &GeSphere::setAnglesInU(double start, double end)
{
    GE_IMP_SPHERE(this->m_pImpl)->startAngleU = start;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleU = end;
    return *this;
}
GeSphere &GeSphere::setAnglesInV(double start, double end)
{
    GE_IMP_SPHERE(this->m_pImpl)->startAngleV = start;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleV = end;
    return *this;
}
GeSphere &GeSphere::set(double radius, const GePoint3d &center)
{
    GE_IMP_SPHERE(this->m_pImpl)->radius = radius;
    GE_IMP_SPHERE(this->m_pImpl)->center = center;
    return *this;
}
GeSphere &GeSphere::set(double radius,
                        const GePoint3d &center,
                        const GeVector3d &northAxis,
                        const GeVector3d &refAxis,
                        double startAngleU,
                        double endAngleU,
                        double startAngleV,
                        double endAngleV)
{
    GE_IMP_SPHERE(this->m_pImpl)->radius = radius;
    GE_IMP_SPHERE(this->m_pImpl)->center = center;
    GE_IMP_SPHERE(this->m_pImpl)->northAxis = northAxis;
    GE_IMP_SPHERE(this->m_pImpl)->refAxis = refAxis;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleU = startAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleU = endAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleV = startAngleV;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleV = endAngleV;
    return *this;
}

GeSphere &GeSphere::operator=(const GeSphere &sphere)
{
    GE_IMP_SPHERE(this->m_pImpl)->radius = GE_IMP_SPHERE(sphere.m_pImpl)->radius;
    GE_IMP_SPHERE(this->m_pImpl)->center = GE_IMP_SPHERE(sphere.m_pImpl)->center;
    GE_IMP_SPHERE(this->m_pImpl)->northAxis = GE_IMP_SPHERE(sphere.m_pImpl)->northAxis;
    GE_IMP_SPHERE(this->m_pImpl)->refAxis = GE_IMP_SPHERE(sphere.m_pImpl)->refAxis;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleU = GE_IMP_SPHERE(sphere.m_pImpl)->startAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleU = GE_IMP_SPHERE(sphere.m_pImpl)->endAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleV = GE_IMP_SPHERE(sphere.m_pImpl)->startAngleV;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleV = GE_IMP_SPHERE(sphere.m_pImpl)->endAngleV;
    return *this;
}

Adesk::Boolean GeSphere::intersectWith(const GeLinearEnt3d &, int &intn,
                                       GePoint3d &p1, GePoint3d &p2,
                                       const GeTol &tol) const
{
    return Adesk::kFalse;
}

void create_flat_sphere(double R,
                        std::vector<double> &positions_spheres,
                        std::vector<double> &normals_spheres,
                        int prec = 36)
{
    // The more small they are, the more precise the Sphere will be.
    //  Must be divisors of 180 and 360.
    const float rings = float(prec) / 2;
    const float sectors = float(prec);
    const float to_rad = static_cast<float>(PI / 180.0);

    float T, P;
    float x[4], y[4], z[4];

    using std::cos;
    using std::sin;

    // Top of the sphere
    for (float t = 0; t < 360.f; t += sectors)
    {

        positions_spheres.push_back(0);
        positions_spheres.push_back(0);
        positions_spheres.push_back(R);

        normals_spheres.push_back(0);
        normals_spheres.push_back(0);
        normals_spheres.push_back(1);

        P = rings * to_rad;
        T = t * to_rad;
        x[1] = sin(P) * cos(T);
        y[1] = sin(P) * sin(T);
        z[1] = cos(P);
        positions_spheres.push_back(R * x[1]);
        positions_spheres.push_back(R * y[1]);
        positions_spheres.push_back(R * z[1]);

        normals_spheres.push_back(x[1]);
        normals_spheres.push_back(y[1]);
        normals_spheres.push_back(z[1]);

        //
        P = rings * to_rad;
        T = (t + sectors) * to_rad;
        x[2] = sin(P) * cos(T);
        y[2] = sin(P) * sin(T);
        z[2] = cos(P);
        positions_spheres.push_back(R * x[2]);
        positions_spheres.push_back(R * y[2]);
        positions_spheres.push_back(R * z[2]);

        normals_spheres.push_back(x[2]);
        normals_spheres.push_back(y[2]);
        normals_spheres.push_back(z[2]);
    }

    // Body of the sphere
    for (float p = rings; p < 180.f - rings; p += rings)
        for (float t = 0; t < 360.f; t += sectors)
        {
            // A
            P = p * to_rad;
            T = t * to_rad;
            x[0] = sin(P) * cos(T);
            y[0] = sin(P) * sin(T);
            z[0] = cos(P);

            positions_spheres.push_back(R * x[0]);
            positions_spheres.push_back(R * y[0]);
            positions_spheres.push_back(R * z[0]);

            normals_spheres.push_back(x[0]);
            normals_spheres.push_back(y[0]);
            normals_spheres.push_back(z[0]);

            // B
            P = (p + rings) * to_rad;
            T = t * to_rad;
            x[1] = sin(P) * cos(T);
            y[1] = sin(P) * sin(T);
            z[1] = cos(P);
            positions_spheres.push_back(R * x[1]);
            positions_spheres.push_back(R * y[1]);
            positions_spheres.push_back(R * z[1]);

            normals_spheres.push_back(x[1]);
            normals_spheres.push_back(y[1]);
            normals_spheres.push_back(z[1]);

            // C
            P = p * to_rad;
            T = (t + sectors) * to_rad;
            x[2] = sin(P) * cos(T);
            y[2] = sin(P) * sin(T);
            z[2] = cos(P);
            positions_spheres.push_back(R * x[2]);
            positions_spheres.push_back(R * y[2]);
            positions_spheres.push_back(R * z[2]);

            normals_spheres.push_back(x[2]);
            normals_spheres.push_back(y[2]);
            normals_spheres.push_back(z[2]);
            // D
            P = (p + rings) * to_rad;
            T = (t + sectors) * to_rad;
            x[3] = sin(P) * cos(T);
            y[3] = sin(P) * sin(T);
            z[3] = cos(P);
            positions_spheres.push_back(R * x[3]);
            positions_spheres.push_back(R * y[3]);
            positions_spheres.push_back(R * z[3]);

            normals_spheres.push_back(x[3]);
            normals_spheres.push_back(y[3]);
            normals_spheres.push_back(z[3]);

            positions_spheres.push_back(R * x[1]);
            positions_spheres.push_back(R * y[1]);
            positions_spheres.push_back(R * z[1]);

            normals_spheres.push_back(x[1]);
            normals_spheres.push_back(y[1]);
            normals_spheres.push_back(z[1]);

            positions_spheres.push_back(R * x[2]);
            positions_spheres.push_back(R * y[2]);
            positions_spheres.push_back(R * z[2]);

            normals_spheres.push_back(x[2]);
            normals_spheres.push_back(y[2]);
            normals_spheres.push_back(z[2]);
        }
    // Bottom of the sphere
    for (float t = 0; t < 360.f; t += sectors)
    {
        positions_spheres.push_back(0);
        positions_spheres.push_back(0);
        positions_spheres.push_back(-R);

        normals_spheres.push_back(0);
        normals_spheres.push_back(0);
        normals_spheres.push_back(-1);

        P = (180 - rings) * to_rad;
        T = t * to_rad;
        x[1] = sin(P) * cos(T);
        y[1] = sin(P) * sin(T);
        z[1] = cos(P);
        positions_spheres.push_back(R * x[1]);
        positions_spheres.push_back(R * y[1]);
        positions_spheres.push_back(R * z[1]);

        normals_spheres.push_back(x[1]);
        normals_spheres.push_back(y[1]);
        normals_spheres.push_back(z[1]);

        P = (180 - rings) * to_rad;
        T = (t + sectors) * to_rad;
        x[2] = sin(P) * cos(T);
        y[2] = sin(P) * sin(T);
        z[2] = cos(P);
        positions_spheres.push_back(R * x[2]);
        positions_spheres.push_back(R * y[2]);
        positions_spheres.push_back(R * z[2]);

        normals_spheres.push_back(x[2]);
        normals_spheres.push_back(y[2]);
        normals_spheres.push_back(z[2]);
    }
}