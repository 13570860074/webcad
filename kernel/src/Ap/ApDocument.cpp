#include "ApDocument.h"
#include "kernel.h"
#include "ApDocManager.h"
#include "ApImpl.h"




void ApDocument::setDocTitle(const ACHAR* title) {
    AP_IMP_DOCUMENTIMPL(this->m_pImpl)->docTitle = title;
}
#if EMSDK
void ApDocument::emsdk_setDocTitle(const std::string title) {
    AP_IMP_DOCUMENTIMPL(this->m_pImpl)->docTitle = title;
}
#endif

Acad::ErrorStatus ApDocument::upgradeDocOpen() {
    AP_IMP_DOCUMENTIMPL(this->m_pImpl)->isReadOnly = false;
    return Acad::ErrorStatus::eOk;
}


Acad::ErrorStatus ApDocument::downgradeDocOpen(bool bPromptForSave) {
    AP_IMP_DOCUMENTIMPL(this->m_pImpl)->isReadOnly = false;
    return Acad::ErrorStatus::eOk;
}