import Ed from '../kernel/Ed';
import { FManager } from '../Application/FManager';
import { application } from "../Application/FApplication";
import { EventState } from '@/EventManager/FEventManager';
import { EPointerType, ITouch, ITouchEvent, IMouseEvent, IMouseWheelEvent, IKeyboardEvent } from './IUserInputEvent';
import { gcadGlobal } from '@/gcadGlobal';



export class FUserInputManager extends FManager {
    constructor() {
        super();
    }

    private m_pointerType: EPointerType;
    private m_workElement: HTMLElement = null;
    private interactiveElement: HTMLElement = null;
    private m_userInputManager: Ed.EdUserInputManager = null;
    private m_curTouchs: Array<{ pointerType: EPointerType, identifier: number }> = [];

    private m_isFocusCanvas: boolean = true;

    // 初始化事件
    async onInit(): Promise<void> {


        // 获得用户输入管理器
        this.m_userInputManager = application.kernelManager().userInputManager();

        // 获得工作区
        this.m_workElement = document.getElementById("scene");
        if (this.m_workElement == null) {
            return;
        }

        this.interactiveElement = document.getElementById("scene");

        /** 监听内核初始化完成事件 */
        application.eventManager().on(EventState.kRuntimeInitialized, this.onRuntimeInitialized);

        /** 鼠标滚动消息 */
        this.interactiveElement.addEventListener('mousewheel', this.onmousewheel, true);
        /** 鼠标移动消息 */
        this.interactiveElement.addEventListener('mousemove', this.onmousemove, true);
        /** 鼠标按下消息 */
        this.interactiveElement.addEventListener('mousedown', this.onmousedown, true);
        /** 鼠标弹起消息 */
        this.interactiveElement.addEventListener('mouseup', this.onmouseup, true);
        /** 鼠标双击消息 */
        this.interactiveElement.addEventListener('dblclick', this.ondblclick, true);
        /** 键盘按下消息 */
        document.body.addEventListener('keydown', this.onkeydown, false);
        /** 键盘弹起消息 */
        document.body.addEventListener('keyup', this.onkeyup, false);

        /** 触摸开始事件 */
        this.interactiveElement.addEventListener('touchstart', this.ontouchstart, true);
        /** 触摸移动事件 */
        this.interactiveElement.addEventListener('touchmove', this.ontouchmove, true);
        /** 触摸结束事件 */
        this.interactiveElement.addEventListener('touchend', this.ontouchend, true);
        /** 手写笔按下事件 */
        this.interactiveElement.addEventListener('pointerdown', this.onpointerdown, true);

        /** 监听焦点修改事件 */
        // application.eventManager().on(EventState.kFocusStateChange, (state: number) => {
        //     if (state === 0) {
        //         this.m_isFocusCanvas = true;
        //     }
        //     else {
        //         this.m_isFocusCanvas = false;
        //     }
        // });
    }

    // 按下esc
    public esc = () => {
        // 设置用户输入
        let obj: {
            altKey: boolean,
            ctrlKey: boolean,
            shiftKey: boolean,
            keyCode: number
        } = {
            altKey: false,
            ctrlKey: false,
            shiftKey: false,
            keyCode: 27
        };
        if (this.m_isFocusCanvas == true) {
            this.m_userInputManager.userInput('keydown', JSON.stringify(obj));
        }
    }

    // 触发移动事件
    public emitMousemove = (data: IMouseEvent) => {

        let top: number = this.m_workElement.getBoundingClientRect().top - gcadGlobal.uiLayoutData.workTop;
        let bottom: number = this.m_workElement.getBoundingClientRect().bottom - gcadGlobal.uiLayoutData.workTop;
        let left: number = this.m_workElement.getBoundingClientRect().left - gcadGlobal.uiLayoutData.workLeft;
        let right: number = this.m_workElement.getBoundingClientRect().right - gcadGlobal.uiLayoutData.workLeft;

        // 获得屏幕缩放
        let devicePixelRatio: number = 1;
        if (window.devicePixelRatio !== undefined) {
            devicePixelRatio = window.devicePixelRatio;
        }

        // 设置屏幕尺寸和绘图区范围
        this.m_userInputManager.setDevicePixelRatio(devicePixelRatio);
        this.m_userInputManager.setRangRect(top, bottom, left, right);
        this.m_userInputManager.setScreenSize(window.screen.width, window.screen.height);

        // 设置用户输入
        this.m_userInputManager.userInput('mousemove', JSON.stringify(data));
    };

