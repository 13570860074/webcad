#include "AcJsonObject.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "AcImpl.h"


static void read(AcJsonObject *object, rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstValueIterator _value);
static void read(AcJsonObject *object, rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstMemberIterator _value);

AcJsonObject::AcJsonObject(const Type _type)
{
	this->m_pImpl = new AcJsonObjectImpl();
	if (_type >= Null && _type <= String)
	{
		this->m_pImpl->type = _type;
	}
	else
	{
		this->m_pImpl->type = Null;
	}
}
AcJsonObject::~AcJsonObject()
{
	for (int i = 0; i < this->length(); i++)
	{
		AcJsonObject *temp = this->at(i);
		delete temp;
	}
	delete this->m_pImpl;
}

AcJsonObject &AcJsonObject::operator[](const char *str)
{

	for (int i = 0; i < this->length(); i++)
	{
		if (strcmp(this->at(i)->key().constPtr(), str) == 0)
		{
			return *this->at(i);
		}
	}

	AcJsonObject *object = new AcJsonObject();
	object->setKey(str);
	this->append(object);
	return *object;
}

AcJsonObject &AcJsonObject::operator=(const bool val)
{
	this->setType(AcJsonObject::Type::Bool);
	this->setBoolean(val);
	return *this;
}
AcJsonObject &AcJsonObject::operator=(const int val)
{
	this->setType(AcJsonObject::Type::Int32);
	this->setInt32(val);
	return *this;
}
AcJsonObject &AcJsonObject::operator=(const unsigned int val)
{
	this->setType(AcJsonObject::Type::Uint32);
	this->setUint32(val);
	return *this;
}
AcJsonObject &AcJsonObject::operator=(const long long val)
{
	this->setType(AcJsonObject::Type::Int64);
	this->setInt64(val);
	return *this;
}
AcJsonObject &AcJsonObject::operator=(const unsigned long long val)
{
	this->setType(AcJsonObject::Type::Uint64);
	this->setUint64(val);
	return *this;
}
AcJsonObject &AcJsonObject::operator=(const double val)
{
	this->setType(AcJsonObject::Type::Float64);
	this->setFloat64(val);
	return *this;
}
AcJsonObject &AcJsonObject::operator=(const AcString &val)
{
	this->setType(AcJsonObject::Type::String);
	this->setString(val);
	return *this;
}
AcJsonObject &AcJsonObject::operator=(const AcArray<int> val)
{
	this->setType(AcJsonObject::Type::Array);
	for (int i = 0; i < val.length(); i++)
	{
		this->append(val.at(i));
	}
	return *this;
}
AcJsonObject &AcJsonObject::operator=(const AcArray<double> val)
{
	this->setType(AcJsonObject::Type::Array);
	for (int i = 0; i < val.length(); i++)
	{
		this->append(val.at(i));
	}
	return *this;
}
AcJsonObject &AcJsonObject::operator=(const AcArray<AcString> val)
{
	this->setType(AcJsonObject::Type::Array);
	for (int i = 0; i < val.length(); i++)
	{
		this->append(val.at(i));
	}
	return *this;
}
AcJsonObject &AcJsonObject::operator=(const AcArray<AcJsonObject> &val)
{
	this->setType(AcJsonObject::Type::Array);
	for (int i = 0; i < val.length(); i++)
	{
		this->append(&val[i]);
	}
	return *this;
}

AcJsonObject::Type AcJsonObject::type() const
{
	return this->m_pImpl->type;
}
void AcJsonObject::setType(const Type _Type)
{
	this->m_pImpl->type = _Type;
}


