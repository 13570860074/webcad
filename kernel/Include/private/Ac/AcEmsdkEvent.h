#ifndef FEMSDKEVENT_H
#define FEMSDKEVENT_H


#include "AcEventBase.h"
#include "AcEventTrigger.h"


class AcEmsdkEvent :public AcEventBase
{
public:
	AcEmsdkEvent()
	{

	}
	virtual ~AcEmsdkEvent() {}


public:
	// 触发事件
	void _emit(int _MsgId) {
		for (int i = 0; i < this->pItems->size(); i++) {
			if (this->pItems->at(i)->MsgId == _MsgId) {
				for (int u = 0; u < this->pItems->at(i)->size(); u++) {
					AcEventTrigger::printLogStart(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
					AC_EVENT_BEGIN_CODE;
					if (AcEventTrigger::isTrigger() == true) {
						AcEventTriggerItem* item = new AcEventTriggerItem();
						item->function = this->pItems->at(i)->at(u)->function;
						item->callback = this->pItems->at(i)->at(u)->callback;
						AcEventTrigger::append(item);
					}
					else {
						if (this->pItems->at(i)->at(u)->function != NULL) {
							((void (*)())this->pItems->at(i)->at(u)->function)();
						}
						if (this->pItems->at(i)->at(u)->callback != NULL) {
							std::function<void()> callback = (std::function<void()>&)this->pItems->at(i)->at(u)->callback;
							callback();
						}
					}
					AC_EVENT_END_CODE;
					AcEventTrigger::printLogEnd(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
				}
			}
		}
	}
	template <typename T1>
	void _emit(int _MsgId, T1 _t1) {
		for (int i = 0; i < this->pItems->size(); i++) {
			if (this->pItems->at(i)->MsgId == _MsgId) {
				for (int u = 0; u < this->pItems->at(i)->size(); u++) {
					AcEventTrigger::printLogStart(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
					AC_EVENT_BEGIN_CODE;
					if (AcEventTrigger::isTrigger() == true) {
						AcEventTriggerItemParam1<T1>* item = new AcEventTriggerItemParam1<T1>;
						item->function = this->pItems->at(i)->at(u)->function;
						item->callback = this->pItems->at(i)->at(u)->callback;
						item->t1 = _t1;
						AcEventTrigger::append(item);
					}
					else {
						if (this->pItems->at(i)->at(u)->function != NULL) {
							((void (*)(T1))this->pItems->at(i)->at(u)->function)(_t1);
						}
						if (this->pItems->at(i)->at(u)->callback != NULL) {
							std::function<void(T1)> callback = (std::function<void(T1)>&)this->pItems->at(i)->at(u)->callback;
							callback(_t1);
						}
					}
					AC_EVENT_END_CODE;
					AcEventTrigger::printLogEnd(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
				}
			}
		}
	}
	template <typename T1, typename T2>
	void _emit(int _MsgId, T1 _t1, T2 _t2) {
		for (int i = 0; i < this->pItems->size(); i++) {
			if (this->pItems->at(i)->MsgId == _MsgId) {
				for (int u = 0; u < this->pItems->at(i)->size(); u++) {
					AcEventTrigger::printLogStart(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
					AC_EVENT_BEGIN_CODE;
					if (AcEventTrigger::isTrigger() == true) {
						AcEventTriggerItemParam2<T1, T2>* item = new AcEventTriggerItemParam2<T1, T2>();
						item->function = this->pItems->at(i)->at(u)->function;
						item->callback = this->pItems->at(i)->at(u)->callback;
						item->t1 = _t1;
						item->t2 = _t2;
						AcEventTrigger::append(item);
					}
					else {
						if (this->pItems->at(i)->at(u)->function != NULL) {
							((void (*)(T1, T2))this->pItems->at(i)->at(u)->function)(_t1, _t2);
						}
						if (this->pItems->at(i)->at(u)->callback != NULL) {
							std::function<void(T1, T2)> callback = (std::function<void(T1, T2)>&)this->pItems->at(i)->at(u)->callback;
							callback(_t1, _t2);
						}
					}
					AC_EVENT_END_CODE;
					AcEventTrigger::printLogEnd(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
				}
			}
		}
	}
	template <typename T1, typename T2, typename T3>
	void _emit(int _MsgId, T1 _t1, T2 _t2, T3 _t3) {
		for (int i = 0; i < this->pItems->size(); i++) {
			if (this->pItems->at(i)->MsgId == _MsgId) {
				for (int u = 0; u < this->pItems->at(i)->size(); u++) {
					AcEventTrigger::printLogStart(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
					AC_EVENT_BEGIN_CODE;
					if (AcEventTrigger::isTrigger() == true) {
						AcEventTriggerItemParam3<T1, T2, T3>* item = new AcEventTriggerItemParam3<T1, T2, T3>();
						item->function = this->pItems->at(i)->at(u)->function;
						item->callback = this->pItems->at(i)->at(u)->callback;
						item->t1 = _t1;
						item->t2 = _t2;
						item->t3 = _t3;
						AcEventTrigger::append(item);
					}
					else {
						if (this->pItems->at(i)->at(u)->function != NULL) {
							((void (*)(T1, T2, T3))this->pItems->at(i)->at(u)->function)(_t1, _t2, _t3);
						}
						if (this->pItems->at(i)->at(u)->callback != NULL) {
							std::function<void(T1, T2, T3)> callback = (std::function<void(T1, T2, T3)>&)this->pItems->at(i)->at(u)->callback;
							callback(_t1, _t2, _t3);
						}
					}
					AC_EVENT_END_CODE;
					AcEventTrigger::printLogEnd(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
				}
			}
		}
	}
	template <typename T1, typename T2, typename T3, typename T4>
	void _emit(int _MsgId, T1 _t1, T2 _t2, T3 _t3, T4 _t4) {
		for (int i = 0; i < this->pItems->size(); i++) {
			if (this->pItems->at(i)->MsgId == _MsgId) {
				for (int u = 0; u < this->pItems->at(i)->size(); u++) {
					AcEventTrigger::printLogStart(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
					AC_EVENT_BEGIN_CODE;
					if (AcEventTrigger::isTrigger() == true) {
						AcEventTriggerItemParam4<T1, T2, T3, T4>* item = new AcEventTriggerItemParam4<T1, T2, T3, T4>();
						item->function = this->pItems->at(i)->at(u)->function;
						item->callback = this->pItems->at(i)->at(u)->callback;
						item->t1 = _t1;
						item->t2 = _t2;
						item->t3 = _t3;
						item->t4 = _t4;
						AcEventTrigger::append(item);
					}
					else {
						if (this->pItems->at(i)->at(u)->function != NULL) {
							((void (*)(T1, T2, T3, T4))this->pItems->at(i)->at(u)->function)(_t1, _t2, _t3, _t4);
						}
						if (this->pItems->at(i)->at(u)->callback != NULL) {
							std::function<void(T1, T2, T3, T4)> callback = (std::function<void(T1, T2, T3, T4)>&)this->pItems->at(i)->at(u)->callback;
							callback(_t1, _t2, _t3, _t4);
						}
					}
					AC_EVENT_END_CODE;
					AcEventTrigger::printLogEnd(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
				}
			}
		}
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void _emit(int _MsgId, T1 _t1, T2 _t2, T3 _t3, T4 _t4, T5 _t5) {
		for (int i = 0; i < this->pItems->size(); i++) {
			if (this->pItems->at(i)->MsgId == _MsgId) {
				for (int u = 0; u < this->pItems->at(i)->size(); u++) {
					AcEventTrigger::printLogStart(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
					AC_EVENT_BEGIN_CODE;
					if (AcEventTrigger::isTrigger() == true) {
						AcEventTriggerItemParam5<T1, T2, T3, T4, T5>* item = new AcEventTriggerItemParam5<T1, T2, T3, T4, T5>();
						item->function = this->pItems->at(i)->at(u)->function;
						item->callback = this->pItems->at(i)->at(u)->callback;
						item->t1 = _t1;
						item->t2 = _t2;
						item->t3 = _t3;
						item->t4 = _t4;
						item->t5 = _t5;
						AcEventTrigger::append(item);
					}
					else {
						if (this->pItems->at(i)->at(u)->function != NULL) {
							((void (*)(T1, T2, T3, T4, T5))this->pItems->at(i)->at(u)->function)(_t1, _t2, _t3, _t4, _t5);
						}
						if (this->pItems->at(i)->at(u)->callback != NULL) {
							std::function<void(T1, T2, T3, T4, T5)> callback = (std::function<void(T1, T2, T3, T4, T5)>&)this->pItems->at(i)->at(u)->callback;
							callback(_t1, _t2, _t3, _t4, _t5);
						}
					}
					AC_EVENT_END_CODE;
					AcEventTrigger::printLogEnd(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
				}
			}
		}
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void _emit(int _MsgId, T1 _t1, T2 _t2, T3 _t3, T4 _t4, T5 _t5, T6 _t6) {
		for (int i = 0; i < this->pItems->size(); i++) {
			if (this->pItems->at(i)->MsgId == _MsgId) {
				for (int u = 0; u < this->pItems->at(i)->size(); u++) {
					AcEventTrigger::printLogStart(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
					AC_EVENT_BEGIN_CODE;
					if (AcEventTrigger::isTrigger() == true) {
						AcEventTriggerItemParam6<T1, T2, T3, T4, T5, T6>* item = new AcEventTriggerItemParam6<T1, T2, T3, T4, T5, T6>();
						item->function = this->pItems->at(i)->at(u)->function;
						item->callback = this->pItems->at(i)->at(u)->callback;
						item->t1 = _t1;
						item->t2 = _t2;
						item->t3 = _t3;
						item->t4 = _t4;
						item->t5 = _t5;
						item->t6 = _t6;
						AcEventTrigger::append(item);
					}
					else {
						if (this->pItems->at(i)->at(u)->function != NULL) {
							((void (*)(T1, T2, T3, T4, T5, T6))this->pItems->at(i)->at(u)->function)(_t1, _t2, _t3, _t4, _t5, _t6);
						}
						if (this->pItems->at(i)->at(u)->callback != NULL) {
							std::function<void(T1, T2, T3, T4, T5, T6)> callback = (std::function<void(T1, T2, T3, T4, T5, T6)>&)this->pItems->at(i)->at(u)->callback;
							callback(_t1, _t2, _t3, _t4, _t5, _t6);
						}
					}
					AC_EVENT_END_CODE;
					AcEventTrigger::printLogEnd(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
				}
			}
		}
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void _emit(int _MsgId, T1 _t1, T2 _t2, T3 _t3, T4 _t4, T5 _t5, T6 _t6, T7 _t7) {
		for (int i = 0; i < this->pItems->size(); i++) {
			if (this->pItems->at(i)->MsgId == _MsgId) {
				for (int u = 0; u < this->pItems->at(i)->size(); u++) {
					AcEventTrigger::printLogStart(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
					AC_EVENT_BEGIN_CODE;
					if (AcEventTrigger::isTrigger() == true) {
						AcEventTriggerItemParam7<T1, T2, T3, T4, T5, T6, T7>* item = new AcEventTriggerItemParam7<T1, T2, T3, T4, T5, T6, T7>();
						item->function = this->pItems->at(i)->at(u)->function;
						item->callback = this->pItems->at(i)->at(u)->callback;
						item->t1 = _t1;
						item->t2 = _t2;
						item->t3 = _t3;
						item->t4 = _t4;
						item->t5 = _t5;
						item->t6 = _t6;
						item->t7 = _t7;
						AcEventTrigger::append(item);
					}
					else {
						if (this->pItems->at(i)->at(u)->function != NULL) {
							((void (*)(T1, T2, T3, T4, T5, T6, T7))this->pItems->at(i)->at(u)->function)(_t1, _t2, _t3, _t4, _t5, _t6, _t7);
						}
						if (this->pItems->at(i)->at(u)->callback != NULL) {
							std::function<void(T1, T2, T3, T4, T5, T6, T7)> callback = (std::function<void(T1, T2, T3, T4, T5, T6, T7)>&)this->pItems->at(i)->at(u)->callback;
							callback(_t1, _t2, _t3, _t4, _t5, _t6, _t7);
						}
					}
					AC_EVENT_END_CODE;
					AcEventTrigger::printLogEnd(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
				}
			}
		}
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void _emit(int _MsgId, T1 _t1, T2 _t2, T3 _t3, T4 _t4, T5 _t5, T6 _t6, T7 _t7, T8 _t8) {
		for (int i = 0; i < this->pItems->size(); i++) {
			if (this->pItems->at(i)->MsgId == _MsgId) {
				for (int u = 0; u < this->pItems->at(i)->size(); u++) {
					AcEventTrigger::printLogStart(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
					AC_EVENT_BEGIN_CODE;
					if (AcEventTrigger::isTrigger() == true) {
						AcEventTriggerItemParam8<T1, T2, T3, T4, T5, T6, T7, T8>* item = new AcEventTriggerItemParam8<T1, T2, T3, T4, T5, T6, T7, T8>();
						item->function = this->pItems->at(i)->at(u)->function;
						item->callback = this->pItems->at(i)->at(u)->callback;
						item->t1 = _t1;
						item->t2 = _t2;
						item->t3 = _t3;
						item->t4 = _t4;
						item->t5 = _t5;
						item->t6 = _t6;
						item->t7 = _t7;
						item->t8 = _t8;
						AcEventTrigger::append(item);
					}
					else {
						if (this->pItems->at(i)->at(u)->function != NULL) {
							((void (*)(T1, T2, T3, T4, T5, T6, T7, T8))this->pItems->at(i)->at(u)->function)(_t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
						}
						if (this->pItems->at(i)->at(u)->callback != NULL) {
							std::function<void(T1, T2, T3, T4, T5, T6, T7, T8)> callback = (std::function<void(T1, T2, T3, T4, T5, T6, T7, T8)>&)this->pItems->at(i)->at(u)->callback;
							callback(_t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
						}
					}
					AC_EVENT_END_CODE;
					AcEventTrigger::printLogEnd(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
				}
			}
		}
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	void _emit(int _MsgId, T1 _t1, T2 _t2, T3 _t3, T4 _t4, T5 _t5, T6 _t6, T7 _t7, T8 _t8, T9 _t9) {
		for (int i = 0; i < this->pItems->size(); i++) {
			if (this->pItems->at(i)->MsgId == _MsgId) {
				for (int u = 0; u < this->pItems->at(i)->size(); u++) {
					AcEventTrigger::printLogStart(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
					AC_EVENT_BEGIN_CODE;
					if (AcEventTrigger::isTrigger() == true) {
						AcEventTriggerItemParam9<T1, T2, T3, T4, T5, T6, T7, T8, T9>* item = new AcEventTriggerItemParam9<T1, T2, T3, T4, T5, T6, T7, T8, T9>();
						item->function = this->pItems->at(i)->at(u)->function;
						item->callback = this->pItems->at(i)->at(u)->callback;
						item->t1 = _t1;
						item->t2 = _t2;
						item->t3 = _t3;
						item->t4 = _t4;
						item->t5 = _t5;
						item->t6 = _t6;
						item->t7 = _t7;
						item->t8 = _t8;
						item->t9 = _t9;
						AcEventTrigger::append(item);
					}
					else {
						if (this->pItems->at(i)->at(u)->function != NULL) {
							((void (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9))this->pItems->at(i)->at(u)->function)(_t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9);
						}
						if (this->pItems->at(i)->at(u)->callback != NULL) {
							std::function<void(T1, T2, T3, T4, T5, T6, T7, T8, T9)> callback = (std::function<void(T1, T2, T3, T4, T5, T6, T7, T8, T9)>&)this->pItems->at(i)->at(u)->callback;
							callback(_t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9);
						}
					}
					AC_EVENT_END_CODE;
					AcEventTrigger::printLogEnd(this->pItems->at(i)->MsgId, this->pItems->at(i)->at(u)->Id);
				}
			}
		}
	}

	// 监听事件
	int on(int _MsgId, long long _function, unsigned int priority = 0);
	int addEventListener(int _MsgId, long long _function, unsigned int priority = 0);

	// 移除事件
	void removeListener(int _MsgId, long long _function);
	void removeEventListener(int _MsgId, long long _function);

	// 通过Id移除事件
	void removeListenerId(int _Id);
	void removeEventListenerId(int _Id);

	// 移除所有事件
	void removeAllListeners();
	void removeAllListeners(int _MsgId);
};




#endif