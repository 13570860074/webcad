#include "RxAsyncManager.h"
#include "ReRendererManager.h"
#include "EdUserInputManager.h"
#include "EdUserInputManager.h"
#include "EvEventManager.h"
#include "AcEmsdkEvent.h"
#include "GePoint2d.h"
#include "kernel.h"





#if EMSDK

EM_ASYNC_JS(void, onAsyncAwait, (), {
	if (window.asyncManagerAwait == null) {
		window.asyncManagerAwait = () => {
			return new Promise(function(resolve) {
				window.asyncManagerResolve = resolve;
			});
		}
	}
	await window.asyncManagerAwait();
	});
EM_JS(void, onAsyncManagerResolve, (), {
	if (window.asyncManagerResolve != null) {
		window.asyncManagerResolve();
	}
	});
EM_JS(void, onAsyncManagerResolveNull, (), {
	window.asyncManagerResolve = null;
	});
#else

void onAsyncAwait() {

}
void onAsyncManagerResolve() {

}
void onAsyncManagerResolveNull() {

}

#endif

RxAsyncManager::RxAsyncManager() {
	this->isInterval = false;
	this->isAwait = false;
}
RxAsyncManager::~RxAsyncManager() {

}



void RxAsyncManager::onInit() {

}
void RxAsyncManager::loopAsync() {

	if (this->isInterval == true) {
		return;
	}
	this->isInterval = true;

	::kernel()->acevEventManager()->_emit(Ev::Async::kAsyncBegin);

	while (true) {


		::kernel()->acevEventManager()->_emit(Ev::Async::kAsyncAwaitBegin);

		// 开启事件触发器
		AcEventTrigger::setTrigger(true);

		this->isAwait = true;

		// 异步等待
		onAsyncAwait();

		this->isAwait = false;

		// 关闭事件触发器
		AcEventTrigger::setTrigger(false);

		::kernel()->acevEventManager()->_emit(Ev::Async::kAsyncAwaitEnd);

		// 判断退出
		for (int i = 0; i < this->asyncFunctionRecords.length(); i++) {
			if (this->asyncFunctionRecords.at(i)->fun() == true) {
				RxAsyncFunctionRecord* pAsyncFunction = this->asyncFunctionRecords.at(i);
				delete pAsyncFunction;
				this->asyncFunctionRecords.removeAt(i--);
			}
		}
		if (this->asyncFunctionRecords.length() == 0) {
			break;
		}
	};

	this->isInterval = false;

	::kernel()->acevEventManager()->_emit(Ev::Async::kAsyncEnd);
}
void RxAsyncManager::resolve() {
	if (this->isInterval == true) {
		if (this->isAwait == true) {
			onAsyncManagerResolve();
			onAsyncManagerResolveNull();
		}
	}
}
void RxAsyncManager::async(std::function<bool()> _fun) {

	RxAsyncFunctionRecord* asyncFunction = new RxAsyncFunctionRecord();
	asyncFunction->mode = RxAsyncFunctionRecord::Mode::kAsync;
	asyncFunction->fun = _fun;
	this->asyncFunctionRecords.append(asyncFunction);

	this->loopAsync();
}
void RxAsyncManager::asyncExec(std::function<bool()> _fun) {

	RxAsyncFunctionRecord* asyncFunction = new RxAsyncFunctionRecord();
	asyncFunction->mode = RxAsyncFunctionRecord::Mode::kAsyncExec;
	asyncFunction->fun = _fun;
	this->asyncFunctionRecords.append(asyncFunction);

	this->loopAsync();
}
