#ifndef ACBINARYFILER_H
#define ACBINARYFILER_H

#include "AcBinary.h"
#include "AcArray.h"

class AcBinaryFilerNode
{
public:
	AcBinaryFilerNode()
	{
		this->index = AcBinary::EType::kInt32;
		this->index = 0;
		this->len = 0;
		this->data = NULL;
	}
	virtual ~AcBinaryFilerNode()
	{
		if (this->data != NULL)
		{
			delete this->data;
			this->data = NULL;
		}
	}

public:
	AcBinary::EType type;
	unsigned short index;
	unsigned int len;
	char *data;
};
class AcBinaryFiler : public IAcBinary
{
public:
	AcBinaryFiler();
	virtual ~AcBinaryFiler();

private:
	unsigned int m_index;
	AcArray<AcBinaryFilerNode *> *m_nodes;

public:
	// 获得节点
	AcArray<AcBinaryFilerNode *> *nodes()const;

	// 开始
	void begin();

	// 写入数据
	virtual void writeBoolean(Adesk::Boolean _val);
	virtual void writeInt8(Adesk::Int8 _val);
	virtual void writeUint8(Adesk::UInt8 _val);
	virtual void writeInt16(Adesk::Int16 _val);
	virtual void writeUint16(Adesk::UInt16 _val);
	virtual void writeInt32(Adesk::Int32 _val);
	virtual void writeUint32(Adesk::UInt32 _val);
	virtual void writeInt64(Adesk::Int64 _val);
	virtual void writeUint64(Adesk::UInt64 _val);
	virtual void writeFloat32(float _val);
	virtual void writeFloat64(double _val);
	virtual void writeString(const AcString &_val);
	virtual void writeString(const char *_val, unsigned int _len);
	virtual void writeAddress(const void *p);
	virtual void writeBinary(char *_data, unsigned int _len);

	// 读取数据
	virtual bool readBoolean(Adesk::Boolean &val);
	virtual bool readInt8(Adesk::Int8 &val);
	virtual bool readUint8(Adesk::UInt8 &val);
	virtual bool readInt16(Adesk::Int16 &val);
	virtual bool readUint16(Adesk::UInt16 &val);
	virtual bool readInt32(Adesk::Int32 &val);
	virtual bool readUint32(Adesk::UInt32 &val);
	virtual bool readInt64(Adesk::Int64 &val);
	virtual bool readUint64(Adesk::UInt64 &val);
	virtual bool readFloat32(float &val);
	virtual bool readFloat64(double &val);
	virtual bool readString(AcString &_val);
	virtual bool readString(char *&_val, unsigned int &_len);
	virtual bool readAddress(void *&val);
	virtual bool readBinary(char *&_data, unsigned int &_len);
};

#endif
