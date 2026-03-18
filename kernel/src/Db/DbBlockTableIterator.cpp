#include "Db.h"
#include "DbSymbolTableRecord.h"
#include "DbBlockTableIterator.h"
#include "DbImpl.h"


DbBlockTableIterator::DbBlockTableIterator()
{
	this->m_pImpl = new DbSymbolTableIteratorImpl();
}
   