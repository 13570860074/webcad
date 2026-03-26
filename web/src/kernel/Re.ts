import Rx from './Rx'
import Ge from './Ge'
import Db from './Db'

declare namespace Module {


    abstract class ReCamera extends Rx.RxObject {
        constructor();

        information(): string;

        // 获得矩阵
        matrix(): Ge.GeMatrix3d;

        // 设置矩阵
        setMatrix(_mat: Ge.GeMatrix3d): void;
    }
    class ReOrthogonalCamera extends ReCamera {
        constructor();
        static cast(inPtr: Rx.RxObject): ReOrthogonalCamera;
        static cast(inPtr: number): ReOrthogonalCamera;

        information(): string;

        //获得范围
        top(): number;
        bottom(): number;
        left(): number;
        right(): number;

        //设置范围
        setTop(_v: number): void;
        setBottom(_v: number): void;
        setLeft(_v: number): void;
        setRight(_v: number): void;

        //获得左下角
        lowerLeftCorner(): Ge.GePoint2d;

        //设置左下角
        setLowerLeftCorner(_v: Ge.GePoint2d): void;

        //获得右上角
        upperRightCorner(): Ge.GePoint2d;

        //设置右上角
        setUpperRightCorner(_v: Ge.GePoint2d): void;
    }

    class ReRenderer extends Rx.RxObject {
        constructor();
        static cast(inPtr: Rx.RxObject): ReRenderer;
        static cast(inPtr: number): ReRenderer;

        isWorkArea(): boolean;
        viewport(): Db.DbObjectId;
        setViewport(id: Db.DbObjectId): void;
        camera(): ReCamera;
        setCamera(_camera: ReCamera): void;
        numEntity(): number;
        entityIds(): Uint32Array;
    }

    class ReRendererReactor extends Rx.RxObject {
        constructor();
        static cast(inPtr: Rx.RxObject): ReRendererReactor;
        static cast(inPtr: number): ReRendererReactor;
        onBeginAnimationFrame(_inPtr: number): void;
        onEndAnimationFrame(_inPtr: number): void;
        onBeginRenderer(_inPtr: number): void;
        onEndRenderer(_inPtr: number): void;
        onClearCanvas(_inPtr: number): void;
        onClearWorkCanvas(_inPtr: number): void;
        onBeginRendererViewport(_inPtr: number): void;
        onEndRendererViewport(_inPtr: number): void;
        onRendererViewport(_inPtr: number): void;
    }
    class ReRendererManager extends Rx.RxObject {
        constructor();
        static cast(inPtr: Rx.RxObject): ReRendererManager;
        static cast(inPtr: number): ReRendererManager;
        animationFrame(): void;
        addReactor(_reactor: ReRendererReactor): void;
    }
}


import Re = Module;
export default Re;

