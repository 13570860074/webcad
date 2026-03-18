#ifndef RXASYNCMANAGER_H
#define RXASYNCMANAGER_H



#include "RxManager.h"
#include "AcArray.h"
#include <functional>


class RxAsyncFunctionRecord {
public:
    enum Mode
    {
        kAsync,
        kAsyncExec,
    };

    RxAsyncFunctionRecord::Mode mode;
    std::function<bool()> fun;
};

class RxAsyncManager : public RxObject, public RxManager {
public:
    RxAsyncManager();
    virtual ~RxAsyncManager();
    RX_DECLARE_MEMBERS(RxAsyncManager);
    RX_MANAGER_DECLARE_MEMBERS(RxAsyncManager);

private:
    bool isInterval;
    bool isAwait;
    AcArray<RxAsyncFunctionRecord*> asyncFunctionRecords;

public:
    void onInit();
    void loopAsync();

public:
    void resolve();
    void async(std::function<bool()> condition);
    void asyncExec(std::function<bool()> condition);
};




#endif




