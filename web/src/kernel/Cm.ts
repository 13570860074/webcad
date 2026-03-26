import Em from './Em'


declare namespace Module {


    //Export
    enum Color {
        kRed,
        kGreen,
        kBlue,
    }
    enum ColorMethod {
        /**Color of the layer on which it resides.*/
        kByLayer = 0xC0,
        /**Color of the block reference in which it is contained.*/
        kByBlock = 0xC1,
        /**Color specified by RGB value.*/
        kByColor = 0xC2,
        /**Color specified by an index (ACI) into a color palette.*/
        kByACI = 0xC3,
        /**Color specified by an index into a pen color table.*/
        kByPen = 0xC4,
        /**Editor foreground Color.*/
        kForeground = 0xC5,
        /**Color specified by an index into a dgn color table.*/
        kByDgnIndex = 0xC7,
        /**No Color.*/
        kNone = 0xC8
    }
    enum ACIcolorMethod {
        kACIbyBlock = 0,    // byBlock
        kACIforeground = 7,    // Foreground
        kACIbyLayer = 256,  // byLayer

        kACIclear = 0,    // Clear

        kACIRed = 1,    // Red
        kACIYellow = 2,    // Yellow
        kACIGreen = 3,    // Green
        kACICyan = 4,    // Cyan
        kACIBlue = 5,    // Blue
        kACIMagenta = 6,    // Magenta
        kACIWhite = 7,    // White

        kACIstandard = 7,
        kACImaximum = 255,
        kACInone = 257,  // None
        kACIminimum = -255
    }
    class CmEntityColor extends Em.EmObject {
        constructor();
        constructor(_color: CmEntityColor);
        constructor(_r: number, _g: number, _b: number);
        setColorMethod(_colorMethod: ColorMethod): void;
        colorMethod(): ColorMethod;
        setColor(_color: number): void;
        color(): number;
        setColorIndex(_colorIndex: number): void;
        colorIndex(): number;
        setRGB(_red: number, _green: number, _blue: number): void;
        setRed(_red: number): void;
        setGreen(_green: number): void;
        setBlue(_blue: number): void;
        red(): number;
        green(): number;
        blue(): number;
        isByColor(): boolean;
        isByLayer(): boolean;
        isByBlock(): boolean;
        isByACI(): boolean;
        isForeground(): boolean;
        isByDgnIndex(): boolean;
        isNone(): boolean;
    }
    abstract class CmColorBase extends Em.EmObject{
        constructor();
        colorMethod(): ColorMethod;
        setColorMethod(_colorMethod: ColorMethod): void;
        isByColor(): boolean;
        isByLayer: boolean;
        isByBlock: boolean;
        isByACI: boolean;
        isForeground: boolean;
        isByDgnIndex: boolean;
        setColor(_color: number): void;
        color(): number;
        setRGB(_red: number, _green: number, _blue: number): void;
        setRed(_red: number): void;
        setGreen(_green: number): void;
        setBlue(_blue: number): void;
        red(): number;
        green(): number;
        blue(): number;
        setColorIndex(_colorIndex: number): void;
        colorIndex(): number;
        setNames(_colorName: string): boolean;
        setNames(_colorName: string, _bookName: string): boolean;
        colorName(): string;
        bookName(): string;
    }
    class CmColor extends CmColorBase {
        constructor();
        constructor(_color: CmColor);
        isNone(): boolean;
        entityColor(): CmEntityColor;
    }

    enum transparencyMethod {
        kByLayer,
        kByBlock,
        kByAlpha,
        kErrorValue,
    }
    class CmTransparency extends Em.EmObject{
        constructor();
        constructor(_val: number);
        setAlpha(alpha: number): void;
        alpha(): number;
        setAlphaPercent(_alphaPercent: number): void;
        alphaPercent(): number;
        method(): transparencyMethod;
        setMethod(method: transparencyMethod): void;
        isByAlpha(): boolean;
        isByBlock(): boolean;
        isByLayer(): boolean;
        isInvalid(): boolean;
        isClear(): boolean;
        isSolid(): boolean;
        serializeOut(): number;
        serializeIn(transparency: number): void;
    }
}


import Cm = Module; 
export default Cm;
