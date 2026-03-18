#include "GiPoints.h"
#include "kernel.h"
#include "GiImpl.h"

GiPoints::GiPoints()
{
    this->m_pImpl = new GiPointsImpl();
}
GiPoints::~GiPoints()
{
}

Gi::EntityType GiPoints::type() const
{
    return Gi::EntityType::kPoints;
}
void GiPoints::update() {

    GiEntity::update();

    /* 分配内存 */
    int sizeVertexData = GI_IMP_POINTS(this->m_pImpl)->geometrys.length() * 3;
    if (GI_IMP_POINTS(this->m_pImpl)->vertexs == NULL) {
        GI_IMP_POINTS(this->m_pImpl)->sizeVertexData = sizeVertexData;
        GI_IMP_POINTS(this->m_pImpl)->vertexs = new double[GI_IMP_POINTS(this->m_pImpl)->sizeVertexData];
    }
    else if (sizeVertexData > GI_IMP_POINTS(this->m_pImpl)->sizeVertexData) {
        delete[] GI_IMP_POINTS(this->m_pImpl)->vertexs;
        GI_IMP_POINTS(this->m_pImpl)->sizeVertexData = sizeVertexData;
        GI_IMP_POINTS(this->m_pImpl)->vertexs = new double[GI_IMP_POINTS(this->m_pImpl)->sizeVertexData];
    }

    /* 设置顶点 */
    GI_IMP_POINTS(this->m_pImpl)->vertexLength = 0;
    for (int i = 0; i < GI_IMP_POINTS(this->m_pImpl)->geometrys.length(); i++)
    {

        GiPointGeometry* pPointGeometry = (GiPointGeometry*)GI_IMP_POINTS(this->m_pImpl)->geometrys[i];
        if (pPointGeometry->isErased() == true)
        {
            continue;
        }
        if (pPointGeometry->visibility() == false)
        {
            continue;
        }
        GI_IMP_POINTS(this->m_pImpl)->vertexs[GI_IMP_POINTS(this->m_pImpl)->vertexLength++] = pPointGeometry->position().x;
        GI_IMP_POINTS(this->m_pImpl)->vertexs[GI_IMP_POINTS(this->m_pImpl)->vertexLength++] = pPointGeometry->position().y;
        GI_IMP_POINTS(this->m_pImpl)->vertexs[GI_IMP_POINTS(this->m_pImpl)->vertexLength++] = pPointGeometry->position().z;
    }
}
#if EMSDK
emscripten::val GiPoints::vertexs()
{
    return val(typed_memory_view(GI_IMP_POINTS(this->m_pImpl)->vertexLength, GI_IMP_ENTITY(this->m_pImpl)->vertexs));
}
#else
double *GiPoints::vertexs() {
    return GI_IMP_POINTS(this->m_pImpl)->vertexs;
}
#endif
GiPointGeometry *GiPoints::appendPointGeometry(const GePoint3d &_position)
{
    GiPointGeometry *pPointGeometry = new GiPointGeometry();
    pPointGeometry->setPosition(_position);
    this->appendPointGeometry(pPointGeometry);
    return pPointGeometry;
}
void GiPoints::appendPointGeometry(GiPointGeometry *pointGeometry)
{
    GI_IMP_ENTITYGEOMETRY(pointGeometry->m_pImpl)->giEntity = this;
    GI_IMP_POINTS(this->m_pImpl)->geometrys.append(pointGeometry);
    GI_IMP_POINTS(this->m_pImpl)->bUpdateVertex = true;
}