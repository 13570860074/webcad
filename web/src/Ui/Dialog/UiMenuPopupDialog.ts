import MenuPopupDialog from "@/components/MenuPopupDialog.vue";
import { UiDialog } from "./UiDialog";
import { UiMenuPopup } from "../UiMenuPopup";


export class UiMenuPopupDialog extends UiDialog {
    constructor() {
        super();
        super.setFrame(false);
    }

    private m_data: UiMenuPopup = null;
    private m_clientX: number = 0;
    private m_clientY: number = 0;


    /** 弹出菜单数据 */
    data(): UiMenuPopup {
        return this.m_data;
    }

    /** 设置数据 */
    setData(data: UiMenuPopup): void {
        this.m_data = data;
    }

    /** 获得位置 */
    clientX(): number {
        return this.m_clientX;
    }
    clientY(): number {
        return this.m_clientY;
    }

    /** 设置位置 */
    setClientX(v: number): void {
        this.m_clientX = v;
    }
    setClientY(v: number): void {
        this.m_clientY = v;
    }

    async exec(): Promise<void> {
        let width: number = super.width();
        let height: number = super.height();
        super.setSize(width - super.position().x, height - super.position().y);
        await super.exec(MenuPopupDialog);
    }
}