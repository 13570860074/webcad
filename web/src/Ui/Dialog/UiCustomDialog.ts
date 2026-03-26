import { UiDialog } from "./UiDialog";
import { createApp } from 'vue';

export class UiCustomDialog extends UiDialog {
    constructor() {
        super();
    }

    /** 创建模态对话框 */
    async exec(): Promise<void>;
    async exec(assembly: any): Promise<void>;
    async exec(param1: any = null): Promise<void> {

        if(param1 == null){
            return;
        }

        let gcadElement: HTMLDivElement = <HTMLDivElement>document.getElementById('gcad');
        if (gcadElement != null) {
            const app = createApp(param1);
            const dialogElement = document.createElement('div');
            dialogElement.style.zIndex = '3';
            dialogElement.style.position = 'absolute';
            dialogElement.style.userSelect = 'none';
            dialogElement.style.top = '0px';
            dialogElement.style.left = '0px';
            dialogElement.style.width = gcadElement.style.width;
            dialogElement.style.height = gcadElement.style.height;
            dialogElement.style.display = 'flex';
            gcadElement.appendChild(dialogElement);
            app.mount(dialogElement);
        }
    }
}