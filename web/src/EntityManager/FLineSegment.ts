import * as THREE from 'three'
import Gi from '../kernel/Gi'
import Ge from '../kernel/Ge'
import { application } from '../Application/FApplication';
import { FEntityRecord } from './FEntityRecord';
import { FMaterial } from '../MaterialManager/FMaterial'
import { FLineMaterial } from '@/MaterialManager/FLineMaterial';
import { LineMaterial } from 'three/addons/lines/LineMaterial.js';
import { LineSegments2 } from 'three/addons/lines/LineSegments2.js';
import { LineSegmentsGeometry } from 'three/addons/lines/LineSegmentsGeometry.js';

export class FLineSegment extends FEntityRecord {
    constructor() {
        super();
    }

    private m_numLineSegment: number = 0;
    private m_isOpenLineWidth: boolean = false;
    private m_bufferGeometry: THREE.BufferGeometry | LineSegmentsGeometry = null;
    private m_float32BufferAttribute: THREE.Float32BufferAttribute = null;

    // 获得实体类型
    type(): Gi.EntityType {
        return Gi.EntityType.kLineSegment;
    }

    // 创建实体
    create(): void {

        if (this.entity != null) {
            return;
        }

        // 获得材质
        let material: FMaterial = application.materialManager().material(this.materialId());

        // 判断是否开启宽度
        this.m_isOpenLineWidth = (<FLineMaterial>material).isOpenLineWidth();

        if (this.m_isOpenLineWidth == false) {

            let variants: Array<number> = new Array<number>();

            // 创建几何对象
            this.m_bufferGeometry = new THREE.BufferGeometry();
            this.m_float32BufferAttribute = new THREE.Float32BufferAttribute(variants, 3);
            this.m_bufferGeometry.setAttribute('position', this.m_float32BufferAttribute);

            // 创建实体
            this.entity = new THREE.LineSegments(this.m_bufferGeometry, material.material());

        } else {

            let variants: Array<number> = new Array<number>();
            for (let i = 0; i < 10000; i++) {
                variants.push(0, 0, 0, 0, 0, 0);
            }

            //创建几何对象
            this.m_bufferGeometry = new LineSegmentsGeometry();
            (<LineSegmentsGeometry>this.m_bufferGeometry).setPositions(variants);

            // 创建实体
            this.entity = new LineSegments2(<LineSegmentsGeometry>this.m_bufferGeometry, <LineMaterial>material.material());
        }
    }

    // 更新顶点
    updateVariants(_vertexs: Float64Array, _len: number): void {

        // 获得顶点数据
        let variants: Array<number> = new Array<number>();
        for (let i = 0; i < _len; i++) {
            variants.push(_vertexs[i]);
        }

        // 更新实体
        if (this.m_isOpenLineWidth == false) {
            this.m_float32BufferAttribute = new THREE.Float32BufferAttribute(variants, 3);
            this.m_bufferGeometry.setAttribute('position', this.m_float32BufferAttribute);
            this.m_bufferGeometry.computeBoundingSphere();
            (<THREE.LineSegments>this.entity).geometry.attributes.position.needsUpdate = true;

            this.m_numLineSegment = _len / 6;
        }
        else {
            let count = variants.length / 6;
            let instanceStart = this.m_bufferGeometry.getAttribute('instanceStart');
            let instanceEnd = this.m_bufferGeometry.getAttribute('instanceEnd');
            for (let i = 0; i < count; i++) {
                if (i < count) {
                    instanceStart.setXYZ(i, variants[i * 6], variants[i * 6 + 1], variants[i * 6 + 2]);
                    instanceEnd.setXYZ(i, variants[i * 6 + 3], variants[i * 6 + 4], variants[i * 6 + 5]);
                }
            }
            for (let i = count; i < this.m_numLineSegment; i++) {
                instanceStart.setXYZ(i, 0, 0, 0);
                instanceEnd.setXYZ(i, 0, 0, 0);
            }
            this.m_numLineSegment = count;
            instanceStart.needsUpdate = true;
            instanceEnd.needsUpdate = true;

            this.m_bufferGeometry.computeBoundingSphere();
            (<LineSegments2>this.entity).geometry.attributes.position.needsUpdate = true;
        }
    }

    // 获得位置
    position(): Ge.GePoint3d {
        if (this.m_isOpenLineWidth == false) {
            return new Ge.GePoint3d((<THREE.LineSegments>this.entity).position.x, (<THREE.LineSegments>this.entity).position.y, (<THREE.LineSegments>this.entity).position.z);
        }
        return new Ge.GePoint3d((<LineSegments2>this.entity).position.x, (<LineSegments2>this.entity).position.y, (<LineSegments2>this.entity).position.z);
    }

    // 设置位置
    setPosition(_position: Ge.GePoint3d): void {
        if (this.m_isOpenLineWidth == false) {
            (<THREE.LineSegments>this.entity).position.x = _position.x;
            (<THREE.LineSegments>this.entity).position.y = _position.y;
            (<THREE.LineSegments>this.entity).position.z = _position.z;
        } else {

            (<LineSegments2>this.entity).position.x = _position.x;
            (<LineSegments2>this.entity).position.y = _position.y;
            (<LineSegments2>this.entity).position.z = _position.z;
        }
    }

    // 设置轴角
    setRotateOnAxis(_axis: Ge.GeVector3d, _angle: number): void {
        if (this.m_isOpenLineWidth == false) {
            (<THREE.LineSegments>this.entity).rotation.x = 0;
            (<THREE.LineSegments>this.entity).rotation.y = 0;
            (<THREE.LineSegments>this.entity).rotation.z = 0;
            (<THREE.LineSegments>this.entity).rotateOnAxis(new THREE.Vector3(_axis.x, _axis.y, _axis.z), _angle);
        } else {
            (<LineSegments2>this.entity).rotation.x = 0;
            (<LineSegments2>this.entity).rotation.y = 0;
            (<LineSegments2>this.entity).rotation.z = 0;
            (<LineSegments2>this.entity).rotateOnAxis(new THREE.Vector3(_axis.x, _axis.y, _axis.z), _angle);
        }
    }

    // 获得缩放
    scale(): Ge.GeScale3d {
        if (this.m_isOpenLineWidth == false) {
            return new Ge.GeScale3d((<THREE.LineSegments>this.entity).scale.x, (<THREE.LineSegments>this.entity).scale.y, (<THREE.LineSegments>this.entity).scale.z);
        }
        return new Ge.GeScale3d((<LineSegments2>this.entity).scale.x, (<LineSegments2>this.entity).scale.y, (<LineSegments2>this.entity).scale.z);
    }

    // 设置缩放
    setScale(_scale: Ge.GeScale3d): void {
        if (this.m_isOpenLineWidth == false) {
            (<THREE.LineSegments>this.entity).scale.x = _scale.sx;
            (<THREE.LineSegments>this.entity).scale.y = _scale.sy;
            (<THREE.LineSegments>this.entity).scale.z = 1;
        }
        else {
            (<LineSegments2>this.entity).scale.x = _scale.sx;
            (<LineSegments2>this.entity).scale.y = _scale.sy;
            (<LineSegments2>this.entity).scale.z = 1;
        }
    }
}
