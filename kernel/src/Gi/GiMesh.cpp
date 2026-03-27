#include "GiMesh.h"
#include "kernel.h"
#include "GiImpl.h"

GiMesh::GiMesh()
{
    this->m_pImpl = new GiMeshImpl();
}
GiMesh::~GiMesh()
{
}

Gi::EntityType GiMesh::type() const
{
    return Gi::EntityType::kMesh;
}

void GiMesh::update() {

    GiEntity::update();

    /** 分配内存 */
    int sizeVertexData = GI_IMP_MESH(this->m_pImpl)->geometrys.length() * 9;
    if (GI_IMP_MESH(this->m_pImpl)->vertexs == NULL)
    {
        GI_IMP_MESH(this->m_pImpl)->sizeVertexData = sizeVertexData;
        GI_IMP_MESH(this->m_pImpl)->vertexs = new double[GI_IMP_MESH(this->m_pImpl)->sizeVertexData];
    }
    else if (sizeVertexData > GI_IMP_MESH(this->m_pImpl)->sizeVertexData)
    {
        delete[] GI_IMP_MESH(this->m_pImpl)->vertexs;
        GI_IMP_MESH(this->m_pImpl)->sizeVertexData = sizeVertexData;
        GI_IMP_MESH(this->m_pImpl)->vertexs = new double[GI_IMP_MESH(this->m_pImpl)->sizeVertexData];
    }

    /** 设置顶点 */
    GI_IMP_MESH(this->m_pImpl)->vertexLength = 0;
    for (int i = 0; i < GI_IMP_MESH(this->m_pImpl)->geometrys.length(); i++) {
        GiTriangleMeshGeometry* pTriangleMeshGeometry = (GiTriangleMeshGeometry*)GI_IMP_MESH(this->m_pImpl)->geometrys[i];
        if (pTriangleMeshGeometry->isErased() == true) {
            continue;
        }
        if (pTriangleMeshGeometry->visibility() == false) {
            continue;
        }
        GI_IMP_MESH(this->m_pImpl)->vertexs[GI_IMP_MESH(this->m_pImpl)->vertexLength++] = pTriangleMeshGeometry->vertex(0).x;
        GI_IMP_MESH(this->m_pImpl)->vertexs[GI_IMP_MESH(this->m_pImpl)->vertexLength++] = pTriangleMeshGeometry->vertex(0).y;
        GI_IMP_MESH(this->m_pImpl)->vertexs[GI_IMP_MESH(this->m_pImpl)->vertexLength++] = pTriangleMeshGeometry->vertex(0).z;

        GI_IMP_MESH(this->m_pImpl)->vertexs[GI_IMP_MESH(this->m_pImpl)->vertexLength++] = pTriangleMeshGeometry->vertex(1).x;
        GI_IMP_MESH(this->m_pImpl)->vertexs[GI_IMP_MESH(this->m_pImpl)->vertexLength++] = pTriangleMeshGeometry->vertex(1).y;
        GI_IMP_MESH(this->m_pImpl)->vertexs[GI_IMP_MESH(this->m_pImpl)->vertexLength++] = pTriangleMeshGeometry->vertex(1).z;

        GI_IMP_MESH(this->m_pImpl)->vertexs[GI_IMP_MESH(this->m_pImpl)->vertexLength++] = pTriangleMeshGeometry->vertex(2).x;
        GI_IMP_MESH(this->m_pImpl)->vertexs[GI_IMP_MESH(this->m_pImpl)->vertexLength++] = pTriangleMeshGeometry->vertex(2).y;
        GI_IMP_MESH(this->m_pImpl)->vertexs[GI_IMP_MESH(this->m_pImpl)->vertexLength++] = pTriangleMeshGeometry->vertex(2).z;
    }
}

#if EMSDK
emscripten::val GiMesh::vertexs()
{
    return val(typed_memory_view(GI_IMP_MESH(this->m_pImpl)->vertexLength, GI_IMP_ENTITY(this->m_pImpl)->vertexs));
}
#else
double *GiMesh::vertexs()
{
    return GI_IMP_MESH(this->m_pImpl)->vertexs;
}
#endif
GiTriangleMeshGeometry *GiMesh::appendTriangleMeshGeometry(const GePoint3d &_vertex1, const GePoint3d &_vertex2, const GePoint3d &_vertex3)
{

    GiTriangleMeshGeometry *pTriangleMeshGeometry = new GiTriangleMeshGeometry();
    pTriangleMeshGeometry->setVertex(0, _vertex1);
    pTriangleMeshGeometry->setVertex(1, _vertex2);
    pTriangleMeshGeometry->setVertex(2, _vertex3);

    this->appendTriangleMeshGeometry(pTriangleMeshGeometry);

    return pTriangleMeshGeometry;
}
void GiMesh::appendTriangleMeshGeometry(GiTriangleMeshGeometry *triangleMesh)
{
    GI_IMP_ENTITYGEOMETRY(triangleMesh->m_pImpl)->giEntity = this;
    GI_IMP_MESH(this->m_pImpl)->geometrys.append(triangleMesh);
    GI_IMP_MESH(this->m_pImpl)->bUpdateVertex = true;
}