import Gi from '@/kernel/Gi';
import { FPoints } from './FPoints';
import { FMesh } from './FMesh';
import { FLineSegment } from './FLineSegment';
import { application } from '../Application/FApplication';
import { FEntityRecord } from './FEntityRecord';
import { FManager } from '../Application/FManager';
import { EventState } from '../EventManager/FEventManager';

export class FEntityManager extends FManager {
    constructor() {
        super();
    }


    private m_entitys: Array<FEntityRecord> = new Array<FEntityRecord>();


    // 监听到创建实体事件
    private onCreateEntity = (entity: Gi.GiEntity) => {
        if (entity.type() == Gi.EntityType.kLineSegment) {
            let lineSegment: FLineSegment = new FLineSegment();
            lineSegment.setId(entity.objectId().asOldId());
            lineSegment.setElement(entity.element());
            lineSegment.setMaterialId(entity.material().objectId().asOldId());
            lineSegment.create();
            this.appendEntity(lineSegment);
            application.eventManager().emit(EventState.kCreateEntityRecord, lineSegment);
        }
        else if (entity.type() == Gi.EntityType.kMesh) {
            let mesh: FMesh = new FMesh();
            mesh.setId(entity.objectId().asOldId());
            mesh.setElement(entity.element());
            mesh.setMaterialId(entity.material().objectId().asOldId());
            mesh.create();
            this.appendEntity(mesh);
            application.eventManager().emit(EventState.kCreateEntityRecord, mesh);
        }
        else if (entity.type() == Gi.EntityType.kPoints) {
            let points: FPoints = new FPoints();
            points.setId(entity.objectId().asOldId());
            points.setElement(entity.element());
            points.setMaterialId(entity.material().objectId().asOldId());
            points.create();
            this.appendEntity(points);
            application.eventManager().emit(EventState.kCreateEntityRecord, points);
        }
    }

    // 监听到修改实体顶点事件
    private onUpdateVertexs = (entity: Gi.GiEntity) => {
        let entityRecord: FEntityRecord = this.m_entitys[entity.objectId().asOldId() - 1];
        if (entityRecord.type() == Gi.EntityType.kLineSegment) {
            let lineSegment: FLineSegment = <FLineSegment>entityRecord;
            lineSegment.updateVariants(entity.vertexs(), entity.vertexLength());
        }
        else if (entityRecord.type() == Gi.EntityType.kMesh) {
            let mesh: FMesh = <FMesh>entityRecord;
            mesh.updateVariants(entity.vertexs(), entity.vertexLength());
        }
        else if (entityRecord.type() == Gi.EntityType.kPoints) {
            let points: FPoints = <FPoints>entityRecord;
            points.updateVariants(entity.vertexs(), entity.vertexLength());
        }
        application.eventManager().emit(EventState.kUpdateEntityVertex, entityRecord);
    }

    // 监听实体可见性变换事件
    private onVisibility = (entity: Gi.GiEntity, visibility: boolean) => {
        let record: FEntityRecord = this.m_entitys[entity.objectId().asOldId() - 1];
        if (record != null) {
            record.setVisibility(visibility);
        }
    }

    // 初始化
    async onInit(): Promise<void> {

        // 监听实体事件
        application.eventManager().on(EventState.kCreateEntity, this.onCreateEntity);
        application.eventManager().on(EventState.kUpdateVertexs, this.onUpdateVertexs);
        application.eventManager().on(EventState.kVisibility, this.onVisibility);
    }

    /** 更新实体可见性 */
    updateVisibility = (_ids: Uint32Array = null) => {
        if (_ids == null) {
            let information: Array<{ id: number, visibility: boolean }> = JSON.parse(application.kernelManager().entityManager().information());
            for (let i = 0; i < information.length; i++) {
                let entityRecord: FEntityRecord = this.m_entitys[information[i].id - 1];
                if (entityRecord == null) {
                    continue;
                }

                entityRecord.setVisibility(information[i].visibility);
            }
        }
        else {
            for (let i = 0; i < this.m_entitys.length; i++) {
                if (this.m_entitys[i] == null) {
                    continue;
                }
                this.m_entitys[i].setVisibility(false);
            }
            for (let i = 0; i < _ids.length; i++) {
                let entityRecord: FEntityRecord = this.m_entitys[_ids[i] - 1];
                if (entityRecord == null) {
                    continue;
                }
                entityRecord.setVisibility(true);
            }
        }
    }

    //添加实体
    appendEntity(_entity: FEntityRecord): void {
        for (let i = this.m_entitys.length; i < _entity.id() + 1; i++) {
            this.m_entitys.push(null);
        }
        this.m_entitys[_entity.id() - 1] = _entity;
    }
}