void read(AcJsonObject *object, rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstMemberIterator _value)
{
	if (_value->value.IsBool() == true)
	{
		object->insert(_value->name.GetString(), _value->value.GetBool());
	}
	else if (_value->value.IsUint64() == true)
	{
		object->insert(_value->name.GetString(), (long long)_value->value.GetUint64());
	}
	else if (_value->value.IsInt64() == true)
	{
		object->insert(_value->name.GetString(), (unsigned long long)_value->value.GetInt64());
	}
	else if (_value->value.IsUint() == true)
	{
		object->insert(_value->name.GetString(), _value->value.GetUint());
	}
	else if (_value->value.IsInt() == true)
	{
		object->insert(_value->name.GetString(), _value->value.GetInt());
	}
	else if (_value->value.IsFloat() == true)
	{
		object->insert(_value->name.GetString(), _value->value.GetFloat());
	}
	else if (_value->value.IsDouble() == true)
	{
		object->insert(_value->name.GetString(), _value->value.GetDouble());
	}
	else if (_value->value.IsString() == true)
	{
		object->insert(_value->name.GetString(), AcString(_value->value.GetString()));
	}
	else if (_value->value.IsObject() == true)
	{
		AcJsonObject *pDatajson = new AcJsonObject();
		pDatajson->setType(AcJsonObject::Type::Object);
		pDatajson->setKey(_value->name.GetString());

		for (rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstMemberIterator children = _value->value.MemberBegin(); children != _value->value.MemberEnd(); ++children)
		{
			read(pDatajson, children);
		}

		object->append(pDatajson);
	}
	else if (_value->value.IsArray() == true)
	{
		AcJsonObject *pDatajson = new AcJsonObject();
		pDatajson->setType(AcJsonObject::Type::Array);
		pDatajson->setKey(_value->name.GetString());

		for (rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstValueIterator children = _value->value.Begin(); children != _value->value.End(); ++children)
		{
			read(pDatajson, children);
		}

		object->append(pDatajson);
	}
}
void read(AcJsonObject *object, rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstValueIterator _value)
{
	do
	{
		if (_value->IsBool() == true)
		{
			object->insert("", _value->GetBool());
		}
		else if (_value->IsInt() == true)
		{
			object->insert("", _value->GetInt());
		}
		else if (_value->IsUint() == true)
		{
			object->insert("", _value->GetUint());
		}
		else if (_value->IsInt64() == true)
		{
			object->insert("", (long long)_value->GetInt64());
		}
		else if (_value->IsUint64() == true)
		{
			object->insert("", (unsigned long long)_value->GetUint64());
		}
		else if (_value->IsFloat() == true)
		{
			object->insert("", _value->GetFloat());
		}
		else if (_value->IsDouble() == true)
		{
			object->insert("", _value->GetDouble());
		}
		else if (_value->IsString() == true)
		{
			object->insert("", AcString(_value->GetString()));
		}
		else if (_value->IsObject() == true)
		{
			AcJsonObject *tempObject = new AcJsonObject();
			tempObject->setType(AcJsonObject::Type::Object);

			for (rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstMemberIterator children = _value->MemberBegin(); children != _value->MemberEnd(); ++children)
			{
				read(tempObject, children);
			}

			object->append(tempObject);
		}
		else if (_value->IsArray() == true)
		{
			AcJsonObject *tempObject = new AcJsonObject();
			tempObject->setType(AcJsonObject::Type::Array);

			for (rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstValueIterator children = _value->Begin(); children != _value->End(); ++children)
			{
				read(tempObject, children);
			}

			object->append(tempObject);
		}
	} while (false);
}

static void writerKey(rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<char>>> *_data, const char *_key)
{
	_data->Key(_key);
}

static void writerKey(rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<char>>> *_data, const std::string _key)
{
	_data->Key(_key.c_str());
}


static void writerString(rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<char>>> *_data, const char *_str)
{
	_data->String(_str);
}

static void writerString(rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<char>>> *_data, const std::string _str)
{
	_data->String(_str.c_str());
}

static void writer(AcJsonObject *object, rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<char>>> *_data)
{
	do
	{
		if (object->isBoolean() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			_data->Bool(object->toBool());
		}
		else if (object->isInt8() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			_data->Int(object->toInt8());
		}
		else if (object->isUint8() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			_data->Int(object->toUint8());
		}
		else if (object->isInt16() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			_data->Int(object->toInt16());
		}
		else if (object->isUint16() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			_data->Uint(object->toUint16());
		}
		else if (object->isInt32() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			_data->Int(object->toInt32());
		}
		else if (object->isUint32() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			_data->Uint(object->toUint32());
		}
		else if (object->isInt64() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			_data->Int64(object->toInt64());
		}
		else if (object->isUint64() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			_data->Uint64(object->toUint64());
		}
		else if (object->isFloat32() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			_data->Double(object->toFloat32());
		}
		else if (object->isFloat64() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			_data->Double(object->toFloat64());
		}
		else if (object->isString() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}
			writerString(_data, object->toString().string());
		}
		else if (object->isObject() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}

			_data->StartObject();
			for (int i = 0; i < object->length(); i++)
			{
				writer(object->at(i), _data);
			}
			_data->EndObject();
		}
		else if (object->isArray() == true)
		{
			if (object->key().length() > 0)
			{
				writerKey(_data, object->key().string());
			}

			_data->StartArray();
			for (int i = 0; i < object->length(); i++)
			{
				writer(object->at(i), _data);
			}
			_data->EndArray();
		}

	} while (false);
}

