#include "Db.h"
#include "DbSymbolTableRecord.h"
#include "DbDimStyleTableIterator.h"
#include "DbImpl.h"


DbDimStyleTableIterator::DbDimStyleTableIterator()
{
	this->m_pImpl = new DbSymbolTableIteratorImpl();
}
   