    // 初始化kernal完成
    private onRuntimeInitialized = async () => {

        // 获得屏幕缩放
        let devicePixelRatio: number = 1;
        if (window.devicePixelRatio !== undefined) {
            devicePixelRatio = window.devicePixelRatio;
        }

        let top: number = this.m_workElement.getBoundingClientRect().top - gcadGlobal.uiLayoutData.workTop;
        let bottom: number = this.m_workElement.getBoundingClientRect().bottom - gcadGlobal.uiLayoutData.workTop;
        let left: number = this.m_workElement.getBoundingClientRect().left - gcadGlobal.uiLayoutData.workLeft;
        let right: number = this.m_workElement.getBoundingClientRect().right - gcadGlobal.uiLayoutData.workLeft;


        // 设置屏幕尺寸和绘图区范围
        this.m_userInputManager.setDevicePixelRatio(devicePixelRatio);
        this.m_userInputManager.setRangRect(top, bottom, left, right);
        this.m_userInputManager.setScreenSize(window.screen.width, window.screen.height);

        // 设置鼠标位置
        let mouseEvent: Ed.EdMouseEvent = new Ed.EdMouseEvent();
        this.m_userInputManager.mousemove(mouseEvent);
    }

    // 鼠标滚动消息
    private onmousewheel = async (event: any) => {

        // 触发事件
        let data: IMouseWheelEvent = {
            button: event.button,
            clientX: event.clientX - gcadGlobal.uiLayoutData.workLeft,
            clientY: event.clientY - gcadGlobal.uiLayoutData.workTop,
            altKey: event.altKey,
            ctrlKey: event.ctrlKey,
            shiftKey: event.shiftKey,
            wheelDelta: event.wheelDelta
        };

        // 设置用户输入
        this.m_userInputManager.userInput('mousewheel', JSON.stringify(data));

        application.eventManager().emit(EventState.kMousewheel, data);
    }

    // 鼠标移动消息
    private onmousemove = async (event: MouseEvent) => {

        let top: number = this.m_workElement.getBoundingClientRect().top - gcadGlobal.uiLayoutData.workTop;
        let bottom: number = this.m_workElement.getBoundingClientRect().bottom - gcadGlobal.uiLayoutData.workTop;
        let left: number = this.m_workElement.getBoundingClientRect().left - gcadGlobal.uiLayoutData.workLeft;
        let right: number = this.m_workElement.getBoundingClientRect().right - gcadGlobal.uiLayoutData.workLeft;

        // 触发事件
        let data: IMouseEvent = {
            button: event.button,
            clientX: event.clientX - gcadGlobal.uiLayoutData.workLeft,
            clientY: event.clientY - gcadGlobal.uiLayoutData.workTop,
            altKey: event.altKey,
            ctrlKey: event.ctrlKey,
            shiftKey: event.shiftKey
        };

        // 设置用户输入
        if (this.m_isFocusCanvas == true) {

            // 获得屏幕缩放
            let devicePixelRatio: number = 1;
            if (window.devicePixelRatio !== undefined) {
                devicePixelRatio = window.devicePixelRatio;
            }

            // 设置屏幕尺寸和绘图区范围
            this.m_userInputManager.setDevicePixelRatio(devicePixelRatio);
            this.m_userInputManager.setRangRect(top, bottom, left, right);
            this.m_userInputManager.setScreenSize(window.screen.width, window.screen.height);

            // 设置用户输入
            this.m_userInputManager.userInput('mousemove', JSON.stringify(data));
        }

        application.eventManager().emit(EventState.kMousemove, data);
    }

    // 鼠标按下消息
    private onmousedown = async (event: MouseEvent) => {

        // 触发事件
        let data: IMouseEvent = {
            button: event.button,
            clientX: event.clientX - gcadGlobal.uiLayoutData.workLeft,
            clientY: event.clientY - gcadGlobal.uiLayoutData.workTop,
            altKey: event.altKey,
            ctrlKey: event.ctrlKey,
            shiftKey: event.shiftKey
        };
        
        application.eventManager().emit(EventState.kMousedown, data);

        // 设置用户输入
        if (this.m_isFocusCanvas == true) {
            this.m_userInputManager.userInput('mousedown', JSON.stringify(data));
        }
    }

