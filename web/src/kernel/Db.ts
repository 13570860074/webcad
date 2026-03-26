import Ge from './Ge'
import Rx from './Rx'
import Ac from './Ac'
import Cm from './Cm'
import Acad from './Acad'

declare namespace Module {


    //Db::OpenMode
    enum OpenMode {
        kNotOpen,
        kForRead,
        kForWrite,
        kForNotify,
    }

    // lineweights are in 100ths of a millimeter
    enum LineWeight {
        kLnWt000 = 0,
        kLnWt005 = 5,
        kLnWt009 = 9,
        kLnWt013 = 13,
        kLnWt015 = 15,
        kLnWt018 = 18,
        kLnWt020 = 20,
        kLnWt025 = 25,
        kLnWt030 = 30,
        kLnWt035 = 35,
        kLnWt040 = 40,
        kLnWt050 = 50,
        kLnWt053 = 53,
        kLnWt060 = 60,
        kLnWt070 = 70,
        kLnWt080 = 80,
        kLnWt090 = 90,
        kLnWt100 = 100,
        kLnWt106 = 106,
        kLnWt120 = 120,
        kLnWt140 = 140,
        kLnWt158 = 158,
        kLnWt200 = 200,
        kLnWt211 = 211,
        kLnWtByLayer = -1,
        kLnWtByBlock = -2,
        kLnWtByLwDefault = -3
    }

    //Db::LineSpacingStyle
    enum LineSpacingStyle {
        kAtLeast,
        kExactly,
    }

    // DbMText::AttachmentPoint
    enum AttachmentPoint {
        kTopLeft,
        kTopCenter,
        kTopRight,
        kMiddleLeft,
        kMiddleCenter,
        kMiddleRight,
        kBottomLeft,
        kBottomCenter,
        kBottomRight,
        kBaseLeft,
        kBaseCenter,
        kBaseRight,
        kBaseAlign,
        kBottomAlign,
        kMiddleAlign,
        kTopAlign,
        kBaseFit,
        kBottomFit,
        kMiddleFit,
        kTopFit,
        kBaseMid,
        kBottomMid,
        kMiddleMid,
        kTopMid,
    }


    class DbHandle {
        constructor();
        constructor(_id: number);
        isNull(): boolean;
        getIntoAsciiBuffer(): string;
    }
    class DbObjectId {
        constructor();
        static cast(inPtr: number): DbObjectId;
        isNull(): boolean;
        setNull(): void;
        asOldId(): number;
        setFromOldId(_oldId: number): void;
        isValid(): boolean;
        isErased(): boolean;
        handle(): DbHandle;
    }
    abstract class GiDrawable extends Rx.RxObject {
    }
    abstract class DbObject extends GiDrawable {
        constructor();
        static cast(inPtr: Rx.RxObject): DbObject;
        static cast(inPtr: number): DbObject;
        objectId(): DbObjectId;
        setFromOldId(_id: number): void;
        getDbHandle(): DbHandle;
        handle(): DbHandle;
        ownerId(): DbObjectId;
        setOwnerId(_id: DbObjectId): void
        createExtensionDictionary(): void;
        extensionDictionary(): DbObjectId;
        releaseExtensionDictionary(): boolean;
        subOpen(_mode: OpenMode): boolean;
        close(): void;
        subClose(): void;
        erase(eraseIt: boolean): boolean;
        subErase(erasing: boolean): boolean;
        isErased(): boolean;
        isReadEnabled(): boolean;
        isWriteEnabled(): boolean;
        isNotifyEnabled(): boolean;
        isModified(): boolean;
        clone(): Rx.RxObject;
        isAProxy(): boolean;
    }


    class DbObjectIdArray extends Ac.AcArray {
        constructor();
    }


    class DbHostApplicationServices {
        constructor();
        load_dxf_data(data: string): void;
        workingDatabase(): DbDatabase;
        setWorkingDatabase(db: DbDatabase): void;
        layoutManager(): DbLayoutManager;
    }


