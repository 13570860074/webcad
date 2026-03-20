#include "GeVertexGenerator.h"
#include <cmath>

GeVertexGenerator::GeVertexGenerator()
{
}
GeVertexGenerator::~GeVertexGenerator()
{
}

void GeVertexGenerator::createBox(const GePoint3d &pos, double length, double width, double height, GePoint3dArray &vertexs, AcArray<unsigned> &indexs)
{
	const double halfLength = std::fabs(length) * 0.5;
	const double halfWidth = std::fabs(width) * 0.5;
	const double halfHeight = std::fabs(height) * 0.5;

    // 顶点
    vertexs.append(GePoint3d(pos.x - halfLength, pos.y - halfWidth, pos.z - halfHeight));
    vertexs.append(GePoint3d(pos.x + halfLength, pos.y - halfWidth, pos.z - halfHeight));
    vertexs.append(GePoint3d(pos.x + halfLength, pos.y + halfWidth, pos.z - halfHeight));
    vertexs.append(GePoint3d(pos.x - halfLength, pos.y + halfWidth, pos.z - halfHeight));
    vertexs.append(GePoint3d(pos.x - halfLength, pos.y - halfWidth, pos.z + halfHeight));
    vertexs.append(GePoint3d(pos.x + halfLength, pos.y - halfWidth, pos.z + halfHeight));
    vertexs.append(GePoint3d(pos.x + halfLength, pos.y + halfWidth, pos.z + halfHeight));
    vertexs.append(GePoint3d(pos.x - halfLength, pos.y + halfWidth, pos.z + halfHeight));

    // 索引
    static const unsigned kFaces[] = {
        0, 1, 2, 0, 2, 3,
        0, 4, 5, 0, 5, 1,
        1, 5, 6, 1, 6, 2,
        2, 6, 7, 2, 7, 3,
        3, 7, 4, 3, 4, 0,
        4, 7, 6, 4, 6, 5
    };
    for (unsigned i = 0; i < sizeof(kFaces) / sizeof(kFaces[0]); ++i) {
        indexs.append(kFaces[i]);
    }
}