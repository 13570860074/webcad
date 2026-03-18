#include "Db.h"
#include "DbSymbolTableRecord.h"
#include "DbViewTableIterator.h"
#include "DbImpl.h"


DbViewTableIterator::DbViewTableIterator()
{
	this->m_pImpl = new DbSymbolTableIteratorImpl();
}
   