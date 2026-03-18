#include "Db.h"
#include "DbSymbolTableRecord.h"
#include "DbUCSTableIterator.h"
#include "DbImpl.h"


DbUCSTableIterator::DbUCSTableIterator()
{
	this->m_pImpl = new DbSymbolTableIteratorImpl();
}
   