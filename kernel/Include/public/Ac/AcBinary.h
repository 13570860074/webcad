#ifndef ACBINARY_H
#define ACBINARY_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "string"
#include <stdarg.h>
#include <stdlib.h>
#include <locale.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <iomanip>
#include <cstdlib>
#include <codecvt>
#include "IAcBinary.h"


class AcBinary :public IAcBinary
{
public:
	AcBinary();
	virtual ~AcBinary();

	AcBinary& operator=(const AcBinary& _data);

	enum EType
	{
		kBoolean,
		kInt8,
		kUint8,
		kInt16,
		kUint16,
		kInt32,
		kUint32,
		kInt64,
		kUint64,
		kFloat32,
		kFloat64,
		kString,
		kAddress,
		kBinary,
		kData,
	};

private:
	void allocateMemory();

private:
	char* m_buff;
	unsigned int m_len;
	unsigned int m_pos;
	unsigned int m_size;

public:
	/** 读取数据 */
	static Adesk::Boolean readBoolean(const char* _data, unsigned int _pos = 0);
	static Adesk::Int8 readInt8(const char* _data, unsigned int _pos = 0);
	static Adesk::UInt8 readUint8(const char* _data, unsigned int _pos = 0);
	static Adesk::Int16 readInt16(const char* _data, unsigned int _pos = 0);
	static Adesk::UInt16 readUint16(const char* _data, unsigned int _pos = 0);
	static Adesk::Int32 readInt32(const char* _data, unsigned int _pos = 0);
	static Adesk::UInt32 readUint32(const char* _data, unsigned int _pos = 0);
	static Adesk::Int64 readInt64(const char* _data, unsigned int _pos = 0);
	static Adesk::UInt64 readUint64(const char* _data, unsigned int _pos = 0);
	static float readFloat32(const char* _data, unsigned int _pos = 0);
	static double readFloat64(const char* _data, unsigned int _pos = 0);
	static std::string readString(const char* _data, unsigned int _pos = 0);
	static void* readAddress(const char* _data, unsigned int _pos = 0);

public:
	// 获得buff数据
	const char* data();
	const char data(unsigned int _pos);
	const char* data(unsigned int _pos, unsigned int _len);

	// 载入数据
	void load(const char* data, unsigned int len);

	// 定位
	int locator(const char* data, unsigned int len);

	// 保存文件
	void saveAs(const AcString& fileName);

	// 加载文件
	void loadFile(const ACHAR fileName[]);

	// 获得位置
	unsigned int position();

	// 设置位置
	void setPosition(unsigned int _pos);

	// 获得数据长度
	unsigned int len();

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
	virtual void writeString(const AcString& _val);
	virtual void writeString(const ACHAR* _val, unsigned int _len);
	virtual void writeAddress(const void* p);
	virtual void writeBinary(char* _data, unsigned int _len);
	virtual void writeData(char* _data, unsigned int _len);

	// 读取数据
	virtual bool readBoolean(Adesk::Boolean& val);
	virtual bool readInt8(Adesk::Int8& val);
	virtual bool readUint8(Adesk::UInt8& val);
	virtual bool readInt16(Adesk::Int16& val);
	virtual bool readUint16(Adesk::UInt16& val);
	virtual bool readInt32(Adesk::Int32& val);
	virtual bool readUint32(Adesk::UInt32& val);
	virtual bool readInt64(Adesk::Int64& val);
	virtual bool readUint64(Adesk::UInt64& val);
	virtual bool readFloat32(float& val);
	virtual bool readFloat64(double& val);
	virtual bool readString(AcString& _val);
	virtual bool readString(ACHAR*& _val, unsigned int& _len);
	virtual bool readAddress(void*& val);
	virtual bool readBinary(char*& _data, unsigned int& _len);
	virtual bool readData(char*& _data, unsigned int _len);
};

#endif
