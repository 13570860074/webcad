#ifndef DBDATABASEIMPL_H
#define DBDATABASEIMPL_H

#include "DbDatabase.h"
#include "DbDatabaseReactor.h"
#include "DbUCSTable.h"
#include "DbViewTable.h"
#include "DbViewportTable.h"

class DbFwgUndoFiler;
class DbDatabaseImpl
{
public:
	DbDatabaseImpl()
	{
		this->isInspect = true;
		this->handseed = 1;

		this->pUCSTable = NULL;
		this->pBlockTable = NULL;
		this->pLayerTable = NULL;
		this->pLinetypeTable = NULL;
		this->pViewportTable = NULL;
		this->pViewTable = NULL;
		this->pRegAppTable = NULL;
		this->pTextStyleTable = NULL;
		this->pDimStyleTable = NULL;
		this->pRootDictionary = NULL;

		this->clayer = NULL;
		this->plinewid = 0.0;
		this->textStyleId = NULL;
		this->cecolor.setColorMethod(CmEntityColor::ColorMethod::kByLayer);
	}
	virtual ~DbDatabaseImpl()
	{

	}

public:
	bool isInspect;
	unsigned int handseed;
	AcArray<DbDatabaseReactor*> reactors;

	DbUCSTable* pUCSTable;
	DbBlockTable* pBlockTable;
	DbLayerTable* pLayerTable;
	DbLinetypeTable* pLinetypeTable;
	DbViewportTable* pViewportTable;
	DbViewTable* pViewTable;
	DbRegAppTable* pRegAppTable;
	DbTextStyleTable* pTextStyleTable;
	DbDimStyleTable* pDimStyleTable;
	DbDictionary *pRootDictionary;

	CmColor cecolor;
	CmTransparency cetransparency;
	double plinewid;
	DbObjectId clayer;
	DbObjectId textStyleId;
};

#endif // !DBDATABASEIMPL_H