    class DbDatabase {
        constructor();
        clayer(): DbObjectId;
        setClayer(_layerId: DbObjectId): Acad.ErrorStatus;
        cecolor(): Cm.CmColor;
        setCecolor(_color: Cm.CmColor): Acad.ErrorStatus;
        cetransparency(): Cm.CmTransparency;
        setCetransparency(_transparency: Cm.CmTransparency): Acad.ErrorStatus;
        getLayerTable(): DbLayerTable;
        getLayerTable(mode: OpenMode): DbLayerTable;
        getLayoutDictionary(): DbDictionary;
        getLayoutDictionary(mode: OpenMode): DbDictionary;
    }


    abstract class DbSymbolTableIterator extends Rx.RxObject {
        getRecordId(): DbObjectId;
        getRecord(): DbSymbolTableRecord;
        getRecord(openErasedRec: boolean): DbSymbolTableRecord;
        getRecord(openMode: OpenMode, openErasedRec: boolean): DbSymbolTableRecord;

        start(): void;
        start(atBeginning: boolean): void;
        start(atBeginning: boolean, skipDeleted: boolean): void;

        done(): boolean;

        step(): void;
        step(forward: boolean): void;
        step(forward: boolean, skipDeleted: boolean): void;
    }
    class DbLayerTableIterator extends DbSymbolTableIterator {
        constructor();
        getRecord(): DbLayerTableRecord;
        getRecord(openErasedRec: boolean): DbLayerTableRecord;
        getRecord(openMode: OpenMode, openErasedRec: boolean): DbLayerTableRecord;
    }


    abstract class DbSymbolTable extends DbObject {
        getAt(_entryName: string): DbSymbolTableRecord;
        getAt(_entryName: string, openMode: OpenMode): DbSymbolTableRecord;
        getAt(_entryName: string, openMode: OpenMode, openErasedRec: boolean): DbSymbolTableRecord;

        getIdAt(entryName: string): DbObjectId;
        getIdAt(entryName: string, getErasedRecord: boolean): DbObjectId;

        has(_name: string): boolean;
        has(_id: DbObjectId): boolean;

        add(pRecord: DbSymbolTableRecord): Acad.ErrorStatus;

        newIterator(): DbSymbolTableIterator;
        newIterator(atBeginning: boolean): DbSymbolTableIterator;
        newIterator(atBeginning: boolean, skipDeleted: boolean): DbSymbolTableIterator;
    }
    class DbLayerTable extends DbSymbolTable {
        constructor();

        getAt(_entryName: string): DbLayerTableRecord;
        getAt(_entryName: string, openMode: OpenMode): DbLayerTableRecord;
        getAt(_entryName: string, openMode: OpenMode, openErasedRec: boolean): DbLayerTableRecord;

        newIterator(): DbLayerTableIterator;

        add(pRecord: DbLayerTableRecord): Acad.ErrorStatus;
    }


    abstract class DbEntity extends DbObject {
        blockId(): DbObjectId;
        color(): Cm.CmColor;
        setColor(_color: Cm.CmColor): Acad.ErrorStatus;
        setColor(_color: Cm.CmColor, doSubents: boolean): Acad.ErrorStatus;
        colorIndex(): number;
    }
    class DbLine extends DbEntity {
        constructor();
        static cast(inPtr: Rx.RxObject): DbLine;
        static cast(inPtr: number): DbLine;

        startPoint(): Ge.GePoint3d;
        setStartPoint(_startPoint: Ge.GePoint3d): Acad.ErrorStatus;
        endPoint(): Ge.GePoint3d;
        setEndPoint(_endPoint: Ge.GePoint3d): Acad.ErrorStatus;
    }
    abstract class DbDimension extends DbEntity {
        static cast(inPtr: Rx.RxObject): DbDimension;
        static cast(inPtr: number): DbDimension;

