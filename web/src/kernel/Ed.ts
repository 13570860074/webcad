import Ge from './Ge'
import Rx from './Rx'
import Ac from './Ac'
import Db from './Db'

declare namespace Module {


    class ads_u_val {
        constructor();
        rreal: number;
        rpoint_x: number;
        rpoint_y: number;
        rpoint_z: number;
        rint: number;
        rstring: string;
    }
    class resbuf {
        constructor();
        rbnext: resbuf;
        restype: number;
        resval: ads_u_val;
    }


    function acedGetVar(_str: string): resbuf;
    function acedSetVar(_str: string, resbuf: resbuf): number;

    function acedGetEnv(sym: string): string;
    function acedSetEnv(sym: string, val: string): number;
    function acedGetCfg(sym: string): string;
    function acedSetCfg(sym: string, val: string): number;


    enum EPointerType {
        kUnknown,
        kMouse,
        kTouch,
        kPen,
    }
    enum EdUserInputEventMode {
        kNull,

        kMousemove,									// 鼠标移动
        kMousewheel,								// 滚轮滚动
        kMouseup,									// 鼠标抬起
        kMousedown,									// 鼠标按下
        kDblclick,									// 双击事件

        kKeydown,									// 键盘按下
        kKeyup,										// 按键弹起

        kTouchstart,								// 触摸开始
        kTouchend,									// 触摸解除
        kTouchmove,									// 触摸移动
    }
    class EdUserInputEvent {
        mode(): EdUserInputEventMode;
        setMode(_mode: EdUserInputEventMode): void;
    }
    class EdTouch {
        constructor();
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
    class EdTouchArray extends Ac.AcArray {
        constructor();
    }
    class EdTouchEvent {
        constructor();
        altKey: boolean;
        ctrlKey: boolean;
        shiftKey: boolean;
        changedTouches(): EdTouchArray;
        touches(): EdTouchArray;
    }

    class EdMouseEvent extends EdUserInputEvent {
        constructor();
        altKey: boolean;
        button: number;
        clientX: number;
        clientY: number;
        ctrlKey: boolean;
        shiftKey: boolean;
        static stringify(mouseEvent: EdMouseEvent): string;
        static parse(): EdMouseEvent;
    }
    class EdWheelEvent extends EdTouchEvent {
        constructor();
        wheelDelta: number;
        static stringify(mouseEvent: EdWheelEvent): string;
        static parse(): EdWheelEvent;
    }
    class EdKeyboardEvent extends EdUserInputEvent {
        constructor();
        altKey: boolean;
        ctrlKey: boolean;
        shiftKey: boolean;
        keyCode: number;
        static stringify(mouseEvent: EdKeyboardEvent): string;
        static parse(): EdKeyboardEvent;
    }

    class EdUserInputManager extends Rx.RxObject {
        constructor();
        userInput(input: 'mousewheel' | 'mousemove' | 'mouseup' | 'mousedown' | 'dblclick' | 'keydown' | 'keyup' | 'touchstart' | 'touchmove' | 'touchend', data: string): void;
        setDevicePixelRatio(_val: number): void;
        setRangRect(_top: number, _bottom: number, _lefy: number, _right: number): void;
        setScreenSize(_width: number, _height: number): void;
        mousewheel(_wheelEvent: EdWheelEvent): void;
        mousewheel(button: number, clientX: number, clientY: number, altKey: boolean, ctrlKey: boolean, shiftKey: boolean, wheelDelta: number): void;
        mousemove(_mouseEvent: EdMouseEvent): void;
        mousemove(button: number, clientX: number, clientY: number, altKey: boolean, ctrlKey: boolean, shiftKey: boolean): void;
        mouseup(_mouseEvent: EdMouseEvent): void;
        mouseup(button: number, clientX: number, clientY: number, altKey: boolean, ctrlKey: boolean, shiftKey: boolean): void;
        mousedown(_mouseEvent: EdMouseEvent): void;
        mousedown(button: number, clientX: number, clientY: number, altKey: boolean, ctrlKey: boolean, shiftKey: boolean): void;
        dblclick(_mouseEvent: EdMouseEvent): void;
        dblclick(button: number, clientX: number, clientY: number, altKey: boolean, ctrlKey: boolean, shiftKey: boolean): void;

        touchstart(event: EdTouchEvent): void;
        touchmove(event: EdTouchEvent): void;
        touchend(event: EdTouchEvent): void;
    }

    abstract class EdCommand extends Rx.RxObject {
        static cast(inPtr: Rx.RxObject): EdCommand;
        static cast(inPtr: number): EdCommand;
        commandUndef(undefIt: boolean): void;
        commandFlags(): number;
        functionAddr(): any;
        functionAddr(fhdl: any): void;
        commandApp(): any;
        functionCode(): number;
        functionCode(fcode: number): void;
        globalName(): string;
        localName(): string;
        defaultCommandVersion(): number;
    }
    class EdRegistryCommand extends EdCommand {
        constructor();
        static cast(inPtr: Rx.RxObject): EdRegistryCommand;
        static cast(inPtr: number): EdRegistryCommand;
        commandUndef(undefIt: boolean): void;
        commandFlags(): number;
        commandFlags(flags: number): void;
        functionAddr(): any;
        functionAddr(fhdl: any): void;
        commandApp(app: any): void;
        commandApp(): any;
        functionCode(): number;
        functionCode(fcode: number): void;
        groupName(str: string): void;
        groupName(): string;
        globalName(str: string): void;
        globalName(): string;
        localName(str: string): void;
        localName(): string;
    }

