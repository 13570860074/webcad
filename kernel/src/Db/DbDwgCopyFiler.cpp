#include "DbDwgCopyFiler.h"
#include "AcString.h"
#include "export.h"
#include "DbImpl.h"


DbDwgCopyFiler::DbDwgCopyFiler() {
	this->m_pImpl = new DbDwgCopyFilerImpl();
}
DbDwgCopyFiler::~DbDwgCopyFiler() {

}
AcBinaryFiler* DbDwgCopyFiler::binary() {
	return (AcBinaryFiler*)DB_IMP_FWGCOPYFILER(this->m_pImpl)->buff;
}
Acad::ErrorStatus DbDwgCopyFiler::filerStatus() const {
	return Acad::ErrorStatus::eOk;
}
Db::FilerType DbDwgCopyFiler::filerType() const {
	return Db::FilerType::kCopyFiler;
}
void DbDwgCopyFiler::setFilerStatus(Acad::ErrorStatus es) {

}
void DbDwgCopyFiler::resetFilerStatus() {

}

Acad::ErrorStatus DbDwgCopyFiler::readInt8(Adesk::Int8* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeInt8(Adesk::Int8 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readString(ACHAR** pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeString(const ACHAR* pVal) {
	this->writeItem(pVal);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readString(AcString& val) {
	char* str = NULL;
	this->readItem(&str);
	val = str;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeString(const AcString& pVal) {
	this->writeItem(pVal);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readBChunk(ads_binary* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeBChunk(const ads_binary& pVal) {
	this->writeItem(pVal);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readDbHandle(DbHandle* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeDbHandle(const DbHandle& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readInt64(Adesk::Int64* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeInt64(Adesk::Int64 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readInt32(Adesk::Int32* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeInt32(Adesk::Int32 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readInt16(Adesk::Int16* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeInt16(Adesk::Int16 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readUInt64(Adesk::UInt64* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeUInt64(Adesk::UInt64 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readUInt32(Adesk::UInt32* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeUInt32(Adesk::UInt32 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readUInt16(Adesk::UInt16* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeUInt16(Adesk::UInt16 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readUInt8(Adesk::UInt8* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeUInt8(Adesk::UInt8 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readBoolean(Adesk::Boolean* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeBoolean(Adesk::Boolean val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readBool(bool* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeBool(bool val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readDouble(double* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeDouble(double val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readPoint2d(GePoint2d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writePoint2d(const GePoint2d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readPoint3d(GePoint3d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writePoint3d(const GePoint3d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readVector2d(GeVector2d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeVector2d(const GeVector2d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readVector3d(GeVector3d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeVector3d(const GeVector3d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgCopyFiler::readScale3d(GeScale3d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgCopyFiler::writeScale3d(const GeScale3d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}