        textDefinedWidth(): number;
        textDefinedHeight(): number;
        setTextDefinedSize(width: number, height: number): void;
        resetTextDefinedSize(): void;
        textPosition(): Ge.GePoint3d;
        setTextPosition(pt: Ge.GePoint3d): Acad.ErrorStatus;
        isUsingDefaultTextPosition(): boolean;
        useSetTextPosition(): Acad.ErrorStatus;
        useDefaultTextPosition(): Acad.ErrorStatus;
        setUsingDefaultTextPosition(): Acad.ErrorStatus;
        normal(): Ge.GeVector3d;
        setNormal(n: Ge.GeVector3d): Acad.ErrorStatus;
        isPlanar(): boolean;
        getPlane(): Ge.GePlane;
        elevation(): number;
        setElevation(v: number): Acad.ErrorStatus;
        dimensionText(): string;
        setDimensionText(text: string): Acad.ErrorStatus;
        textRotation(): number;
        setTextRotation(r: number): Acad.ErrorStatus;
        dimensionStyle(): Db.DbObjectId;
        setDimensionStyle(styleId: Db.DbObjectId): Acad.ErrorStatus;
        textAttachment(): AttachmentPoint;
        setTextAttachment(eAtt: AttachmentPoint): Acad.ErrorStatus;
        textLineSpacingStyle(): LineSpacingStyle;
        setTextLineSpacingStyle(eStyle: LineSpacingStyle): Acad.ErrorStatus;
        textLineSpacingFactor(): number;
        setTextLineSpacingFactor(dFactor: number): Acad.ErrorStatus;
    }
    class DbAlignedDimension extends DbDimension {
        constructor();
        constructor(xLine1Point: Ge.GePoint3d, xLine2Point: Ge.GePoint3d, dimLinePoint: Ge.GePoint3d);
        static cast(inPtr: Rx.RxObject): DbAlignedDimension;
        static cast(inPtr: number): DbAlignedDimension;

        xLine1Point(): Ge.GePoint3d;
        setXLine1Point(pt: Ge.GePoint3d): Acad.ErrorStatus;
        xLine2Point(): Ge.GePoint3d;
        setXLine2Point(pt: Ge.GePoint3d): Acad.ErrorStatus;
        dimLinePoint(): Ge.GePoint3d;
        setDimLinePoint(pt: Ge.GePoint3d): Acad.ErrorStatus;
        oblique(): number;
        setOblique(v: number): Acad.ErrorStatus;
        jogSymbolOn(): boolean;
        setJogSymbolOn(v: boolean): Acad.ErrorStatus;
        jogSymbolPosition(): Ge.GePoint3d;
        setJogSymbolPosition(pt: Ge.GePoint3d): Acad.ErrorStatus;
    }
    class DbArcDimension extends DbDimension {
        constructor();
        constructor(centerPoint: Ge.GePoint3d, xLine1Point: Ge.GePoint3d, xLine2Point: Ge.GePoint3d, arcPoint: Ge.GePoint3d);
        static cast(inPtr: Rx.RxObject): DbArcDimension;
        static cast(inPtr: number): DbArcDimension;

        // Query method which returns a point on the arc length dimension's dimension arc.
        arcPoint(): Ge.GePoint3d;
        // Set method to set the point which the arc length dimension's dimension arc passes through.
        setArcPoint(arcPt: Ge.GePoint3d): Acad.ErrorStatus;

        // Query method which returns the start point for the arc length dimension's first extension line.
        xLine1Point(): Ge.GePoint3d;
        // Set method which sets the start point for the arc length dimension's first extension line.        
        setXLine1Point(xLine1Pt: Ge.GePoint3d): Acad.ErrorStatus;

        // Query method which returns the start point for the arc length dimension's second extension line.
        xLine2Point(): Ge.GePoint3d;
        // Set method which sets the start point for the arc length dimension's second extension line.    
        setXLine2Point(xLine2Pt: Ge.GePoint3d): Acad.ErrorStatus;

        // Query method which returns the center point of the arc dimensioned by the arc length dimension.
        centerPoint(): Ge.GePoint3d;
        // Set method which sets the center point of the arc dimensioned by the arc length dimension.    
        setCenterPoint(ctrPt: Ge.GePoint3d): Acad.ErrorStatus;

        // Query method which returns true if this arc length dimension was specified using two points along the arc, 
        // false if the arc length dimension dimensions the entire arc segment.
        isPartial(): boolean;
        // Set method if passed true, the arc length dimension was specified using the entire arc segment, 
        // if passed false the arc length dimension arc length dimension was specified using two points along the arc.
        setIsPartial(partial: boolean): Acad.ErrorStatus;

