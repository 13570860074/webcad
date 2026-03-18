#ifndef APDOCUMENT_H
#define APDOCUMENT_H


#include "Ap.h"
#include "Db.h"
#include "RxObject.h"


class DbDatabase;
class ApDocument :public RxObject
{
public:
    RX_DECLARE_MEMBERS_VIRTUAL(ApDocument);


    // Returns the full path and filename of the database
    virtual const ACHAR* fileName() const = 0;
#if EMSDK
    virtual const emstring emsdk_fileName() const = 0;
#endif

    virtual DbDatabase* database() const = 0;

    // Is there a command, lisp, script, or ARX command active 
    virtual bool    isQuiescent() const = 0;

    // Return the documents Window title
    virtual const ACHAR* docTitle() const = 0;
    void setDocTitle(const ACHAR* title);
#if EMSDK
    virtual const std::string emsdk_docTitle() const = 0;
    void emsdk_setDocTitle(const std::string title);
#endif

    // Return TRUE if the document is read only else false
    virtual bool isReadOnly() const = 0;

    // Convert the document status from read only to write if possible.
    //
    Acad::ErrorStatus upgradeDocOpen();

    // Convert the document status from writeable to read only.
    // If bPromptForSave is true - and the drawing is modified (based on dbmod)
    // the user is prompted to save the drawing. If bPromptForSave is
    // false and the drawing has been modified, an error is returned.
    //
    Acad::ErrorStatus downgradeDocOpen(bool bPromptForSave);
};



#endif
