#ifndef DBLAYOUTMANAGERREACTOR_H
#define DBLAYOUTMANAGERREACTOR_H


#include "export.h"
#include "RxObject.h"
#include "DbObjectId.h"


class DbLayoutManagerReactor : public RxObject {

public:
	RX_DECLARE_MEMBERS(DbLayoutManagerReactor);

    virtual void layoutCreated(const char* newLayoutName, const DbObjectId& layoutId);
    virtual void layoutToBeRemoved(const char* layoutName, const DbObjectId& layoutId);
    virtual void layoutRemoved(const char* layoutName, const DbObjectId& layoutId);
    virtual void abortLayoutRemoved(const char* layoutName, const DbObjectId& layoutId);
    virtual void layoutToBeCopied(const char* layoutName, const DbObjectId& oldLayoutId);
    virtual void layoutCopied(const char* oldLayoutName, const DbObjectId& oldLayoutId, const char* newLayoutname, const DbObjectId& newLayoutId);
    virtual void abortLayoutCopied(const char* layoutName, const DbObjectId& layoutId);
    virtual void layoutToBeRenamed(const char* oldName, const char* newName, const DbObjectId& layoutId);
    virtual void layoutRenamed(const char* oldName, const char* newName, const DbObjectId& layoutId);
    virtual void abortLayoutRename(const char* oldName, const char* newName, const DbObjectId& layoutId);
    virtual void layoutSwitched(const char* newLayoutname, const DbObjectId& newLayoutId);
    virtual void plotStyleTableChanged(const char* newTableName, const DbObjectId& layoutId);
    virtual void layoutsReordered();
    virtual void refreshLayoutTabs();

#if EMSDK
public:
    DbLayoutManagerReactor();
    virtual ~DbLayoutManagerReactor();

    void onLayoutCreated(int ptr);
    void onLayoutToBeRemoved(int ptr);
    void onLayoutRemoved(int ptr);
    void onAbortLayoutRemoved(int ptr);
    void onLayoutToBeCopied(int ptr);
    void onLayoutCopied(int ptr);
    void onAbortLayoutCopied(int ptr);
    void onLayoutToBeRenamed(int ptr);
    void onLayoutRenamed(int ptr);
    void onAbortLayoutRename(int ptr);
    void onLayoutSwitched(int ptr);
    void onPlotStyleTableChanged(int ptr);
    void onLayoutsReordered(int ptr);
    void onRefreshLayoutTabs(int ptr);
#endif
};



#endif