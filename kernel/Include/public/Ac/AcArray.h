#ifndef ARRAY_H
#define ARRAY_H


#include "vector"
#include <algorithm>


template<typename T>
class AcArray {
public:
	typedef bool (*Comp)(T, T);

public:
	AcArray(unsigned int count = 0) {
		if (count > 0) {
			this->list.reserve(count);
		}
	}
	virtual ~AcArray() {}

private:
	std::vector<T> list;

public:
	T& operator [] (unsigned int i) {
		return this->list[i];
	}
	const T& operator [] (unsigned int i)const {
		return this->list[i];
	}

public:
	int length() const
	{
		return this->list.size();
	}
	void splice(unsigned int index) {
		for (int i = this->length() - 1; i >= index; i--) {
			this->removeAt(i);
		}
	}
	void remove(T _data) {
		for (int i = this->length() - 1; i >= 0; i--)
		{
			if (this->at(i) == _data)
			{
				this->removeAt(i);
				break;
			}
		}
	}
	void removeAt(unsigned int i)
	{
		this->list.erase(this->list.begin() + i);
	}
	void removeAll()
	{
		this->list.clear();
	}
	void append(T _data)
	{
		this->list.push_back(_data);
	}
	void insert(int index, T _data) {
		this->list.insert(this->list.begin() + index, _data);
	}
	T& at(unsigned int i) {
		return this->list.at(i);
	}
	const T& at(unsigned int i) const {
		return this->list.at(i);
	}
	void setAt(unsigned int i, T _data) {
		this->list.at(i) = _data;
	}
	void sort(Comp _fun){
		std::sort(this->list.begin(), this->list.end(), _fun);
	}
};




#define realloc_all(x)\
	if(x != NULL){\
		for (int i = x->length() - 1; i >= 0; i--) {\
			auto* p = x->at(i);\
			delete p;\
		}\
		delete x;\
		x = NULL;\
	}\

#define realloc_all_object(x)\
	if(x != NULL){\
		for (int i = x->length() - 1; i >= 0; i--) {\
			auto* p = x->at(i);\
			delete p;\
		}\
	}\
	x->removeAll();\

#define realloc_at(x,i)\
	if(x != NULL){\
		auto *p = x->at(i);\
		delete p;\
		x->removeAt(i);\
	}\

#define realloc_arry_all(x)\
	for (int i = x.length() - 1; i >= 0; i--) {\
		auto* p = x.at(i);\
		delete p;\
	}\
	x.removeAll();\

#define realloc_arry_at(x,i)\
	auto* p = x.at(i);\
	delete p;\
	x.removeAt(i);\


#define ACARRAY_METHODS_DEFINE(CLASSNAME)\
public:\
	typedef bool (*Comp)(CLASSNAME, CLASSNAME);\
private:\
	std::vector<CLASSNAME> list;\
public:\
	CLASSNAME& operator [] (unsigned int i) {\
		return this->list[i];\
	}\
public:\
	CLASSNAME& at(unsigned int i){\
		return this->list.at(i);\
	}\
	const CLASSNAME at(unsigned int i) const{\
		return this->list.at(i);\
	}\
	int length() const {\
		return this->list.size();\
	}\
	void remove(CLASSNAME _data) {\
		for (int i = this->length() - 1; i >= 0; i--)\
		{\
			if (this->at(i) == _data)\
			{\
				this->removeAt(i);\
				break;\
			}\
		}\
	}\
	void removeAt(unsigned int i) {\
		this->list.erase(this->list.begin() + i);\
	}\
	void removeAll() {\
		this->list.clear();\
	}\
	void append(CLASSNAME _data) {\
		this->list.push_back(_data);\
	}\
	void insert(int index, CLASSNAME _data) {\
		this->list.insert(this->list.begin() + index, _data);\
	}\
	void setAt(unsigned int i, CLASSNAME _data) {\
		this->list[i] = _data;\
	}\
	void sort(Comp _fun){\
		std::sort(this->list.begin(), this->list.end(), _fun);\
	}\


#define ACARRAY_METHODS(CLASSNAME)\
	ACARRAY_METHODS_DEFINE(CLASSNAME)\
	const CLASSNAME& operator [] (unsigned int i)const {\
		return this->list[i];\
	}\


#define ACARRAY_PTR_METHODS(CLASSNAME)\
	ACARRAY_METHODS_DEFINE(CLASSNAME)\
	const CLASSNAME operator [] (unsigned int i)const {\
		return this->list[i];\
	}\




#endif


