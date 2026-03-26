import * as THREE from 'three'
import Gi from '@/kernel/Gi'

export abstract class FMaterial {
    constructor() {

    }

    protected m_id: number = 0;
    protected m_material: THREE.Material | null = null;
    protected m_color: Array<number> = new Array<number>(3);
    protected m_transparency: number = 1;


    // 获得id
    id(): number {
        return this.m_id;
    }

    // 获得类型
    type(): Gi.MaterialType {
        return Gi.MaterialType.kUnknownMaterial;
    }

    // 获得颜色
    color(): Array<number> {
        return this.m_color;
    }

    // 获得透明度
    transparency(): number {
        return this.m_transparency;
    }

    // 获得THREEJS材质
    material(): THREE.Material {
        return this.m_material;
    }


    // 设置id
    setId(_id: number): void {
        this.m_id = _id;
    }

    // 设置颜色
    setColor(r: number, g: number, b: number): void {
        this.m_color[0] = r;
        this.m_color[1] = g;
        this.m_color[2] = b;
    }

    // 设置透明度
    setTransparency(_val: number): void {
        this.m_transparency = _val;
    }

    // 获得THREEJS材质
    setMaterial(_material: THREE.Material): void {
        this.m_material = _material;
    }
}