#ifndef GSCOORDSYSTEM_H
#define GSCOORDSYSTEM_H

#include "GsElement.h"
#include "EdMouseEvent.h"

class DbObject;
class ApDocument;
class GsCoordSystem : public GsElement
{
public:
    GsCoordSystem();
    virtual ~GsCoordSystem();
    RX_DECLARE_MEMBERS(GsCoordSystem);


    enum ECoordSystemStyle
    {
        kOrdinaryCoordSys,  // 普通坐标系
        kLayoutCoordSys,    // 布局坐标系
    };

public:

    /** 初始化 */
    virtual void onInit();

    /** 更新 */
    void update();

    /** 光标样式 */
    ECoordSystemStyle coordSystemStyle() const;

    /** 设置光标样式 */
    void setCoordSystemStyle(const ECoordSystemStyle _style);

    /** 判断坐标系是否在视口范围内 */
    bool extents(const DbObject* viewport) const;

    /** 是否在进行拖拽(拖拽时不显示中间方形框) */
    bool isDrag() const;

    /** 设置是否在进行拖拽(拖拽时不显示中间方形框) */
    void setDrag(const bool isDrag);
};

#endif