    // 鼠标弹起消息
    private onmouseup = async (event: MouseEvent) => {

        // 触发事件
        let data: IMouseEvent = {
            button: event.button,
            clientX: event.clientX - gcadGlobal.uiLayoutData.workLeft,
            clientY: event.clientY - gcadGlobal.uiLayoutData.workTop,
            altKey: event.altKey,
            ctrlKey: event.ctrlKey,
            shiftKey: event.shiftKey
        };
        application.eventManager().emit(EventState.kMouseup, data);

        // 设置用户输入
        if (this.m_isFocusCanvas == true) {
            this.m_userInputManager.userInput('mouseup', JSON.stringify(data));
        }
    }

    // 鼠标双击事件
    private ondblclick = async (event: MouseEvent) => {

        // 触发事件
        let data: IMouseEvent = {
            button: event.button,
            clientX: event.clientX - gcadGlobal.uiLayoutData.workLeft,
            clientY: event.clientY - gcadGlobal.uiLayoutData.workTop,
            altKey: event.altKey,
            ctrlKey: event.ctrlKey,
            shiftKey: event.shiftKey
        };
        application.eventManager().emit(EventState.kMouseDbclick, data);

        // 设置用户输入
        if (this.m_isFocusCanvas == true) {
            this.m_userInputManager.dblclick(data.button, data.clientX, data.clientY, data.altKey, data.ctrlKey, data.shiftKey);
        }
    }


    // 转触摸事件
    private touchEvent = (e: TouchEvent): ITouchEvent => {

        let top: number = this.m_workElement.getBoundingClientRect().top;
        let left: number = this.m_workElement.getBoundingClientRect().left;

        let touchEvent: ITouchEvent = {
            altKey: false,
            ctrlKey: false,
            shiftKey: false,
            changedTouches: [],
            touches: []
        };
        touchEvent.altKey = e.altKey;
        touchEvent.ctrlKey = e.ctrlKey;
        touchEvent.shiftKey = e.shiftKey;
        for (let i = 0; i < e.changedTouches.length; i++) {
            let touch: ITouch = {
                clientX: e.changedTouches[i].clientX - left,
                clientY: e.changedTouches[i].clientY - top,
                force: e.changedTouches[i].force,
                identifier: e.changedTouches[i].identifier,
                pageX: e.changedTouches[i].pageX,
                pageY: e.changedTouches[i].pageY,
                radiusX: e.changedTouches[i].radiusX,
                radiusY: e.changedTouches[i].radiusY,
                rotationAngle: e.changedTouches[i].rotationAngle,
                screenX: e.changedTouches[i].screenX,
                screenY: e.changedTouches[i].screenY,
                pointerType: this.m_pointerType,
            }
            touchEvent.changedTouches.push(touch);
        }
        for (let i = 0; i < e.touches.length; i++) {

            // 获得当前触摸类型
            let pointerType: EPointerType = this.m_pointerType;
            for (let j = 0; j < this.m_curTouchs.length; j++) {
                if (this.m_curTouchs[j].identifier == e.touches[i].identifier) {
                    pointerType = this.m_curTouchs[j].pointerType;
                    break;
                }
            }

            let touch: ITouch = {
                clientX: e.touches[i].clientX - left,
                clientY: e.touches[i].clientY - top,
                force: e.touches[i].force,
                identifier: e.touches[i].identifier,
                pageX: e.touches[i].pageX,
                pageY: e.touches[i].pageY,
                radiusX: e.touches[i].radiusX,
                radiusY: e.touches[i].radiusY,
                rotationAngle: e.touches[i].rotationAngle,
                screenX: e.touches[i].screenX,
                screenY: e.touches[i].screenY,
                pointerType: pointerType,
            }
            touchEvent.touches.push(touch);
        }
        return touchEvent;
    }

    // 触摸开始事件
    private ontouchstart = async (e: TouchEvent) => {

        let touchEvent: ITouchEvent = this.touchEvent(e);
        touchEvent.altKey = e.altKey;
        touchEvent.ctrlKey = e.ctrlKey;
        touchEvent.shiftKey = e.shiftKey;
        this.m_userInputManager.userInput('touchstart', JSON.stringify(touchEvent));

        // 组织浏览器默认行为
        e.preventDefault();
        e.stopPropagation();
    }

