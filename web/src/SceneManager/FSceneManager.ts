import * as THREE from 'three'
import Gi from '@/kernel/Gi';
import Re from '@/kernel/Re';
import { FManager } from '../Application/FManager';
import { application } from '@/Application/FApplication';
import { EventState } from '@/EventManager/FEventManager';
import { FEntityRecord } from '@/EntityManager/FEntityRecord';
import { FLinePixelDashedMaterial } from '@/MaterialManager/FLinePixelDashedMaterial';
import { IUiLayoutData, gcadGlobal } from '@/gcadGlobal';



interface IOrthogonalCamera {
    matrix: Array<number>,
    top: number,
    bottom: number,
    left: number,
    right: number,
    lowerLeftCorner: { x: number, y: number },
    upperRightCorner: { x: number, y: number }
}
interface IRenderer extends IOrthogonalCamera {
    renderer: Re.ReRenderer,
}





export class FSceneManager extends FManager {
    constructor() {
        super();
    }

    public domElement: HTMLDivElement = null;
    private devicePixelRatio: number = 1;

    private width: number = 0;
    private height: number = 0;

    private m_scene: THREE.Scene = null;
    private m_renderer: THREE.WebGLRenderer = null;
    private m_camera: THREE.OrthographicCamera = null;

    private m_viewportRenderers: Array<IRenderer> = new Array<IRenderer>();

    private m_nIndex: number = 0;
    private m_bMouseMove: boolean = false;
    private m_reusableMatrix: THREE.Matrix4 = new THREE.Matrix4();

    entityRecords: Array<FEntityRecord> = new Array<FEntityRecord>();
    lowerLeftCorner: { x: number, y: number } = { x: 0, y: 0 };
    upperRightCorner: { x: number, y: number } = { x: 1, y: 1 };


    // 创建实体记录
    private onCreateEntityRecord = (entityRecord: FEntityRecord) => {
        let scene = this.m_scene;
        if (entityRecord.type() == Gi.EntityType.kLineSegment) {
            scene.add(entityRecord.lineSegments());
        }
        else if (entityRecord.type() == Gi.EntityType.kMesh) {
            scene.add(entityRecord.mesh());
        }
        else if (entityRecord.type() == Gi.EntityType.kPoints) {
            scene.add(entityRecord.points());
        }

        this.entityRecords.push(entityRecord);
    }

    async onInit(): Promise<void> {

        application.eventManager().on(EventState.kCreateEntityRecord, this.onCreateEntityRecord);
        application.eventManager().on(EventState.kMousemove, () => {
            this.m_bMouseMove = true;
        });
        application.eventManager().on(EventState.kMousewheel, () => {
            this.m_bMouseMove = true;
        });
        application.eventManager().on(EventState.kRendererViewport, (_renderer: Re.ReRenderer) => {
            let renderer: IRenderer = JSON.parse(_renderer.camera().information());
            renderer.renderer = _renderer;
            this.m_viewportRenderers.push(renderer);
        });
        application.eventManager().on(EventState.kEndRenderer, () => {
            this.onRenderer();
        });

        await this.load();
    }

