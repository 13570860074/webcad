#include "AcBinary.h"

AcBinary::AcBinary()
{
	this->m_pos = 0;
	this->m_len = 0;
	this->m_size = 0;
	this->m_buff = NULL;
}
AcBinary::~AcBinary()
{
	if (this->m_buff != NULL)
	{
		delete[] this->m_buff;
		this->m_buff = NULL;
	}
}

AcBinary &AcBinary::operator=(const AcBinary &_data)
{
	this->m_pos = _data.m_pos;
	this->m_len = _data.m_len;
	this->m_size = _data.m_size;
	if (this->m_size == 0)
	{
		this->m_buff = NULL;
	}
	else
	{
		this->m_buff = new char[this->m_size];
		for (unsigned int i = 0; i < this->m_size; i++)
		{
			this->m_buff[i] = _data.m_buff[i];
		}
	}
	return *this;
}

void AcBinary::allocateMemory()
{

	if (this->m_size == 0)
	{
		this->m_pos = 0;
		this->m_len = 0;
		this->m_size = 512;
		this->m_buff = new char[this->m_size];
	}
	else
	{
		char *temp = new char[this->m_size];
		for (unsigned int i = 0; i < this->m_size; i++)
		{
			temp[i] = this->m_buff[i];
		}

		delete[] this->m_buff;
		this->m_buff = new char[this->m_size * 2];
		for (unsigned int i = 0; i < this->m_size; i++)
		{
			this->m_buff[i] = temp[i];
		}
		this->m_size *= 2;

		delete[] temp;
	}
}

// 获得buff数据
const char *AcBinary::data()
{
	return this->m_buff;
}
const char AcBinary::data(unsigned int _pos)
{
	return this->m_buff[_pos];
}
const char *AcBinary::data(unsigned int _pos, unsigned int _len)
{
	char *val = new char[_len];
	for (int i = 0; i < _len; i++)
	{
		val[i] = this->m_buff[i + _pos];
	}
	return val;
}

// 载入buff
void AcBinary::load(const char *_data, unsigned int _len)
{
	do
	{
		if (this->m_buff != NULL)
		{
			delete[] this->m_buff;
			this->m_buff = NULL;
		}

		this->m_buff = new char[_len];
		for (int i = 0; i < _len; i++)
		{
			this->m_buff[i] = _data[i];
		}

		this->m_pos = 0;
		this->m_len = _len;
		this->m_size = this->m_len;

	} while (false);
}

int AcBinary::locator(const char *_data, unsigned int _len)
{
	int val = -1;

	for (unsigned int i = this->m_pos; i < this->m_len; i++)
	{
		if (i + _len > this->m_len)
		{
			break;
		}
		for (int u = 0; u < _len; u++)
		{
			if (_data[u] != this->m_buff[i + u])
			{
				break;
			}
			if (u == _len - 1)
			{
				val = i;
			}
		}
		if (val >= 0)
		{
			break;
		}
	}

	return val;
}

// 保存文件
void AcBinary::saveAs(const AcString& _fileName)
{
	std::ofstream outFile(_fileName.constPtr(), std::ios::out | std::ios::binary);
	outFile.write(m_buff, this->len());
	outFile.close();
}

// 加载文件
void AcBinary::loadFile(const ACHAR _fileName[])
{
	do
	{
		this->m_pos = 0;

		// 读数据
		std::ifstream *file = new std::ifstream();
		file->open(_fileName, std::ios::in | std::ios::binary);
		if (file == NULL)
		{
			break;
		}

		// 获得长度
		std::streampos ps = file->tellg();
		file->seekg(0, std::ios::end);
		this->m_len = file->tellg();
		file->seekg(ps);

		this->m_size = this->m_len;
		this->m_buff = new char[this->m_size];
		file->read(this->m_buff, this->m_size);
		file->close();

	} while (false);
}

// 获得位置
unsigned int AcBinary::position()
{
	return this->m_pos;
}

// 设置位置
void AcBinary::setPosition(unsigned int _pos)
{
	this->m_pos = _pos;
}

// 获得数据长度
unsigned int AcBinary::len()
{
	return this->m_len;
}

