
import Cm from './Cm';
import Db from './Db';

declare namespace Module {

    // RxObject
    abstract class RxObject {
        constructor();
        static cast(inPtr: RxObject): RxObject;
        static desc(): Rx.RxClass;
        isA(): Rx.RxClass;
    }

    // RxManager
    class RxManager extends RxObject {
        constructor();
    }

    // RxClass
    class RxClass extends RxObject {
        constructor();

        name(): string;
    }

    // RxIterator
    abstract class RxIterator extends RxObject {
        done(): boolean;
        next(): boolean;
        object(): RxObject;
    }

    // RxDictionaryIterator
    abstract class RxDictionaryIterator extends RxIterator {
        static cast(inPtr: RxObject): RxDictionaryIterator;
        static cast(inPtr: number): RxDictionaryIterator;
        key(): string;
        object(): RxObject;
        id(): number;
    }

    // RxDictionary
    abstract class RxDictionary extends RxObject {
        static cast(inPtr: RxObject): RxDictionary;
        static cast(inPtr: number): RxDictionary;
        at(key: string): RxObject;
        at(id: number): RxObject;
        atPut(key: string, object: RxObject): RxObject;
        atPut(key: string, object: RxObject, retId: number): RxObject;
        atPut(id: number, object: RxObject): RxObject;
        resetKey(oldKey: string, newKey: string): boolean;
        resetKey(id: number, newKey: string): boolean;
        atKeyAndIdPut(newKey: string, id: number, object: RxObject): boolean;
        remove(key: string): RxObject;
        remove(id: number): RxObject;
        has(entryName: string): boolean;
        has(id: number): boolean;
        idAt(key: string): number;
        keyAt(id: number): string;
        numEntries(): number;
        newIterator(): RxDictionaryIterator;
        deletesObjects(): boolean;
        isCaseSensitive(): boolean;
        isSorted(): boolean;
    }

    // RxEventReactor
    abstract class RxEventReactor extends RxObject {
        static cast(inPtr: RxObject): RxEventReactor;
        static cast(inPtr: number): RxEventReactor;
    }

    // RxEvent
    abstract class RxEvent extends RxObject {
        static cast(inPtr: RxObject): RxEvent;
        static cast(inPtr: number): RxEvent;
        abstract addReactor(reactor: RxEventReactor): void;
        abstract removeReactor(reactor: RxEventReactor): void;
    }


    enum EPropertyType {
		kNone,
		kMultiple,
		
		kString,
		kInt,
		kReal,
		kColor,
		kObjectId,
		kLineWeight,
		kTransparency,
    }
    abstract class RxPropertyValue {
        propertyType(): EPropertyType;
        setPropertyType(t: EPropertyType): void;
    }
    class RxPropertyString extends RxPropertyValue {
        constructor();
        value(): string;
        setValue(v: string): void;
    }
    class RxPropertyInt extends RxPropertyValue {
        constructor();
        value(): number;
        setValue(v: number): void;
    }
    class RxPropertyReal extends RxPropertyValue {
        constructor();
        value(): number;
        setValue(v: number): void;
    }
    class RxPropertyObjectId extends RxPropertyValue {
        constructor();
        value(): Db.DbObjectId;
        setValue(v: Db.DbObjectId): void;
    }
    class RxPropertyColor extends RxPropertyValue {
        constructor();
        value(): Cm.CmColor;
        setValue(v: Cm.CmColor): void;
    }
    class RxPropertyLineWeight extends RxPropertyValue {
        constructor();
        value(): Db.LineWeight;
        setValue(v: Db.LineWeight): void;
    }
    class RxPropertyTransparency extends RxPropertyValue {
        constructor();
        value(): Cm.CmTransparency;
        setValue(v: Cm.CmTransparency): void;
    }

    class RxPropertyTable {
        constructor();
        static cast(inPtr: RxObject): RxPropertyTable;
        static cast(inPtr: number): RxPropertyTable;
        numEntity(): number;
        setNumEntity(n: number): void;
        layer(): RxPropertyObjectId;
        color(): RxPropertyColor;
        lineWeight(): RxPropertyLineWeight;
        transparency(): RxPropertyTransparency;
    }

    class RxPropertyReactor extends RxObject {
        constructor();
        static cast(inPtr: RxObject): RxPropertyReactor;
        static cast(inPtr: number): RxPropertyReactor;
        onUpdatePropertyTable(inPtr: number): void;
    }
    class RxPropertyManager extends RxObject{
        constructor();
        static cast(inPtr: RxObject): RxPropertyManager;
        static cast(inPtr: number): RxPropertyManager;

        updatePropertyTable(): void;
        addReactor(reactor: RxPropertyReactor): void;
        removeReactor(reactor: RxPropertyReactor): void;
    }
}



import Rx = Module;
export default Rx;


