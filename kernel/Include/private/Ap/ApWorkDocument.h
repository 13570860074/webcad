#ifndef APWORKDOCUMENT_H
#define APWORKDOCUMENT_H

#include "ApDocument.h"

class ApWorkDocument :public ApDocument {
public:
	ApWorkDocument();
	virtual ~ApWorkDocument();
	RX_DECLARE_MEMBERS_VIRTUAL(ApWorkDocument);


	virtual DbDatabase* database() const;

	virtual bool    isQuiescent() const;

	virtual const ACHAR* fileName() const;
#if EMSDK
	virtual const emstring emsdk_fileName() const;
#endif
	virtual Acad::ErrorStatus setFileName(const AcString& _fileName);

	virtual const ACHAR* docTitle() const;
#if EMSDK
	virtual const emstring emsdk_docTitle() const;
#endif

	void setDocTitle(const ACHAR* title);
#if EMSDK
	void emsdk_setDocTitle(const emstring title);
#endif


	bool isReadOnly() const;

	bool isModified();
	void setModified(bool _val);
};



#endif