        // Query method which returns the parameter of the arc being dimensioned cooresponding to the arc length dimension's first definiton point.
        arcStartParam(): number;
        // Set method which sets the parameter of the arc being dimensioned cooresponding to the arc length dimension's first definiton point.        
        setArcStartParam(arcParam: number): Acad.ErrorStatus;

        // Query method which returns the parameter of the arc being dimensioned cooresponding to the arc length dimension's second definiton point.
        arcEndParam(): number;
        // Set method which sets the parameter of the arc being dimensioned cooresponding to the arc length dimension's second definiton point.    
        setArcEndParam(arcParam: number): Acad.ErrorStatus;

        // Query method which returns true if this arc length dimension has an extra leader drawn to resolve ambiguity, false if the arc length dimension has no extra leader drawn.
        hasLeader(): boolean;
        // Set method if passed true, the arc length dimension has an extra leader drawn to resolve ambiguity, if passed false the arc length dimension has no extra leader drawn.    
        setHasLeader(leaderVal: boolean): Acad.ErrorStatus;

        // Query method which returns the start point for the arc length dimension's extra leader, if drawn.
        leader1Point(): Ge.GePoint3d;
        // Set method which sets the start point for the arc length dimension's extra leader, if drawn.        
        setLeader1Point(ldr1Pt: Ge.GePoint3d): Acad.ErrorStatus;

        // Query method which returns the end point for the arc length dimension's extra leader, if drawn.
        leader2Point(): Ge.GePoint3d;
        // Set method which sets the end point for the arc length dimension's extra leader, if drawn.    
        setLeader2Point(ldr2Pt: Ge.GePoint3d): Acad.ErrorStatus;

        // Query method which returns the type of symbol used within the arc length dimension's text string:
        // 0 - Arc symbol precedes text, 1 - Arc symbol is above text, 2 - No arc symbol is used.
        arcSymbolType(): number;
        // Set method which sets the type of symbol to use within the arc length dimension's text string:
        // 0 - Arc symbol precedes text, 1 - Arc symbol is above text, 2 - No arc symbol is used.
        // This overrides the setting of this value in the dimension's style.
        setArcSymbolType(symbol: number): Acad.ErrorStatus;
    }
    class DbJigRealDimension extends DbDimension {
        constructor();
        static cast(inPtr: Rx.RxObject): DbJigRealDimension;
        static cast(inPtr: number): DbJigRealDimension;

        value(): number;
        setValue(v: number): void;
    }
    class DbJigStringDimension extends DbDimension {
        constructor();
        static cast(inPtr: Rx.RxObject): DbJigStringDimension;
        static cast(inPtr: number): DbJigStringDimension;

        value(): string;
        setValue(v: string): void;
    }
    class DbJigDistDimension extends DbJigRealDimension {
        constructor();
        static cast(inPtr: Rx.RxObject): DbJigDistDimension;
        static cast(inPtr: number): DbJigDistDimension;
    }
    class DbJigAngleDimension extends DbJigRealDimension {
        constructor();
        static cast(inPtr: Rx.RxObject): DbJigAngleDimension;
        static cast(inPtr: number): DbJigAngleDimension;
    }
    class DbOrdinateDimension extends DbDimension {
        constructor();
        constructor(useXAxis: boolean, definingPoint: Ge.GePoint3d, leaderEndPoint: Ge.GePoint3d);
        static cast(inPtr: Rx.RxObject): DbOrdinateDimension;
        static cast(inPtr: number): DbOrdinateDimension;

        isUsingXAxis(): boolean;
        isUsingYAxis(): boolean;
        useXAxis(): Acad.ErrorStatus;
        useYAxis(): Acad.ErrorStatus;
        setUsingXAxis(value: boolean): Acad.ErrorStatus;
        setUsingYAxis(value: boolean): Acad.ErrorStatus;
        origin(): Ge.GePoint3d;
        setOrigin(pt: Ge.GePoint3d): Acad.ErrorStatus;
        definingPoint(): Ge.GePoint3d;
        setDefiningPoint(pt: Ge.GePoint3d): Acad.ErrorStatus;
        leaderEndPoint(): Ge.GePoint3d;
        setLeaderEndPoint(pt: Ge.GePoint3d): Acad.ErrorStatus;
    }
    class DbRadialDimension extends DbDimension {
        constructor();
        constructor(center: Ge.GePoint3d, chordPoint: Ge.GePoint3d, leaderLength: number);
        static cast(inPtr: Rx.RxObject): DbRadialDimension;
        static cast(inPtr: number): DbRadialDimension;