std::string AcJsonObject::stringify(AcJsonObject *_data)
{

	rapidjson::GenericStringBuffer<rapidjson::UTF8<char>> StrJsonBuff;
	rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<char>>> *pJson = new rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<char>>>(StrJsonBuff);

	do
	{
		if (_data->type() == AcJsonObject::Type::Object)
		{
			pJson->StartObject();

			for (int i = 0; i < _data->length(); i++)
			{
				writer(_data->at(i), pJson);
			}

			pJson->EndObject();
		}
		else if (_data->type() == Type::Array)
		{
			pJson->StartArray();

			for (int i = 0; i < _data->length(); i++)
			{
				writer(_data->at(i), pJson);
			}

			pJson->EndArray();
		}
	} while (false);

	const char *pString = StrJsonBuff.GetString();
	delete pJson;

	return pString;
}

// 解析json数据
AcJsonObject *AcJsonObject::parse(const AcString &_data)
{
	return AcJsonObject::parse(_data.constPtr());
}
AcJsonObject *AcJsonObject::parse(const char *_data)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->isEmpty();

	// 解析Json
	rapidjson::GenericDocument<rapidjson::UTF8<char>> *pDocument = new rapidjson::GenericDocument<rapidjson::UTF8<char>>();
	pDocument->Parse(_data);

	// 解析对象
	if (pDocument->IsObject() == true)
	{
		pObject->setType(Type::Object);
		pObject->setKey("");

		for (rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstMemberIterator SubValue = pDocument->MemberBegin(); SubValue != pDocument->MemberEnd(); ++SubValue)
		{
			read(pObject, SubValue);
		}
	}
	else if (pDocument->IsArray() == true)
	{
		pObject->setType(Type::Array);
		pObject->setKey("");

		for (rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstValueIterator SubValue = pDocument->Begin(); SubValue != pDocument->End(); ++SubValue)
		{
			read(pObject, SubValue);
		}
	}

	// 释放内存
	delete pDocument;

	return pObject;
}

int AcJsonObject::length() const
{
	return AcArray<AcJsonObject *>::length();
}

void AcJsonObject::append(AcJsonObject _pObject)
{
	AcArray<AcJsonObject *>::append(&_pObject);
}
void AcJsonObject::append(AcJsonObject *_pObject)
{
	AcArray<AcJsonObject *>::append(_pObject);
}
void AcJsonObject::append(const bool _value)
{
	if (this->type() == Type::Array)
	{
		AcJsonObject *pObject = new AcJsonObject(Type::Bool);
		pObject->setBoolean(_value);
		AcArray<AcJsonObject *>::append(pObject);
	}
}
void AcJsonObject::append(const int _value)
{
	if (this->type() == Type::Array)
	{
		AcJsonObject *pObject = new AcJsonObject(Type::Int32);
		pObject->setInt32(_value);
		AcArray<AcJsonObject *>::append(pObject);
	}
}
void AcJsonObject::append(const unsigned int _value)
{
	if (this->type() == Type::Array)
	{
		AcJsonObject *pObject = new AcJsonObject(Type::Uint32);
		pObject->setUint32(_value);
		AcArray<AcJsonObject *>::append(pObject);
	}
}
void AcJsonObject::append(const long long _value)
{
	if (this->type() == Type::Array)
	{
		AcJsonObject *pObject = new AcJsonObject(Type::Int64);
		pObject->setInt64(_value);
		AcArray<AcJsonObject *>::append(pObject);
	}
}
void AcJsonObject::append(const unsigned long long _value)
{
	if (this->type() == Type::Array)
	{
		AcJsonObject *pObject = new AcJsonObject(Type::Uint64);
		pObject->setUint64(_value);
		AcArray<AcJsonObject *>::append(pObject);
	}
}
void AcJsonObject::append(const double _value)
{
	if (this->type() == Type::Array)
	{
		AcJsonObject *pObject = new AcJsonObject();
		pObject->setType(AcJsonObject::Float64);
		pObject->setFloat64(_value);
		AcArray<AcJsonObject *>::append(pObject);
	}
}
void AcJsonObject::append(const AcString &_value)
{
	if (this->type() == Type::Array)
	{
		AcJsonObject *pObject = new AcJsonObject();
		pObject->setType(AcJsonObject::String);
		pObject->setString(_value);
		AcArray<AcJsonObject *>::append(pObject);
	}
}

