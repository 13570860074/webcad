import * as THREE from 'three'
import Gi from '@/kernel/Gi';
import { FMaterial } from './FMaterial';
import { LineMaterial } from 'three/addons/lines/LineMaterial.js';

export class FLineMaterial extends FMaterial {
    constructor() {
        super();
    }

    private m_lineWidth: number = 1;

    // 获得材料样式
    type(): Gi.MaterialType {
        return Gi.MaterialType.kLineBasic;
    }

    // 创建
    create(): void {
        if (this.isOpenLineWidth() == false) {
            let rgb: Array<number> = this.color();
            let color = "#" + ((1 << 24) + (rgb[0] << 16) + (rgb[1] << 8) + rgb[2]).toString(16).slice(1);
            let lineBasicMaterial: THREE.LineBasicMaterial = new THREE.LineBasicMaterial({ color: color });
            if (this.transparency() > 0) {
                lineBasicMaterial.transparent = true;
                lineBasicMaterial.opacity = this.transparency() / 100.0;
            }
            this.m_material = lineBasicMaterial;
        } else {
            let rgb: Array<number> = this.color();
            let color = "#" + ((1 << 24) + (rgb[0] << 16) + (rgb[1] << 8) + rgb[2]).toString(16).slice(1);
            const lineMaterial = new LineMaterial({
                color: color,
                linewidth: this.m_lineWidth,
                vertexColors: false,
                dashed: false,
                alphaToCoverage: true,
                transparent: false,
            });
            if (this.transparency() > 0) {
                lineMaterial.transparent = true;
                lineMaterial.opacity = this.transparency() / 100.0;
            }
            lineMaterial.resolution.set(window.innerWidth, window.innerHeight);
            this.m_material = lineMaterial;
        }
    }

    // 是否开启宽度
    isOpenLineWidth(): boolean {
        if (this.m_lineWidth <= 1.00002) {
            return false;
        }
        return true;
    }

    // 获得线宽
    lineWidth(): number {
        return this.m_lineWidth;
    }

    // 设置线宽
    setLineWidth(_lineWidth: number): void {
        this.m_lineWidth = _lineWidth;
    }
}