        leaderLength(): number;
        setLeaderLength(v: number): Acad.ErrorStatus;
        center(): Ge.GePoint3d;
        setCenter(cen: Ge.GePoint3d): Acad.ErrorStatus;
        chordPoint(): Ge.GePoint3d;
        setChordPoint(pt: Ge.GePoint3d): Acad.ErrorStatus;
        // Radial dimension extension arc
        extArcStartAngle(): number;
        setExtArcStartAngle(newAngle: number): Acad.ErrorStatus;
        extArcEndAngle(): number;
        setExtArcEndAngle(newAngle: number): Acad.ErrorStatus;
    }
    class DbRadialDimensionLarge extends DbDimension {
        constructor();
        constructor(center: Ge.GePoint3d, chordPoint: Ge.GePoint3d, overrideCenter: Ge.GePoint3d, jogPoint: Ge.GePoint3d, jogAngle: number);
        static cast(inPtr: Rx.RxObject): DbRadialDimensionLarge;
        static cast(inPtr: number): DbRadialDimensionLarge;

        // Query method which returns the center point of the arc dimensioned by the jogged radius dimension.    
        center(): Ge.GePoint3d
        // Set method which sets the center point of the arc dimensioned by the jogged radius dimension.        
        setCenter(centerPoint: Ge.GePoint3d): Acad.ErrorStatus;

        // Query method which returns the chord point on the arc dimensioned by the jogged radius dimension.    
        chordPoint(): Ge.GePoint3d;
        // Set method which sets the chord point on the arc dimensioned by the jogged radius dimension.        
        setChordPoint(chordPoint: Ge.GePoint3d): Acad.ErrorStatus;

        // Query method which returns the overidden center point used by the jogged radius dimension.    
        overrideCenter(): Ge.GePoint3d
        // Set method which sets the overidden center point to use for the jogged radius dimension.        
        setOverrideCenter(overrideCenterPoint: Ge.GePoint3d): Acad.ErrorStatus;

        // Query method which returns the jog point used by the jogged radius dimension.    
        jogPoint(): Ge.GePoint3d
        // Set method which sets the jog point used by the jogged radius dimension.        
        setJogPoint(jogPoint: Ge.GePoint3d): Acad.ErrorStatus;

        // Query method which returns the jog angle used by the jogged radius dimension.    
        jogAngle(): number;
        // Set method which sets the jog angle used by the jogged radius dimension.        
        setJogAngle(jogAngle: number): Acad.ErrorStatus;

        // Used exclusively by property palette
        // This function is required because once the jogged radius dimension's defining points are updated, all the other defining points need to be updated too.
        setOverrideCenterPP(overrideCenterPointPP: Ge.GePoint3d): Acad.ErrorStatus;
        // Used exclusively by property palette
        // This function is required because once the jogged radius dimension's defining points are updated, all the other defining points need to be updated too.    
        setJogPointPP(jogPointPP: Ge.GePoint3d): Acad.ErrorStatus;
        // Used exclusively by property palette
        // This function is required because once the jogged radius dimension's defining points are updated, all the other defining points need to be updated too.    
        setTextPositionPP(textPointPP: Ge.GePoint3d): Acad.ErrorStatus;

        // Radial large dimension extension arc
        extArcStartAngle(): number;
        setExtArcStartAngle(newAngle: number): Acad.ErrorStatus;

        extArcEndAngle(): number;
        setExtArcEndAngle(newAngle: number): Acad.ErrorStatus;
    }
    class DbRotatedDimension extends DbDimension {
        constructor();
        constructor(rotation: number, xLine1Point: Ge.GePoint3d, xLine2Point: Ge.GePoint3d, dimLinePoint: Ge.GePoint3d);
        static cast(inPtr: Rx.RxObject): DbRotatedDimension;
        static cast(inPtr: number): DbRotatedDimension;

