import Gi from '@/kernel/Gi';
import { application } from '@/Application/FApplication';
import { FManager } from "@/Application/FManager";
import { FInterface } from '@/Core/FInterface';
import { EventState } from "@/EventManager/FEventManager";
import { FPointMaterial } from "./FPointMaterial";
import { FLineMaterial } from "./FLineMaterial";
import { FLinePixelDashedMaterial } from "./FLinePixelDashedMaterial";
import { FMaterial } from './FMaterial';
import { FMeshMaterial } from "./FMeshMaterial";


export class FMaterialManager extends FManager {
    constructor() {
        super();
    }


    private m_materials: Array<FMaterial> = new Array<FMaterial>();

    /** 初始化 */
    async onInit(): Promise<void> {
        application.eventManager().on(EventState.kCreateMaterial, this.onCreateMaterial);
    }

    /** 监听创建材质事件 */
    private onCreateMaterial = (material: Gi.GiMaterial): void => {
        if (material.type() == Gi.MaterialType.kLineBasic) {
            let record: FLineMaterial = new FLineMaterial();
            record.setId(material.objectId().asOldId());
            let rgb: Array<number> = FInterface.converArgbToRgb(material.color());
            record.setColor(rgb[0], rgb[1], rgb[2]);
            record.setTransparency(material.transparency());
            record.setLineWidth((<Gi.GiLineMaterial>material).lineWidth());
            record.create();
            application.materialManager().appendMaterial(record);
        }
        else if (material.type() == Gi.MaterialType.kLinePixelDashed) {

            let record: FLinePixelDashedMaterial = new FLinePixelDashedMaterial();
            record.setId(material.objectId().asOldId());
            let rgb: Array<number> = FInterface.converArgbToRgb(material.color());
            record.setColor(rgb[0], rgb[1], rgb[2]);
            record.setTransparency(material.transparency());
            record.setDashSize((<Gi.GiLinePixelDashedMaterial>material).dashSize());
            record.setGapSize((<Gi.GiLinePixelDashedMaterial>material).gapSize());
            record.create();
            application.materialManager().appendMaterial(record);
        }
        else if (material.type() == Gi.MaterialType.kMeshBasic) {
            let record: FMeshMaterial = new FMeshMaterial();
            record.setId(material.objectId().asOldId());
            let rgb: Array<number> = FInterface.converArgbToRgb(material.color());
            record.setColor(rgb[0], rgb[1], rgb[2]);
            record.setTransparency(material.transparency());
            record.create();
            application.materialManager().appendMaterial(record);
        }
        else if (material.type() == Gi.MaterialType.kPointBasic) {
            let record: FPointMaterial = new FPointMaterial();
            record.setId(material.objectId().asOldId());
            let rgb: Array<number> = FInterface.converArgbToRgb(material.color());
            record.setColor(rgb[0], rgb[1], rgb[2]);
            record.setTransparency(material.transparency());
            record.setSize((<Gi.GiPointMaterial>material).size());
            record.create();
            application.materialManager().appendMaterial(record);
        }
    }

    /** 添加材质 */
    appendMaterial(_material: FMaterial): void {
        this.m_materials.push(_material);
    }

    /** 通过id获得材质 */
    material(_id: number): FMaterial {
        for (let i = 0; i < this.m_materials.length; i++) {
            if (this.m_materials[i].id() == _id) {
                return this.m_materials[i];
            }
        }
        return null;
    }

    /** 获得像素虚线材质 */
    linePixelDashedMaterial(): Array<FLinePixelDashedMaterial> {
        let materials: Array<FLinePixelDashedMaterial> = new Array<FLinePixelDashedMaterial>();
        for (let i = 0; i < this.m_materials.length; i++) {
            if (this.m_materials[i].type() == Gi.MaterialType.kLinePixelDashed) {
                materials.push(<FLinePixelDashedMaterial>this.m_materials[i]);
            }
        }
        return materials;
    }
}