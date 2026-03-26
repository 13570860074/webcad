import SelectColorDialog from "@/components/SelectColorDialog.vue";
import { UiDialog } from "./UiDialog";


export class UiSelectColorDialog extends UiDialog {
    constructor() {
        super();
    }


    private m_colorIndex: number = 256;
    m_funConfirm: (colorIndex: number) => void = null;
    private m_disableLayerInput: boolean = false;
    private m_disableBlockInput: boolean = false;

    /** 颜色 */
    colorIndex(): number {
        return this.m_colorIndex;
    }

    /** 设置颜色 */
    setColorIndex(colorIndex: number): void {
        this.m_colorIndex = colorIndex;
    }

    /** 是否禁用块输入 */
    isDisableLayerInput(): boolean {
        return this.m_disableLayerInput;
    }

    /** 是否禁用块输入 */
    isDisableBlockInput(): boolean {
        return this.m_disableBlockInput;
    }

    /** 设置禁用块输入 */
    setDisableLayerInput(disable: boolean): void {
        this.m_disableLayerInput = disable;
    }

    /** 设置禁用块输入 */
    setDisableBlockInput(disable: boolean): void {
        this.m_disableBlockInput = disable;
    }

    /** 监听确定事件 */
    onConfirm(fun: (colorIndex: number) => void): void {
        this.m_funConfirm = fun;
    }

    async exec(): Promise<void> {
        await super.exec(SelectColorDialog);
    }
}