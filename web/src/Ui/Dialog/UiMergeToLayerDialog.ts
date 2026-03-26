import MergeToLayerDialog from "@/components/MergeToLayerDialog.vue";
import { UiDialog } from "./UiDialog";


export class UiMergeToLayerDialog extends UiDialog {
    constructor() {
        super();
        this.setTitle("合并到图层");
        this.setSize(250, 310);
    }

    private m_layerNames: string[] = [];
    private m_selectedLayer: string = "";

    /** 获取图层列表 */
    layerNames(): Array<string> {
        return this.m_layerNames;
    }

    /** 设置图层 */
    setLayerNames(layers: Array<string>): void {
        for (let i = 0; i < layers.length; i++) {
            this.m_layerNames.push(layers[i]);
        }
    }

    /** 获取选中图层 */
    selectedLayer(): string {
        return this.m_selectedLayer;
    }

    /** 设置选中图层 */
    setSelectedLayer(layer: string): void {
        this.m_selectedLayer = layer;
    }

    async exec(): Promise<void> {
        await super.exec(MergeToLayerDialog);
    }
}