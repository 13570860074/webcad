import { application } from "./Application/FApplication";
import { EventState } from "./EventManager/FEventManager";



export interface IUiLayoutData {

    /** 设置工作区(相对于dody的尺寸) */
    workLeft?: number,
    workTop?: number,
    workRight?: number,
    workBottom?: number,

    /** 设置画布(相对于工作区的尺寸) */
    canvasLeft?: number,
    canvasTop?: number,
    canvasRight?: number,
    canvasBottom?: number,

    /**菜单栏高度 */
    menubarHeight?: number,

    /**工具栏高度 */
    toolbarHeight?: number,
}
export interface gcadGlobal {
    globalZIndex?: number,
    globalDialogId?: number,
    uiLayoutData: IUiLayoutData,
}


export var gcadGlobal: gcadGlobal = {
    uiLayoutData: null,
    globalZIndex: 1,
    globalDialogId: 0,
};




/** 布局数据 */
export var gcadGlobal_uiLayoutData = (): IUiLayoutData => {
    return gcadGlobal.uiLayoutData;
}

/** 更新布局数据 */
export var gcadGlobal_updateUiLayoutData = (data: IUiLayoutData, isEvent: boolean = true): void => {
    if (data.workLeft != null) {
        gcadGlobal.uiLayoutData.workLeft = data.workLeft;
    }
    if (data.workTop != null) {
        gcadGlobal.uiLayoutData.workTop = data.workTop;
    }
    if (data.workRight != null) {
        gcadGlobal.uiLayoutData.workRight = data.workRight;
    }
    if (data.workBottom != null) {
        gcadGlobal.uiLayoutData.workBottom = data.workBottom;
    }

    if (data.canvasLeft != null) {
        gcadGlobal.uiLayoutData.canvasLeft = data.canvasLeft;
    }
    if (data.canvasTop != null) {
        gcadGlobal.uiLayoutData.canvasTop = data.canvasTop;
    }
    if (data.canvasRight != null) {
        gcadGlobal.uiLayoutData.canvasRight = data.canvasRight;
    }
    if (data.canvasBottom != null) {
        gcadGlobal.uiLayoutData.canvasBottom = data.canvasBottom;
    }

    if (data.menubarHeight != null) {
        gcadGlobal.uiLayoutData.menubarHeight = data.menubarHeight;
    }

    let workWidth = gcadGlobal.uiLayoutData.workRight - gcadGlobal.uiLayoutData.workLeft;
    let workHeight = gcadGlobal.uiLayoutData.workBottom - gcadGlobal.uiLayoutData.workTop;
    let canvasWidth = workWidth;
    let canvasHeight = workHeight - data.menubarHeight;
    gcadGlobal.uiLayoutData.canvasLeft = 0;
    gcadGlobal.uiLayoutData.canvasRight = gcadGlobal.uiLayoutData.canvasLeft + canvasWidth;
    gcadGlobal.uiLayoutData.canvasTop = data.menubarHeight;
    gcadGlobal.uiLayoutData.canvasBottom = gcadGlobal.uiLayoutData.canvasTop + canvasHeight;


    if (isEvent == true) {
        application.eventManager().emit(EventState.kUpdateLayoutData, gcadGlobal.uiLayoutData);
    }
}