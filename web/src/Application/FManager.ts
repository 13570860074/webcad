

export class FManager {
    constructor() {
        FManager.m_managers.push(this);
    }

    static m_managers: Array<FManager> = new Array<FManager>();

    static trigger(): void {
        FManager.m_managers.forEach((manager) => {
            manager.onInit();
        });
    }

    async onInit(): Promise<void> {
        return;
    }
}