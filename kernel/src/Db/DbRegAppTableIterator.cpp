#include "Db.h"
#include "DbSymbolTableRecord.h"
#include "DbRegAppTableIterator.h"
#include "DbImpl.h"


DbRegAppTableIterator::DbRegAppTableIterator()
{
	this->m_pImpl = new DbSymbolTableIteratorImpl();
}
   