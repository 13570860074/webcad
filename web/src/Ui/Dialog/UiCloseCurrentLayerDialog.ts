import CloseCurrentLayerDialog from "@/components/CloseCurrentLayerDialog.vue";
import { UiDialog } from "./UiDialog";


export class UiCloseCurrentLayerDialog extends UiDialog {
    constructor() {
        super();
    }

    private m_state: number = 0;

    /** 状态(0:关闭,1:不关闭) */
    state(): number {
        return this.m_state;
    }

    /** 设置状态 */
    setState(state: number): void {
        this.m_state = state;
    }

    /** 执行 */
    async exec(): Promise<void> {
        this.setSize(334, 180);
        await super.exec(CloseCurrentLayerDialog);
    }
}