        xLine1Point(): Ge.GePoint3d;
        setXLine1Point(pt: Ge.GePoint3d): Acad.ErrorStatus;
        xLine2Point(): Ge.GePoint3d;
        setXLine2Point(pt: Ge.GePoint3d): Acad.ErrorStatus;
        dimLinePoint(): Ge.GePoint3d;
        setDimLinePoint(pt: Ge.GePoint3d): Acad.ErrorStatus;
        oblique(): number;
        setOblique(v: number): Acad.ErrorStatus;
        rotation(): number;
        setRotation(r: number): Acad.ErrorStatus;
        // Rotated dimension Jog symbol
        jogSymbolOn(): boolean;
        setJogSymbolOn(value: boolean): Acad.ErrorStatus;
        jogSymbolPosition(): Ge.GePoint3d;
        setJogSymbolPosition(pt: Ge.GePoint3d): Acad.ErrorStatus;
    }
    class Db2LineAngularDimension extends DbDimension {
        constructor();
        constructor(xLine1Start: Ge.GePoint3d, xLine1End: Ge.GePoint3d, xLine2Start: Ge.GePoint3d, xLine2End: Ge.GePoint3d, arcPoint: Ge.GePoint3d);
        static cast(inPtr: Rx.RxObject): Db2LineAngularDimension;
        static cast(inPtr: number): Db2LineAngularDimension;

        arcPoint(): Ge.GePoint3d;
        setArcPoint(pt: Ge.GePoint3d): Acad.ErrorStatus;
        xLine1Start(): Ge.GePoint3d;
        setXLine1Start(pt: Ge.GePoint3d): Acad.ErrorStatus;
        xLine1End(): Ge.GePoint3d;
        setXLine1End(pt: Ge.GePoint3d): Acad.ErrorStatus;
        xLine2Start(): Ge.GePoint3d;
        setXLine2Start(pt: Ge.GePoint3d): Acad.ErrorStatus;
        xLine2End(): Ge.GePoint3d;
        setXLine2End(pt: Ge.GePoint3d): Acad.ErrorStatus;
        // Angular dimension extension arc for quadrant
        extArcOn(): boolean;
        setExtArcOn(value: boolean): Acad.ErrorStatus;
    }
    class Db3PointAngularDimension extends DbDimension {
        constructor();
        constructor(centerPoint: Ge.GePoint3d, xLine1Point: Ge.GePoint3d, xLine2Point: Ge.GePoint3d, arcPoint: Ge.GePoint3d);
        static cast(inPtr: Rx.RxObject): Db3PointAngularDimension;
        static cast(inPtr: number): Db3PointAngularDimension;

        arcPoint(): Ge.GePoint3d;
        setArcPoint(pt: Ge.GePoint3d): Acad.ErrorStatus;
        xLine1Point(): Ge.GePoint3d;
        setXLine1Point(pt: Ge.GePoint3d): Acad.ErrorStatus;
        xLine2Point(): Ge.GePoint3d;
        setXLine2Point(pt: Ge.GePoint3d): Acad.ErrorStatus;
        centerPoint(): Ge.GePoint3d;
        setCenterPoint(pt: Ge.GePoint3d): Acad.ErrorStatus;
        // Angular dimension extension arc for quadrant
        extArcOn(): boolean;
        setExtArcOn(value: boolean): Acad.ErrorStatus;
    }


    abstract class DbSymbolTableRecord extends DbObject {
        getName(): string;
        setName(_name: string): Acad.ErrorStatus;
    }
    class DbLayerTableRecord extends DbSymbolTableRecord {
        constructor();
        static cast(inPtr: Rx.RxObject): DbLayerTableRecord;
        static cast(inPtr: number): DbLayerTableRecord;

        isFrozen(): boolean;
        setIsFrozen(frozen: boolean): Acad.ErrorStatus;

        isOff(): boolean;
        setIsOff(frozen: boolean): void;

        VPDFLT(): boolean;
        setVPDFLT(frozen: boolean): void;

        isLocked(): boolean;
        setIsLocked(frozen: boolean): void;

