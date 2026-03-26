import * as THREE from 'three'
import Gi from '../kernel/Gi'
import Ge from '../kernel/Ge'
import { FEntityRecord } from './FEntityRecord';
import { application } from '../Application/FApplication';
import { FMaterial } from '../MaterialManager/FMaterial';



export class FMesh extends FEntityRecord {
    constructor() {
        super();
    }

    private m_bufferGeometry: THREE.BufferGeometry = null;
    private m_float32BufferAttribute: THREE.Float32BufferAttribute = null;


    // 获得实体类型
    type(): Gi.EntityType {
        return Gi.EntityType.kMesh;
    }

    // 创建实体
    create(): void {

        if (this.entity != null) {
            return;
        }

        let variants: Array<number> = new Array<number>();

        //创建几何对象
        this.m_bufferGeometry = new THREE.BufferGeometry();
        this.m_float32BufferAttribute = new THREE.Float32BufferAttribute(variants, 3);
        this.m_bufferGeometry.setAttribute('position', this.m_float32BufferAttribute);

        //获得材质
        let material: FMaterial = application.materialManager().material(this.materialId());

        //创建对象
        this.entity = new THREE.Mesh(this.m_bufferGeometry, material.material());
    }

    // 更新顶点
    updateVariants(_vertexs: Float64Array, _len: number): void {

        let variants: Array<number> = new Array<number>();
        for (let i = 0; i < _len; i++) {
            variants.push(_vertexs[i]);
        }

        this.m_float32BufferAttribute = new THREE.Float32BufferAttribute(variants, 3);
        this.m_bufferGeometry.setAttribute('position', this.m_float32BufferAttribute);
        this.m_bufferGeometry.computeBoundingSphere();
        (<THREE.Mesh>this.entity).geometry.attributes.position.needsUpdate = true;
    }

    // 获得位置
    position(): Ge.GePoint3d {
        return new Ge.GePoint3d((<THREE.Mesh>this.entity).position.x, (<THREE.Mesh>this.entity).position.y, (<THREE.Mesh>this.entity).position.z);
    }

    // 设置位置
    setPosition(_position: Ge.GePoint3d): void {
        (<THREE.Mesh>this.entity).position.x = _position.x;
        (<THREE.Mesh>this.entity).position.y = _position.y;
        (<THREE.Mesh>this.entity).position.z = _position.z;
    }

    // 获得缩放
    scale(): Ge.GeScale3d {
        return new Ge.GeScale3d((<THREE.Mesh>this.entity).scale.x, (<THREE.Mesh>this.entity).scale.y, (<THREE.Mesh>this.entity).scale.z);
    }

    // 设置缩放
    setScale(_scale: Ge.GeScale3d): void {
        (<THREE.Mesh>this.entity).scale.x = _scale.sx;
        (<THREE.Mesh>this.entity).scale.y = _scale.sy;
        (<THREE.Mesh>this.entity).scale.z = 1;
    }

    // 设置轴角
    setRotateOnAxis(_axis: Ge.GeVector3d, _angle: number): void {
        (<THREE.Mesh>this.entity).rotateOnAxis(new THREE.Vector3(_axis.x, _axis.y, _axis.z), _angle);
    }

}
