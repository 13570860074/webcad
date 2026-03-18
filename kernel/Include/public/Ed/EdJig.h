#ifndef EDJIG_H
#define EDJIG_H


#include "adesk.h"
#include "acadstrc.h"
#include "RxObject.h"
#include "GePoint3d.h"
#include "DbObjectId.h"
#include "AcString.h"
#include "DbDimData.h"

class  EdDragStyle
{
public:

    /// <summary>
    /// Enumeration controlling the graphics representation of the drag entity during dragging process.
    /// </summary>
    enum StyleType
    {
        /// <summary> Dragger will not modify the entity's visual style during drag. </summary>
        kNone = 0,

        /// <summary> Entity will be hidden during drag. </summary>
        kHide,

        /// <summary> The dragger will apply 25% transparency to entity. </summary>
        kTransparent25,

        /// <summary> The dragger will apply 75% transparency to entity. </summary>
        kTransparent75,

        /// <summary> The dragger will apply the deletion effect to entity. </summary>
        kDeletedEffect,

        /// <summary> The dragger will apply highlight effect to entity. </summary>
        kHighlight,

        /// <summary> Style was set by client calling setVisualStyle(). </summary>
        kNotSet,

        /// <summary> For internal use only. </summary>
        kOpacity
    };

    /// <summary> Default constructor. The entity will be displayed in its default visual
    /// style.  This is equivalent to setting the style type to kNone. </summary>
    EdDragStyle();

    /// <summary> Construct by specifying style types for original and dragged entity. </summary>
    EdDragStyle(StyleType styleTypeForOriginal, StyleType styleTypeForDragged);

    /// <summary> Copy constructor. </summary>
    EdDragStyle(const EdDragStyle& dragStyle);

    /// <summary> Destructor. </summary>
    ~EdDragStyle();

    /// <summary> Return the style type that was used to define the original entity's visual style.</summary>
    /// <returns> StyleType enum value that was used to define the original entity's visual style.</returns>
    StyleType             styleTypeForOriginal() const;

    /// <summary> Return the style type that was used to define the dragged entity's visual style.</summary>
    /// <returns> StyleType enum value that was used to define the dragged entity's visual style.</returns>
    StyleType             styleTypeForDragged() const;

    /// <summary> Set the original entity's visual style by specifying a style type. </summary>
    /// <param name="styleType"> Style type for the original entity.</param>
    /// <returns> eOk if sucessful.</returns>
    Acad::ErrorStatus     setStyleTypeForOriginal(StyleType styleType);

    /// <summary> Set the dragged entity's visual style by specifying a style type. </summary>
    /// <param name="styleType"> Style type for the dragged entity.</param>
    /// <returns> eOk if sucessful.</returns>
    Acad::ErrorStatus     setStyleTypeForDragged(StyleType styleType);

    /// <summary> Assignment operator. </summary>
    EdDragStyle& operator = (const EdDragStyle& src);

    /// <summary> Test equality</summary>
    bool operator == (const EdDragStyle& other) const;
    bool operator != (const EdDragStyle& other) const;
};


class DbEntity;
class DbDimData;
class EdJig : public RxObject
{
public:
    RX_DECLARE_MEMBERS(EdJig);

    enum UserInputControls {

        /* 是否启用正交(只有开启了捕捉正交才有效) */
        kGovernedByOrthoMode = 0x000001,       
        
        /*  接受空输入(开启后进行空输入返回"kNull"),
        *   如果不开启则不会返回(这时如果开启了接受字符串输入会返回"kOther")
        */
        kNullResponseAccepted = 0x000002,

        /*  返回"kCancel"取消操作时不显示"*cancel提示" */
        kDontEchoCancelForCtrlC = 0x000004,

        /* 禁止更新最后一个点 */
        kDontUpdateLastPoint = 0x000008,

        /* 不检查图形界限 */
        kNoDwgLimitsChecking = 0x000010,

        /* 禁止输入0值 */
        kNoZeroResponseAccepted = 0x000020,

        /* 禁止接收否定答案? */
        kNoNegativeResponseAccepted = 0x000040,

        /* 接受三维点输入(未开启的情况下输入三个坐标不会返回,并提示"二维点无效。") */
        kAccept3dCoordinates = 0x000080,

        /* 接受鼠标按键松开为选择点(默认状态下鼠标点击为选择点) 
        * 开启后除了鼠标点击会选择点外,鼠标按键松开也会选择点(但要求和点击时的点不一样)
        */
        kAcceptMouseUpAsPoint = 0x000100, 
        
        /* 任何空白终止输入?(未知) */
        kAnyBlankTerminatesInput = 0x000200,