// 写入数据
void AcBinary::writeBoolean(Adesk::Boolean _val)
{
	if (this->m_pos + 1 >= this->m_size)
	{
		this->allocateMemory();
	}
	char *binary = (char *)&_val;
	this->m_buff[this->m_pos++] = binary[0];

	this->m_len = this->m_pos ;
}
void AcBinary::writeInt8(Adesk::Int8 _val)
{
	if (this->m_pos + 1 >= this->m_size)
	{
		this->allocateMemory();
	}
	this->m_buff[this->m_pos++] = _val;

	this->m_len = this->m_pos ;
}
void AcBinary::writeUint8(Adesk::UInt8 _val)
{
	if (this->m_pos + 1 >= this->m_size)
	{
		this->allocateMemory();
	}
	this->m_buff[this->m_pos++] = _val;

	this->m_len = this->m_pos ;
}
void AcBinary::writeInt16(Adesk::Int16 _val)
{
	if (this->m_pos + 2 >= this->m_size)
	{
		this->allocateMemory();
	}
	char *binary = (char *)&_val;
	this->m_buff[this->m_pos++] = binary[0];
	this->m_buff[this->m_pos++] = binary[1];

	this->m_len = this->m_pos ;
}
void AcBinary::writeUint16(Adesk::UInt16 _val)
{
	if (this->m_pos + 2 >= this->m_size)
	{
		this->allocateMemory();
	}
	char *binary = (char *)&_val;
	this->m_buff[this->m_pos++] = binary[0];
	this->m_buff[this->m_pos++] = binary[1];

	this->m_len = this->m_pos ;
}
void AcBinary::writeInt32(Adesk::Int32 _val)
{
	if (this->m_pos + 4 >= this->m_size)
	{
		this->allocateMemory();
	}
	char *binary = (char *)&_val;
	this->m_buff[this->m_pos++] = binary[0];
	this->m_buff[this->m_pos++] = binary[1];
	this->m_buff[this->m_pos++] = binary[2];
	this->m_buff[this->m_pos++] = binary[3];

	this->m_len = this->m_pos ;
}
void AcBinary::writeUint32(Adesk::UInt32 _val)
{
	if (this->m_pos + 4 >= this->m_size)
	{
		this->allocateMemory();
	}
	char *binary = (char *)&_val;
	this->m_buff[this->m_pos++] = binary[0];
	this->m_buff[this->m_pos++] = binary[1];
	this->m_buff[this->m_pos++] = binary[2];
	this->m_buff[this->m_pos++] = binary[3];

	this->m_len = this->m_pos ;
}
void AcBinary::writeInt64(Adesk::Int64 _val)
{
	if (this->m_pos + 8 >= this->m_size)
	{
		this->allocateMemory();
	}
	char *binary = (char *)&_val;
	this->m_buff[this->m_pos++] = binary[0];
	this->m_buff[this->m_pos++] = binary[1];
	this->m_buff[this->m_pos++] = binary[2];
	this->m_buff[this->m_pos++] = binary[3];
	this->m_buff[this->m_pos++] = binary[4];
	this->m_buff[this->m_pos++] = binary[5];
	this->m_buff[this->m_pos++] = binary[6];
	this->m_buff[this->m_pos++] = binary[7];

	this->m_len = this->m_pos ;
}
void AcBinary::writeUint64(Adesk::UInt64 _val)
{
	if (this->m_pos + 8 >= this->m_size)
	{
		this->allocateMemory();
	}
	char *binary = (char *)&_val;
	this->m_buff[this->m_pos++] = binary[0];
	this->m_buff[this->m_pos++] = binary[1];
	this->m_buff[this->m_pos++] = binary[2];
	this->m_buff[this->m_pos++] = binary[3];
	this->m_buff[this->m_pos++] = binary[4];
	this->m_buff[this->m_pos++] = binary[5];
	this->m_buff[this->m_pos++] = binary[6];
	this->m_buff[this->m_pos++] = binary[7];

	this->m_len = this->m_pos ;
}
void AcBinary::writeFloat32(float _val)
{
	if (this->m_pos + 4 >= this->m_size)
	{
		this->allocateMemory();
	}
	char *binary = (char *)&_val;
	this->m_buff[this->m_pos++] = binary[0];
	this->m_buff[this->m_pos++] = binary[1];
	this->m_buff[this->m_pos++] = binary[2];
	this->m_buff[this->m_pos++] = binary[3];

	this->m_len = this->m_pos ;
}
void AcBinary::writeFloat64(double _val)
{
	if (this->m_pos + 8 >= this->m_size)
	{
		this->allocateMemory();
	}
	char *binary = (char *)&_val;
	this->m_buff[this->m_pos++] = binary[0];
	this->m_buff[this->m_pos++] = binary[1];
	this->m_buff[this->m_pos++] = binary[2];
	this->m_buff[this->m_pos++] = binary[3];
	this->m_buff[this->m_pos++] = binary[4];
	this->m_buff[this->m_pos++] = binary[5];
	this->m_buff[this->m_pos++] = binary[6];
	this->m_buff[this->m_pos++] = binary[7];

	this->m_len = this->m_pos ;
}
void AcBinary::writeString(const AcString& _val) {
	this->writeString(_val.constPtr(), _val.length());
}
void AcBinary::writeString(const char* _val, unsigned int _len) {
	this->writeUint32(_len);
	while (this->m_pos + _len >= this->m_size)
	{
		this->allocateMemory();
	}
	for (int i = 0; i < _len; i++)
	{
		this->m_buff[this->m_pos++] = _val[i];
	}

	this->m_len = this->m_pos;
}
void AcBinary::writeAddress(const void *p)
{

	unsigned int vlen = sizeof(void*);
	if (this->m_pos + vlen >= this->m_size)
	{
		this->allocateMemory();
	}
	char* binary = (char*)&p;
	for (int i = 0; i < vlen; i++) {
		this->m_buff[this->m_pos++] = binary[i];
	}
	this->m_len = this->m_pos ;
}
void AcBinary::writeBinary(char *_data, unsigned int _len)
{
	this->writeUint32(_len);
	while (this->m_pos + _len >= this->m_size)
	{
		this->allocateMemory();
	}
	for (unsigned int i = 0; i < _len; i++)
	{
		this->m_buff[this->m_pos++] = _data[i];
	}
	this->m_len = this->m_pos ;
}
void AcBinary::writeData(char *_data, unsigned int _len)
{
	while (this->m_pos + _len >= this->m_size)
	{
		this->allocateMemory();
	}
	for (unsigned int i = 0; i < _len; i++)
	{
		this->m_buff[this->m_pos++] = _data[i];
	}
	this->m_len = this->m_pos ;
}


