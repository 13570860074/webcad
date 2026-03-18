#include "DbDwgUndoFiler.h"
#include "AcString.h"
#include "export.h"
#include "DbImpl.h"


DbDwgUndoFiler::DbDwgUndoFiler() {
	this->m_pImpl = new DbDwgUndoFilerImpl();
}
DbDwgUndoFiler::~DbDwgUndoFiler() {

}
AcBinaryFiler* DbDwgUndoFiler::binary() {
	return (AcBinaryFiler*)DB_IMP_FWGUNDOFILER(this->m_pImpl)->buff;
}
Acad::ErrorStatus DbDwgUndoFiler::filerStatus() const {
	return Acad::ErrorStatus::eOk;
}
Db::FilerType DbDwgUndoFiler::filerType() const {
	return Db::FilerType::kUndoFiler;
}
void DbDwgUndoFiler::setFilerStatus(Acad::ErrorStatus es) {

}
void DbDwgUndoFiler::resetFilerStatus() {

}

Acad::ErrorStatus DbDwgUndoFiler::readInt8(Adesk::Int8* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeInt8(Adesk::Int8 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readString(ACHAR** pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeString(const ACHAR* pVal) {
	this->writeItem(pVal);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readString(AcString& val) {
	char* str = NULL;
	this->readItem(&str);
	val = str;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeString(const AcString& pVal) {
	this->writeItem(pVal);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readBChunk(ads_binary* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeBChunk(const ads_binary& pVal) {
	this->writeItem(pVal);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readDbHandle(DbHandle* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeDbHandle(const DbHandle& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readInt64(Adesk::Int64* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeInt64(Adesk::Int64 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readInt32(Adesk::Int32* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeInt32(Adesk::Int32 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readInt16(Adesk::Int16* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeInt16(Adesk::Int16 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readUInt64(Adesk::UInt64* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeUInt64(Adesk::UInt64 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readUInt32(Adesk::UInt32* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeUInt32(Adesk::UInt32 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readUInt16(Adesk::UInt16* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeUInt16(Adesk::UInt16 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readUInt8(Adesk::UInt8* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeUInt8(Adesk::UInt8 val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readBoolean(Adesk::Boolean* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeBoolean(Adesk::Boolean val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readBool(bool* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeBool(bool val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readDouble(double* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeDouble(double val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readPoint2d(GePoint2d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writePoint2d(const GePoint2d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readPoint3d(GePoint3d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writePoint3d(const GePoint3d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readVector2d(GeVector2d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeVector2d(const GeVector2d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readVector3d(GeVector3d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeVector3d(const GeVector3d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDwgUndoFiler::readScale3d(GeScale3d* pVal) {
	this->readItem(pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgUndoFiler::writeScale3d(const GeScale3d& val) {
	this->writeItem(val);
	return Acad::ErrorStatus::eOk;
}


