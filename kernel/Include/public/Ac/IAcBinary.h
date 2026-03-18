#ifndef IACBINARY_H
#define IACBINARY_H

#include "adesk.h"
#include "AcString.h"

class IAcBinary
{

public:
	/** 写入数据 */
	virtual void writeBoolean(Adesk::Boolean _val) = 0;
	virtual void writeInt8(Adesk::Int8 _val) = 0;
	virtual void writeUint8(Adesk::UInt8 _val) = 0;
	virtual void writeInt16(Adesk::Int16 _val) = 0;
	virtual void writeUint16(Adesk::UInt16 _val) = 0;
	virtual void writeInt32(Adesk::Int32 _val) = 0;
	virtual void writeUint32(Adesk::UInt32 _val) = 0;
	virtual void writeInt64(Adesk::Int64 _val) = 0;
	virtual void writeUint64(Adesk::UInt64 _val) = 0;
	virtual void writeFloat32(float _val) = 0;
	virtual void writeFloat64(double _val) = 0;
	virtual void writeString(const AcString& _val) = 0;
	virtual void writeString(const ACHAR *_val, unsigned int _len) = 0;
	virtual void writeAddress(const void *p) = 0;
	virtual void writeBinary(char *_data, unsigned int _len) = 0;

	/** 读取数据 */
	virtual bool readBoolean(Adesk::Boolean &val) = 0;
	virtual bool readInt8(Adesk::Int8 &val) = 0;
	virtual bool readUint8(Adesk::UInt8 &val) = 0;
	virtual bool readInt16(Adesk::Int16 &val) = 0;
	virtual bool readUint16(Adesk::UInt16& val) = 0;
	virtual bool readInt32(Adesk::Int32 &val) = 0;
	virtual bool readUint32(Adesk::UInt32 &val) = 0;
	virtual bool readInt64(Adesk::Int64 &val) = 0;
	virtual bool readUint64(Adesk::UInt64 &val) = 0;
	virtual bool readFloat32(float &val) = 0;
	virtual bool readFloat64(double &val) = 0;
	virtual bool readString(AcString& _val) = 0;
	virtual bool readString(ACHAR*& _val, unsigned int& _len) = 0;
	virtual bool readAddress(void *&val) = 0;
	virtual bool readBinary(char *&_data, unsigned int &_len) = 0;
};


#endif
