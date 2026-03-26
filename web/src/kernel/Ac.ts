

declare namespace Module {

    class AcArray {
        length: () => number;
        remove: (_data: any) => void;
        removeAt: (i: number) => void;
        removeAll: () => void;
        append: (_data: any) => void;
        at: (_i: number) => any;
        toVector: () => any;
    }

    class AcString {
        constructor();
        constructor(str: string);
        static ptr_char_to_string(inPtr: number): string;
        static ptr_string_to_string(inPtr: number): string;
        static ptr_AcString_to_string(inPtr: number): string;
        clear(): void;
        length(): number;
        tcharLength(): number;
        isEmpty(): boolean;
        get(): string;
        set(str: string): void;
    }

}



import Ac = Module; 
export default Ac;


