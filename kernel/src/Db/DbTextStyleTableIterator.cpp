#include "Db.h"
#include "DbSymbolTableRecord.h"
#include "DbTextStyleTableIterator.h"
#include "DbImpl.h"


DbTextStyleTableIterator::DbTextStyleTableIterator()
{
	this->m_pImpl = new DbSymbolTableIteratorImpl();
}
   