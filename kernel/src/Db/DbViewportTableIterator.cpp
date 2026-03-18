#include "Db.h"
#include "DbSymbolTableRecord.h"
#include "DbViewportTableIterator.h"
#include "DbImpl.h"


DbViewportTableIterator::DbViewportTableIterator()
{
	this->m_pImpl = new DbSymbolTableIteratorImpl();
}
   