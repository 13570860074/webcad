#ifndef GEVERTEXGENERATOR_H
#define GEVERTEXGENERATOR_H

#include "GePoint3d.h"
#include "GePoint3dArray.h"

class GeVertexGenerator
{
public:
    GeVertexGenerator();
    virtual ~GeVertexGenerator();

    /** 创建包围盒 
     * @param pos 盒子中心点
     * @param length 盒子长度
     * @param width 盒子宽度
     * @param height 盒子高度
     * @param vertexs 顶点数组
     * @param indexs 索引数组
    */
    static void createBox(const GePoint3d &pos, double length, double width, double height, GePoint3dArray &vertexs, AcArray<unsigned> &indexs);
};

#endif