        color(): Cm.CmColor;
        setColor(_color: Cm.CmColor): void;
        entityColor(): Cm.CmEntityColor;

        transparency(): Cm.CmTransparency;
        setTransparency(_transparency: Cm.CmTransparency): Acad.ErrorStatus;

        linetypeObjectId(): DbObjectId;
        setLinetypeObjectId(id: DbObjectId): Acad.ErrorStatus;

        isPlottable(): boolean;
        setIsPlottable(plot: boolean): Acad.ErrorStatus;
    }


    class DbDictionary extends DbObject {
        constructor();
        static cast(inPtr: Rx.RxObject): DbDictionary;
        static cast(inPtr: number): DbDictionary;

        getAt(name: string): DbObjectId;
        getAt(name: string, mode: OpenMode): DbObject;
        nameAt(id: DbObjectId): string;
        has(name: string): boolean;
        numEntries(): number;
        remove(name: string): DbObjectId;
        remove(id: DbObjectId): Acad.ErrorStatus;
        setName(oldName: string, newName: string): boolean;
        setAt(srchKey: string, newValue: DbObject): DbObjectId;
        newIterator(): DbDictionaryIterator;
    }
    class DbDictionaryTest {
        constructor();
    }
    class DbDictionaryIterator extends Rx.RxObject {
        constructor();
        static cast(inPtr: Rx.RxObject): DbDictionaryIterator;
        static cast(inPtr: number): DbDictionaryIterator;

        name(): string;
        object(): DbObject;
        object(mode: OpenMode): DbObject;
        objectId(): DbObjectId;
        done(): boolean;
        next(): boolean;
    }


    class DbPlotSettings extends Db.DbObject {
        constructor();
        static cast(inPtr: Rx.RxObject): DbPlotSettings;
        static cast(inPtr: number): DbPlotSettings;
    }
    class DbLayout extends Db.DbPlotSettings {
        constructor();
        static cast(inPtr: Rx.RxObject): DbLayout;
        static cast(inPtr: number): DbLayout;

        getBlockTableRecordId(): DbObjectId;
        setBlockTableRecordId(BlockTableRecordId: DbObjectId): Acad.ErrorStatus;

        //addToLayoutDict(towhichDb: DbDatabase, BlockTableRecordId: DbObjectId): Acad.ErrorStatus;

        getName(): string;
        getLayoutName(): string;
        setLayoutName(layoutName: string): Acad.ErrorStatus;

        getTabOrder(): number;
        setTabOrder(newOrder: number): void;

        getTabSelected(): boolean;
        setTabSelected(tabSelected: boolean): void;

        //getViewportArray(): DbObjectIdArray;
    }
    class DbLayoutManagerReactor extends Rx.RxObject {
        constructor();
        static cast(inPtr: Rx.RxObject): DbLayoutManagerReactor;
        static cast(inPtr: number): DbLayoutManagerReactor;

        onLayoutCreated(_inPtr: number): void;
        onLayoutToBeRemoved(_inPtr: number): void;
        onLayoutRemoved(_inPtr: number): void;
        onAbortLayoutRemoved(_inPtr: number): void;
        onLayoutToBeCopied(_inPtr: number): void;
        onLayoutCopied(_inPtr: number): void;
        onAbortLayoutCopied(_inPtr: number): void;
        onLayoutToBeRenamed(_inPtr: number): void;
        onLayoutRenamed(_inPtr: number): void;
        onAbortLayoutRename(_inPtr: number): void;
        onLayoutSwitched(_inPtr: number): void;
        onPlotStyleTableChanged(_inPtr: number): void;
        onLayoutsReordered(_inPtr: number): void;
        onRefreshLayoutTabs(_inPtr: number): void;
    }
    class DbLayoutManager extends Rx.RxObject {
        constructor();
        static cast(inPtr: Rx.RxObject): DbLayoutManager;
        static cast(inPtr: number): DbLayoutManager;

        setCurrentLayout(_name: string): Acad.ErrorStatus;
        setCurrentLayout(_name: string, db: DbDatabase): Acad.ErrorStatus;
        setCurrentLayoutId(id: DbObjectId): Acad.ErrorStatus;

