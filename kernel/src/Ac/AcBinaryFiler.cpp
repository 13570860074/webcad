#include "AcBinaryFiler.h"

AcBinaryFiler::AcBinaryFiler()
{
	this->m_index = 0;
	this->m_nodes = new AcArray<AcBinaryFilerNode *>();
}
AcBinaryFiler::~AcBinaryFiler()
{
	realloc_all(this->m_nodes);
}

// 获得节点
AcArray<AcBinaryFilerNode *> *AcBinaryFiler::nodes() const
{
	return this->m_nodes;
}

void AcBinaryFiler::begin()
{
	this->m_index = 0;
}

// 写入数据
void AcBinaryFiler::writeBoolean(Adesk::Boolean _val)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kBoolean;
	node->index = this->m_nodes->length();
	node->len = 1;
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_val, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeInt8(Adesk::Int8 _val)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kInt8;
	node->index = this->m_nodes->length();
	node->len = 1;
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_val, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeUint8(Adesk::UInt8 _val)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kUint8;
	node->index = this->m_nodes->length();
	node->len = 1;
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_val, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeInt16(Adesk::Int16 _val)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kInt16;
	node->index = this->m_nodes->length();
	node->len = 2;
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_val, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeUint16(Adesk::UInt16 _val)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kUint16;
	node->index = this->m_nodes->length();
	node->len = 2;
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_val, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeInt32(Adesk::Int32 _val)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kInt32;
	node->index = this->m_nodes->length();
	node->len = 4;
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_val, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeUint32(Adesk::UInt32 _val)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kUint32;
	node->index = this->m_nodes->length();
	node->len = 4;
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_val, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeInt64(Adesk::Int64 _val)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kInt64;
	node->index = this->m_nodes->length();
	node->len = 8;
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_val, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeUint64(Adesk::UInt64 _val)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kUint64;
	node->index = this->m_nodes->length();
	node->len = 8;
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_val, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeFloat32(float _val)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kFloat32;
	node->index = this->m_nodes->length();
	node->len = 4;
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_val, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeFloat64(double _val)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kFloat64;
	node->index = this->m_nodes->length();
	node->len = 8;
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_val, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeString(const AcString &_val)
{
	this->writeString(_val.constPtr(), _val.length());
}
void AcBinaryFiler::writeString(const char *_val, unsigned int _len)
{
	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kString;
	node->index = this->m_nodes->length();
	node->len = _len;
	node->data = new char[node->len];
	for (int i = 0; i < node->len; i++)
	{
		node->data[i] = _val[i];
	}
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeAddress(const void *p)
{

	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kAddress;
	node->index = this->m_nodes->length();
	node->len = sizeof(void *);
	node->data = new char[node->len];
	memcpy(node->data, (char *)&p, node->len);
	this->m_nodes->append(node);
}
void AcBinaryFiler::writeBinary(char *_data, unsigned int _len)
{

	AcBinaryFilerNode *node = new AcBinaryFilerNode();
	node->type = AcBinary::EType::kBinary;
	node->index = this->m_nodes->length();
	node->len = sizeof(void *);
	node->data = new char[node->len];
	memcpy(node->data, (char *)&_data, node->len);
	this->m_nodes->append(node);
}

// 读取数据
bool AcBinaryFiler::readBoolean(Adesk::Boolean &val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kBoolean)
	{
		val = *((Adesk::Boolean *)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readInt8(Adesk::Int8 &val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kInt8)
	{
		val = *((Adesk::Int8 *)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readUint8(Adesk::UInt8 &val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kUint8)
	{
		val = *((Adesk::UInt8 *)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readInt16(Adesk::Int16 &val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kInt16)
	{
		val = *((Adesk::Int16 *)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readUint16(Adesk::UInt16 &val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kUint16)
	{
		val = *((Adesk::UInt16 *)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readInt32(Adesk::Int32 &val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kInt32)
	{
		val = *((Adesk::Int32 *)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readUint32(Adesk::UInt32 &val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kUint32)
	{
		val = *((Adesk::UInt32 *)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readInt64(Adesk::Int64 &val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kInt64)
	{
		val = *((Adesk::Int64 *)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readUint64(Adesk::UInt64 &val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kUint64)
	{
		val = *((Adesk::UInt64 *)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readFloat32(float &val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kFloat32)
	{
		val = *((float *)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readFloat64(double &val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kFloat64)
	{
		val = *((double *)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readString(AcString &_val)
{
	char *str;
	unsigned int len = 0;
	if (this->readString(str, len) == true)
	{
		_val = str;
		delete str;
		return true;
	}
	return false;
}
bool AcBinaryFiler::readString(char *&_val, unsigned int &_len)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kString)
	{
		_len = this->m_nodes->at(this->m_index)->len;
		_val = new char[_len + 1];
		for (int i = 0; i < _len; i++)
		{
			_val[i] = this->m_nodes->at(this->m_index)->data[i];
		}
		_val[_len] = '\0';
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readAddress(void *&val)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kAddress)
	{
		val = *((void **)(this->m_nodes->at(this->m_index)->data));
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
bool AcBinaryFiler::readBinary(char *&_data, unsigned int &_len)
{
	if (this->m_index >= this->m_nodes->length())
	{
		return false;
	}
	if (this->m_nodes->at(this->m_index)->type == AcBinary::kBinary)
	{
		_len = this->m_nodes->at(this->m_index)->len;
		_data = new char[_len];
		for (int i = 0; i < _len; i++)
		{
			_data[i] = this->m_nodes->at(this->m_index)->data[i];
		}
		this->m_index++;
		return true;
	}
	this->m_index++;
	return false;
}
