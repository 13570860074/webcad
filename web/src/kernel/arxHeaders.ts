

// export * from './Acad';
// export * from './Cm';
// export * from './Rx';
import Ac from './Ac'
import Gi from './Gi'
import Ap from './Ap'
import Re from './Re'
import Ge from './Ge'
import Rx from './Rx'
import Ed from './Ed'
import Ui from './Ui'
import Db from './Db'
import Cm from './Cm'


declare namespace Module {

	abstract class EmObject {
	}


	/** delete指针 */
	function emsdk_free(p: EmObject): void;

	/** 指针转类 */
	function emsdk_class_convert_GePoint3d(p: number): Ge.GePoint3d;
	function emsdk_ptr_class_convert_DbDimDataPtrArray(p: number): Db.DbDimDataPtrArray;

	function dxf_test(_str: string): void;
	function acrxSysRegistry(): Rx.RxDictionary;
	function acrxPropertyManager(): Rx.RxPropertyManager;
	function acDocManagerPtr(): Ap.ApDocManager;
	function acgiMaterialManager(): Gi.GiMaterialManager;
	function acgiEntityManager(): Gi.GiEntityManager;
	function acreRendererManager(): Re.ReRendererManager;
	function acedUserInputManager(): Ed.EdUserInputManager;
	function acuiInterfaceManager(): Ui.UiInterfaceManager;
	function acedEditorReactorManager(): Ed.EditorReactorManager;
	function acedDragManager(): Ed.EdDragManager;
	function acdbHostApplicationServices(): Db.DbHostApplicationServices;

	function openDbObject(_id: Db.DbObjectId): Db.DbObject;
	function openDbObject(_id: Db.DbObjectId, _mode: Db.OpenMode): Db.DbObject;
	function openDbObject(_id: Db.DbObjectId, _mode: Db.OpenMode, _openErased: boolean): Db.DbObject;


	class ObjectArx {

		static workingDatabase(): Db.DbDatabase;
		static setWorkingDatabase(_database: Db.DbDatabase): void;
		static setCurrentLayer(_layerName: string): boolean;

		static setLayerColorIndex(id: Db.DbObjectId, _colorIndex: number): boolean;

		static setEntityLayer(_id: Db.DbObjectId, _layerName: string): boolean;
		static setEntityLayer(_ids: Db.DbObjectIdArray, _layerName: string): boolean;
		static setEntityColor(_id: Db.DbObjectId, _color: Cm.CmColor): boolean;
		static setEntityColor(_ids: Db.DbObjectIdArray, _color: Cm.CmColor): boolean;

		static selectionSet(): Db.DbObjectIdArray;
	}
}





import Arx = Module;
export default Arx;






