import Ap from './Ap'
import Rx from './Rx'
import Ac from './Ac'
import Gi from './Gi'
import Re from './Re'
import Ui from './Ui'
import Db from './Db'
import Ed from './Ed'
import Ge from './Ge'
import Arx from './arxHeaders'


export function emsdk_isEqualTo_ptr(p1: any, p2: any): boolean {
    if (p1.$$.ptr == p2.$$.ptr) {
        return true;
    }
    return false;
}



export class ArxEvent {

    // GiMaterialReactor
    static GiMaterialReactor_onCreateMaterial(_MaterialReactor: Gi.GiMaterialReactor, _fun: (material: Gi.GiMaterial) => void): void {
        function fun(_inPtr: number) {
            console.log(_inPtr);
            _fun(Gi.GiMaterial.cast(_inPtr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _MaterialReactor.onCreateMaterial(inPtr);
    }

    // GiEntityReactor
    static GiEntityReactor_onCreateEntity(_EntityReactor: Gi.GiEntityReactor, _fun: (entity: Gi.GiEntity) => void): void {
        function fun(_inPtr: number) {
            _fun(Gi.GiEntity.cast(_inPtr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _EntityReactor.onCreateEntity(inPtr);
    }
    static GiEntityReactor_onUpdateVertexs(_EntityReactor: Gi.GiEntityReactor, _fun: (entity: Gi.GiEntity) => void): void {
        function fun(_inPtr: number) {
            _fun(Gi.GiEntity.cast(_inPtr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _EntityReactor.onUpdateVertexs(inPtr);
    }
    static GiEntityReactor_onTransformation(_EntityReactor: Gi.GiEntityReactor, _fun: (entity: Gi.GiEntity) => void): void {
        function fun(_inPtr: number) {
            _fun(Gi.GiEntity.cast(_inPtr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _EntityReactor.onTransformation(inPtr);
    }
    static GiEntityReactor_onVisibility(_EntityReactor: Gi.GiEntityReactor, _fun: (entity: Gi.GiEntity, _isVisibility: boolean) => void): void {
        function fun(_inPtr: number, _isVisibility: boolean) {
            _fun(Gi.GiEntity.cast(_inPtr), _isVisibility);
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vii');
        _EntityReactor.onVisibility(inPtr);
    }

    // ApDocManagerReactor
    static ApDocManagerReactor_onDocumentCreated(_DocManagerReactor: Ap.ApDocManagerReactor, _fun: (document: Ap.ApDocument) => void): void {
        function fun(_inPtr: number) {
            _fun(Ap.ApDocument.cast(_inPtr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _DocManagerReactor.onDocumentCreated(inPtr);
    }
    static ApDocManagerReactor_onDocumentActivated(_docManagerReactor: Ap.ApDocManagerReactor, _fun: (document: Ap.ApDocument) => void): void {
        function fun(_inPtr: number) {
            _fun(Ap.ApDocument.cast(_inPtr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _docManagerReactor.onDocumentActivated(inPtr);
    }
    static ApDocManagerReactor_onDocumentToBeDestroyed(_docManagerReactor: Ap.ApDocManagerReactor, _fun: (document: Ap.ApDocument) => void): void {
        function fun(_inPtr: number) {
            _fun(Ap.ApDocument.cast(_inPtr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _docManagerReactor.onDocumentToBeDestroyed(inPtr);
    }
    static ApDocManagerReactor_onDocumentDestroyed(_docManagerReactor: Ap.ApDocManagerReactor, _fun: (fileName: string) => void): void {
        function fun(fileName: number) {
            _fun(Ac.AcString.ptr_char_to_string(fileName));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _docManagerReactor.onDocumentDestroyed(inPtr);
    }

    // DbLayoutManagerReactor
    static DbLayoutManagerReactor_onLayoutCreated(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (_newname: string, id: Db.DbObjectId) => void): void {
        function fun(_newname: number, layoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(_newname), Db.DbObjectId.cast(layoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vii');
        _layoutManagerReactor.onLayoutCreated(inPtr);
    }
    static DbLayoutManagerReactor_onLayoutToBeRemoved(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (layoutName: string, layoutId: Db.DbObjectId) => void): void {
        function fun(layoutName: number, layoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(layoutName), Db.DbObjectId.cast(layoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vii');
        _layoutManagerReactor.onLayoutToBeRemoved(inPtr);
    }
    static DbLayoutManagerReactor_onLayoutRemoved(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (layoutName: string, layoutId: Db.DbObjectId) => void): void {
        function fun(layoutName: number, layoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(layoutName), Db.DbObjectId.cast(layoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vii');
        _layoutManagerReactor.onLayoutRemoved(inPtr);
    }
    static DbLayoutManagerReactor_onAbortLayoutRemoved(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (layoutName: string, layoutId: Db.DbObjectId) => void): void {
        function fun(layoutName: number, layoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(layoutName), Db.DbObjectId.cast(layoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vii');
        _layoutManagerReactor.onAbortLayoutRemoved(inPtr);
    }
    static DbLayoutManagerReactor_onLayoutToBeCopied(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (layoutName: string, oldLayoutId: Db.DbObjectId) => void): void {
        function fun(layoutName: number, oldLayoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(layoutName), Db.DbObjectId.cast(oldLayoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vii');
        _layoutManagerReactor.onLayoutToBeCopied(inPtr);
    }
    static DbLayoutManagerReactor_onLayoutCopied(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (oldLayoutName: string, oldLayoutId: Db.DbObjectId, newLayoutname: string, newLayoutId: Db.DbObjectId) => void): void {
        function fun(oldLayoutName: number, oldLayoutId: number, newLayoutname: number, newLayoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(oldLayoutName), Db.DbObjectId.cast(oldLayoutId),
                Ac.AcString.ptr_char_to_string(newLayoutname), Db.DbObjectId.cast(newLayoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'viiii');
        _layoutManagerReactor.onLayoutCopied(inPtr);
    }
    static DbLayoutManagerReactor_onAbortLayoutCopied(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (layoutName: string, layoutId: Db.DbObjectId) => void): void {
        function fun(layoutName: number, layoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(layoutName), Db.DbObjectId.cast(layoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vii');
        _layoutManagerReactor.onAbortLayoutCopied(inPtr);
    }
    static DbLayoutManagerReactor_onLayoutToBeRenamed(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (oldName: string, newName: string, layoutId: Db.DbObjectId) => void): void {
        function fun(oldName: number, newName: number, layoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(oldName), Ac.AcString.ptr_char_to_string(newName), Db.DbObjectId.cast(layoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'viii');
        _layoutManagerReactor.onLayoutToBeRenamed(inPtr);
    }
    static DbLayoutManagerReactor_onLayoutRenamed(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (oldName: string, newName: string, layoutId: Db.DbObjectId) => void): void {
        function fun(oldName: number, newName: number, layoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(oldName), Ac.AcString.ptr_char_to_string(newName), Db.DbObjectId.cast(layoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'viii');
        _layoutManagerReactor.onLayoutRenamed(inPtr);
    }
    static DbLayoutManagerReactor_onAbortLayoutRename(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (oldName: string, newName: string, layoutId: Db.DbObjectId) => void): void {
        function fun(oldName: number, newName: number, layoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(oldName), Ac.AcString.ptr_char_to_string(newName), Db.DbObjectId.cast(layoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'viii');
        _layoutManagerReactor.onAbortLayoutRename(inPtr);
    }
    static DbLayoutManagerReactor_onLayoutSwitched(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (newLayoutname: string, newLayoutId: Db.DbObjectId) => void): void {
        function fun(newLayoutname: number, newLayoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(newLayoutname), Db.DbObjectId.cast(newLayoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vii');
        _layoutManagerReactor.onLayoutSwitched(inPtr);
    }
    static DbLayoutManagerReactor_onPlotStyleTableChanged(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: (newTableName: string, layoutId: Db.DbObjectId) => void): void {
        function fun(newTableName: number, layoutId: number) {
            _fun(Ac.AcString.ptr_char_to_string(newTableName), Db.DbObjectId.cast(layoutId));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vii');
        _layoutManagerReactor.onPlotStyleTableChanged(inPtr);
    }
    static DbLayoutManagerReactor_onLayoutsReordered(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        _layoutManagerReactor.onLayoutsReordered(inPtr);
    }
    static DbLayoutManagerReactor_onRefreshLayoutTabs(_layoutManagerReactor: Db.DbLayoutManagerReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        _layoutManagerReactor.onRefreshLayoutTabs(inPtr);
    }

    // ReRendererReactor
    static ReRendererReactor_onBeginAnimationFrame(_rendererReactor: Re.ReRendererReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        _rendererReactor.onBeginAnimationFrame(inPtr);
    }
    static ReRendererReactor_onEndAnimationFrame(_rendererReactor: Re.ReRendererReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        _rendererReactor.onEndAnimationFrame(inPtr);
    }
    static ReRendererReactor_onBeginRenderer(_rendererReactor: Re.ReRendererReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        _rendererReactor.onBeginRenderer(inPtr);
    }
    static ReRendererReactor_onEndRenderer(_rendererReactor: Re.ReRendererReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        _rendererReactor.onEndRenderer(inPtr);
    }
    static ReRendererReactor_onClearCanvas(_rendererReactor: Re.ReRendererReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        _rendererReactor.onClearCanvas(inPtr);
    }
    static ReRendererReactor_onClearWorkCanvas(_rendererReactor: Re.ReRendererReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        _rendererReactor.onClearWorkCanvas(inPtr);
    }
    static ReRendererReactor_onBeginRendererViewport(_rendererReactor: Re.ReRendererReactor, _fun: (_viewport: Db.DbObject) => void): void {
        function fun(_inPtr: number) {
            _fun(Db.DbObject.cast(_inPtr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _rendererReactor.onBeginRendererViewport(inPtr);
    }
    static ReRendererReactor_onEndRendererViewport(_rendererReactor: Re.ReRendererReactor, _fun: (_viewport: Db.DbObject) => void): void {
        function fun(_inPtr: number) {
            _fun(Db.DbObject.cast(_inPtr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _rendererReactor.onEndRendererViewport(inPtr);
    }
    static ReRendererReactor_onRendererViewport(_rendererReactor: Re.ReRendererReactor, _fun: (_renderer: Re.ReRenderer) => void): void {
        function fun(_inPtr: number) {
            _fun(Re.ReRenderer.cast(_inPtr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _rendererReactor.onRendererViewport(inPtr);
    }

    // EditorReactor
    static EditorReactor_onCommandWillStart(_editorReactor: Ed.EditorReactor, _fun: (cmdStr: string) => void): void {
        function fun(cmdStr: number) {
            _fun(Ac.AcString.ptr_char_to_string(cmdStr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _editorReactor.onCommandWillStart(inPtr);
    }
    static EditorReactor_onCommandEnded(_editorReactor: Ed.EditorReactor, _fun: (cmdStr: string) => void): void {
        function fun(cmdStr: number) {
            _fun(Ac.AcString.ptr_char_to_string(cmdStr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _editorReactor.onCommandEnded(inPtr);
    }
    static EditorReactor_onCommandCancelled(_editorReactor: Ed.EditorReactor, _fun: (cmdStr: string) => void): void {
        function fun(cmdStr: number) {
            _fun(Ac.AcString.ptr_char_to_string(cmdStr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _editorReactor.onCommandCancelled(inPtr);
    }
    static EditorReactor_onCommandFailed(_editorReactor: Ed.EditorReactor, _fun: (cmdStr: string) => void): void {
        function fun(cmdStr: number) {
            _fun(Ac.AcString.ptr_char_to_string(cmdStr));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _editorReactor.onCommandFailed(inPtr);
    }
    static EditorReactor_onSysVarChanged(_editorReactor: Ed.EditorReactor, _fun: (varName: string, success: boolean) => void): void {
        function fun(varName: number, success: boolean) {
            _fun(Ac.AcString.ptr_char_to_string(varName), success);
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vii');
        _editorReactor.onSysVarChanged(inPtr);
    }
    static EditorReactor_onSysVarWillChange(_editorReactor: Ed.EditorReactor, _fun: (varName: string) => void): void {
        function fun(varName: number) {
            _fun(Ac.AcString.ptr_char_to_string(varName));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _editorReactor.onSysVarWillChange(inPtr);
    }

    // UiInterfaceReactor
    static UiInterfaceReactor_onCreateCustomDialog(interfaceReactor: Ui.UiInterfaceReactor, _fun: (str: Ui.UiCustomDialog) => void): void {
        function fun(dig: number) {
            _fun(Ui.UiCustomDialog.cast(dig));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        interfaceReactor.onCreateCustomDialog(inPtr);
    }
    static UiInterfaceReactor_onCreateContextMenu(interfaceReactor: Ui.UiInterfaceReactor, _fun: (str: string) => void): void {
        function fun(_text: number) {
            _fun(Ac.AcString.ptr_char_to_string(_text));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        interfaceReactor.onCreateContextMenu(inPtr);
    }
    static UiInterfaceReactor_onPrintMsgbar(interfaceReactor: Ui.UiInterfaceReactor, _fun: (str: string) => void): void {
        function fun(_text: number) {
            _fun(Ac.AcString.ptr_char_to_string(_text));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        interfaceReactor.onPrintMsgbar(inPtr);
    }
    static UiInterfaceReactor_onPrintGraphCoord(interfaceReactor: Ui.UiInterfaceReactor, _fun: (str: string) => void): void {
        function fun(_text: number) {
            _fun(Ac.AcString.ptr_char_to_string(_text));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        interfaceReactor.onPrintGraphCoord(inPtr);
    }
    static UiInterfaceReactor_onUpdateCommandlineState(interfaceReactor: Ui.UiInterfaceReactor, _fun: (state: string) => void): void {
        function fun(state: number) {
            _fun(Ac.AcString.ptr_char_to_string(state));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        interfaceReactor.onUpdateCommandlineState(inPtr);
    }

    // EdDragReactor
    static EdDragReactor_onBeginDrag(dragManager: Ed.EdDragReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        dragManager.onBeginDrag(inPtr);
    }
    static EdDragReactor_onEndDrag(dragManager: Ed.EdDragReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        dragManager.onEndDrag(inPtr);
    }
    static EdDragReactor_onBeginUpdate(dragManager: Ed.EdDragReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        dragManager.onBeginUpdate(inPtr);
    }
    static EdDragReactor_onEndUpdate(dragManager: Ed.EdDragReactor, _fun: () => void): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        dragManager.onEndUpdate(inPtr);
    }
    static EdDragReactor_onJigBeginDrag(dragManager: Ed.EdDragReactor, _fun: (jig: Ed.EdJig) => void): void {
        function fun(jig: number) {
            _fun(Ed.EdJig.cast(jig));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        dragManager.onJigBeginDrag(inPtr);
    }
    static EdDragReactor_onJigUpdateDynDim(dragManager: Ed.EdDragReactor, _fun: (position: Ge.GePoint3d, dimDatas: Db.DbDimDataPtrArray) => void): void {
        function fun(p1: number, p2: number) {
            _fun(Arx.emsdk_class_convert_GePoint3d(p1), Arx.emsdk_ptr_class_convert_DbDimDataPtrArray(p2));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vii');
        dragManager.onJigUpdateDynDim(inPtr);
    }
    static EdDragReactor_onJigEndDrag(dragManager: Ed.EdDragReactor, _fun: (jig: Ed.EdJig) => void): void {
        function fun(jig: number) {
            _fun(Ed.EdJig.cast(jig));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        dragManager.onJigEndDrag(inPtr);
    }

    // RxPropertyReactor
    static RxPropertyReactor_onUpdatePropertyTable(propertyReactor: Rx.RxPropertyReactor, _fun: (table: Rx.RxPropertyTable) => void): void {
        function fun(table: number) {
            _fun(Rx.RxPropertyTable.cast(table));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        propertyReactor.onUpdatePropertyTable(inPtr);
    };

    // UiDialogReactor
    static UiDialogReactor_onMessage(_dialogReactor: Ui.UiDialogReactor, _fun: (data: string) => void): void {
        function fun(_data: number) {
            _fun(Ac.AcString.ptr_char_to_string(_data));
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'vi');
        _dialogReactor.onMessage(inPtr);
    }
    static UiDialogReactor_onClose(_dialogReactor: Ui.UiDialogReactor, _fun: Function): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        _dialogReactor.onClose(inPtr);
    }
    static UiDialogReactor_onExec(_dialogReactor: Ui.UiDialogReactor, _fun: Function): void {
        function fun() {
            _fun();
        }

        let inPtr = (<any>window).Module.addFunction(fun, 'v');
        _dialogReactor.onExec(inPtr);
    }

    // 添加命令
    static acedRegCmds_addCommand(_cmdGroupName: string, _cmdGlobalName: string, _cmdLocalName: string, _fun: Function): void {

    }
}

