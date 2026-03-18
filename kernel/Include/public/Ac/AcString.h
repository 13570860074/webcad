#ifndef STRING_H
#define STRING_H


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
#include <sstream>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#if WIN32
#include <windows.h>
#include <Windows.h>
#endif


#define __T(x)      L ## x
#define _T(x)       __T(x)
#define _TEXT(x)    __T(x)

#define STRING_MACRO(str) #str



typedef char ACHAR;

class AcString
{
public:
	AcString();
	AcString(const ACHAR* str);
	AcString(const std::string str);
	AcString(const AcString& str);
	~AcString();

	enum Encoding {
		Utf8
	};

private:
	std::string buff;

public:

	ACHAR& operator [] (unsigned int i) {
		return this->buff.at(i);
	}

	void set(const std::string str);
	const std::string get();

	AcString& operator = (const ACHAR* str) {
		this->buff = str;
		return *this;
	}
	AcString& operator = (const std::string& str) {
		this->buff = str;
		return *this;
	}
	AcString& operator = (const AcString& str) {
		this->buff = str.buff;
		return *this;
	}

	AcString operator+(const ACHAR* str) const {
		AcString data;
		data.append(this->buff);
		data.append(str);
		return data;
	}
	AcString operator+(const std::string& str) const {
		AcString data;
		data.append(this->buff);
		data.append(str);
		return data;
	}
	AcString operator+(const AcString& str) const {
		AcString data;
		data.append(this->buff);
		data.append(str);
		return data;
	}

	AcString& operator += (const ACHAR byte) {
		this->append(byte);
		return *this;
	}
	AcString& operator += (const ACHAR* str) {
		this->append(str);
		return *this;
	}
	AcString& operator += (const std::string& str) {
		this->append(str);
		return *this;
	}
	AcString& operator += (const AcString& str) {
		this->append(str.buff);
		return *this;
	}

	bool operator == (const ACHAR* _str)const {
		if (strcmp(this->buff.c_str(), _str) == 0)
		{
			return true;
		}
		return false;
	}
	bool operator == (const std::string& _str) const {
		if (strcmp(this->buff.c_str(), _str.c_str()) == 0)
		{
			return true;
		}
		return false;
	}
	bool operator == (const AcString& _str)const {
		if (strcmp(this->buff.c_str(), _str.buff.c_str()) == 0)
		{
			return true;
		}
		return false;
	}

	bool operator != (const ACHAR* _str)const {
		return !(*this == _str);
	}
	bool operator != (const std::string& _str)const {
		return !(*this == _str);
	}
	bool operator != (const AcString& _str)const {
		return !(*this == _str);
	}

	static std::string ptr_char_to_string(unsigned int _p);
	static std::string ptr_string_to_string(unsigned int _p);
	static std::string ptr_AcString_to_string(unsigned int _p);

	/** 宽字符转多字节 */
	static void wstring_to_string(const wchar_t* wchSrc, char*& chDest);
	static std::string wstring_to_string(const std::wstring& ws);

	/** 多字节转宽字符 */
	static void string_to_wstring(const char* chSrc, wchar_t*& wchDest);
	static std::wstring string_to_wstring(const std::string& s);

	/** utf8转utf16 */
	static std::wstring utf8_to_utf16(const std::string& str);

	/** utf16转utf8 */
	static std::string utf16_to_utf8(const wchar_t str);
	static std::string utf16_to_utf8(const wchar_t* str);
	static std::string utf16_to_utf8(const std::wstring& str);
	static void utf16_to_utf8(const wchar_t* str, char*& output, unsigned int& len);

	/** 判断是否相等 */
	static bool isEqual(const char* _str1, const char* _str2, bool _isIgnoreCase = false);
	static bool isEqual(const std::wstring& _str1, const std::wstring& _str2, bool _isIgnoreCase = false);
	static bool isEqual(const AcString& _str1, const AcString& _str2, bool _isIgnoreCase = false);

