#ifndef REORTHOGONALCAMERA_H
#define REORTHOGONALCAMERA_H


#include "ReCamera.h"


class ReOrthogonalCamera : public ReCamera {
public:
    ReOrthogonalCamera();
    virtual ~ReOrthogonalCamera();
    RX_DECLARE_MEMBERS(ReOrthogonalCamera);


public:

    // 信息
    virtual const std::string information();

    //获得范围
    double top();
    double bottom();
    double left();
    double right();

    //设置范围
    void setTop(double _v);
    void setBottom(double _v);
    void setLeft(double _v);
    void setRight(double _v);

    //获得左下角点
    GePoint2d lowerLeftCorner() const;

    //获得右下角点
    GePoint2d upperRightCorner() const;

    //设置左下角点
    void setLowerLeftCorner(const GePoint2d& _point);

    //设置右下角点
    void setUpperRightCorner(const GePoint2d& _point);
};




#endif