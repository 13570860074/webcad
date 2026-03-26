import * as THREE from 'three'
import Gi from '@/kernel/Gi';
import { FMaterial } from './FMaterial';


export class FMeshMaterial extends FMaterial {
    constructor() {
        super();
    }

    // 获得材质样式
    type(): Gi.MaterialType {
        return Gi.MaterialType.kMeshBasic;
    }

    // 创建材质
    create(): void {
        let rgb: Array<number> = this.color();
        let color = "#" + ((1 << 24) + (rgb[0] << 16) + (rgb[1] << 8) + rgb[2]).toString(16).slice(1);
        let meshBasicMaterial: THREE.MeshBasicMaterial = new THREE.MeshBasicMaterial({ color: color, side: THREE.DoubleSide });
        if (this.transparency() > 0) {
            meshBasicMaterial.transparent = true;
            meshBasicMaterial.opacity = this.transparency() / 100.0;
        }
        this.m_material = meshBasicMaterial;
    }
}