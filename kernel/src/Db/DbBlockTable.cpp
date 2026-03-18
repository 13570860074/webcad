#include "Db.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTableIterator.h"
#include "DbBlockTableRecordIterator.h"
#include "DbImpl.h"
#include "DbDatabaseImpl.h"

DbBlockTable::DbBlockTable()
{
	this->m_pImpl = new DbBlockTableImpl();
}
DbBlockTable::~DbBlockTable()
{
}

Acad::ErrorStatus DbBlockTable::newIterator(DbBlockTableIterator*& pIterator,
	bool atBeginning,
	bool skipDeleted) const
{
	pIterator = new DbBlockTableIterator();
	return DbSymbolTable::newIterator((DbSymbolTableIterator*&)pIterator, atBeginning, skipDeleted);
}

Acad::ErrorStatus DbBlockTable::add(DbBlockTableRecord* pRecord)
{
	DbObjectId recordId;
	return this->add(recordId, pRecord, false);
}
Acad::ErrorStatus DbBlockTable::add(DbObjectId& recordId, DbBlockTableRecord* pRecord)
{
	return this->add(recordId, pRecord, false);
}
Acad::ErrorStatus DbBlockTable::add(DbBlockTableRecord* pRecord, bool _isAllowSamename)
{
	DbObjectId recordId;
	return this->add(recordId, pRecord, _isAllowSamename);
}
Acad::ErrorStatus DbBlockTable::add(DbObjectId& recordId, DbBlockTableRecord* pRecord, bool _isAllowSamename)
{

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		// 判空
		if (pRecord == NULL)
		{
			break;
		}

		bool isAppendEntitty = false;
		if (pRecord->database() == NULL)
		{
			isAppendEntitty = true;
		}

		/* 不进行检查 */
		if (this->database() != NULL && this->database()->pImpl->isInspect == false)
		{
			DbSymbolTable::add(pRecord);
			es = Acad::ErrorStatus::eOk;
			break;
		}

		// 获得名称
		AcString blockName;
		pRecord->getName(blockName);
		if (blockName.length() == 0)
		{
			break;
		}

		// 判断是否是布局块表记录
		if (blockName.length() > 1 && blockName[0] == '*')
		{

			/* 匿名块 */
			if (blockName[1] == 'U')
			{

				AcString key;
				key.format("*U%d", DB_IMP_BLOCKTABLE(this->m_pImpl)->indexAnonymousBlock);
				while (true)
				{
					if (DB_IMP_BLOCKTABLE(this->m_pImpl)->pMap->find(key) == NULL)
					{
						break;
					}
					DB_IMP_BLOCKTABLE(this->m_pImpl)->indexAnonymousBlock++;
					key.format("*U%d", DB_IMP_BLOCKTABLE(this->m_pImpl)->indexAnonymousBlock);
				}
				DB_IMP_BLOCKTABLE(this->m_pImpl)->indexAnonymousBlock++;
				_isAllowSamename = true;
			}

			/* 布局块表记录 */
			else if (blockName.find(ACDB_PAPER_SPACE) == 0)
			{
				int index = 0;
				while (this->has(blockName) == true)
				{
					blockName.format("*Paper_Space%d", index);
					pRecord->setName(blockName);
					index++;
				}
				_isAllowSamename = true;
			}
		}

		// 判断是否存在
		if (_isAllowSamename == false)
		{
			if (this->has(blockName) == true)
			{
				break;
			}
		}

		// 添加到符号表中
		es = DbSymbolTable::add(recordId, pRecord);

		// 添加实体
		if (isAppendEntitty == true)
		{
			for (int i = 0; i < DB_IMP_BLOCKTABLERECORD(pRecord->m_pImpl)->entitys.length(); i++)
			{
				DbObjectId pOutputId;
				this->database()->addDbObject(pOutputId, DB_IMP_BLOCKTABLERECORD(pRecord->m_pImpl)->entitys[i]);
			}
		}

	} while (false);

	return es;
}

#if EMSDK
DbBlockTableRecord* DbBlockTable::emsdk_getAt(const emstring& entryName) const
{
	return this->emsdk_getAt(entryName, Db::OpenMode::kForRead, true);
}
DbBlockTableRecord* DbBlockTable::emsdk_getAt(const emstring& entryName, Db::OpenMode openMode) const
{
	return this->emsdk_getAt(entryName, openMode, true);
}
DbBlockTableRecord* DbBlockTable::emsdk_getAt(const emstring& entryName, Db::OpenMode openMode, bool openErasedRec) const
{
	DbBlockTableRecord* pBlockTableRecord = NULL;
	this->getAt(entryName.c_str(), pBlockTableRecord, openMode, openErasedRec);
	return pBlockTableRecord;
}

DbBlockTableIterator* DbBlockTable::emsdk_newIterator() const
{
	return this->emsdk_newIterator(true);
}
DbBlockTableIterator* DbBlockTable::emsdk_newIterator(bool atBeginning) const
{
	return this->emsdk_newIterator(true, true);
}
DbBlockTableIterator* DbBlockTable::emsdk_newIterator(bool atBeginning, bool skipDeleted) const
{
	DbBlockTableIterator* pBlockTableIterator = NULL;
	this->newIterator(pBlockTableIterator, atBeginning, skipDeleted);
	return pBlockTableIterator;
}
#endif

Acad::ErrorStatus DbBlockTable::dwgInFields(DbDwgFiler* pFiler)
{

	Acad::ErrorStatus es = DbSymbolTable::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockTable::dwgOutFields(DbDwgFiler* pFiler) const
{

	Acad::ErrorStatus es = DbSymbolTable::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}
