#ifndef ACEVENTBASE_H
#define ACEVENTBASE_H


#include "vector"
#include <iostream>
#include <functional>


typedef void (*em_fun)();
typedef void (*em_fun1)(void* p1);
typedef void (*em_fun2)(void* p1, void* p2);
typedef void (*em_fun3)(void* p1, void* p2, void* p3);
typedef void (*em_fun4)(void* p1, void* p2, void* p3, void* p4);
typedef void (*em_fun5)(void* p1, void* p2, void* p3, void* p4, void* p5);
typedef void (*em_fun6)(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6);
typedef void (*em_fun7)(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7);
typedef void (*em_fun8)(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8);
typedef void (*em_fun9)(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9);
typedef void (*em_fun10)(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10);

typedef void (*_fun)(void* lpParamter);
typedef void (*_fun1)(void* lpParamter, void* p1);
typedef void (*_fun2)(void* lpParamter, void* p1, void* p2);
typedef void (*_fun3)(void* lpParamter, void* p1, void* p2, void* p3);
typedef void (*_fun4)(void* lpParamter, void* p1, void* p2, void* p3, void* p4);
typedef void (*_fun5)(void* lpParamter, void* p1, void* p2, void* p3, void* p4, void* p5);
typedef void (*_fun6)(void* lpParamter, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6);
typedef void (*_fun7)(void* lpParamter, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7);
typedef void (*_fun8)(void* lpParamter, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8);
typedef void (*_fun9)(void* lpParamter, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9);
typedef void (*_fun10)(void* lpParamter, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10);

typedef void(*fun)(void* _plpParamter, void* _p1, void* _p2, void* _p3, void* _p4, void* _p5, void* _p6, void* _p7, void* _p8, void* _p9, void* _p10);


class AcEventItem
{
public:
	AcEventItem()
	{
		this->Id = -1;
		this->function = NULL;
		this->callback = NULL;
		this->lpParamter = NULL;
		this->priority = 0;
	}
	virtual ~AcEventItem()
	{

	}


public:
	int Id;
	void* function;
	std::function<void()> callback;
	void* lpParamter;
	unsigned int priority;
};
class AcEventItemGroup :public std::vector<AcEventItem*>
{
public:
	AcEventItemGroup() {
		this->MsgId = 0;
	}
	virtual ~AcEventItemGroup() {
		for (int i = this->size() - 1; i >= 0; i--) {
			AcEventItem* p = this->at(i);
			delete p;
		}
	}

	int MsgId;

	void push_back(AcEventItem *item);
};

class AcEventBase
{
public:
	AcEventBase()
	{
		this->Index = 0;
		this->pItems = new std::vector<AcEventItemGroup*>();
	}
	virtual ~AcEventBase()
	{
		for (int i = this->pItems->size() - 1; i >= 0; i--) {
			AcEventItemGroup* p = this->pItems->at(i);
			delete p;
		}
		delete this->pItems;
	}

protected:
	int Index;
	std::vector<AcEventItemGroup*>* pItems;

public:
	/** 事件数量 */
	unsigned int numEvents();
	unsigned int numEvents(int _msgId);
};

#include "AcDebug.h"
#define AC_EVENT_BEGIN_CODE \

	//AcDebug::startTag();\

#define AC_EVENT_END_CODE \

	//unsigned int time = AcDebug::endTag(false);\
	//if(time > 1){\
	//	std::cout<<"time:"<< time << "MsgId:"<<this->pItems->at(i)->MsgId<<", Id:"<<this->pItems->at(i)->at(u)->Id<<std::endl;\
	//}\


#endif