        /* 初始空白终止输入?(未知) */
        kInitialBlankTerminatesInput = 0x000400,

        /*  该选项表接受可以输入字符串输入返回"kOther"包括空输入也返回"kOther",
        *   但是同时开启了"kNullResponseAccepted"进行空输入会返回"kNull",
        *   如果不设置该项输入非数值输入都会提示输入无效(acquireString获取到的字符串永远为空),不会返回)
        */
        kAcceptOtherInputString = 0x000800,

        /* (未知) */
        kGovernedByUCSDetect = 0x001000,

        /*  禁用Z方向正交(如果启用该选项,开启正交后,
        *   拖拽点只能限定在XY平面中,输入坐标无影响) 
        */
        kNoZDirectionOrtho = 0x002000,
        
        /* 未知 */
        kImpliedFaceForUCSChange = 0x004000,

        /* 未知 */
        kUseBasePointElevation = 0x008000,

        ///<summary> Disables direct distance input. When this flag is ON a
        ///distance input such as integer or real will not be accepted unless
        ///kAcceptOtherInputString is ON, in that case returned DragStatus is
        ///kOther. </summary>
        kDisableDirectDistanceInput = 0x010000,
    };

    enum DragStatus {
        kModeless = -17,
        kNoChange = -6,
        kCancel = -4,
        kOther = -3,
        kNull = -1,
        kNormal = 0,
        kKW1,
        kKW2,
        kKW3,
        kKW4,
        kKW5,
        kKW6,
        kKW7,
        kKW8,
        kKW9
    };

    /** 
    注:一下起作用的只有
        kNoSpecialCursor:无特殊光标
        kCrosshair:十字光标
        kRectCursor:拐角拖动样式
        kRubberBand:带拖拽线的十字光标
        kEntitySelect:正方形的选择框
    */
    enum CursorType {
        kNoSpecialCursor = -1,           // No Special Cursor Specified (默认为十字光标)
        kCrosshair = 0,                  // Full Screen Cross Hair.(十字光标)
        kRectCursor,                     // Rectangular cursor. (拐角拖动样式)
        kRubberBand,                     // Rubber band line. (带拖拽线的十字光标)
        kNotRotated,                     // NotRotated Type. (隐藏正方形选择框不隐藏光标,显示极轴捕捉)
        kTargetBox,                      // Target Box Type. 
        kRotatedCrosshair,               // Rotated Crosshair w/ rubber band. 
        kCrosshairNoRotate,              // Crosshairs forced non-rotated. 
        kInvisible,                      // Invisible cursor. 
        kEntitySelect,                   // Entity selection target cursor. (正方形的选择框)
        kParallelogram,                  // Parallelogram cursor. 
        kEntitySelectNoPersp,            // Pickbox, suppressed in persp. 
        kPkfirstOrGrips,                 // Auto-select cursor. (光标为默认样式,显示动态捕捉中的极轴捕捉)
        kCrosshairDashed,               // 15 dashed style crosshair cursor(光标隐藏,显示动态捕捉中的极轴捕捉)
        kSelectionSet,                  // 内部使用
    };
    EdJig();
    virtual ~EdJig();

    DragStatus drag();
    DragStatus drag(const EdDragStyle& style);
    virtual DragStatus sampler();
    virtual Adesk::Boolean update();
    DbObjectId append();

    const ACHAR* keywordList();
    void        setKeywordList(const ACHAR*);

    const ACHAR* dispPrompt();
    void        setDispPrompt(const ACHAR*, ...);


    DragStatus acquireString(AcString& s);
    // This overload is deprecated and will be removed. Please use the above overload instead
    DragStatus acquireString(ACHAR(&str)[2049]);
    DragStatus acquireAngle(double& ang);
    DragStatus acquireAngle(double& ang, const GePoint3d& basePnt);
    DragStatus acquireDist(double& dist);
    DragStatus acquireDist(double& dist, const GePoint3d& basePnt);
    DragStatus acquirePoint(GePoint3d&);
    DragStatus acquirePoint(GePoint3d&, const GePoint3d& basePnt);

    EdJig::CursorType specialCursorType();
    void                setSpecialCursorType(CursorType);

    EdJig::UserInputControls userInputControls();
    void                       setUserInputControls(EdJig::UserInputControls);

    virtual DbEntity* entity() const;
    virtual DbDimDataPtrArray* dimData(const double dimScale);
    virtual Acad::ErrorStatus setDimValue(const DbDimData* dimData, const double dimValue);

#if EMSDK
    const std::string emsdk_keywordList();
    const std::string emsdk_dispPrompt();
#endif
};


#endif