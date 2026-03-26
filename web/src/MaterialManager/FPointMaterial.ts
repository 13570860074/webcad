import * as THREE from 'three'
import Gi from '@/kernel/Gi';
import { FMaterial } from './FMaterial';


export class FPointMaterial extends FMaterial {
    constructor() {
        super();
    }

    private m_size: number = 1;

    // 创建材质
    create(): void {
        let rgb: Array<number> = this.color();
        let color = "#" + ((1 << 24) + (rgb[0] << 16) + (rgb[1] << 8) + rgb[2]).toString(16).slice(1);
        let pointMaterial: THREE.PointsMaterial = new THREE.PointsMaterial({ color: color, size: 1 });
        if (this.transparency() > 0) {
            pointMaterial.transparent = true;
            pointMaterial.opacity = this.transparency() / 100.0;
        }
        this.m_material = pointMaterial;
    }

    // 获得材料样式
    type(): Gi.MaterialType {
        return Gi.MaterialType.kPointBasic;
    }

    // 获得尺寸
    size(): number {
        return this.m_size;
    }

    // 设置尺寸
    setSize(_size: number): void {
        this.m_size = _size;
    }
}