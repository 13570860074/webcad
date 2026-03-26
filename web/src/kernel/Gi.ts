
import Rx from './Rx'
import Ge from './Ge'


declare namespace Module {

	/* 材质类型 */
	enum MaterialType {
		kUnknownMaterial = 100,					// 未知材质
		kPointBasic,							// 点
		kLineBasic,                             // 线
		kMeshBasic,                             // mesh
		kLineDashed,                            // 虚线
		kLinePixelDashed,                       // 不随相机变化的的虚线(主要是用在高亮线上)
	}

	/* 几何对象类型 */
	enum EntityType {
		kUnknownEntity = 200,
		kPoints,
		kLineSegment,
		kMesh,
		kLinear,
		kRay,
	}

	/* 图元类型 */
	enum ElementType {
        kUnknownElement = 400,
        kElement,
        kDragging,
        kHighlight,
        kCursor,
        kCoordSystem,
        kGrip,
        kOsnap,
        kGridline,
        kDynDim,
	}


	class GiObjectId {
		constructor();
		isNull(): boolean;
		setNull(): void;
		asOldId(): number;
		setFromOldId(oldId: number): GiObjectId;
		isValid(): boolean;
	}
	abstract class GiObject extends Rx.RxObject {
		static cast(inPtr: Rx.RxObject): GiObject;
		static cast(inPtr: number): GiObject;
		objectId(): GiObjectId;
		setFromOldId(oldId: number): GiObjectId;
	}
	abstract class GiMaterial extends GiObject {
		static cast(inPtr: Rx.RxObject): GiMaterial;
		static cast(inPtr: number): GiMaterial;
		abstract type(): MaterialType;
		color(): number;
		colorIndex(): number;
		transparency(): number;
		setColor(_color: number): void;
		setColorIndex(_color: number): void;
		setTransparency(_transparency: number): void;
	}
	class GiPointMaterial extends GiMaterial {
		constructor();
		static cast(inPtr: Rx.RxObject): GiPointMaterial;
		static cast(inPtr: number): GiPointMaterial;

		type(): MaterialType;
		size(): number;
		setSize(_size: number): void;
	}
	class GiLineMaterial extends GiMaterial {
		constructor();
		static cast(inPtr: Rx.RxObject): GiLineMaterial;
		static cast(inPtr: number): GiLineMaterial;

		type(): MaterialType;
		lineWidth(): number;
		setLineWidth(_lineWidth: number): void;
	}
	class GiLineDashedMaterial extends GiMaterial {
		constructor();
		static cast(inPtr: Rx.RxObject): GiLineDashedMaterial;
		static cast(inPtr: number): GiLineDashedMaterial;

		type(): MaterialType;
		dashSize(): number;
		setDashSize(val: number): void;

		gapSize(): number;
		setGapSize(val: number): void;

		lineWidth(): number;
		setLineWidth(_lineWidth: number): void;
	}
	class GiLinePixelDashedMaterial extends GiMaterial {
		constructor();
		static cast(inPtr: Rx.RxObject): GiLinePixelDashedMaterial;
		static cast(inPtr: number): GiLinePixelDashedMaterial;

		type(): MaterialType;

		pixelRatio(): number;
		setPixelRatio(val: number): void;

		dashSize(): number;
		setDashSize(val: number): void;

		gapSize(): number;
		setGapSize(val: number): void;

		lineWidth(): number;
		setLineWidth(_lineWidth: number): void;
	}
	class GiMeshMaterial extends GiMaterial {
		constructor();
		static cast(inPtr: Rx.RxObject): GiMeshMaterial;
		static cast(inPtr: number): GiMeshMaterial;

		type(): MaterialType;
	}
	class GiMaterialIterator extends Rx.RxObject {
		constructor();
		static cast(inPtr: Rx.RxObject): GiMaterialIterator;
		static cast(inPtr: number): GiMaterialIterator;
		start(): void;
		done(): boolean;
		step(): void;
		getMaterial(): GiMaterial;
	}
	class GiMaterialReactor extends Rx.RxObject {
		constructor();
		static cast(inPtr: Rx.RxObject): GiMaterialReactor;
		static cast(inPtr: number): GiMaterialReactor;
		onCreateMaterial(_inPtr: number): void;
		onRemoveMaterial(_inPtr: number): void;
	}
	class GiMaterialManager extends GiObject {
		constructor();
		static cast(inPtr: Rx.RxObject): GiMaterialManager;
		static cast(inPtr: number): GiMaterialManager;
		addMaterial(_pMaterial: GiMaterial): GiObjectId
		addReactor(_pReactor: GiMaterialReactor): void;
		newIterator(): GiMaterialIterator;
	}


	abstract class GiEntity extends GiObject {
		static cast(inPtr: Rx.RxObject): GiEntity;
		static cast(inPtr: number): GiEntity;

		abstract type(): EntityType;
		appendVertex(_x: number, _y: number, _z: number): void;
		vertexLength(): number;
		vertexs(): Float64Array;
		element(): ElementType;
		visibility(): boolean;
		material(): GiMaterial;
		matrix(): Ge.GeMatrix3d;
		setMatrix(_matrix: Ge.GeMatrix3d): void;
		applyMatrix(_matrix: Ge.GeMatrix3d): Ge.GeMatrix3d;
		quaternion(): Ge.GeQuaternion;
		position(): Ge.GePoint3d;

		setElement(_style: ElementType): void;
		setVisibility(_isVisibility: boolean): void;
		setMaterial(m: GiMaterial): void;
		setPosition(_point: Ge.GePoint3d): void;
	}
	class GiLineSegment extends GiEntity {
		constructor();
		static cast(inPtr: Rx.RxObject): GiLineSegment;
		static cast(inPtr: number): GiLineSegment;

		type(): EntityType;
		vertexs(): Float64Array;
	}
	class GiMesh extends GiEntity {
		constructor();

		type(): EntityType;
		vertexs(): Float64Array;
	}
	class GiEntityIterator extends Rx.RxObject {
		constructor();
		static cast(inPtr: Rx.RxObject): GiEntityIterator;
		static cast(inPtr: number): GiEntityIterator;
		start(): void;
		done(): boolean;
		step(): void;
		getEntity(): GiEntity;
	}
	class GiEntityReactor extends Rx.RxObject {
		constructor();
		static cast(inPtr: Rx.RxObject): GiEntityReactor;
		static cast(inPtr: number): GiEntityReactor;
		onCreateEntity(_inPtr: number): void;
		onUpdateVertexs(_inPtr: number): void;
		onTransformation(_inPtr: number): void;
		onVisibility(_inPtr: number): void;
	}
	class GiEntityManager extends Rx.RxManager {
		constructor();
		static cast(inPtr: Rx.RxObject): GiEntityManager;
		static cast(inPtr: number): GiEntityManager;
		information(): string;
		addEntity(_pEntity: GiEntity): GiObjectId
		addReactor(_pReactor: GiEntityReactor): void;
		newIterator(): GiEntityIterator;
	}
}


import Gi = Module;
export default Gi;

