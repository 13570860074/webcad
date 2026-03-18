#ifndef GIMESH_H
#define GIMESH_H


#include "GiEntity.h"
#include "GiTriangleMeshGeometry.h"



class GiMesh : public GiEntity {
public:
    GiMesh();
    virtual ~GiMesh();
    RX_DECLARE_MEMBERS(GiMesh);

public:

    /** 获得类型 */
    virtual Gi::EntityType type()const;

    /** 更新 */
    virtual void update();

    /** 获得顶点数据 */
#if EMSDK
    virtual emscripten::val vertexs();
#else
    virtual double* vertexs();
#endif

    /** 添加三角网格几何数据 */
    void appendTriangleMeshGeometry(GiTriangleMeshGeometry* triangleMesh);
    GiTriangleMeshGeometry *appendTriangleMeshGeometry(const GePoint3d& _vertex1, const GePoint3d& _vertex2, const GePoint3d& _vertex3);
};




#endif