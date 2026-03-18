#ifndef RECAMERA_H
#define RECAMERA_H


#include "RxObject.h"
#include "GeMatrix3d.h"


class ReCamera : public RxObject {
public:
    ReCamera();
    virtual ~ReCamera();
    RX_DECLARE_MEMBERS(ReCamera);


public:

    // 信息
    virtual const std::string information();

    // 获得矩阵
    GeMatrix3d matrix() const;

    // 设置矩阵
    void setMatrix(const GeMatrix3d& _mat);
};




#endif