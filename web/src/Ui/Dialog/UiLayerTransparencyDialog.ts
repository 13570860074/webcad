import LayerTransparencyDialog from "@/components/LayerTransparencyDialog.vue";
import { UiDialog } from "./UiDialog";


export class UiLayerTransparencyDialog extends UiDialog {
    constructor() {
        super();
        this.setTitle("图层透明度");
    }

    private m_value: number = 0;

    /** 透明度 */
    alpha(): number {
        return this.m_value;
    }

    /** 设置透明度 */
    setAlpha(value: number): void {
        this.m_value = value;
    }

    /** 执行 */
    async exec(): Promise<void> {
        await super.exec(LayerTransparencyDialog);
    }
}