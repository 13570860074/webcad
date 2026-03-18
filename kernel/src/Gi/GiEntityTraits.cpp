#include "GiEntityTraits.h"
#include "DbObjectManager.h"
#include "arxHeaders.h"
#include "kernel.h"
#include "GiImpl.h"
#include "DbImpl.h"


GiEntityTraits::GiEntityTraits() {
    this->m_pImpl = new GiEntityTraitsImpl();
}
GiEntityTraits::~GiEntityTraits() {

}


void GiEntityTraits::init() {
    // 标记删除实体
    for (int i = 0; i < DB_IMP_ENTITY(this->entity()->m_pImpl)->entitys.length(); i++) {
        DB_IMP_ENTITY(this->entity()->m_pImpl)->entitys[i]->erased();
    }
    for (int i = 0; i < DB_IMP_ENTITY(this->entity()->m_pImpl)->highlightEntitys.length(); i++) {
        DB_IMP_ENTITY(this->entity()->m_pImpl)->highlightEntitys[i]->erased();
    }

    // 移除实体
    DB_IMP_ENTITY(this->entity()->m_pImpl)->entitys.removeAll();
    DB_IMP_ENTITY(this->entity()->m_pImpl)->highlightEntitys.removeAll();
}

void GiEntityTraits::setEntity(DbEntity* pEntity) {

    GI_IMP_ENTITYTRAITS(this->m_pImpl)->pEntity = pEntity;

    // 判断实体是否被删除
    this->setVisibility(true);
    if (pEntity->isErased() == true) {
        this->setVisibility(false);
        return;
    }

    // 获得当前图层
    DbObjectId layerId = pEntity->layerId();
    if (layerId == NULL) {
        if (pEntity->database() == NULL) {
            layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
        }
        else {
            layerId = pEntity->database()->clayer();
        }
    }

    // 获得层表记录
    DbLayerTableRecord* pLayerTableRecord = (DbLayerTableRecord*)::kernel()->acdbObjectManager()->openDbObject(layerId);
    if (pLayerTableRecord == NULL) {
        return;
    }

    // 如果图层是关闭,冻结状态则不显示
    if (pLayerTableRecord->isOff() == true || pLayerTableRecord->isHidden() == true || pLayerTableRecord->isFrozen() == true) {
        this->setVisibility(false);
        return;
    }

    // 设置颜色
    int colorIndex = pEntity->colorIndex();
    if (colorIndex == 0) {
        this->setColor(7);
    }
    else if (colorIndex == 256) {
        if (pLayerTableRecord != NULL) {
            this->setColor(pLayerTableRecord->color().colorIndex());
        }
        else {
            this->setColor(7);
        }
    }
    else {
        this->setColor(colorIndex);
    }

    // 设置线宽(这里暂时不设置线宽)
    if (pLayerTableRecord != NULL && pEntity->lineWeight() == Db::LineWeight::kLnWtByLayer) {
        this->setLineWeight(pLayerTableRecord->lineWeight());
    }
    else if (pEntity->lineWeight() == Db::LineWeight::kLnWtByLwDefault) {
        this->setLineWeight(Db::LineWeight::kLnWt000);
    }
    else {
        this->setLineWeight(pEntity->lineWeight());
    }
    this->setLineWeight(Db::LineWeight::kLnWt000);

    // 设置线型
    if (pLayerTableRecord != NULL && pEntity->linetype() == "ByLayer") {
        this->setLineType(pLayerTableRecord->linetypeObjectId());
    }
    else{
        this->setLineType(pEntity->linetypeId());
    }

    // 设置透明度
    if (pLayerTableRecord->isLocked() == true) {
        this->setTransparency(CmTransparency(0.5));
    }
    else if (pLayerTableRecord != NULL && pEntity->transparency().method() == CmTransparency::transparencyMethod::kByLayer) {
        this->setTransparency(pLayerTableRecord->transparency());
    }
    else if (pEntity->transparency().method() == CmTransparency::transparencyMethod::kByAlpha) {
        this->setTransparency(pEntity->transparency());
    }

    // 设置线性比例
    this->setLineTypeScale(pEntity->linetypeScale());
}
DbEntity* GiEntityTraits::entity()const {
    return  GI_IMP_ENTITYTRAITS(this->m_pImpl)->pEntity;
}
void GiEntityTraits::setDocument(ApDocument* _document) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->document = _document;
}
ApDocument *GiEntityTraits::document() {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->document;
}
void GiEntityTraits::setRootEntity(DbEntity* entity) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->parentEntity = entity;
}
DbEntity* GiEntityTraits::rootEntity()const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->parentEntity;
}
void GiEntityTraits::openMatrix3d(const bool isMatrix) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->isMatrix3d = isMatrix;
}
bool GiEntityTraits::isOpenMatrix3d()const {
    return  GI_IMP_ENTITYTRAITS(this->m_pImpl)->isMatrix3d;
}
void GiEntityTraits::setMatrix3d(const GeMatrix3d& mat) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->isMatrix3d = true;
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->matrix3d = mat;
}
const GeMatrix3d& GiEntityTraits::matrix3d()const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->matrix3d;
}
void GiEntityTraits::setStake(DbEntityStake* stake) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->stake = stake;
}
DbEntityStake* GiEntityTraits::stake()const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->stake;
}

