import * as THREE from 'three'
import Gi from '../kernel/Gi'
import Ge from '../kernel/Ge'


export abstract class FEntityRecord {
    constructor() {

    }

    protected m_visibility: boolean = true;
    protected elementType: Gi.ElementType;
    protected entityId: number = 0;
    protected entity: THREE.Object3D = null;
    protected material: number = 0;
    protected m_matrix: Ge.GeMatrix3d = new Ge.GeMatrix3d();


    // 获得实体id
    id(): number {
        return this.entityId;
    }

    // 获得实体类型
    type(): Gi.EntityType {
        return Gi.EntityType.kUnknownEntity;
    }

    // 获得图元类型
    element(): Gi.ElementType {
        return this.elementType;
    }

    //获得材质id
    materialId(): number {
        return this.material;
    }

    // 获得位置
    abstract position(): Ge.GePoint3d;

    // 获得缩放
    abstract scale(): Ge.GeScale3d;

    // 获得可见性
    visibility(): boolean {
        if (this.entity.layers.mask == 0) {
            return true;
        }
        return false;
    }

    // 获得THREE.Points
    points(): THREE.Points {
        if (this.type() == Gi.EntityType.kPoints) {
            return <THREE.Points>this.entity;
        }
        return null;
    }

    // 获得THREE.LineSegments
    lineSegments(): THREE.LineSegments {
        if (this.type() == Gi.EntityType.kLineSegment) {
            return <THREE.LineSegments>this.entity;
        }
        return null;
    }

    // 获得THREE.Mesh
    mesh(): THREE.Mesh {
        if (this.type() == Gi.EntityType.kMesh) {
            return <THREE.Mesh>this.entity;
        }
        return null;
    }


    //设置实体id
    setId(_id: number): void {
        this.entityId = _id;
    }

    //设置材质id
    setMaterialId(_id: number): void {
        this.material = _id;
    }

    //设置图元类型
    setElement(_type: Gi.ElementType): void {
        this.elementType = _type;
    }

    //设置位置
    abstract setPosition(_position: Ge.GePoint3d): void;

    //设置缩放
    abstract setScale(_scale: Ge.GeScale3d): void;

    //设置可见性
    setVisibility(_isVisibility: boolean): void {
        if (_isVisibility == true && this.m_visibility == false) {
            this.entity.layers.set(0);
        }
        else if (_isVisibility == false && this.m_visibility == true) {
            this.entity.layers.set(1);
        }
        this.m_visibility = _isVisibility;
    }

    // 获得矩阵
    matrix(): Ge.GeMatrix3d {
        return this.m_matrix;
    }

    // 设置矩阵
    setMatrix(_matrix: Ge.GeMatrix3d): void {
        this.m_matrix = _matrix;
        if (this.entity != null) {
            let mat = new THREE.Matrix4();
            mat.set(
                _matrix.get(0, 0), _matrix.get(0, 1), _matrix.get(0, 2), _matrix.get(0, 3),
                _matrix.get(1, 0), _matrix.get(1, 1), _matrix.get(1, 2), _matrix.get(1, 3),
                _matrix.get(2, 0), _matrix.get(2, 1), _matrix.get(2, 2), _matrix.get(2, 3),
                _matrix.get(3, 0), _matrix.get(3, 1), _matrix.get(3, 2), _matrix.get(3, 3));

            this.entity.position.x = 0;
            this.entity.position.y = 0;
            this.entity.position.z = 0;
            this.entity.rotation.x = 0;
            this.entity.rotation.y = 0;
            this.entity.rotation.z = 0;
            this.entity.scale.x = 1;
            this.entity.scale.y = 1;
            this.entity.scale.z = 1;
            this.entity.quaternion.x = 0;
            this.entity.quaternion.y = 0;
            this.entity.quaternion.z = 0;
            this.entity.quaternion.w = 1;

            let quaternion: Ge.GeQuaternion = new Ge.GeQuaternion();
            quaternion.setMatrix(_matrix);
            this.entity.quaternion.x = 0 - quaternion.x;
            this.entity.quaternion.y = 0 - quaternion.y;
            this.entity.quaternion.z = 0 - quaternion.z;
            this.entity.quaternion.w = 0 - quaternion.w;

            this.entity.scale.x = _matrix.get(0, 0);
            this.entity.scale.y = _matrix.get(1, 1);
            this.entity.scale.z = _matrix.get(2, 2);

            this.entity.position.x = _matrix.get(0, 3);
            this.entity.position.y = _matrix.get(1, 3);
            this.entity.position.z = _matrix.get(2, 3);

        }
    }
}