    class EditorReactor extends Rx.RxEventReactor {
        constructor();
        static cast(inPtr: Rx.RxObject): EditorReactor;
        static cast(inPtr: number): EditorReactor;

        onUnknownCommand(ptr: number): void;
        onCommandWillStart(ptr: number): void;
        onCommandEnded(ptr: number): void;
        onCommandCancelled(ptr: number): void;
        onCommandFailed(ptr: number): void;

        onLispWillStart(ptr: number): void;
        onLispEnded(ptr: number): void;
        onLispCancelled(ptr: number): void;

        onBeginDwgOpen(ptr: number): void;
        onEndDwgOpen(ptr: number): void;

        onSysVarChanged(ptr: number): void;
        onSysVarWillChange(ptr: number): void;

        onUndoSubcommandAuto(ptr: number): void;
        onUndoSubcommandControl(ptr: number): void;
        onUndoSubcommandBegin(ptr: number): void;
        onUndoSubcommandEnd(ptr: number): void;
        onUndoSubcommandMark(ptr: number): void;
        onUndoSubcommandBack(ptr: number): void;
        onUndoSubcommandNumber(ptr: number): void;

        onPickfirstModified(ptr: number): void;
        onLayoutSwitched(ptr: number): void;
        onLayoutToBeSwitched(ptr: number): void;
        onDwgViewResized(ptr: number): void;

        onDocFrameMovedOrResized(ptr: number): void;
        onMainFrameMovedOrResized(ptr: number): void;

        onBeginDoubleClick(ptr: number): void;
        onBeginRightClick(ptr: number): void;

        onToolbarBitmapSizeWillChange(ptr: number): void;
        onToolbarBitmapSizeChanged(ptr: number): void;

        onBeginDocClose(ptr: number): void;
        onDocCloseAborted(ptr: number): void;

        onDocCloseWillStart(ptr: number): void;

        onBeginClose(ptr: number): void;

        onBeginCloseAll(ptr: number): void;
        onBeginQuit(ptr: number): void;
        onQuitAborted(ptr: number): void;
        onQuitWillStart(ptr: number): void;

        onViewChanged(ptr: number): void;
    }

    abstract class Editor extends Rx.RxEvent {
        static cast(inPtr: Rx.RxObject): Editor;
        static cast(inPtr: number): Editor;
        abstract addReactor(reactor: EditorReactor): void;
        abstract removeReactor(reactor: EditorReactor): void;
    }
    class EditorReactorManager extends Editor {
        constructor();
        static cast(inPtr: Rx.RxObject): EditorReactorManager;
        static cast(inPtr: number): EditorReactorManager;
        addReactor(reactor: EditorReactor): void;
        removeReactor(reactor: EditorReactor): void;
    }

    abstract class EdJig extends Rx.RxObject {
        static cast(inPtr: Rx.RxObject): EdJig;
        static cast(inPtr: number): EdJig;

        keywordList(): string;
        dispPrompt(): string;
    }
    enum EdPointerMode {
        kNone,
        kCoord,
        kDistAndAngle,
        kAngle,
    }
    class EdJigSetter extends Rx.RxObject {
        constructor();
        static cast(inPtr: Rx.RxObject): EdJigSetter;
        static cast(inPtr: number): EdJigSetter;

        dimMode(): number;
        setPointerMode(mode: EdPointerMode): void;
        setPointerMode(mode: EdPointerMode, bEnablePointer: boolean): void;
        updateDimData(): void;
        dimDataPosition(dimData: Db.DbDimData): Ge.GePoint2d;
        dimDataType(dimData: Db.DbDimData): number;
        dimDataValueString(dimData: Db.DbDimData): string;
        setDimDataValue(dimData: Db.DbDimData, v: number): void;
        setDimDataString(dimData: Db.DbDimData, v: string): void;
        dimDatas(): Db.DbDimDataPtrArray;
    }
    class EdDragReactor extends Rx.RxEventReactor {
        constructor();
        static cast(inPtr: Rx.RxObject): EdDragReactor;
        static cast(inPtr: number): EdDragReactor;

        onBeginDrag(ptr: number): void;
        onEndDrag(ptr: number): void;
        onBeginUpdate(ptr: number): void;
        onEndUpdate(ptr: number): void;
        onJigBeginDrag(ptr: number): void;
        onJigUpdateDynDim(ptr: number): void;
        onJigEndDrag(ptr: number): void;
    }
    class EdDragManager extends Rx.RxObject {
        constructor();
        static cast(inPtr: Rx.RxObject): EdDragManager;
        static cast(inPtr: number): EdDragManager;
        jigSetter(): EdJigSetter;
        addReactor(reactor: EdDragReactor): void;
        removeReactor(reactor: EdDragReactor): void;
    }
}


import Ed = Module;
export default Ed;

