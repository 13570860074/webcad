#include "AcString.h"


AcString::AcString() {}
AcString::AcString(const ACHAR *str)
{
	this->buff = str;
}
AcString::AcString(const std::string str)
{
	this->buff = str;
}
AcString::AcString(const AcString &str)
{
	this->buff = str.buff;
}
AcString::~AcString()
{
}

void AcString::set(const std::string str)
{
	this->buff = str;
}
const std::string AcString::get()
{
	return this->buff;
}

std::string AcString::_format(const char* args, va_list list)
{

	std::basic_ostringstream<char, std::char_traits<char>, std::allocator<char>> ostr;
	ostr.precision(9);

	int len = strlen(args);

	std::string str;
	for (int i = 0; i < len; i++)
	{
		if (args[i] == '%')
		{
			// 判断是否是%d,%s,%f,%c...等
			if (i + 1 >= len)
			{
				continue;
			}

			// 判断是否是%d
			if (args[i + 1] == 'd')
			{
				if (str.length() > 0)
				{
					ostr << str;
					str.clear();
				}

				ostr << va_arg(list, int);
				i++;
			}
			// 判断是否是%f
			else if (args[i + 1] == 'f')
			{
				if (str.length() > 0)
				{
					ostr << str;
					str.clear();
				}

				ostr << va_arg(list, double);
				i++;
			}
			// 判断是否是%s
			else if (args[i + 1] == 's')
			{
				if (str.length() > 0)
				{
					ostr << str;
					str.clear();
				}

				ostr << va_arg(list, const char*);
				i++;
			}
			// 判断是否是%c
			else if (args[i + 1] == 'c')
			{
				if (str.length() > 0)
				{
					ostr << str;
					str.clear();
				}

				ostr << va_arg(list, char);
				i++;
			}
			// 判断是否是%%
			else if (args[i + 1] == '%')
			{
				if (str.length() > 0)
				{
					ostr << str;
					str.clear();
				}

				ostr << "%";
				i++;
			}
			// 判断是否是%g
			else if (args[i + 1] == 'g')
			{
				if (str.length() > 0)
				{
					ostr << str;
					str.clear();
				}

				ostr << va_arg(list, double);
				i++;
			}
			// 判断是否是%i
			else if (args[i + 1] == 'i')
			{
				if (str.length() > 0)
				{
					ostr << str;
					str.clear();
				}

				ostr << va_arg(list, int);
				i++;
			}

			// 判断是否是%.f,%ld,%ud...等
			if (i + 2 >= len)
			{
				continue;
			}

			// 判断是否是%ld
			if (args[i + 1] == 'l' && args[i + 2] == 'd')
			{
				if (str.length() > 0)
				{
					ostr << str;
					str.clear();
				}

				ostr << va_arg(list, int);
				i = i + 2;
			}
			else if (args[i + 1] == 'u' && args[i + 2] == 'd')
			{
				if (str.length() > 0)
				{
					ostr << str;
					str.clear();
				}

				ostr << va_arg(list, unsigned int);
				i = i + 2;
			}

			// 判断是否是%.xf,%lld...等
			if (i + 3 >= len)
			{
				continue;
			}

			// 判断是否是%.xf
			if (args[i + 1] == '.' && args[i + 2] > 47 && args[i + 2] < 58 && args[i + 3] == 'f')
			{
				if (str.length() > 0)
				{
					ostr << str;
					str.clear();
				}

				ostr.precision(args[i + 2] - 48 + 1);
				ostr << std::fixed << std::setprecision(2) << va_arg(list, double);
				ostr.precision(9);
				i = i + 3;
			}
			// 判断是否是%lld
			else if (args[i + 1] == 'l' && args[i + 2] == 'l' && args[i + 3] == 'd')
			{
				if (str.length() > 0)
				{
					ostr << str;
					str.clear();
				}

				ostr << va_arg(list, long long);
				i = i + 3;
			}
		}
		else
		{
			str += args[i];
		}
	}

	if (str.length() > 0)
	{
		ostr << str;
	}

	return ostr.str();
}
std::string AcString::_format(const char args[], ...)
{
	va_list list;
	va_start(list, args);
	std::string str = _format(args, list);
	va_end(list);
	return str;
}