        getActiveLayoutName(allowModel: boolean): string;
        getActiveLayoutName(allowModel: boolean, db: DbDatabase): string;

        findActiveLayout(allowModel: boolean): string;
        findActiveLayout(allowModel: boolean, db: DbDatabase): string;

        getActiveLayoutBTRId(): DbObjectId;
        getActiveLayoutBTRId(db: DbDatabase): DbObjectId;

        findLayoutNamed(_name: string): DbObjectId;
        findLayoutNamed(_name: string, db: DbDatabase): DbObjectId;

        getLayoutNamed(_name: string): DbLayout;
        getLayoutNamed(_name: string, db: DbDatabase): DbLayout;

        layoutExists(_name: string): boolean;
        layoutExists(_name: string, db: DbDatabase): boolean;

        copyLayout(copyname: string, newname: string): Acad.ErrorStatus;
        copyLayout(copyname: string, newname: string, db: DbDatabase): Acad.ErrorStatus;

        deleteLayout(delname: string): Acad.ErrorStatus;
        deleteLayout(delname: string, db: DbDatabase): Acad.ErrorStatus;

        createLayout(newname: string): Acad.ErrorStatus;
        createLayout(newname: string, db: DbDatabase): Acad.ErrorStatus;

        renameLayout(oldname: string, newname: string): Acad.ErrorStatus;
        renameLayout(oldname: string, newname: string, db: DbDatabase): Acad.ErrorStatus;

        cloneLayout(pLBTR: DbLayout, newname: string): Acad.ErrorStatus;
        cloneLayout(pLBTR: DbLayout, newname: string, newTabOrder: boolean): Acad.ErrorStatus;
        cloneLayout(pLBTR: DbLayout, newname: string, newTabOrder: boolean, db: DbDatabase): Acad.ErrorStatus;

        getNonRectVPIdFromClipId(clipId: DbObjectId): DbObjectId;

        isVpnumClipped(index: number): boolean;
        isVpnumClipped(index: number, db: DbDatabase): boolean;

        countLayouts(): number;
        countLayouts(db: DbDatabase): number;

        addReactor(newObj: DbLayoutManagerReactor): void;

        removeReactor(delObj: DbLayoutManagerReactor): void;
    }

    class DbDimData {
        constructor();

        ownerId(): Db.DbObjectId;
        setOwnerId(objId: DbObjectId): Acad.ErrorStatus;
        dimension(): Db.DbDimension;
        setDimension(dim: Db.DbDimension): Acad.ErrorStatus;
        bitFlags(): number;
        setBitFlags(flags: number): Acad.ErrorStatus;
        isDimFocal(): boolean;
        setDimFocal(focal: boolean): Acad.ErrorStatus;
        isDimEditable(): boolean;
        setDimEditable(editable: boolean): Acad.ErrorStatus;
        isDimInvisible(): boolean;
        setDimInvisible(invisible: boolean): Acad.ErrorStatus;
        isDimHideIfValueIsZero(): boolean;
        setDimHideIfValueIsZero(hide: boolean): Acad.ErrorStatus;
        isDimResultantLength(): boolean;
        setDimResultantLength(bValue: boolean): Acad.ErrorStatus;
        isDimDeltaLength(): boolean;
        setDimDeltaLength(bValue: boolean): Acad.ErrorStatus;
        isDimResultantAngle(): boolean;
        setDimResultantAngle(bValue: boolean): Acad.ErrorStatus;
        isDimDeltaAngle(): boolean;
        setDimDeltaAngle(bValue: boolean): Acad.ErrorStatus;
        isDimRadius(): boolean;
        setDimRadius(bValue: boolean): Acad.ErrorStatus;
        isCustomDimValue(): boolean;
        setCustomDimValue(custom: boolean): Acad.ErrorStatus;
        isConstrained(): boolean;
        setConstrain(bValue: boolean): Acad.ErrorStatus;
        isCustomString(): boolean;
        setCustomString(bValue: boolean): Acad.ErrorStatus;
    }

    class DbDimDataPtrArray extends Ac.AcArray {
        constructor();
    }
    class DbDimDataArray extends Ac.AcArray {
        constructor();
    }
}

import Db = Module;
export default Db;