    async load(): Promise<void> {

        this.devicePixelRatio = window.devicePixelRatio || 1;

        /** 获得元素在屏幕中的位置和尺寸 */
        this.width = gcadGlobal.uiLayoutData.canvasRight - gcadGlobal.uiLayoutData.canvasLeft;
        this.height = gcadGlobal.uiLayoutData.canvasBottom - gcadGlobal.uiLayoutData.canvasTop;
        let sceneElement = document.getElementById("scene");

        /** 创建DIV */
        this.domElement = document.createElement("div");
        this.domElement.style.top = `${0}px`;
        this.domElement.style.left = `${0}px`;
        this.domElement.style.width = `${this.width}px`;
        this.domElement.style.height = `${this.height}px`;
        this.domElement.style.position = 'absolute';
        this.domElement.style.backgroundColor = 'rgb(0,0,0)';
        this.domElement.oncontextmenu = function (event: MouseEvent) {
            event.preventDefault();
            return false;
        }
        sceneElement.appendChild(this.domElement);

        /** 创建场景 */
        this.m_scene = new THREE.Scene();

        /** 创建渲染器 */
        this.m_renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });
        this.m_renderer.setSize(this.width, this.height);
        this.m_renderer.setClearAlpha(0);
        this.m_renderer.localClippingEnabled = false;
        this.m_renderer.setPixelRatio(this.devicePixelRatio);
        this.m_renderer.autoClear = false;
        this.m_renderer.domElement.style.position = 'absolute'
        this.domElement.appendChild(this.m_renderer.domElement);

        /** 创建相机 */
        this.m_camera = new THREE.OrthographicCamera(-10, 10, 10, -10, 10000000, -10000000);
        this.m_camera.position.set(0, 0, 0);
        this.m_camera.layers.set(0);


        let animate = () => {

            requestAnimationFrame(animate);

            this.m_nIndex++;
            if(this.m_nIndex > 1 || this.m_bMouseMove){
                this.m_nIndex = 0;
                this.m_bMouseMove = false;
                application.kernelManager().rendererManager().animationFrame();
            }
        }
        animate();


        /** 监听设置Ui布局数据 */
        application.eventManager().on(EventState.kUpdateLayoutData, (_data: IUiLayoutData) => {
            this.width = _data.canvasRight - _data.canvasLeft;
            this.height = _data.canvasBottom - _data.canvasTop;
            this.domElement.style.width = `${this.width}px`;
            this.domElement.style.height = `${this.height}px`;
            this.m_renderer.setSize(this.width, this.height);
        });
    }

    /**更新相机 */
    private updateCamera = (_camera: IRenderer): void => {


        // 获得视口左下和右上角
        let lowerLeftCorner: { x: number, y: number } = _camera.lowerLeftCorner;
        let upperRightCorner: { x: number, y: number } = _camera.upperRightCorner;

        // 设置像素
        let pixelRatios: Array<FLinePixelDashedMaterial> = application.materialManager().linePixelDashedMaterial();
        for (let i = 0; i < pixelRatios.length; i++) {
            pixelRatios[i].setPixelRatio(this.width * (upperRightCorner.x - lowerLeftCorner.x), this.height * (upperRightCorner.x - lowerLeftCorner.x));
        }

        // 更新相机属性（复用已有相机对象，避免每帧 GC）
        this.m_camera.left = _camera.left;
        this.m_camera.right = _camera.right;
        this.m_camera.top = _camera.top;
        this.m_camera.bottom = _camera.bottom;
        this.m_camera.position.set(0, 0, 0);
        this.m_camera.rotation.set(0, 0, 0);

        // 设置矩阵（复用 m_reusableMatrix，避免每帧分配）
        this.m_reusableMatrix.set(
            _camera.matrix[0], _camera.matrix[1], _camera.matrix[2], _camera.matrix[3],
            _camera.matrix[4], _camera.matrix[5], _camera.matrix[6], _camera.matrix[7],
            _camera.matrix[8], _camera.matrix[9], _camera.matrix[10], _camera.matrix[11],
            _camera.matrix[12], _camera.matrix[13], _camera.matrix[14], _camera.matrix[15]);

        // 应用矩阵到相机
        this.m_camera.matrix.identity();
        this.m_camera.applyMatrix4(this.m_reusableMatrix);

        // 更新相机数据
        this.m_camera.updateProjectionMatrix();
    }

    // 渲染
    private onRenderer = () => {
        // this.mlllllll++;
        // if( this.mlllllll < 3){
        //     return;
        // }
        // this.mlllllll = 0;
        this.m_renderer.clear();
        // 渲染
        for (let i = 0; i < this.m_viewportRenderers.length; i++) {

            // 更新相机
            this.updateCamera(this.m_viewportRenderers[i]);

            // 获得左下角和右上角
            let lowerLeftCorner: { x: number, y: number } = this.m_viewportRenderers[i].lowerLeftCorner;
            let upperRightCorner: { x: number, y: number } = this.m_viewportRenderers[i].upperRightCorner;

            // 渲染
            this.m_renderer.setViewport(
                lowerLeftCorner.x * this.width,
                lowerLeftCorner.y * this.height,
                (upperRightCorner.x - lowerLeftCorner.x) * this.width,
                (upperRightCorner.y - lowerLeftCorner.y) * this.height);
            this.m_renderer.render(this.m_scene, this.m_camera);
        }

        this.m_viewportRenderers.splice(0, this.m_viewportRenderers.length);
    }
}