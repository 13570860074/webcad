import { FManager } from "../Application/FManager";
import { FEvent } from "../Core/FEvent";



export enum EventState {


    // 与kernel交互的事件
    /** ------------------------------------------------------------------ */

    // window
    kMessage,                       // 监听到消息事件

    /* 文档事件 */
    kDocumentCreated,               // 创建文档
    kDocumentActivated,             // 激活文档
    kDocumentToBeDestroyed,         // 文档开始删除
    kDocumentDestroyed,             // 文档删除

    /* 渲染事件 */
    kBeginAnimationFrame,
    kEndAnimationFrame,
    kBeginRenderer,                 // 开始渲染
    kEndRenderer,                   // 完成渲染
    kClearCanvas,                   // 清理画布
    kClearWorkCanvas,               // 清理工作画布
    kBeginRendererViewport,         // 开始渲染视口
    kEndRendererViewport,           // 渲染视口完成
    kRendererViewport,              // 渲染视口

    /* 实体事件 */
    kCreateEntity,                  // 创建实体
    kUpdateVertexs,                 // 更新实体顶点
    kTransformation,                // 对实体进行矩阵变换
    kVisibility,                    // 设置实体可见性

    kCreateEntityRecord,            // 创建实体记录(这里是由js发出)
    kUpdateEntityVertex,            // 更新实体顶点(这里是由js发出)

    /* 材质事件 */
    kCreateMaterial,                // 创建材质事件

    /* 布局事件 */
    kLayoutCreated,
    kLayoutToBeRemoved,
    kLayoutRemoved,
    kAbortLayoutRemoved,
    kLayoutToBeCopied,
    kLayoutCopied,
    kAbortLayoutCopied,
    kLayoutToBeRenamed,
    kLayoutRenamed,
    kAbortLayoutRename,
    kLayoutSwitched,
    kPlotStyleTableChanged,
    kLayoutsReordered,
    kRefreshLayoutTabs,

    /* 编辑反应器 */
    kUnknownCommand,
    kCommandWillStart,              // 命令将要开始
    kCommandEnded,                  // 命令结束
    kCommandCancelled,              // 命令取消
    kCommandFailed,                 // 命令失败
    kSysVarChanged,                 // 系统变量改变
    kSysVarWillChange,              // 系统变量将要改变

    /** 拖拽事件 */
    kBeginDrag,					    // 开始拖拽
    kEndDrag,						// 结束拖拽
    kUserInput,						// 用户输入
    kBeginUpdate,					// 开始更新
    kEndUpdate,						// 结束更新
    
    kEdJigBeginDrag,				// EdJig开始拖拽
    kJigUpdateDynDim,				// EdJig更新动态尺寸
    kEdJigEndDrag,					// EdJig结束拖拽

    /* Ui交互事件 */
    kCreateCustomDialog,            // 创建自定义对话框
    kUpdateLayoutData,              // 更新布局数据
    kPrintMsgbar,                   // 打印消息栏
    kPrintGraphCoords,              // 打印图形坐标
    kUpdateCommandlineState,        // 更新命令行状态(0:不启用命令行,1:启用普通命令行,2:启用动态命令行)
    kUpdateInterfaceState,          // 更新交互状态

    /** 交互事件 */
    kMouseenterWork,                // 鼠标进入工作区
    kMouseoutWork,                  // 鼠标离开工作区
    kMousedown,                     // 鼠标按下
    kMouseup,                       // 鼠标抬起
    kMousemove,                     // 鼠标移动
    kMousewheel,                    // 鼠标滚轮
    kMouseleave,                    // 鼠标离开
    kMouseenter,                    // 鼠标进入
    kMouseclick,                    // 鼠标点击
    kMouseDbclick,                  // 鼠标双击
    kKeydown,                       // 键盘按下
    kKeyup,                         // 键盘弹起
    kTouchESC,                      // 触摸按下ESC事件

    // 特性事件
    kUpdatePropertyTable,           // 更新特性表

    /** ------------------------------------------------------------------ */



    /** ------------------------------------------------------------------ */

    kVueLoadDone,                   // vue加载完成事件
    kRuntimeInitialized,            // wasm模块加载完成事件

    kFocusStateChange,              // 焦点状态改变事件(焦点在画布上为state为0,在对话框上时为1,在弹出面板上为2)
    kPopupComBoxList,               // 弹出下拉框列表

    /** 弹出式面板 */
    kActivePopupPanel,              // 激活弹出面板
    kReActivatePopupPanel,          // 取消激活弹出面板

    /** 菜单栏事件 */
    kUpdateMenubar,                 // 更新菜单栏事件
    kCreateMenuPopups,              // 创建菜单弹出框
    kMenuPopupsClose,               // 关闭菜单弹出框

    /** ribbon事件 */
    kUpdateRibbon,                  // 更新ribbon事件

    /** 对话框事件 */
    kCreateDialog,                  // 创建对话框
    kCloseDialog,                   // 关闭对话框
    kFocusDialog,                   // 对话框获得焦点事件
    kBlurDialog,                    // 对话框失去焦点事件

    /** ------------------------------------------------------------------ */
}




export class FEventManager extends FManager {
    constructor() {
        super();
    }

    private m_event: FEvent = new FEvent();


    async onInit(): Promise<void> {

    }


    // 监听事件
    on(_event: any, fun: Function): number {
        return this.m_event.on(_event, fun);
    }

    // 触发事件
    emit(_event: number, ...param: any[]): void {
        this.m_event.emit(_event, ...param);
    }

    // 移除事件
    removeEventListenerId(_id: number): void {
        this.m_event.removeEventListenerId(_id);
    }

}