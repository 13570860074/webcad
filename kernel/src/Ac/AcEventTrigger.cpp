#include "AcEventTrigger.h"
#include "AcDebug.h"

std::vector<char *> AcEventTrigger::tchar_params;
bool AcEventTrigger::trigger = false;
bool AcEventTrigger::m_isPrintLog = false;
std::vector<AcEventTriggerItem *> AcEventTrigger::items = std::vector<AcEventTriggerItem *>();
std::vector<int> AcEventTrigger::ignoreIds = std::vector<int>();

void AcEventTrigger::append(AcEventTriggerItem *item)
{
	unsigned int id = 0;
	for (int i = 0; i < AcEventTrigger::items.size(); i++)
	{
		if (AcEventTrigger::items[i]->id >= id)
		{
			id = AcEventTrigger::items[i]->id + 1;
		}
	}
	item->id = id;
	AcEventTrigger::items.push_back(item);
}
void AcEventTrigger::removeAllEvent()
{
	for (int i = AcEventTrigger::items.size() - 1; i >= 0; i--)
	{
		AcEventTriggerItem *item = AcEventTrigger::items[i];
		delete item;
	}
	AcEventTrigger::items.clear();
}
void AcEventTrigger::emitAllEvent(bool _isRemoveAll)
{
	for (int i = 0; i < AcEventTrigger::items.size(); i++)
	{
		AcEventTrigger::items[i]->_emit();
	}
	if (_isRemoveAll == true)
	{
		AcEventTrigger::removeAllEvent();
	}
	for (int i = AcEventTrigger::tchar_params.size() - 1; i >= 0; i--)
	{
		char *temp = AcEventTrigger::tchar_params[i];
		delete[] temp;
	}
	AcEventTrigger::tchar_params.clear();
}
void AcEventTrigger::setTrigger(bool _trigger)
{
	AcEventTrigger::trigger = _trigger;
}
bool AcEventTrigger::isTrigger()
{
	return AcEventTrigger::trigger;
}
bool AcEventTrigger::isPrintLog()
{
	return AcEventTrigger::m_isPrintLog;
}
void AcEventTrigger::setPrintLog(bool _isPrintLog)
{
	AcEventTrigger::m_isPrintLog = _isPrintLog;
}
void AcEventTrigger::ignoreLog(int _id)
{
	AcEventTrigger::ignoreIds.push_back(_id);
}
void AcEventTrigger::printLogStart(int _eventId, int _id)
{

	if (AcEventTrigger::isPrintLog() == false)
	{
		return;
	}
	for (int i = 0; i < AcEventTrigger::ignoreIds.size(); i++)
	{
		if (AcEventTrigger::ignoreIds[i] == _eventId)
		{
			return;
		}
	}

	bool isDebug = AcDebug::isPrintDebug();
	if (isDebug == false)
	{
		AcDebug::openDebug();
	}

	AcString str;
	str.format("eventStart:{eventId:%d,id:%d}", _eventId, _id);
	AcDebug::print_debug(str.constPtr());

	if (isDebug == false)
	{
		AcDebug::closeDebug();
	}
}
void AcEventTrigger::printLogEnd(int _eventId, int _id)
{

	if (AcEventTrigger::isPrintLog() == false)
	{
		return;
	}
	for (int i = 0; i < AcEventTrigger::ignoreIds.size(); i++)
	{
		if (AcEventTrigger::ignoreIds[i] == _eventId)
		{
			return;
		}
	}

	bool isDebug = AcDebug::isPrintDebug();
	if (isDebug == false)
	{
		AcDebug::openDebug();
	}

	AcString str;
	str.format("eventEnd:{eventId:%d,id:%d}", _eventId, _id);
	AcDebug::print_debug(str.constPtr());

	if (isDebug == false)
	{
		AcDebug::closeDebug();
	}
}