void GiEntityTraits::setColor(const UInt16 color) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->isTrueColor = false;
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->colorIndex = color;
}
void GiEntityTraits::setTrueColor(const CmEntityColor& color) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->isTrueColor = true;
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->trueColor = color;
}
void GiEntityTraits::setLayer(const DbObjectId layerId) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->layerId = layerId;

    DbLayerTableRecord* pLayerTableRecord = NULL;
    ::acdbOpenObject(pLayerTableRecord, layerId);
    if (pLayerTableRecord!= NULL) {

        if (this->entity()->colorIndex() == 256) {
            this->setColor(pLayerTableRecord->color().colorIndex());
        }

        if (this->entity()->lineWeight() == Db::LineWeight::kLnWtByLayer) {
            this->setLineWeight(pLayerTableRecord->lineWeight());
        }

        if (this->entity()->linetype() == "ByLayer") {
            this->setLineType(pLayerTableRecord->linetypeObjectId());
        }

        if (this->entity()->transparency().method() == CmTransparency::transparencyMethod::kByLayer) {
            this->setTransparency(pLayerTableRecord->transparency());
        }

        pLayerTableRecord->close();
    }
}
void GiEntityTraits::setLineType(const DbObjectId linetypeId) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->lineType = linetypeId;
}
void GiEntityTraits::setSelectionMarker(void* markerId) {

}
void GiEntityTraits::setFillType(const GiFillType _type) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->fillType = _type;
}
void GiEntityTraits::setLineWeight(const Db::LineWeight lw) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->lineWidth = lw;
}
void GiEntityTraits::setLineTypeScale(double dScale) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->lineTypeScale = dScale;
}
void GiEntityTraits::setThickness(double dThickness) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->thickness = dThickness;
}
void GiEntityTraits::setVisualStyle(const DbObjectId visualStyleId) {

}
void GiEntityTraits::setPlotStyleName(Db::PlotStyleNameType type,const DbObjectId & id) {

}
void GiEntityTraits::setMaterial(const DbObjectId materialId) {

}
void GiEntityTraits::setSectionable(bool bSectionable) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->sectionable = bSectionable;
}
Acad::ErrorStatus GiEntityTraits::setDrawFlags(UInt32 flags) {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus GiEntityTraits::setShadowFlags(ShadowFlags flags) {
    return Acad::ErrorStatus::eOk;
}
void GiEntityTraits::setSelectionGeom(bool bSelectionflag) {
    
}
void GiEntityTraits::setTransparency(const CmTransparency& transparency) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->transparency = transparency;
}
void GiEntityTraits::setFill(const GiFill* pFill) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->pFill = (GiFill*)pFill;
}
void GiEntityTraits::setVisibility(bool _isVisibility) {
    GI_IMP_ENTITYTRAITS(this->m_pImpl)->isVisibility = _isVisibility;
}

    // Return current settings.
    //
UInt16 GiEntityTraits::color(void) const {
    if (GI_IMP_ENTITYTRAITS(this->m_pImpl)->isTrueColor == false) {
        return GI_IMP_ENTITYTRAITS(this->m_pImpl)->colorIndex;
    }
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->trueColor.colorIndex();
}
CmEntityColor GiEntityTraits::trueColor(void) const {
    if (GI_IMP_ENTITYTRAITS(this->m_pImpl)->isTrueColor == false) {
        CmEntityColor color;
        color.setColorIndex(GI_IMP_ENTITYTRAITS(this->m_pImpl)->colorIndex);
        return color;
    }
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->trueColor;
}
DbObjectId GiEntityTraits::layerId(void) const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->layerId;
}
DbObjectId GiEntityTraits::lineTypeId(void) const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->lineType;
}
GiFillType GiEntityTraits::fillType(void) const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->fillType;
}
Db::LineWeight GiEntityTraits::lineWeight(void) const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->lineWidth;
}
double GiEntityTraits::lineTypeScale(void) const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->lineTypeScale;
}
double GiEntityTraits::thickness(void) const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->thickness;
}
DbObjectId GiEntityTraits::visualStyle(void) const {
    return DbObjectId();
}
DbObjectId GiEntityTraits::materialId(void) const {
    return NULL;
}
bool GiEntityTraits::sectionable(void) const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->sectionable;
}
UInt32 GiEntityTraits::drawFlags(void) const {
    return 0;
}
GiEntityTraits::ShadowFlags GiEntityTraits::shadowFlags(void) const {
    return GiEntityTraits::ShadowFlags::kShadowsCastAndReceive;
}
bool GiEntityTraits::selectionGeom(void) const {
    return false;
}
CmTransparency GiEntityTraits::transparency(void) const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->transparency;
}
const GiFill* GiEntityTraits::fill(void) const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->pFill;
}
bool GiEntityTraits::isVisibility()const {
    return GI_IMP_ENTITYTRAITS(this->m_pImpl)->isVisibility;
}

void GiEntityTraits::pushMarkerOverride(bool flag, const void* markerId) {
}
void GiEntityTraits::popMarkerOverride(void) {
}
void GiEntityTraits::clearMarkerOverride(void) { 
}

