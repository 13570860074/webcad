#include "Db.h"
#include "DbSymbolTableRecord.h"
#include "DbLinetypeTableIterator.h"
#include "DbImpl.h"


DbLinetypeTableIterator::DbLinetypeTableIterator()
{
	this->m_pImpl = new DbSymbolTableIteratorImpl();
}
   