bool AcBinary::readBoolean(Adesk::Boolean& val) {
	val = *((bool*)(this->m_buff + this->m_pos));
	this->m_pos += 1;
	return true;
}

bool AcBinary::readInt8(Adesk::Int8& val) {
	val = *((char*)(this->m_buff + this->m_pos));
	this->m_pos += 1;
	return true;
}

bool AcBinary::readUint8(Adesk::UInt8& val) {
	val = *((unsigned char*)(this->m_buff + this->m_pos));
	this->m_pos += 1;
	return true;
}

bool AcBinary::readInt16(Adesk::Int16& val) {
	val = *((short*)(this->m_buff + this->m_pos));
	this->m_pos += 2;
	return true;
}

bool AcBinary::readUint16(Adesk::UInt16& val) {
	val = *((unsigned short*)(this->m_buff + this->m_pos));
	this->m_pos += 2;
	return true;
}

bool AcBinary::readInt32(Adesk::Int32& val) {
	val = *((int*)(this->m_buff + this->m_pos));
	this->m_pos += 4;
	return true;
}

bool AcBinary::readUint32(Adesk::UInt32& val) {
	val = *((unsigned int*)(this->m_buff + this->m_pos));
	this->m_pos += 4;
	return true;
}

bool AcBinary::readInt64(Adesk::Int64& val) {
	val = *((long long*)(this->m_buff + this->m_pos));
	this->m_pos += 8;
	return true;
}

