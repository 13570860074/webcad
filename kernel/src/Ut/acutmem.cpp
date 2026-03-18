#include "acutmem.h" 


Acad::ErrorStatus acutNewBuffer(void*& pOutput, size_t size) {
    if (size > 0) {
        pOutput = new char[size];
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}


Acad::ErrorStatus acutNewString(ACHAR *&pOutput, Adesk::Int32 nNumChars) {
    if (nNumChars > 0) {
        pOutput = new char[nNumChars];
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus acutNewString(const ACHAR* pInput, ACHAR*& pOutput) {
    int size = strlen(pInput);
    if (size > 0) {
        pOutput = new char[size + 1];
        for (int i = 0; i < size; i++)
        {
            pOutput[i] = pInput[i];
        }
        pOutput[size] = '\0';
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus acutUpdString(const ACHAR* pInput, ACHAR*& pOutput) {
    return acutNewString(pInput, pOutput);
}
void acutDelBuffer(void *& pBuffer) {
    if (pBuffer != NULL) {
        delete[]pBuffer;
        pBuffer = NULL;
    }
}

Acad::ErrorStatus acutCopyStringToBuffer(const ACHAR* pInput, ACHAR* pOutput, size_t nOutputSize) {
    if (pInput == NULL || pOutput == NULL || nOutputSize == 0) {
        return Acad::ErrorStatus::eBufferTooSmall;
    }
    pOutput = new ACHAR[nOutputSize];
    for (int i = 0; i < nOutputSize; i++) {
        pOutput[i] = pInput[i];
    }
    return Acad::ErrorStatus::eOk;
}