    // 触摸移动事件
    private ontouchmove = async (e: TouchEvent) => {

        let touchEvent: ITouchEvent = this.touchEvent(e);
        touchEvent.altKey = e.altKey;
        touchEvent.ctrlKey = e.ctrlKey;
        touchEvent.shiftKey = e.shiftKey;
        this.m_userInputManager.userInput('touchmove', JSON.stringify(touchEvent));

        // 组织浏览器默认行为
        e.preventDefault();
        e.stopPropagation();
    }

    // 触摸结束事件
    private ontouchend = async (e: TouchEvent) => {

        let touchEvent: ITouchEvent = this.touchEvent(e);
        touchEvent.altKey = e.altKey;
        touchEvent.ctrlKey = e.ctrlKey;
        touchEvent.shiftKey = e.shiftKey;
        this.m_userInputManager.userInput('touchend', JSON.stringify(touchEvent));
    }

    // 判断是触摸还是鼠标
    private onpointerdown = async (e: PointerEvent) => {
        const w = Number(e.width);
        const h = Number(e.height);
        if (w < 10 && h < 10) {
            this.m_pointerType = EPointerType.kPen;
        } else {
            this.m_pointerType = EPointerType.kTouch;
        }
    }


    // 键盘按下消息
    private onkeydown = async (event: KeyboardEvent) => {

        // 阻止默认Ctrl+S命令
        if (event.ctrlKey && event.key === 's') {
            event.preventDefault();
        }
        // 阻止默认tab命令
        else if (event.key === 'Tab') {
            event.preventDefault();
        }
        // 阻止默认F1-F12命令
        else if (event.keyCode >= 112 && event.keyCode <= 123) {
            event.preventDefault();
        }
        // 阻止默认功能键
        else if (event.keyCode >= 173 && event.keyCode <= 175) {
            event.preventDefault();
        }

        // 设置用户输入
        let data: IKeyboardEvent = {
            altKey: event.altKey,
            ctrlKey: event.ctrlKey,
            shiftKey: event.shiftKey,
            keyCode: event.keyCode,
            key: event.key
        };
        if (data.keyCode == 177) {
            data.key = "F7";
            data.keyCode = 118;
        }
        if (data.keyCode == 179) {
            data.key = "F8";
            data.keyCode = 119;
        }
        if (data.keyCode == 176) {
            data.key = "F9";
            data.keyCode = 120;
        }
        if (data.keyCode == 173) {
            data.key = "F10";
            data.keyCode = 121;
        }
        if (data.keyCode == 174) {
            data.key = "F11";
            data.keyCode = 122;
        }
        if (data.keyCode == 175) {
            data.key = "F12";
            data.keyCode = 123;
        }

        // 触发事件
        application.eventManager().emit(EventState.kKeydown, data);

        this.m_userInputManager.userInput('keydown', JSON.stringify(data));
    }

    // 键盘弹起消息
    private onkeyup = async (event: KeyboardEvent) => {

        // 设置用户输入
        let data: IKeyboardEvent = {
            altKey: event.altKey,
            ctrlKey: event.ctrlKey,
            shiftKey: event.shiftKey,
            keyCode: event.keyCode,
            key: event.key
        };
        if (data.keyCode == 177) {
            data.key = "F7";
            data.keyCode = 118;
        }
        if (data.keyCode == 179) {
            data.key = "F8";
            data.keyCode = 119;
        }
        if (data.keyCode == 176) {
            data.key = "F9";
            data.keyCode = 120;
        }
        if (data.keyCode == 173) {
            data.key = "F10";
            data.keyCode = 121;
        }
        if (data.keyCode == 174) {
            data.key = "F11";
            data.keyCode = 122;
        }
        if (data.keyCode == 175) {
            data.key = "F12";
            data.keyCode = 123;
        }

        if (this.m_isFocusCanvas == true) {
            this.m_userInputManager.userInput('keyup', JSON.stringify(data));
        }

        // 触发事件
        application.eventManager().emit(EventState.kKeyup, data);
    }

}