bool AcBinary::readUint64(Adesk::UInt64& val) {
	val = *((unsigned long long*)(this->m_buff + this->m_pos));
	this->m_pos += 8;
	return true;
}

bool AcBinary::readFloat32(float& val) {
	val = *((float*)(this->m_buff + this->m_pos));
	this->m_pos += 4;
	return true;
}

bool AcBinary::readFloat64(double& val) {
	val = *((double*)(this->m_buff + this->m_pos));
	this->m_pos += 8;
	return true;
}
bool AcBinary::readString(AcString& _val) {

	char* temp = NULL;
	unsigned int vlen = 0;
	this->readString(temp, vlen);

	_val.clear();
	_val.append(temp);
	return true;
}
bool AcBinary::readString(ACHAR*& _val, unsigned int& _len) {

	this->readUint32(_len);
	_val = new char[_len + 1];
	for (int i = 0; i < _len; i++)
	{
		_val[i] = this->m_buff[this->m_pos++];
	}
	_val[_len] = '\0';
	return _val;
}

bool AcBinary::readAddress(void*& val) {
	val = ((void*)(this->m_buff + this->m_pos));
	this->m_pos += sizeof(void*);
	return true;
}

bool AcBinary::readBinary(char*& _data, unsigned int& _len) {

	this->readUint32(_len);
	_data = new char[_len];
	for (int i = 0; i < _len; i++)
	{
		_data[i] = this->m_buff[this->m_pos++];
	}
	return true;
}

bool AcBinary::readData(char*& _data, unsigned int _len) {
	for (int i = 0; i < _len; i++)
	{
		_data[i] = this->m_buff[this->m_pos++];
	}
	return true;
}




Adesk::Boolean AcBinary::readBoolean(const char* _data, unsigned int _pos) {
	Adesk::Boolean val = *((bool*)(_data + _pos));
	return val;
}
Adesk::Int8 AcBinary::readInt8(const char* _data, unsigned int _pos) {
	Adesk::Int8 val = *((char*)(_data + _pos));
	return val;
}
Adesk::UInt8 AcBinary::readUint8(const char* _data, unsigned int _pos) {
	Adesk::UInt8 val = *((char*)(_data + _pos));
	return val;
}
Adesk::Int16 AcBinary::readInt16(const char* _data, unsigned int _pos) {
	Adesk::Int16 val = *((short*)(_data + _pos));
	return val;
}
Adesk::UInt16 AcBinary::readUint16(const char* _data, unsigned int _pos) {
	Adesk::UInt16 val = *((unsigned short*)(_data + _pos));
	return val;
}
Adesk::Int32 AcBinary::readInt32(const char* _data, unsigned int _pos) {
	Adesk::Int32 val = *((int*)(_data + _pos));
	return val;
}
Adesk::UInt32 AcBinary::readUint32(const char* _data, unsigned int _pos) {
	Adesk::UInt32 val = *((unsigned int*)(_data + _pos));
	return val;
}
Adesk::Int64 AcBinary::readInt64(const char* _data, unsigned int _pos) {
	Adesk::Int64 val = *((long long*)(_data + _pos));
	return val;
}
Adesk::UInt64 AcBinary::readUint64(const char* _data, unsigned int _pos) {
	Adesk::UInt64 val = *((unsigned long long*)(_data + _pos));
	return val;
}
float AcBinary::readFloat32(const char* _data, unsigned int _pos) {
	float val = *((float*)(_data + _pos));
	return val;
}
double AcBinary::readFloat64(const char* _data, unsigned int _pos) {
	double val = *((double*)(_data + _pos));
	return val;
}
std::string AcBinary::readString(const char* _data, unsigned int _pos) {
	unsigned int len = *((unsigned int*)(_data + _pos));
	char* temp = new char[len + 1];
	for (int i = 0; i < len; i++)
	{
		temp[i] = _data[_pos + 4 + i];
	}
	temp[len] = '\0';
	std::string str = temp;
	delete[] temp;
	return str;
}
void* AcBinary::readAddress(const char* _data, unsigned int _pos) {
	void* val = ((void*)(_data + _pos));
	return val;
}
