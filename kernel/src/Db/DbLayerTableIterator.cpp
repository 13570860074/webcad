#include "Db.h"
#include "DbSymbolTableRecord.h"
#include "DbLayerTableIterator.h"
#include "DbImpl.h"


DbLayerTableIterator::DbLayerTableIterator()
{
	this->m_pImpl = new DbSymbolTableIteratorImpl();
}
   