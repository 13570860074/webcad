import { FEntityManager } from "@/EntityManager/FEntityManager";
import { FManager } from "./FManager";
import { EventState, FEventManager } from "@/EventManager/FEventManager";
import { FKernelManager } from "@/KernelManager/FKernelManager";
import { FMaterialManager } from "@/MaterialManager/FMaterialManager";
import { FSceneManager } from "@/SceneManager/FSceneManager";
import { FUserInputManager } from "@/UserInputManager/FUserInputManager";


export class FApplication {
    constructor() {

    }

    async load(): Promise<void> {

        const intervalId = setInterval(() => {
            if ((<any>window).globalRuntimeInitialized != null) {
                (<any>window).globalRuntimeInitialized = null;
                clearInterval(intervalId);

                // 触发初始化事件
                FManager.trigger();

                // 触发wasm加载完成事件
                this.m_eventManager.emit(EventState.kRuntimeInitialized);
            }
        }, 100);
    }

    private m_eventManager: FEventManager = new FEventManager();
    private m_kernelManager: FKernelManager = new FKernelManager();
    private m_sceneManager: FSceneManager = new FSceneManager();
    private m_materialManager: FMaterialManager = new FMaterialManager();
    private m_entityManager: FEntityManager = new FEntityManager();
    private m_userInputManager: FUserInputManager = new FUserInputManager();

    /** 事件管理器 */
    eventManager(): FEventManager {
        return this.m_eventManager;
    }

    /** 内核管理器 */
    kernelManager(): FKernelManager {
        return this.m_kernelManager;
    }

    /** 场景管理器 */
    sceneManager(): FSceneManager {
        return this.m_sceneManager;
    }

    /** 材质管理器 */
    materialManager(): FMaterialManager {
        return this.m_materialManager;
    }

    /** 材质管理器 */
    entityManager(): FEntityManager {
        return this.m_entityManager;
    }

    /** 用户输入管理器 */
    userInputManager(): FUserInputManager {
        return this.m_userInputManager;
    }
}


export var application: FApplication = new FApplication();