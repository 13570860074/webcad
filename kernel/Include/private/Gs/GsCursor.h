#ifndef GSCURSOR_H
#define GSCURSOR_H

#include "GsElement.h"
#include "EdMouseEvent.h"

class ApDocument;
class GsCursor : public GsElement
{
public:
    GsCursor();
    virtual ~GsCursor();
    RX_DECLARE_MEMBERS(GsCursor);

    enum ECursorStyle
    {
        kNone,                          // 无光标
        kTarget = 0x000001,             // 靶框
        kCrossCursor = 0x000002,        // 十字光标
        kSelectionSet = 0x000004,        // 选择集
        kCorner = 0x000008,              // 拐角
    };

public:

    /** 初始化 */
    virtual void onInit();

    /** 刷新 */
    void update();

    /** 是否启用了光标 */
    bool isEnable( )const;

    /** 设置是否被启用 */
    void setEnable(const bool bEnable);

    /** 光标样式 */
    ECursorStyle cursorStyle() const;

    /** 设置光标样式 */
    void setCursorStyle(const ECursorStyle _style);
};

#endif