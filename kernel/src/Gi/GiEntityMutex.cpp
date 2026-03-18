#include "GiEntityMutex.h"



GiEntityMutex::GiEntityMutex() {

}
GiEntityMutex::~GiEntityMutex() {

}



/* 线程锁:图元实体 */
std::mutex& GiEntityMutex::mutexElement() {
	return this->m_element;
}

/* 线程锁:拖拽实体 */
std::mutex& GiEntityMutex::mutexDragging() {
	return this->m_dragging;
}