std::string AcString::ptr_char_to_string(unsigned int _p)
{
	const char *data = (const char *)_p;
	return std::string(data);
}
std::string AcString::ptr_string_to_string(unsigned int _p)
{

	std::string data = (std::string &)_p;
	return data;
}
std::string AcString::ptr_AcString_to_string(unsigned int _p)
{
	AcString data = (AcString &)_p;
	return data.string();
}

void AcString::wstring_to_string(const wchar_t *wchSrc, char *&chDest)
{
	std::string strLocale = setlocale(LC_ALL, "");
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	setlocale(LC_ALL, strLocale.c_str());
}
std::string AcString::wstring_to_string(const std::wstring &ws)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t *wchSrc = ws.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char *chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[] chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}

void AcString::string_to_wstring(const char *chSrc, wchar_t *&wchDest)
{
	std::string strLocale = setlocale(LC_ALL, "");
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	setlocale(LC_ALL, strLocale.c_str());
}
std::wstring AcString::string_to_wstring(const std::string &s)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char *chSrc = s.c_str();
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchar_t *wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[] wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}

std::wstring AcString::utf8_to_utf16(const std::string &str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}
std::string AcString::utf16_to_utf8(const wchar_t str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(str);
}
std::string AcString::utf16_to_utf8(const wchar_t* str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(str);
}
std::string AcString::utf16_to_utf8(const std::wstring &str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(str);
}
void AcString::utf16_to_utf8(const wchar_t* str, char*& output, unsigned int& len) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string content = converter.to_bytes(str);
	len = content.length();
	output = new char[len];
	memcpy(output, content.c_str(), len);
}
bool AcString::isEqual(const char* _str1, const char* _str2, bool _isIgnoreCase)
{
	if (_isIgnoreCase == false)
	{
		if (strcmp(_str1, _str2) == 0)
		{
			return true;
		}
	}
	else
	{
		std::string s1 = _str1;
		std::string s2 = _str2;
		std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
		std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
		if (strcmp(s1.c_str(), s2.c_str()) == 0)
		{
			return true;
		}
	}
	return false;
}
bool AcString::isEqual(const std::wstring& _str1, const std::wstring& _str2, bool _isIgnoreCase)
{
	if (_isIgnoreCase == false)
	{
		if (wcscmp(_str1.c_str(), _str2.c_str()) == 0)
		{
			return true;
		}
	}
	else
	{
		std::wstring s1 = _str1;
		std::wstring s2 = _str2;
		std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
		std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
		if (wcscmp(s1.c_str(), s2.c_str()) == 0)
		{
			return true;
		}
	}
	return false;
}
bool AcString::isEqual(const AcString& _str1, const AcString& _str2, bool _isIgnoreCase) {
	if (_isIgnoreCase == false)
	{
		if (strcmp(_str1.constPtr(), _str2.constPtr()) == 0)
		{
			return true;
		}
	}
	else
	{
		std::string s1 = _str1.buff;
		std::string s2 = _str2.buff;
		std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
		std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
		if (strcmp(s1.c_str(), s2.c_str()) == 0)
		{
			return true;
		}
	}
	return false;
}
int AcString::toInt32(const char *buf)
{
	return std::stol(buf);
}
int AcString::toInt32(const wchar_t *buf)
{
	return std::stol(buf);
}
int AcString::toInt32(const AcString& buf) {
	return AcString::toInt32(buf.constPtr());
}
bool AcString::toInt32(const char* buf, int& val) {

	int len = strlen(buf);
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || buf[i] == '-' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stol(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toInt32(const wchar_t* buf, int& val) {
	int len = wcslen(buf);
	if (len == 0) {
		return false;
	}
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || buf[i] == '-' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stol(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toInt32(const AcString& buf, int& val) {
	return AcString::toInt32(buf.constPtr(), val);
}

unsigned int AcString::toUint32(const char *buf)
{
	return std::stoul(buf);
}
unsigned int AcString::toUint32(const wchar_t *buf)
{
	return std::stoul(buf);
}
unsigned int AcString::toUint32(const AcString& buf) {
	return AcString::toUint32(buf.constPtr());
}
bool AcString::toUint32(const char* buf, unsigned int& val) {
	int len = strlen(buf);
	if (len == 0) {
		return false;
	}
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stoul(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toUint32(const wchar_t* buf, unsigned int& val) {
	int len = wcslen(buf);
	if (len == 0) {
		return false;
	}
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stoul(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toUint32(const AcString& buf, unsigned int& val) {
	return AcString::toUint32(buf.constPtr());
}

long long AcString::toInt64(const char *buf)
{
	return std::stoll(buf);
}
long long AcString::toInt64(const wchar_t *buf)
{
	return std::stoll(buf);
}
long long AcString::toInt64(const AcString& buf) {
	return AcString::toInt64(buf.constPtr());
}
bool AcString::toInt64(const char* buf, long long& val) {
	int len = strlen(buf);
	if (len == 0) {
		return false;
	}
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || buf[i] == '-' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stoll(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toInt64(const wchar_t* buf, long long& val) {
	int len = wcslen(buf);
	if (len == 0) {
		return false;
	}
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || buf[i] == '-' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stoll(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toInt64(const AcString& buf, long long& val) {
	return AcString::toInt64(buf.constPtr(), val);
}

unsigned long long AcString::toUint64(const char *buf)
{
	return std::stoull(buf);
}
unsigned long long AcString::toUint64(const wchar_t *buf)
{
	return std::stoull(buf);
}
unsigned long long AcString::toUint64(const AcString& buf) {
	return AcString::toUint64(buf.constPtr());
}
bool AcString::toUint64(const char* buf, unsigned long long& val) {
	int len = strlen(buf);
	if (len == 0) {
		return false;
	}
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stoull(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toUint64(const wchar_t* buf, unsigned long long& val) {
	int len = wcslen(buf);
	if (len == 0) {
		return false;
	}
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stoull(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toUint64(const AcString& buf, unsigned long long& val) {
	return AcString::toUint64(buf.constPtr(), val);
}

float AcString::toFloat32(const char *buf)
{
	return std::stof(buf);
}
float AcString::toFloat32(const wchar_t *buf)
{
	return std::stof(buf);
}
float AcString::toFloat32(const AcString& buf) {
	return AcString::toFloat32(buf.constPtr());
}
bool AcString::toFloat32(const char* buf, float& val) {
	int len = strlen(buf);
	if (len == 0) {
		return false;
	}
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || buf[i] == '-' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stof(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toFloat32(const wchar_t* buf, float& val) {

	int len = wcslen(buf);
	if (len == 0) {
		return false;
	}
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || buf[i] == '-' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stof(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toFloat32(const AcString& buf, float& val) {
	return AcString::toFloat32(buf.constPtr(), val);
}

double AcString::toFloat64(const char *buf)
{
	return std::stod(buf);
}
double AcString::toFloat64(const wchar_t *buf)
{
	return std::stod(buf);
}
double AcString::toFloat64(const AcString& buf) {
	return AcString::toFloat64(buf.constPtr());
}
bool AcString::toFloat64(const char* buf, double& val) {
	int len = strlen(buf);
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || buf[i] == '-'|| buf[i] == '.' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stod(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toFloat64(const wchar_t* buf, double& val) {

	int len = wcslen(buf);
	if (len == 0) {
		return false;
	}
	bool isSuccess = true;
	for (int i = 0; i < len; i++) {
		if (buf[i] == '+' || buf[i] == '-' || buf[i] == '.' || (buf[i] >= '0' && buf[i] <= '9'))
		{
			continue;
		}
		isSuccess = false;
		break;
	}
	if (isSuccess == false) {
		return isSuccess;
	}

	try {
		val = std::stod(buf);
	}
	catch (const std::invalid_argument& e) {
		isSuccess = false;
	}
	catch (const std::out_of_range& e) {
		isSuccess = false;
	}
	return isSuccess;
}
bool AcString::toFloat64(const AcString& buf, double& val) {
	return AcString::toFloat64(buf.constPtr(), val);
}

void AcString::toUpperCase(ACHAR * &str) {
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (str[i] >= 'a' && str[i] <= 'z') {
			str[i] += ('A' - 'a');
		}
	}
}
void AcString::toLowerCase(ACHAR*& str) {
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] += ('a' - 'z');
		}
	}
}

void AcString::clear()
{
	this->buff.clear();
}
unsigned int AcString::length() const
{
	return this->buff.length();
}
unsigned int AcString::tcharLength() const
{
	return this->length();
}
bool AcString::isEmpty() const
{
	if (this->length() <= 0)
	{
		return true;
	}
	return false;
}
unsigned int AcString::capacity() const
{
	return this->buff.capacity();
}
const char &AcString::at(unsigned int i) const
{
	return this->buff.at(i);
}
int AcString::find(ACHAR ch) const
{
	std::string str;
	str += ch;
	return this->buff.find(str);
}
int AcString::find(ACHAR ch, int nStartPos) const
{
	std::string str;
	str += ch;
	return this->buff.find(str, nStartPos);
}
int AcString::find(const ACHAR *psz) const
{
	return this->buff.find(psz);
}
int AcString::find(const ACHAR *psz, int nStartPos) const
{
	return this->buff.find(psz, nStartPos);
}
int AcString::find(const AcString &acs) const
{
	return this->buff.find(acs.buff);
}
bool AcString::isEqual(const char *_buff, bool _isIgnoreCase) const
{
	if (_isIgnoreCase == false)
	{
		if (strcmp(_buff, this->buff.c_str()) == 0)
		{
			return true;
		}
	}
	else
	{
		std::string str1Cpy(_buff);
		std::string str2Cpy(this->buff);
		std::transform(str1Cpy.begin(), str1Cpy.end(), str1Cpy.begin(), ::tolower);
		std::transform(str2Cpy.begin(), str2Cpy.end(), str2Cpy.begin(), ::tolower);
		if (strcmp(str1Cpy.c_str(), str2Cpy.c_str()) == 0)
		{
			return true;
		}
	}
	return false;
}
void AcString::append(const char byte)
{
	this->buff += byte;
}
void AcString::append(const std::string &str)
{
	this->buff.append(str);
}
void AcString::append(const char *str)
{
	this->buff.append(str);
}
void AcString::append(const AcString &str)
{
	this->buff.append(str.buff.c_str());
}
const char *AcString::utf8Ptr() const
{
	return this->buff.c_str();
}
const std::string AcString::string() const
{
	return this->buff;
}


const char *AcString::constPtr() const
{
	return this->buff.c_str();
}
const ACHAR *AcString::kACharPtr() const
{
	return this->constPtr();
}
AcString::operator const char *() const
{
	return constPtr();
}
int AcString::find_last_of(const char *val) const
{
	return this->buff.find_last_of(val);
}
int AcString::find_last_of(const char val) const
{
	return this->buff.find_last_of(val);
}
AcString AcString::substr(int _index) const
{
	return AcString(this->buff.substr(_index));
}
AcString AcString::makeUpper()const
{
	AcString str = *this;
	std::string::iterator byte = str.buff.begin();
	while (byte != str.buff.end())
	{
		if ((*byte >= 'a') && (*byte <= 'z'))
		{
			*byte = *byte + ('A' - 'a');
		}
		byte++;
	}
	return str;
}
AcString AcString::makeLower()const
{
	AcString str = *this;
	std::string::iterator byte = str.buff.begin();
	while (byte != str.buff.end())
	{
		if ((*byte >= 'A') && (*byte <= 'Z'))
		{
			*byte = *byte + ('a' - 'A');
		}
		byte++;
	}
	return str;
}
AcString &AcString::format(const char args[], ...)
{
	va_list list;
	va_start(list, args);
	std::string str = _format(args, list);
	va_end(list);
	this->clear();
	this->append(str.c_str());
	return *this;
}
AcString AcString::slice(int start) const
{
	if (start < 0)
	{
		start = this->length() + start;
	}

	AcString str;
	for (int i = start; i < this->length(); i++)
	{
		str.buff += this->buff.at(i);
	}
	return str;
}
AcString AcString::slice(int start, int end) const
{

	if (start < 0)
	{
		start = this->length() + start;
	}
	if (end < 0)
	{
		end = this->length() + end;
	}
	if (start < 0)
	{
		start = 0;
	}
	if (end < 0)
	{
		end = 0;
	}
	if (end < start)
	{
		end = this->length();
	}
	if (end > this->length())
	{
		end = this->length();
	}

	AcString str;
	for (int i = start; i < end; i++)
	{
		str.buff += this->at(i);
	}
	return str;
}