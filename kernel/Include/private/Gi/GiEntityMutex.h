#ifndef GIENTITYMUTEX_H
#define GIENTITYMUTEX_H

#include <mutex>


class GiEntityMutex
{
public:
	GiEntityMutex();
	virtual ~GiEntityMutex();


private:
	std::mutex m_element;
	std::mutex m_dragging;

public:

	/* 线程锁:图元实体 */
	std::mutex& mutexElement();

	/* 线程锁:拖拽实体 */
	std::mutex& mutexDragging();
};

#endif