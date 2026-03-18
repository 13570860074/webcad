#include "ApWorkDocument.h"
#include "ApImpl.h"


ApWorkDocument::ApWorkDocument() {
    this->m_pImpl = new ApWorkDocumentImpl();
}
ApWorkDocument::~ApWorkDocument() {
    
}
DbDatabase* ApWorkDocument::database() const {
    return AP_IMP_WORKDOCUMENT(this->m_pImpl)->database;
}
bool ApWorkDocument::isQuiescent() const {
    return AP_IMP_WORKDOCUMENT(this->m_pImpl)->isQuiescent;
}

const ACHAR* ApWorkDocument::fileName() const {
    return AP_IMP_WORKDOCUMENT(this->m_pImpl)->fileName.constPtr();
}
#if EMSDK
const emstring ApWorkDocument::emsdk_fileName() const {
    return AP_IMP_WORKDOCUMENT(this->m_pImpl)->fileName.constPtr();
}
#endif
Acad::ErrorStatus ApWorkDocument::setFileName(const AcString& _fileName) {
    
    // 设置标题
    AcString docTitle;
    for (int i = 0; i < _fileName.length(); i++) {
        if (_fileName[i] == '/' || _fileName[i] == '\\') {
            docTitle.clear();
        }
        else {
            docTitle += _fileName[i];
        }
    }
    AP_IMP_WORKDOCUMENT(this->m_pImpl)->docTitle = docTitle;

    // 设置文件名称
    AP_IMP_WORKDOCUMENT(this->m_pImpl)->fileName = _fileName;

    return Acad::ErrorStatus::eOk;
}


const ACHAR* ApWorkDocument::docTitle() const {
    return AP_IMP_WORKDOCUMENT(this->m_pImpl)->docTitle.constPtr();
}
#if EMSDK
const emstring ApWorkDocument::emsdk_docTitle() const {
    return AP_IMP_WORKDOCUMENT(this->m_pImpl)->docTitle.constPtr();
}
#endif
void ApWorkDocument::setDocTitle(const ACHAR* title) {

    // 获得路径部分
    AcString temp;
    AcString filePath;
    int len = strlen(title);
    for (int i = 0; i < len; i++) {
        if (title[i] == '/' || title[i] == '\\') {
            filePath += temp;
            temp.clear();
}
        else {
            temp += title[i];
        }
    }

    // 设置标题
    AP_IMP_WORKDOCUMENT(this->m_pImpl)->docTitle = title;

    // 设置文件名称
    AP_IMP_WORKDOCUMENT(this->m_pImpl)->fileName = filePath;
    AP_IMP_WORKDOCUMENT(this->m_pImpl)->fileName += title;
}
#if EMSDK
void ApWorkDocument::emsdk_setDocTitle(const emstring title) {
    this->setDocTitle(title.c_str());
}
#endif


bool ApWorkDocument::isReadOnly() const {
    return AP_IMP_WORKDOCUMENT(this->m_pImpl)->isReadOnly;
}

bool ApWorkDocument::isModified() {
    return AP_IMP_WORKDOCUMENT(this->m_pImpl)->isModified;
}
void ApWorkDocument::setModified(bool _val) {
    AP_IMP_WORKDOCUMENT(this->m_pImpl)->isModified = _val;
}