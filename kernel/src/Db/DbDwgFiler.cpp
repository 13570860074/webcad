#include "DbDwgFiler.h"
#include "AcString.h"
#include "DbImpl.h"



DbDwgFiler::DbDwgFiler() {

}
DbDwgFiler::~DbDwgFiler() {

}


Acad::ErrorStatus  DbDwgFiler::dwgVersion(unsigned int& ver) const {
	 ver = 0;
	 return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus  DbDwgFiler::readAddress(void** pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readAddress(*pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus  DbDwgFiler::writeAddress(const void* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeAddress(pVal);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus  DbDwgFiler::readItem(ACHAR** pVal) {
	unsigned int len = 0;
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readString(*pVal, len);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus  DbDwgFiler::writeItem(const ACHAR* val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeString(val, strlen(val));
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFiler::writeItem(const AcString& val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeString(val.constPtr(), val.length());
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::readItem(ads_binary* pVal) {
	unsigned int len = 0;
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readBinary(pVal->buf, len);
	pVal->clen = len;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(const ads_binary& val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeBinary(val.buf, val.clen);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(DbHandle* pVal) {
	unsigned int id = 0;
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readUint32(id);
	*pVal = id;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(const DbHandle& val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeUint32(val);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFiler::readItem(Adesk::Int64* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readInt64(*pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFiler::writeItem(Adesk::Int64 val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeInt64(val);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::readItem(Adesk::Int32* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readInt32(*pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(Adesk::Int32 val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeInt32(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(Adesk::Int16* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readInt16(*pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(Adesk::Int16 val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeInt16(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(Adesk::Int8* pVal) {
	Adesk::Int8 val;
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readInt8(val);
	*pVal = val;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(Adesk::Int8 val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeInt8(val);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFiler::readItem(Adesk::UInt64* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readUint64(*pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDwgFiler::writeItem(Adesk::UInt64 val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeUint64(val);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::readItem(Adesk::UInt32* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readUint32(*pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(Adesk::UInt32 val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeUint32(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(Adesk::UInt16* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readUint16(*pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(Adesk::UInt16 val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeUint16(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(Adesk::UInt8* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readUint8(*pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(Adesk::UInt8 val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeUint8(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(Adesk::Boolean* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readBoolean(*pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(Adesk::Boolean val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeBoolean(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(double* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(*pVal);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(double val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(GePoint2d* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->x);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->y);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(const GePoint2d& val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.x);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.y);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(GePoint3d* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->x);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->y);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->z);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(const GePoint3d& val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.x);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.y);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.z);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(GeVector2d* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->x);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->y);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(const GeVector2d& val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.x);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.y);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(GeVector3d* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->x);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->y);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->z);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(const GeVector3d& val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.x);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.y);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.z);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(GeScale3d* pVal) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->sx);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->sy);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readFloat64(pVal->sz);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(const GeScale3d& val) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.sx);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.sy);
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeFloat64(val.sz);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus   DbDwgFiler::readItem(void** pp) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->readAddress(*pp);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus   DbDwgFiler::writeItem(const void* p) {
	DB_IMP_FWGFILER(this->m_pImpl)->buff->writeAddress(p);
	return Acad::ErrorStatus::eOk;
}


DbDwgFiler& DbDwgFiler::operator = (const DbDwgFiler&) {
	return *this;
}


