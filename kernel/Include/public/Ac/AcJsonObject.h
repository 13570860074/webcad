#ifndef AcJsonObject_H
#define AcJsonObject_H


#include "AcArray.h"
#include "AcString.h"


class AcJsonObjectImpl;
class AcJsonObject :public AcArray<AcJsonObject*>
{
public:
	enum Type
	{
		Null,
		Object,
		Array,
		Bool,
		Int8,
		Uint8,
		Int16,
		Uint16,
		Int32,
		Uint32,
		Int64,
		Uint64,
		Float32,
		Float64,
		String,
	};

	enum ECode {
		Utf8,
		Unicode,
	};


public:
	AcJsonObject(Type _type = Type::Object);
	virtual ~AcJsonObject();

	AcJsonObject& operator [] (const char* str);

	AcJsonObject& operator = (const bool val);
	AcJsonObject& operator = (const int val);
	AcJsonObject& operator = (const unsigned int val);
	AcJsonObject& operator = (const long long val);
	AcJsonObject& operator = (const unsigned long long val);
	AcJsonObject& operator = (const double val);
	AcJsonObject& operator = (const AcString& val);
	AcJsonObject& operator = (const AcArray<int> val);
	AcJsonObject& operator = (const AcArray<double> val);
	AcJsonObject& operator = (const AcArray<AcString> val);
	AcJsonObject& operator = (const AcArray<AcJsonObject>& val);

private:
	AcJsonObjectImpl* m_pImpl;


public:
	/** 转字符串 */
	static std::string stringify(AcJsonObject* _data);

	/** 解析json数据 */
	static AcJsonObject* parse(const AcString& _data);
	static AcJsonObject* parse(const char* _data);

public:

	/** 获得数据类型 */
	Type type() const;

	/** 设置数据类型 */
	void setType(const Type _Type);

	/** 获得长度 */
	int length() const;

	/** 追加对象 */
	void append(AcJsonObject _pObject);
	void append(AcJsonObject* _pObject);
	void append(const bool _value);
	void append(const int _value);
	void append(const unsigned int _value);
	void append(const long long _value);
	void append(const unsigned long long _value);
	void append(const double _value);
	void append(const AcString& _value);

	/** 插入对象 */
	void insert(const AcString& _key, const bool _value);
	void insert(const AcString& _key, const int _value);
	void insert(const AcString& _key, const unsigned int _value);
	void insert(const AcString& _key, const long long _value);
	void insert(const AcString& _key, const unsigned long long _value);
	void insert(const AcString& _key, const double _value);
	void insert(const AcString& _key, const char* _value);
	void insert(const AcString& _key, const AcString& _value);
	void insert(const AcString& _key, AcJsonObject* _value);
	void insert(const AcString& _key, const AcArray<int> _value);
	void insert(const AcString& _key, const AcArray<double> _value);
	void insert(const AcString& _key, const AcArray<unsigned int> _value);
	void insert(const AcString& _key, const AcArray<AcString> _value);
	void insert(const AcString& _key, const AcArray<AcJsonObject*>& _value);
	void insert(const AcString& _key, const AcArray<AcJsonObject>& _value);

	/** 获得key */
	AcString& key() const;

	/** 设置key */
	void setKey(const AcString& _Key);

	/** 通过key获得对象 */
	AcJsonObject* get(const AcString& _key) const;

	/** 判断 */
	bool isEmpty(const AcString& _Key) const;
	bool isEmpty() const;
	bool isBoolean() const;
	bool isInt8() const;
	bool isUint8() const;
	bool isInt16() const;
	bool isUint16() const;
	bool isInt32() const;
	bool isUint32() const;
	bool isInt64() const;
	bool isUint64() const;
	bool isFloat32() const;
	bool isFloat64() const;
	bool isString() const;
	bool isObject() const;
	bool isArray() const;

	/** 转换 */
	bool toBool() const;
	char toInt8() const;
	char toUint8() const;
	short toInt16() const;
	unsigned short toUint16() const;
	int toInt32() const;
	unsigned int toUint32() const;
	long long toInt64() const;
	unsigned long long toUint64() const;
	float toFloat32() const;
	double toFloat64() const;
	AcString& toString() const;

	/** 设置 */
	void setEmpty();
	void setBoolean(const bool v);
	void setInt8(const char v);
	void setUint8(const unsigned char v);
	void setInt16(const short v);
	void setUint16(const unsigned short v);
	void setInt32(const int v);
	void setUint32(const unsigned int v);
	void setInt64(const long long v);
	void setUint64(const unsigned long long v);
	void setFloat32(const float v);
	void setFloat64(const double v);
	void setString(const AcString& v);
};




#endif





