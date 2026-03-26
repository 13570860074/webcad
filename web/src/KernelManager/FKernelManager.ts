import Ap from '@/kernel/Ap'
import Rx from '@/kernel/Rx'
import Re from '@/kernel/Re'
import Gi from '@/kernel/Gi'
import Db from '@/kernel/Db'
import Ed from '@/kernel/Ed'
import Ui from '@/kernel/Ui'
import Arx from '@/kernel/arxHeaders'
import { ArxEvent } from '@/kernel/arxEvent'
import { FManager } from '@/Application/FManager';
import { application } from '@/Application/FApplication'
import { EventState } from '@/EventManager/FEventManager'
import Ge from '@/kernel/Ge'



export class FKernelManager extends FManager {
    constructor() {
        super();
    }

    private m_rendererManager: Re.ReRendererManager = null;
    private m_materialManager: Gi.GiMaterialManager = null;
    private m_entityManager: Gi.GiEntityManager = null;
    private m_userInputManager: Ed.EdUserInputManager = null;
    private m_docManagerPtr: Ap.ApDocManager = null;

    async onInit(): Promise<void> {

        this.m_rendererManager = Arx.acreRendererManager();
        this.m_materialManager = Arx.acgiMaterialManager();
        this.m_entityManager = Arx.acgiEntityManager();
        this.m_userInputManager = Arx.acedUserInputManager();
        this.m_docManagerPtr = Arx.acDocManagerPtr();

        /** 监听材质事件 */
        let materialReactor: Gi.GiMaterialReactor = new Gi.GiMaterialReactor();
        ArxEvent.GiMaterialReactor_onCreateMaterial(materialReactor, (_material: Gi.GiMaterial) => {
            application.eventManager().emit(EventState.kCreateMaterial, _material);
        });
        this.m_materialManager.addReactor(materialReactor);

        /** 监听实体事件 */
        let entityReactor: Gi.GiEntityReactor = new Gi.GiEntityReactor();
        ArxEvent.GiEntityReactor_onCreateEntity(entityReactor, (entity: Gi.GiEntity) => {
            application.eventManager().emit(EventState.kCreateEntity, entity);
        });
        ArxEvent.GiEntityReactor_onUpdateVertexs(entityReactor, (entity: Gi.GiEntity) => {
            application.eventManager().emit(EventState.kUpdateVertexs, entity);
        });
        this.m_entityManager.addReactor(entityReactor);

        /** 渲染事件 */
        let rendererReactor: Re.ReRendererReactor = new Re.ReRendererReactor();
        ArxEvent.ReRendererReactor_onBeginRenderer(rendererReactor, () => {
            application.eventManager().emit(EventState.kBeginRenderer);
        });
        ArxEvent.ReRendererReactor_onEndRenderer(rendererReactor, () => {
            application.eventManager().emit(EventState.kEndRenderer);
        });
        ArxEvent.ReRendererReactor_onRendererViewport(rendererReactor, (renderer: Re.ReRenderer) => {
            application.eventManager().emit(EventState.kRendererViewport, renderer);
        });
        this.m_rendererManager.addReactor(rendererReactor);

        /** 监听文档事件 */
        let docManagerReactor: Ap.ApDocManagerReactor = new Ap.ApDocManagerReactor();
        ArxEvent.ApDocManagerReactor_onDocumentCreated(docManagerReactor, (document: Ap.ApDocument) => {
            application.eventManager().emit(EventState.kDocumentCreated, document);
        });
        ArxEvent.ApDocManagerReactor_onDocumentActivated(docManagerReactor, (document: Ap.ApDocument) => {
            application.eventManager().emit(EventState.kDocumentActivated, document);
        });
        ArxEvent.ApDocManagerReactor_onDocumentToBeDestroyed(docManagerReactor, (document: Ap.ApDocument) => {
            application.eventManager().emit(EventState.kDocumentToBeDestroyed, document);
        });
        this.m_docManagerPtr.addReactor(docManagerReactor);
    }


    /** 获得渲染管理器 */
    rendererManager(): Re.ReRendererManager {
        return this.m_rendererManager;
    }

    /** 获得材质管理器 */
    materialManager(): Gi.GiMaterialManager {
        return this.m_materialManager;
    }

    /** 获得实体管理器 */
    entityManager(): Gi.GiEntityManager {
        return this.m_entityManager;
    }

    /** 获得实体管理器 */
    userInputManager(): Ed.EdUserInputManager {
        return this.m_userInputManager;
    }

    /** 文档管理器 */
    docManagerPtr(): Ap.ApDocManager {
        return this.m_docManagerPtr;
    }
}