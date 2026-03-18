#include "DbDwgFileFiler.h"
#include "AcString.h"
#include "export.h"
#include "DbImpl.h"


DbDwgFileFiler::DbDwgFileFiler() {
	this->m_pImpl = new DbDwgFileFilerImpl();
}
DbDwgFileFiler::~DbDwgFileFiler() {

}
AcBinary* DbDwgFileFiler::binary() {
	return (AcBinary*)DB_IMP_FWGFILEFILER(this->m_pImpl)->buff;
}
Acad::ErrorStatus DbDwgFileFiler::filerStatus() const {
	return Acad::ErrorStatus::eOk;
}
Db::FilerType DbDwgFileFiler::filerType() const {
	return Db::FilerType::kFileFiler;
}
void DbDwgFileFiler::setFilerStatus(Acad::ErrorStatus es) {

}
void DbDwgFileFiler::resetFilerStatus() {

}

Acad::ErrorStatus DbDwgFileFiler::readInt8(Adesk::Int8* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeInt8(Adesk::Int8 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readString(ACHAR** pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeString(const ACHAR* pVal) {
	this->writeItem(pVal);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readString(AcString& val) {
	char* str = NULL;
	this->readItem(&str);
	val = str;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeString(const AcString& pVal) {
	this->writeItem(pVal);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readBChunk(ads_binary* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeBChunk(const ads_binary& pVal) {
	this->writeItem(pVal);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readDbHandle(DbHandle* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeDbHandle(const DbHandle& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readInt64(Adesk::Int64* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeInt64(Adesk::Int64 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readInt32(Adesk::Int32* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeInt32(Adesk::Int32 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readInt16(Adesk::Int16* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeInt16(Adesk::Int16 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readUInt64(Adesk::UInt64* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeUInt64(Adesk::UInt64 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readUInt32(Adesk::UInt32* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeUInt32(Adesk::UInt32 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readUInt16(Adesk::UInt16* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeUInt16(Adesk::UInt16 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readUInt8(Adesk::UInt8* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeUInt8(Adesk::UInt8 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readBoolean(Adesk::Boolean* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeBoolean(Adesk::Boolean val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readBool(bool* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeBool(bool val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readDouble(double* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeDouble(double val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readPoint2d(GePoint2d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writePoint2d(const GePoint2d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readPoint3d(GePoint3d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writePoint3d(const GePoint3d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readVector2d(GeVector2d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeVector2d(const GeVector2d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readVector3d(GeVector3d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeVector3d(const GeVector3d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgFileFiler::readScale3d(GeScale3d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFileFiler::writeScale3d(const GeScale3d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}
