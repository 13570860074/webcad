


class FEventItem {
    public Id: number = -1;
    public event: any = null;
    public fun: any = null;
}



export class FEvent {
    constructor() {
    }


    private Index: number = 0;
    private Items: Map<any, Array<FEventItem>> = new Map();
    private IdMap: Map<number, FEventItem> = new Map();


    //添加事件
    private appendItem(_item: FEventItem): number {
        _item.Id = this.Index++;
        let list = this.Items.get(_item.event);
        if (!list) {
            list = [];
            this.Items.set(_item.event, list);
        }
        list.push(_item);
        this.IdMap.set(_item.Id, _item);
        return _item.Id;
    }

    //触发事件
    emit(event: any, ...param: any[]): void {
        const items = this.Items.get(event);
        if (items) {
            for (let i = 0; i < items.length; i++) {
                items[i].fun.call(this, ...param);
            }
        }
    }
    dispatchEvent(event: any, ...param: any[]): void {
        this.emit(event, ...param);
    }

    //监听事件
    on(event: any, fun: Function): number {
        const item = new FEventItem();
        item.event = event;
        item.fun = fun;
        return this.appendItem(item);
    }
    addEventListener(event: any, fun: Function): number {
        return this.on(event, fun);
    }

    //移除事件
    removeListener(event: any, fun: any): void {
        const list = this.Items.get(event);
        if (!list) return;
        for (let i = list.length - 1; i >= 0; i--) {
            if (list[i].fun === fun) {
                this.IdMap.delete(list[i].Id);
                list.splice(i, 1);
            }
        }
    }
    removeEventListener(event: any, fun: any): void {
        this.removeListener(event, fun);
    }

    //通过Id移除事件
    removeListenerId(_Id: number): void {
        const item = this.IdMap.get(_Id);
        if (!item) return;
        this.IdMap.delete(_Id);
        const list = this.Items.get(item.event);
        if (list) {
            const idx = list.indexOf(item);
            if (idx !== -1) list.splice(idx, 1);
        }
    }
    removeEventListenerId(_Id: number): void {
        this.removeListenerId(_Id);
    }

    //移除所有事件
    removeAllListeners(): void {
        this.Items.clear();
        this.IdMap.clear();
    }
    removeAllEventListeners(): void {
        this.removeAllListeners();
    }
}