#include "GeVertexGenerator.h"

GeVertexGenerator::GeVertexGenerator()
{
}
GeVertexGenerator::~GeVertexGenerator()
{
}

void GeVertexGenerator::createBox(const GePoint3d &pos, double length, double width, double height, GePoint3dArray &vertexs, AcArray<unsigned> &indexs)
{
    // 顶点
    vertexs.append(GePoint3d(pos.x - length / 2, pos.y - width / 2, pos.z - height / 2));
    vertexs.append(GePoint3d(pos.x + length / 2, pos.y - width / 2, pos.z - height / 2));
    vertexs.append(GePoint3d(pos.x + length / 2, pos.y + width / 2, pos.z - height / 2));
    vertexs.append(GePoint3d(pos.x - length / 2, pos.y + width / 2, pos.z - height / 2));
    vertexs.append(GePoint3d(pos.x - length / 2, pos.y - width / 2, pos.z + height / 2));
    vertexs.append(GePoint3d(pos.x + length / 2, pos.y - width / 2, pos.z + height / 2));
    vertexs.append(GePoint3d(pos.x + length / 2, pos.y + width / 2, pos.z + height / 2));
    vertexs.append(GePoint3d(pos.x - length / 2, pos.y + width / 2, pos.z + height / 2));

    // 索引
    indexs.append(0); indexs.append(1); indexs.append(2);
    indexs.append(0); indexs.append(2); indexs.append(3);
    indexs.append(0); indexs.append(4); indexs.append(5);
    indexs.append(0); indexs.append(5); indexs.append(1);
    indexs.append(1); indexs.append(5); indexs.append(6);
    indexs.append(1); indexs.append(6); indexs.append(2);
    indexs.append(2); indexs.append(6); indexs.append(7);
    indexs.append(2); indexs.append(7); indexs.append(3);
    indexs.append(3); indexs.append(7); indexs.append(4);
    indexs.append(3); indexs.append(4); indexs.append(0);
    indexs.append(4); indexs.append(7); indexs.append(6);
    indexs.append(4); indexs.append(6); indexs.append(5);
}