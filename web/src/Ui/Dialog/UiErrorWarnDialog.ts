import ErrorWarnDialog from "@/components/ErrorWarnDialog.vue";
import { UiDialog } from "./UiDialog";


export class UiErrorWarnDialog extends UiDialog {
    constructor() {
        super();
    }

    private m_content: string = "";
    private m_titlePrompt: string = "";
    private m_prompts: Array<string> = new Array<string>();

    /** 内容(蓝色大字提醒) */
    content(): string {
        return this.m_content;
    }

    /** 设置内容(蓝色大字提醒) */
    setContent(content: string): void {
        this.m_content = content;
    }

    /** 提示标题 */
    titlePrompt(): string {
        return this.m_titlePrompt;
    }

    /** 设置提示标题 */
    setTitlePrompt(v: string): void {
        this.m_titlePrompt = v;
    }

    /** 提示 */
    prompts(): Array<string> {
        return this.m_prompts;
    }

    /** 执行 */
    async exec(): Promise<void> {
        let width = this.content().length * 16;
        if(width < this.titlePrompt().length * 12){
            width = this.titlePrompt().length * 12;
        }
        if(width < this.width()){
            width = this.width();
        }
        let titleHeight = this.titleHeight() + 5 + 30 + 5 + 20 + this.prompts().length * 20 + 10 + 10 + 20 + 10;
        this.setSize(width, titleHeight);
        await super.exec(ErrorWarnDialog);
    }
}