void AcJsonObject::insert(const AcString &_key, const bool _value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Bool);
	pObject->setBoolean(_value);
	this->insert(_key, pObject);
}
void AcJsonObject::insert(const AcString &_key, const int _value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Int32);
	pObject->setInt32(_value);
	this->insert(_key, pObject);
}
void AcJsonObject::insert(const AcString &_key, const unsigned int _value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Uint32);
	pObject->setInt64(_value);
	this->insert(_key, pObject);
}
void AcJsonObject::insert(const AcString &_key, const long long _value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Int64);
	pObject->setInt64(_value);
	this->insert(_key, pObject);
}
void AcJsonObject::insert(const AcString &_key, const unsigned long long _value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Uint64);
	pObject->setUint64(_value);
	this->insert(_key, pObject);
}
void AcJsonObject::insert(const AcString &_key, const double _value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Float64);
	pObject->setFloat64(_value);
	this->insert(_key, pObject);
}
void AcJsonObject::insert(const AcString &_key, const char *_value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::String);
	pObject->setString(_value);
	this->insert(_key, pObject);
}
void AcJsonObject::insert(const AcString &_key, const  AcString &_value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::String);
	pObject->setString(_value);
	this->insert(_key, pObject);
}
void AcJsonObject::insert(const AcString &_key, AcJsonObject *_value)
{
	_value->setKey(_key);
	this->append(_value);
}
void AcJsonObject::insert(const AcString &_key, const AcArray<int> _value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Array);
	pObject->setKey(_key);
	for (int i = 0; i < _value.length(); i++)
	{
		pObject->append(_value.at(i));
	}
	this->append(pObject);
}
void AcJsonObject::insert(const AcString &_key, const AcArray<double> _value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Array);
	pObject->setKey(_key);
	for (int i = 0; i < _value.length(); i++)
	{
		pObject->append(_value.at(i));
	}
	this->append(pObject);
}
void AcJsonObject::insert(const AcString &_key, const AcArray<unsigned int> _value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Array);
	pObject->setKey(_key);
	for (int i = 0; i < _value.length(); i++)
	{
		pObject->append(_value.at(i));
	}
	this->append(pObject);
}
void AcJsonObject::insert(const AcString &_key, const AcArray<AcString> _value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Array);
	pObject->setKey(_key);
	for (int i = 0; i < _value.length(); i++)
	{
		pObject->append(_value.at(i));
	}
	this->append(pObject);
}
void AcJsonObject::insert(const AcString& _key, const AcArray<AcJsonObject*>& _value){
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Array);
	pObject->setKey(_key);
	for (int i = 0; i < _value.length(); i++)
	{
		pObject->append(_value[i]);
	}
	this->append(pObject);
}
void AcJsonObject::insert(const AcString &_key, const AcArray<AcJsonObject> &_value)
{
	AcJsonObject *pObject = new AcJsonObject();
	pObject->setType(AcJsonObject::Array);
	pObject->setKey(_key);
	for (int i = 0; i < _value.length(); i++)
	{
		pObject->append(&_value[i]);
	}
	this->append(pObject);
}

AcString &AcJsonObject::key()const
{
	return this->m_pImpl->key;
}

void AcJsonObject::setKey(const AcString &_Key)
{
	this->m_pImpl->key = _Key;
}

AcJsonObject *AcJsonObject::get(const AcString &_key)const
{
	for (int i = 0; i < this->length(); i++)
	{
		if (this->at(i)->key() == _key)
		{
			return this->at(i);
		}
	}
	return NULL;
}

