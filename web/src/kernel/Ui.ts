import Rx from './Rx'



declare namespace Module {

    enum ECustomComponentId {
        kNone,
        kLayerProperty,
    }

    class UiInterfaceManager extends Rx.RxManager {
        constructor();
        static cast(inPtr: Rx.RxObject): UiInterfaceManager;
        static cast(inPtr: number): UiInterfaceManager;
        addReactor(_reactor: UiInterfaceReactor): void;
        removeReactor(_reactor: UiInterfaceReactor): void;
    }
    class UiInterfaceReactor extends Rx.RxObject {
        constructor();
        onCreateCustomDialog(inPtr: number): void;
        onCreateContextMenu(inPtr: number): void;
        onPrintMsgbar(inPtr: number): void;
        onPrintGraphCoord(inPtr: number): void;
        onUpdateCommandlineState(inPtr: number): void;
    }


    abstract class UiDialog extends Rx.RxObject {
        static cast(inPtr: Rx.RxObject): UiDialog;
        static cast(inPtr: number): UiDialog;

        addReactor(_reactor: UiDialogReactor): void;
        removeReactor(_reactor: UiDialogReactor): void;
        trigger(): UiDialogReactor;

        title(): string;
        setTitle(_val: string): void;

        width(): number;
        height(): number;
        setSize(_w: number, _h: number): void;

        positionX(): number;
        positionY(): number;
        setPosition(_x: number, _y: number): void;

        isFrame(): boolean;
        setFrame(_val: boolean): void;

        titleHeight(): number;
        setTitleHeight(_val: number): void;

        isModal(): boolean;
        setModal(_val: boolean): void;

        backgroundColorR(): number;
        backgroundColorG(): number;
        backgroundColorB(): number;
        setBackgroundColor(_r: number, _g: number, _b: number): void;

        icon(): string;
        setIcon(_path: string): void;

        close(): void;
        exec(): void;

        emitMessage(id: number, data: string): void;
        postMessage(id: number, str: string): void;
        onMessage(ptr: number): void;
    }
    class UiCustomDialog extends UiDialog {
        constructor();
        static cast(inPtr: Rx.RxObject): UiCustomDialog;
        static cast(inPtr: number): UiCustomDialog;
        componentId(): number;
        setComponentId(id: number): void;
    }


    class UiDialogReactor extends Rx.RxObject {
        constructor();
        static cast(inPtr: Rx.RxObject): UiDialogReactor;
        static cast(inPtr: number): UiDialogReactor;

        onExec(ptr: number): void;
        onClose(ptr: number): void;
        onMessage(ptr: number): void;
    }
}


import Ui = Module;
export default Ui;

