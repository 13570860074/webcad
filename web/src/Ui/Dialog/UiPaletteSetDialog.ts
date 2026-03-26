import { UiDialog } from "./UiDialog";


export class UiPaletteSetDialog extends UiDialog {
    constructor() {
        super();
        super.setModal(false);
        super.setFrame(false);
        super.setBackgroundColor(46, 52, 64);
    }
}