bool AcJsonObject::isEmpty(const AcString &_Key)const
{
	for (int i = 0; i < this->length(); i++)
	{
		if (this->at(i)->key() == _Key)
		{
			return false;
		}
	}
	return true;
}
bool AcJsonObject::isEmpty()const
{
	if (this->type() == Type::Null)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isBoolean()const
{
	if (this->type() == Type::Bool)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isInt8()const
{
	if (this->type() == Type::Int8)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isUint8()const
{
	if (this->type() == Type::Uint8)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isInt16()const
{
	if (this->type() == Type::Int16)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isUint16()const
{
	if (this->type() == Type::Uint16)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isInt32()const
{
	if (this->type() == Type::Int32)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isUint32()const
{
	if (this->type() == Type::Uint32)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isInt64()const
{
	if (this->type() == Type::Int64)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isUint64()const
{
	if (this->type() == Type::Uint64)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isFloat32()const
{
	if (this->type() == Type::Float32)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isFloat64()const
{
	if (this->type() == Type::Float64)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isString()const
{
	if (this->type() == Type::String)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isObject()const
{
	if (this->type() == Type::Object)
	{
		return true;
	}
	return false;
}
bool AcJsonObject::isArray()const
{
	if (this->type() == Type::Array)
	{
		return true;
	}
	return false;
}

bool AcJsonObject::toBool()const
{
	return this->m_pImpl->integer;
}
char AcJsonObject::toInt8()const
{
	return this->m_pImpl->integer;
}
char AcJsonObject::toUint8()const
{
	return this->m_pImpl->integer;
}
short AcJsonObject::toInt16()const
{
	return this->m_pImpl->integer;
}
unsigned short AcJsonObject::toUint16()const
{
	return this->m_pImpl->integer;
}
int AcJsonObject::toInt32()const
{
	return this->m_pImpl->integer;
}
unsigned int AcJsonObject::toUint32()const
{
	return this->m_pImpl->integer;
}
long long AcJsonObject::toInt64()const
{
	return this->m_pImpl->integer;
}
unsigned long long AcJsonObject::toUint64()const
{
	return this->m_pImpl->integer;
}
float AcJsonObject::toFloat32()const
{
	if (this->m_pImpl->type == Type::Int8 ||
		this->m_pImpl->type == Type::Int16 ||
		this->m_pImpl->type == Type::Int32 ||
		this->m_pImpl->type == Type::Int64 ||
		this->m_pImpl->type == Type::Uint8 ||
		this->m_pImpl->type == Type::Uint16 ||
		this->m_pImpl->type == Type::Uint32 ||
		this->m_pImpl->type == Type::Uint64) {
		return (double)this->m_pImpl->integer;
	}
	return this->m_pImpl->real;
}
double AcJsonObject::toFloat64()const
{
	if (this->m_pImpl->type == Type::Int8 ||
		this->m_pImpl->type == Type::Int16 ||
		this->m_pImpl->type == Type::Int32 || 
		this->m_pImpl->type == Type::Int64 || 
		this->m_pImpl->type == Type::Uint8 ||
		this->m_pImpl->type == Type::Uint16 ||
		this->m_pImpl->type == Type::Uint32 || 
		this->m_pImpl->type == Type::Uint64) {
		return (double)this->m_pImpl->integer;
	}
	return this->m_pImpl->real;
}
AcString &AcJsonObject::toString()const
{
	return this->m_pImpl->string;
}

void AcJsonObject::setEmpty()
{
	this->setType(Type::Null);
}
void AcJsonObject::setBoolean(const bool _Boolean)
{
	this->m_pImpl->integer = _Boolean;
}
void AcJsonObject::setInt8(const char _Int8)
{
	this->m_pImpl->integer = _Int8;
}
void AcJsonObject::setUint8(const unsigned char _Uint8)
{
	this->m_pImpl->integer = _Uint8;
}
void AcJsonObject::setInt16(const short _Int16)
{
	this->m_pImpl->integer = _Int16;
}
void AcJsonObject::setUint16(const unsigned short _Uint16)
{
	this->m_pImpl->integer = _Uint16;
}
void AcJsonObject::setInt32(const int _Int32)
{
	this->m_pImpl->integer = _Int32;
}
void AcJsonObject::setUint32(const unsigned int _Uint32)
{
	this->m_pImpl->integer = _Uint32;
}
void AcJsonObject::setInt64(const long long _Int64)
{
	this->m_pImpl->integer = _Int64;
}
void AcJsonObject::setUint64(const unsigned long long _Uint64)
{
	this->m_pImpl->integer = _Uint64;
}
void AcJsonObject::setFloat32(const float v)
{
	this->m_pImpl->real = v;
}
void AcJsonObject::setFloat64(const double v)
{
	this->m_pImpl->real = v;
}
void AcJsonObject::setString(const AcString &_String)
{
	this->m_pImpl->string = _String;
}
