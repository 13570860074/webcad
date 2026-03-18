#include "DbHandle.h"
#include "acutmem.h"


DbHandle::DbHandle(const ACHAR* _buff) {
	this->m_val = std::stol(_buff, nullptr, 16);
}
bool DbHandle::getIntoAsciiBuffer(ACHAR* pBuf, size_t nBufLen) const {

	if (this->isNull() == true) {
		return false;
	}

	std::stringstream ss;
	ss << std::hex << this->m_val;

	std::string data = ss.str();
	for (int i = 0; i < data.length(); i++) {
		if (i >= nBufLen) {
			break;
		}
		pBuf[i] = data[i];
	}
	if (nBufLen > data.length()) {
		pBuf[data.length()] = 0;
	}

	return true;
}
#if EMSDK
emstring DbHandle::emsdk_getIntoAsciiBuffer() const {
	emstring val;
	ACHAR* data = new ACHAR[1024];
	if (this->getIntoAsciiBuffer(data, 1024) == true) {
		val = data;
	}
	delete[]data;
	return val;
}
#endif