	/** 字符串转int32 */
	static int toInt32(const char* buf);
	static int toInt32(const wchar_t* buf);
	static int toInt32(const AcString& buf);
	static bool toInt32(const char* buf, int& val);
	static bool toInt32(const wchar_t* buf, int& val);
	static bool toInt32(const AcString& buf, int& val);

	/** 字符串转UInt32 */
	static unsigned int toUint32(const char* buf);
	static unsigned int toUint32(const wchar_t* buf);
	static unsigned int toUint32(const AcString& buf);
	static bool toUint32(const char* buf, unsigned int& val);
	static bool toUint32(const wchar_t* buf, unsigned int& val);
	static bool toUint32(const AcString& buf, unsigned int& val);

	/** 字符串转int64 */
	static long long toInt64(const char* buf);
	static long long toInt64(const wchar_t* buf);
	static long long toInt64(const AcString& buf);
	static bool toInt64(const char* buf, long long& val);
	static bool toInt64(const wchar_t* buf, long long& val);
	static bool toInt64(const AcString& buf, long long& val);

	/** 字符串转UInt64 */
	static unsigned long long toUint64(const char* buf);
	static unsigned long long toUint64(const wchar_t* buf);
	static unsigned long long toUint64(const AcString& buf);
	static bool toUint64(const char* buf, unsigned long long& val);
	static bool toUint64(const wchar_t* buf, unsigned long long& val);
	static bool toUint64(const AcString& buf, unsigned long long& val);

	/** 字符串转Float32 */
	static float toFloat32(const char* buf);
	static float toFloat32(const wchar_t* buf);
	static float toFloat32(const AcString& buf);
	static bool toFloat32(const char* buf, float& val);
	static bool toFloat32(const wchar_t* buf, float& val);
	static bool toFloat32(const AcString& buf, float& val);

	/** 字符串转Float64 */
	static double toFloat64(const char* buf);
	static double toFloat64(const wchar_t* buf);
	static double toFloat64(const AcString& buf);
	static bool toFloat64(const char* buf, double& val);
	static bool toFloat64(const wchar_t* buf, double& val);
	static bool toFloat64(const AcString& buf, double& val);

	/** 格式化 */
	static std::string _format(const ACHAR* args, va_list list);
	static std::string _format(const ACHAR args[], ...);

	/** 转为大写 */
	static void toUpperCase(ACHAR*& str);

	/** 转为小写 */
	static void toLowerCase(ACHAR*& str);

public:

	/* 清理 */
	void clear();

	/* 获得长度 */
	unsigned int length() const;
	unsigned int tcharLength() const;

	/* 判断是否为空 */
	bool isEmpty() const;

	/* 容量 */
	unsigned int capacity() const;

	/* 获得一个字符 */
	const char& at(unsigned int i)const;

	/* 查找 */
	int find(ACHAR ch) const;
	int find(ACHAR ch, int nStartPos) const;
	int find(const ACHAR* psz) const;
	int find(const ACHAR* psz, int nStartPos) const;
	int find(const AcString& acs) const;

	/* 判断 */
	bool isEqual(const char* _buff, bool _isIgnoreCase = false)const;

	/* 追加字符 */
	void append(const ACHAR byte);
	void append(const std::string& str);
	void append(const ACHAR* str);
	void append(const AcString& str);

	/* 到utf8指针 */
	const char* utf8Ptr() const;

	/* 转char */
	const std::string string() const;

	/* 到char */
	const ACHAR* constPtr() const;
	const ACHAR* kACharPtr() const;

	operator const ACHAR* () const;

	int find_last_of(const ACHAR*val) const;
	int find_last_of(const ACHAR val) const;

	AcString substr(int _index)const;

	/* 转大写 */
	AcString makeUpper()const;

	/* 转为小写 */
	AcString makeLower()const;

	/* 格式化 */
	AcString& format(const ACHAR args[], ...);

	/* 截取字符串 */
	AcString slice(int start) const;
	AcString slice(int start, int end) const;
};





#endif





