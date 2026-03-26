import { UiPaletteSetDialog } from "./UiPaletteSetDialog";
import LayerPropertyDialog from "@/components/LayerPropertyDialog.vue";


export enum ELayerPropertyDialogEvent {
    kInitDone = 0,			   // 初始化完成
    kCreateLayerDone,		   // 创建图层完成
    kDeleteLayerDone,		   // 删除图层完成
    kSetLayerNameDone,		   // 设置图层名称完成
    kSetCurLayerDone,		   // 设置当前图层完成
    kSetLayerOffDone,		   // 设置图层开关完成
    kSetLayerFrozenDone,	   // 设置图层冻结完成
    kSetLayerLockedDone,	   // 设置图层锁定完成
    kSetLayerPlottableDone,	   // 设置图层打印完成
    kSetLayerColorDone,		   // 设置图层颜色完成
    kSetLayerLineWeightDone,   // 设置图层线宽完成
    kSetLayerTransparencyDone, // 设置图层透明度完成
    kSetLayerDescriptionDone,  // 设置图层说明完成
    kSetMergeLayerDone,		   // 设置合并图层完成

    kUpdateLayerTable,			  // 更新图层表
    kCreateLayer,				  // 创建图层
    kDeleteLayer,				  // 删除图层
    kSetCurLayer,				  // 设置当前图层
    kSetLayerName,				  // 设置图层名称
    kSetLayerOff,				  // 设置图层开关
    kSetLayerFrozen,			  // 设置图层冻结
    kSetLayerLocked,			  // 设置图层锁定
    kSetLayerPlottable,			  // 设置图层打印
    kSetLayerColor,				  // 设置图层颜色
    kSetLayerLineWeight,		  // 设置图层线宽
    kSetLayerTransparency,		  // 设置图层透明度
    kSetLayerDescription,		  // 设置图层说明
    kSetAllViewFrozen,			  // 设置在所有视口中冻结(布局空间中使用)
    kSetExcludeCurViewFrozen,	  // 设置除当前视口外冻结(布局空间中使用)
    kSetLayerInAllViewThawFrozen, // 设置图层在所有视口中解冻
    kSetIsolateSelectedLayers,	  // 设置隔离选中的图层
    kSetMergeLayer,				  // 设置合并图层(将源图层合并到目标图层)
}


export class UiLayerPropertyDialog extends UiPaletteSetDialog {
    constructor() {
        super();
    }

    async exec(): Promise<void> {
        await super.exec(LayerPropertyDialog);
    }
}