import LineWeightDialog from "@/components/LineWeightDialog.vue";
import { UiDialog } from "./UiDialog";


export class UiLineWeightDialog extends UiDialog {
    constructor() {
        super();
        this.setTitle("线宽");
        this.setSize(250, 310);
    }

    private m_value: number = 0;

    /** 线宽 */
    lineWeight(): number {
        return this.m_value;
    }

    /** 设置线宽 */
    setLineWeight(value: number): void {
        this.m_value = value;
    }

    /** 执行 */
    async exec(): Promise<void> {
        await super.exec(LineWeightDialog);
    }
}