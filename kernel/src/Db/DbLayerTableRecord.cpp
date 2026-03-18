#include "DbLayerTableRecord.h"
#include "DbDatabase.h"
#include "DbImpl.h"



DbLayerTableRecord::DbLayerTableRecord()
{
	this->m_pImpl = new DbLayerTableRecordImpl();
}
DbLayerTableRecord::~DbLayerTableRecord()
{

}


bool DbLayerTableRecord::isFrozen() const {
	return DB_IMP_LAYERTABLERECORD(this->m_pImpl)->isFrozen;
}
Acad::ErrorStatus DbLayerTableRecord::setIsFrozen(bool frozen) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_LAYERTABLERECORD(this->m_pImpl)->isFrozen = frozen;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbLayerTableRecord::isOff() const {
	return DB_IMP_LAYERTABLERECORD(this->m_pImpl)->isOff;
}
void DbLayerTableRecord::setIsOff(bool off) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_LAYERTABLERECORD(this->m_pImpl)->isOff = off;
	}
}

bool DbLayerTableRecord::VPDFLT() const {
	return DB_IMP_LAYERTABLERECORD(this->m_pImpl)->VPDFLT;
}
void DbLayerTableRecord::setVPDFLT(bool frozen) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_LAYERTABLERECORD(this->m_pImpl)->VPDFLT = frozen;
	}
}

bool DbLayerTableRecord::isLocked() const {
	return DB_IMP_LAYERTABLERECORD(this->m_pImpl)->isLocked;
}
void DbLayerTableRecord::setIsLocked(bool locked) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_LAYERTABLERECORD(this->m_pImpl)->isLocked = locked;
	}
}

CmColor DbLayerTableRecord::color() const {
	return DB_IMP_LAYERTABLERECORD(this->m_pImpl)->color;
}
void DbLayerTableRecord::setColor(const CmColor& color) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_LAYERTABLERECORD(this->m_pImpl)->color = color;
	}
}
CmEntityColor DbLayerTableRecord::entityColor(void) const {
	return DB_IMP_LAYERTABLERECORD(this->m_pImpl)->color.entityColor();
}

CmTransparency DbLayerTableRecord::transparency(void) const {
	return DB_IMP_LAYERTABLERECORD(this->m_pImpl)->transparency;
}
Acad::ErrorStatus DbLayerTableRecord::setTransparency(const CmTransparency& trans) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_LAYERTABLERECORD(this->m_pImpl)->transparency = trans;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

DbObjectId DbLayerTableRecord::linetypeObjectId() const {
	return DB_IMP_LAYERTABLERECORD(this->m_pImpl)->linetypeObjectId;
}
Acad::ErrorStatus DbLayerTableRecord::setLinetypeObjectId(DbObjectId id) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_LAYERTABLERECORD(this->m_pImpl)->linetypeObjectId = id;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbLayerTableRecord::isPlottable() const {
	return DB_IMP_LAYERTABLERECORD(this->m_pImpl)->isPlottable;
}
Acad::ErrorStatus DbLayerTableRecord::setIsPlottable(bool plot) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_LAYERTABLERECORD(this->m_pImpl)->isPlottable = plot;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Db::LineWeight DbLayerTableRecord::lineWeight() const {
	return DB_IMP_LAYERTABLERECORD(this->m_pImpl)->lineWeight;
}
Acad::ErrorStatus DbLayerTableRecord::setLineWeight(Db::LineWeight weight) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_LAYERTABLERECORD(this->m_pImpl)->lineWeight = weight;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}



bool DbLayerTableRecord::isInUse() const {
	return true;
}
ACHAR* DbLayerTableRecord::description() const
{
	char* tchar = NULL;
	::acutNewString(DB_IMP_LAYERTABLERECORD(this->m_pImpl)->description, tchar);
	return tchar;
}
Acad::ErrorStatus DbLayerTableRecord::description(AcString& desc) const
{
	desc = AcString(DB_IMP_LAYERTABLERECORD(this->m_pImpl)->description);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLayerTableRecord::setDescription(const ACHAR* description) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_LAYERTABLERECORD(this->m_pImpl)->description = description;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
#if EMSDK
emstring DbLayerTableRecord::emsdk_description() const {
	AcString str;
	this->description(str);
	return str.constPtr();
}
Acad::ErrorStatus DbLayerTableRecord::emsdk_setDescription(const emstring& description) {
	return this->setDescription(description.c_str());
}
#endif

bool DbLayerTableRecord::isHidden() const
{
	return DB_IMP_LAYERTABLERECORD(this->m_pImpl)->isHidden;
}
Acad::ErrorStatus DbLayerTableRecord::setIsHidden(bool on)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_LAYERTABLERECORD(this->m_pImpl)->isHidden = on;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
bool DbLayerTableRecord::isHidden(DbObjectId)
{
	return false;
}







Acad::ErrorStatus DbLayerTableRecord::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	bool isFrozen = false;
	pFiler->readBoolean(&isFrozen);
	this->setIsFrozen(isFrozen);

	bool isOff = false;
	pFiler->readBoolean(&isOff);
	this->setIsOff(isOff);

	bool VPDFLT = false;
	pFiler->readBoolean(&VPDFLT);
	this->setVPDFLT(VPDFLT);

	bool isLocked = false;
	pFiler->readBoolean(&isLocked);
	this->setIsLocked(isLocked);

	unsigned int linetypeObjectHandle;
	pFiler->readUInt32(&linetypeObjectHandle);
	DbObjectId linetypeObjectId = NULL;
	if (this->database()->getDbObjectId(linetypeObjectId, true, linetypeObjectHandle) == Acad::ErrorStatus::eOk) {
		this->setLinetypeObjectId(linetypeObjectId);
	}

	bool isPlottable = false;
	pFiler->readBoolean(&isPlottable);
	this->setIsPlottable(isPlottable);

	int lineWeight = 0;
	pFiler->readInt32(&lineWeight);
	this->setLineWeight((Db::LineWeight)lineWeight);

	char* description;
	pFiler->readString(&description);
	this->setDescription(description);

	bool isHidden = false;
	pFiler->readBoolean(&isHidden);
	this->setIsHidden(isHidden);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLayerTableRecord::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}



	pFiler->writeBoolean(this->isFrozen());

	pFiler->writeBoolean(this->isOff());

	pFiler->writeBoolean(this->VPDFLT());

	pFiler->writeBoolean(this->isLocked());

	pFiler->writeUInt32(this->linetypeObjectId().handle());

	pFiler->writeBoolean(this->isPlottable());

	pFiler->writeInt32(this->lineWeight());

	AcString description;
	this->description(description);
	pFiler->writeString(description);

	pFiler->writeBoolean(this->isHidden());


	return Acad::ErrorStatus::eOk;
}



