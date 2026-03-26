



export enum EPointerType {
    kUnknown,
    kMouse,
    kTouch,
    kPen,
}
export interface ITouch {
    clientX: number;
    clientY: number;
    force: number;
    identifier: number;
    pageX: number;
    pageY: number;
    radiusX: number;
    radiusY: number;
    rotationAngle: number;
    screenX: number;
    screenY: number;
    pointerType: EPointerType;
}
export interface ITouchEvent {
    altKey: boolean;
    ctrlKey: boolean;
    shiftKey: boolean;
    changedTouches: Array<ITouch>;
    touches: Array<ITouch>;
}


export interface IMouseEvent {
    altKey: boolean,
    button: number,			// 0表示左键，1表示中键，2表示右键
    clientX: number,
    clientY: number,
    ctrlKey: boolean,
    shiftKey: boolean,
}
export interface IMouseWheelEvent extends IMouseEvent{
    wheelDelta?: number;
}

export interface IKeyboardEvent {
    altKey: boolean,
    ctrlKey: boolean,
    shiftKey: boolean,
    keyCode: number,
    key: string,
}