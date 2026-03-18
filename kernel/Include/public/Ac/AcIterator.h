#ifndef ACITERATOR_H
#define ACITERATOR_H

#include <vector>
#include "AcArray.h"

template<typename T>
class AcIterator
{
public:
	AcIterator(){
        this->m_index = 0;
	}
    AcIterator(const AcArray<T> objs) {
        this->m_index = 0;
        this->m_records = objs;
    }
    AcIterator(const AcArray<T> *objs) {
        this->m_index = 0;
        this->m_records = *objs;
    }
	virtual ~AcIterator(){}

private:
    int m_index;
    AcArray<T> m_records;

public:

    // 判断是否为空
    bool isNull() {
        if (this->m_index > this->m_records.length() - 1) {
            return true;
        }
        return false;
    }

    // 获得记录
    T record() {
        return this->m_records.at(this->m_index);
    }

    // 获得记录
    AcArray<T>& records()
    {
        return this->m_records;
    }

    // 插入
    void insert(T obj) {
        this->m_records.push_back(obj);
    }

    // 开始
    void start() {
        this->m_index = 0;
    }

    // 指向下一个记录
    void next() {
        this->m_index = this->m_index + 1;
    }

    // 判断是否结束
    bool done() {
        return this->isNull();
    }
};



#endif