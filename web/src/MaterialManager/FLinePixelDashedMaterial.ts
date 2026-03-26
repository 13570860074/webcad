import * as THREE from 'three'
import { FMaterial } from './FMaterial';
import Gi from '@/kernel/Gi';




export class FLinePixelDashedMaterial extends FMaterial {
    constructor() {
        super();

        this.vshader = `
                    flat out vec4 p0;
                    out vec4 p;
                    void main() {
                        vec4 p1 = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
                        gl_Position = p1;
                        p  = p1;
                        p0 = p1;
                    }
                `
        this.fshader = `
                    precision highp float;
                    flat in vec4 p0;
                    in vec4 p;
                    uniform vec2  canvas;
                    uniform float dash;
                    uniform float gap;
                    uniform vec3 color;
                    uniform float transparency;
                    void main(){
                        vec2 dir = ((p.xyz / p.w).xy - (p0.xyz / p0.w).xy) * canvas.xy / 2.0;
                        float dist = length(dir);
                        if (fract(dist / (dash + gap)) > dash / (dash + gap)){
        	                gl_FragColor = vec4(0, 0, 0, 0);
                        }else{
                            gl_FragColor = vec4(color.r,color.g,color.b, transparency);
                        }
                    }
                    `;
    }

    private m_lineWidth: number = 1;
    private m_dashSize: number = 3;
    private m_gapSize: number = 3;

    private vshader: string = '';
    private fshader: string = '';
    private uniforms: any = null;

    type(): Gi.MaterialType {
        return Gi.MaterialType.kLinePixelDashed;
    }

    create(): void {
        this.uniforms = {
            canvas: { type: 'v2', value: { x: 1, y: 1 } },
            dash: { type: 'f', value: this.dashSize() },
            gap: { type: 'f', value: this.gapSize() },
            color: { type: 'v3', value: { r: this.color()[0] / 255, g: this.color()[1] / 255, b: this.color()[2] / 255 } },
            transparency: { type: 'f', value: this.transparency() }
        }
        let shaderMaterial = new THREE.ShaderMaterial({
            uniforms: this.uniforms,
            vertexShader: this.vshader,
            fragmentShader: this.fshader,
        })
        shaderMaterial.transparent = true;
        this.m_material = shaderMaterial;
    }

    // 是否开启宽度
    isOpenLineWidth(): boolean {
        return false;
    }
    lineWidth(): number {
        return this.m_lineWidth;
    }
    setLineWidth(_lineWidth: number): void {
        this.m_lineWidth = _lineWidth;
    }

    setPixelRatio(width: number, height: number): void {
        this.uniforms.canvas.value.x = width
        this.uniforms.canvas.value.y = height
    }

    dashSize(): number {
        return this.m_dashSize;
    }
    setDashSize(val: number) {
        this.m_dashSize = val;
    }

    gapSize(): number {
        return this.m_gapSize;
    }
    setGapSize(val: number): void {
        this.m_gapSize = val;
    }
}