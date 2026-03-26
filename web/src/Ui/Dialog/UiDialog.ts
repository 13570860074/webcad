import { FEvent } from "@/Core/FEvent";
import { createApp } from 'vue';
import { gcadGlobal } from '@/gcadGlobal';


export enum EDialogTrigger {
    kDestructor,
    kSetTitle,
    kSetSize,
    kSetFrame,
    kSetTitleHeight,
    kSetBackgroundColor,
    kSetIcon,
    kSetPosition,
};



export abstract class UiDialog {
    constructor() {

    }

    public id: number = 0;
    public trigger: FEvent = null;               // 内部使用的事件器

    private m_width: number = 0;
    private m_height: number = 0;

    private m_bFrame: boolean = true;
    private m_zIndex: number = 0;
    private m_modal: boolean = true;
    private m_title: string = "";
    private m_titleHeight: number = 28;
    private m_position: { x: number, y: number } = { x: -1, y: -1 };

    private event: FEvent = null;

    private m_icon: string = "";
    private m_backgroundColor: { r: number, g: number, b: number } = { r: 249, g: 249, b: 249 };

    private m_resolve: any = null;


    /** 标题 */
    title(): string {
        return this.m_title;
    }

    /** 设置标题 */
    setTitle(title: string): void {
        this.m_title = title;
        if (this.trigger != null) {
            this.trigger.emit(EDialogTrigger.kSetTitle, title);
        }
    }

    /** 获取宽度 */
    width(): number {
        return this.m_width;
    }

    /** 获取高度 */
    height(): number {
        return this.m_height;
    }

    /** 设置尺寸 */
    setSize(width: number, height: number): void {
        this.m_width = width;
        this.m_height = height;
        if (this.trigger != null) {
            this.trigger.emit(EDialogTrigger.kSetSize, { width: width, height: height });
        }
    }

    /** 是否使用边框 */
    isFrame(): boolean {
        return this.m_bFrame;
    }

    /** 设置是否使用边框 */
    setFrame(b: boolean): void {
        this.m_bFrame = b;
        if (this.trigger != null) {
            this.trigger.emit(EDialogTrigger.kSetFrame, b);
        }
    }

    /** 获取标题栏高度 */
    titleHeight(): number {
        return this.m_titleHeight;
    }

    /** 设置标题栏高度 */
    setTitleHeight(height: number): void {
        this.m_titleHeight = height;
        if (this.trigger != null) {
            this.trigger.emit(EDialogTrigger.kSetTitleHeight, height);
        }
    }

    /** 获取背景颜色 */
    backgroundColor(): { r: number, g: number, b: number } {
        return this.m_backgroundColor;
    }

    /** 设置背景颜色 */
    setBackgroundColor(r: number, g: number, b: number): void {
        this.m_backgroundColor = { r: r, g: g, b: b };
        if (this.trigger != null) {
            this.trigger.emit(EDialogTrigger.kSetBackgroundColor, this.m_backgroundColor);
        }
    }

    /** 获取图标 */
    icon(): string {
        return this.m_icon;
    }

    /** 设置图标 */
    setIcon(icon: string): void {
        this.m_icon = icon;
        if (this.trigger != null) {
            this.trigger.emit(EDialogTrigger.kSetIcon, icon);
        }
    }

    /** 获取位置 */
    position(): { x: number, y: number } {
        return this.m_position;
    }

    /** 设置位置 */
    setPosition(x: number, y: number): void {
        this.m_position = { x: x, y: y };
        if (this.trigger != null) {
            this.trigger.emit(EDialogTrigger.kSetPosition, this.m_position);
        }
    }

    /** 是否模态对话框 */
    isModal(): boolean {
        return this.m_modal;
    }

    /** 设置是否是模态对话框 */
    setModal(m: boolean): void {
        this.m_modal = m;
    }

    /** 关闭对话框 */
    close(): void {

        /** 触发外部的关闭事件 */
        this.event.emit('close', this);

        /** 触发析构函数 */
        this.trigger.emit(EDialogTrigger.kDestructor, this);

        /** 释放ZIndex */
        if (gcadGlobal.globalZIndex === this.m_zIndex) {
            gcadGlobal.globalZIndex--;
        }

        /** 返回 */
        this.m_resolve();

        /** 移除组件 */
        let gcadElement: HTMLDivElement = <HTMLDivElement>document.getElementById('gcad');
        if (gcadElement != null) {
            let dialogElement: HTMLDivElement = <HTMLDivElement>document.getElementById(`DialogBox${this.id}`);
            if (dialogElement != null) {
                gcadElement.removeChild(document.getElementById(`DialogBox${this.id}`));
            }
        }
    }

    /**
     * 监听事件
     */
    on(event: "close", fun: Function): void {
        this.event.on(event, fun);
    }

    /** 创建模态对话框 */
    async exec(): Promise<void>;
    async exec(url: string): Promise<void>;
    async exec(component: any): Promise<void>;
    async exec(param1: any = null): Promise<void> {
        return new Promise(async (resolve) => {
            this.m_resolve = resolve;
            this.event = new FEvent();
            this.trigger = new FEvent();
            this.id = gcadGlobal.globalDialogId;
            gcadGlobal.globalDialogId++
            this.m_zIndex = gcadGlobal.globalZIndex;
            gcadGlobal.globalZIndex++;

            if (typeof param1 === 'string') {

            }
            else if (param1 != null) {

                let gcadElement: HTMLDivElement = <HTMLDivElement>document.getElementById('gcad');
                if (gcadElement != null) {
                    const app = createApp(param1, { data: this });
                    const dialogElement = document.createElement('div');
                    dialogElement.id = `DialogBox${this.id}`;
                    dialogElement.style.zIndex = `${this.m_zIndex}`;
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

            if (this.isModal() == false) {
                resolve();
            }
        });
    }
}