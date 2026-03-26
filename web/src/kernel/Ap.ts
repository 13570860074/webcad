
import Rx from './Rx'
import Db from './Db'
import Acad from './Acad'


declare namespace Module {

	//Ap
	enum DocLockMode {
		kNone = 0x00,
		kAutoWrite = 0x01,
		kNotLocked = 0x02,
		kWrite = 0x04,
		kProtectedAutoWrite = 0x14,
		kRead = 0x20,
		kXWrite = 0x40
	}
	abstract class ApDocument extends Rx.RxObject {
		static cast(inPtr: Rx.RxObject): ApDocument;
		static cast(inPtr: number): ApDocument;
		abstract fileName(): string;
		setFromOldId(_id: number): ApDocument;
		asOldId(): number;
		abstract database(): Db.DbDatabase;
		abstract test(): void;
		abstract docTitle(): string;
		setDocTitle(_title: string): void;
		abstract isReadOnly(): boolean;
		upgradeDocOpen(): Acad.ErrorStatus;
		downgradeDocOpen(): Acad.ErrorStatus;
	}
	class ApDocumentIterator extends Rx.RxObject {
		constructor();
		static cast(inPtr: Rx.RxObject): ApDocumentIterator;
		static cast(inPtr: number): ApDocumentIterator;
		start(): void;
		done(): boolean;
		step(): void;
		document(): ApDocument;
	}
	class ApDocManagerReactor extends Rx.RxObject {
		constructor();
		static cast(inPtr: Rx.RxObject): ApDocManagerReactor;
		static cast(inPtr: number): ApDocManagerReactor;
		onDocumentCreateStarted(ptr: number): void;
		onDocumentCreated(ptr: number): void;
		onDocumentToBeDestroyed(ptr: number): void;
		onDocumentDestroyed(ptr: number): void;
		onDocumentCreateCanceled(ptr: number): void;
		onDocumentLockModeWillChange(ptr: number): void;
		onDocumentLockModeChangeVetoed(ptr: number): void;
		onDocumentLockModeChanged(ptr: number): void;
		onDocumentBecameCurrent(ptr: number): void;
		onDocumentToBeActivated(ptr: number): void;
		onDocumentToBeDeactivated(ptr: number): void;
		onDocumentActivationModified(ptr: number): void;
		onDocumentActivated(ptr: number): void;
		onDocumentTitleUpdated(ptr: number): void;
	}
	class ApDocManager extends Rx.RxObject {
		constructor();
		static cast(inPtr: Rx.RxObject): ApDocManager;
		static cast(inPtr: number): ApDocManager;
		curDocument(): ApDocument;
		mdiActiveDocument(): ApDocument;
		isApplicationContext(): boolean;
		document(_database: any): ApDocument;
		lockDocument(pDoc: ApDocument, _mode: DocLockMode, pGlobalCmdName: string, pLocalCmdName: string, prompt: boolean): Acad.ErrorStatus;
		unlockDocument(pDoc: ApDocument): Acad.ErrorStatus;
		newApDocumentIterator(): ApDocumentIterator;
		addReactor(_Reactor: ApDocManagerReactor): void;
		removeReactor(_Reactor: ApDocManagerReactor): void;
		setCurDocument(pDoc: ApDocument, _mode: DocLockMode, activate: boolean): Acad.ErrorStatus;
		activateDocument(pAcTargetDocument: ApDocument, bPassScript: boolean): Acad.ErrorStatus;
		sendStringToExecute(pAcTargetDocument: ApDocument, pszExecute: string): Acad.ErrorStatus;
		sendStringToExecute(pAcTargetDocument: ApDocument, pszExecute: string, bActivate: boolean, bWrapUpInactiveDoc: boolean, bEchoString: boolean): Acad.ErrorStatus;
		appContextNewDocument(pszTemplateName: string): Acad.ErrorStatus;
		appContextOpenDocument(pszTemplateName: string): Acad.ErrorStatus;
		appContextRecoverDocument(pszTemplateName: string): Acad.ErrorStatus;
		appContextPromptNewDocument(): Acad.ErrorStatus;
		appContextPromptOpenDocument(): Acad.ErrorStatus;
		appContextCloseDocument(pDoc: ApDocument): Acad.ErrorStatus;
		documentCount(): number;
	}
}


import Ap = Module; 
export default Ap;

