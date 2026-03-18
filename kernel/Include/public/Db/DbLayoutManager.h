#ifndef DBLAYOUTMANAGER_H
#define DBLAYOUTMANAGER_H

#include "Db.h"
#include "DbObject.h"
#include "AcString.h"

class DbLayout;
class DbLayoutManagerReactor;
class DbLayoutManager : public RxObject {
public:
    DbLayoutManager();
    virtual ~DbLayoutManager();
    RX_DECLARE_MEMBERS(DbLayoutManager);

    /// <sumary>
    /// This function sets the layout named newname as the current layout in the database pointed to by pDb. If pDb is NULL, then the current workingDatabase will be used.
    /// </sumary>
    /// <param name="newname">Input name of layout to be set as current.</param>
    /// <param name="pDb">Input DbDatabase to use.</param>
    /// <returns>Returns Acad::eOk if successful.</returns>
#if EMSDK
    virtual Acad::ErrorStatus emsdk_setCurrentLayout(const emstring& newname);
    virtual Acad::ErrorStatus emsdk_setCurrentLayout(const emstring& newname, DbDatabase* pDb);
#endif
    virtual Acad::ErrorStatus setCurrentLayout(const ACHAR* newname);
    virtual Acad::ErrorStatus setCurrentLayout(const ACHAR* newname, DbDatabase* pDb);

    /// <sumary>
    /// This function makes the DbLayout object associated with the given DbObjectId the current layout in the active database. 
    /// </sumary>
    /// <param name="layoutId">Input DbObjectId for the DbLayout object to make current </param>
    /// <returns>Returns Acad::eOk if successful.</returns>
    virtual Acad::ErrorStatus setCurrentLayoutId(DbObjectId layoutId);

    /// <sumary>
    /// If allowModel is true, then this function will return the name of the active layout in the database pointed to by pDb (or the workingDatabase if pDb is NULL) even if the layout is the ModelSpace layout. If allowModel is false, then the current PaperSpace layout name will be returned even if it is not the currently active layout.
    /// </sumary>
    /// <param name="allowModel">Input bool indicating whether or not to allow the ModelSpace layout to be included.</param>
    /// <param name="pDb">Input DbDatabase to use.</param>
#if EMSDK
    virtual emstring emsdk_getActiveLayoutName(bool allowModel);
    virtual emstring emsdk_getActiveLayoutName(bool allowModel, const DbDatabase* pDb);
#endif
    virtual Acad::ErrorStatus getActiveLayoutName(AcString& sName, bool allowModel);
    virtual Acad::ErrorStatus getActiveLayoutName(AcString& sName, bool allowModel, const DbDatabase* pDb);

    // This overload is deprecated and will be removed. Please use getActiveLayoutName()
#if EMSDK
    virtual emstring emsdk_findActiveLayout(bool allowModel);
    virtual emstring emsdk_findActiveLayout(bool allowModel, const DbDatabase* pDb);      // deprecated
#endif
    virtual const ACHAR* findActiveLayout(bool allowModel, const DbDatabase* pDb = NULL) final;      // deprecated

    /// <sumary>
    /// This function returns the block table record id for the current DbLayout object. If TILEMODE is 1, then this will retrieve the *MODEL_SPACE block table record. If TILEMODE is 0, then this will retrieve the *PAPER_SPACE block table record; which represents the current paper space layout. 
    ///
    /// To retrieve the associated layout, open the returned block table record for read and then use the getLayoutId() method to retrieve the object ID of the associated layout.
    /// </sumary>
    /// <param name="pDb">Input DbDatabase to use.</param>
    virtual DbObjectId getActiveLayoutBTRId();
    virtual DbObjectId getActiveLayoutBTRId(const DbDatabase* pDb);

    /// <sumary>
    /// This function returns the objectId of the layout named name in the database pointed to by pDb.  If pDb is NULL, then the workingDatabase will be used.  If no layout is found, then the returned id will be null.
    /// </sumary>
    /// <param name="name">Input name of layout to find.</param>
    /// <param name="pDb">Input DbDatabase to use.</param>
#if EMSDK
    virtual DbObjectId emsdk_findLayoutNamed(const emstring& name);
    virtual DbObjectId emsdk_findLayoutNamed(const emstring& name, const DbDatabase* pDb);
#endif
    virtual DbObjectId findLayoutNamed(const ACHAR* name);
    virtual DbObjectId findLayoutNamed(const ACHAR* name, const DbDatabase* pDb);

    /// <sumary>
    /// This function will find the layout named name in the database pointed to by pDb, open it in the specified mode, and set pLayout to point to the opened DbLayout object.   If pDb is NULL, then the workingDatabase will be used.
    /// </sumary>
    /// <param name="name">Input name of layout to get.</param>
    /// <param name="pLayout">Output pointer to layout object.</param>
    /// <param name="mode">Input mode to open layout.</param>
    /// <param name="pDb">Input DbDatabase to use.</param>
    /// <returns>Returns Acad::eOk if successful.</returns>
#if EMSDK
    DbLayout* emsdk_getLayoutNamed(const emstring& name);
    DbLayout* emsdk_getLayoutNamed(const emstring& name, Db::OpenMode mode, const DbDatabase* pDb);
#endif
    Acad::ErrorStatus getLayoutNamed(const ACHAR* name, DbLayout*& pLayout);
    Acad::ErrorStatus getLayoutNamed(const ACHAR* name, DbLayout*& pLayout, Db::OpenMode mode, const DbDatabase* pDb);

    /// <sumary>
    /// This function returns true if the layout named name is found in the database pointed to by pDb.  If pDb is NULL, then the workingDatabase will be used.
    /// </sumary>
    /// <param name="name">Input name of layout to find.</param>
    /// <param name="pDb">Input DbDatabase to use.</param>
#if EMSDK
    bool emsdk_layoutExists(const emstring& name);
    bool emsdk_layoutExists(const emstring& name, const DbDatabase* pDb);
#endif
    bool layoutExists(const ACHAR* name);
    bool layoutExists(const ACHAR* name, const DbDatabase* pDb);

    /// <sumary>
    /// This function creates a copy of the layout named copyname in the database pointed to by pDb (or the workingDatabase if pDb is NULL) and names the copy newname.
    /// </sumary>
    /// <param name="copyname">Input name of layout to copy.</param>
    /// <param name="newname">Input name to be given to the layout copy.</param>
    /// <param name="pDb">Input DbDatabase to use.</param>
    /// <returns>Returns Acad::eOk if successful. Returns Acad::eMaxLayouts if there are already the maximum number of layouts in the database. Returns Acad::eRenameInvalidLayoutName if newname &gt; 255 characters long. Returns Acad::eCopyDoesNotExist if there is no layout named copyname. Returns Acad::eCopyIsModelSpace if copyname is the ModelSpace layout.</returns>
#if EMSDK
    virtual Acad::ErrorStatus emsdk_copyLayout(const emstring& copyname, const emstring& newname);
    virtual Acad::ErrorStatus emsdk_copyLayout(const emstring& copyname, const emstring& newname, DbDatabase* pDb);
#endif
    virtual Acad::ErrorStatus copyLayout(const ACHAR* copyname, const ACHAR* newname);
    virtual Acad::ErrorStatus copyLayout(const ACHAR* copyname, const ACHAR* newname, DbDatabase* pDb);

    /// <sumary>
    /// This function deletes the layout named delname from the database pointed to by pDb (or the workingDatabase if pDb is NULL).
    /// </sumary>
    /// <param name="delname">Input name of layout to delete.</param>
    /// <param name="pDb">Input DbDatabase to use.</param>
    /// <returns>Returns Acad::eOk if successful. Returns Acad::eDelLastLayout if delname is the last PaperSpace layout in the database. Returns Acad::eDelIsModelSpace if delname is the ModelSpace layout. Returns Acad::eDelDoesNotExist if delname is not found in the database.</returns>
#if EMSDK
    virtual Acad::ErrorStatus emsdk_deleteLayout(const emstring& delname);
    virtual Acad::ErrorStatus emsdk_deleteLayout(const emstring& delname, DbDatabase* pDb);
#endif
    virtual Acad::ErrorStatus deleteLayout(const ACHAR* delname);
    virtual Acad::ErrorStatus deleteLayout(const ACHAR* delname, DbDatabase* pDb);

    /// <sumary>
    /// This function creates a new DbLayout object given a unique layout name. The default viewports for the new layout are not created until the layout is first activated.
    /// </sumary>
    /// <param name="newname">Input pointer of name to give new DbLayout object</param>
    /// <param name="layoutId">Output reference to DbObjectId for the newly created DbLayout object</param>
    /// <param name="blockTableRecId">Output reference to DbObject block table record Id for the newly created DbLayout object</param>
    /// <param name="pDb">Optional input pointer to DbDatabase. The default is the current database.</param>
    /// <returns>Returns Acad::eOk if successful.</returns>
#if EMSDK
    virtual DbObjectId emsdk_createLayout(const emstring& newname);
    virtual DbObjectId emsdk_createLayout(const emstring& newname, DbDatabase* pDb);
#endif
    virtual Acad::ErrorStatus createLayout(const ACHAR* newname, DbObjectId& layoutId, DbObjectId& blockTableRecId);
    virtual Acad::ErrorStatus createLayout(const ACHAR* newname, DbObjectId& layoutId, DbObjectId& blockTableRecId, DbDatabase* pDb);

    /// <sumary>
    /// This function renames the layout named "oldname" to the new name "newname" in the database pointed to by pDb (or the workingDatabase if pDb is NULL).
    /// </sumary>
    /// <param name="oldname">Input name of layout to rename.</param>
    /// <param name="newname">Input new name for layout.</param>
    /// <param name="pDb">Input DbDatabase to use.</param>
    /// <returns>Returns Acad::eOk if successful. Returns Acad::eRenameDoesNotExist if oldname isn't in the database. Returns Acad::eRenameIsModelSpace if the oldname is the ModelSpace layout. Returns Acad::eRenameInvalidLayoutName if newname is not a valid layout name. Returns Acad::eRenameLayoutAlreadyExists if newname already exists in the database.</returns>
#if EMSDK
    virtual Acad::ErrorStatus emsdk_renameLayout(const emstring& oldname, const emstring& newname);
    virtual Acad::ErrorStatus emsdk_renameLayout(const emstring& oldname, const emstring& newname, DbDatabase* pDb);
#endif
    virtual Acad::ErrorStatus renameLayout(const ACHAR* oldname, const ACHAR* newname);
    virtual Acad::ErrorStatus renameLayout(const ACHAR* oldname, const ACHAR* newname, DbDatabase* pDb);

    /// <sumary>
    /// This function copies the layout pointed to by pLBTR which must be in the database pointed to by pDb (or the workingDatabase if pDb is NULL), adds it to the same database using the name newname, sets it in the layout tab order at index newTabOrder, then closes both the layout pointed to by pLBTR and the newly created layout.
    /// </sumary>
    /// <param name="pLBTR">Input pointer to layout to clone</param>
    /// <param name="newname">Input name for layout clone.</param>
    /// <param name="newTabOrder">Input tab index for the location of the new layout.</param>
    /// <param name="pDb">Input DbDatabase to use.</param>
    /// <returns>Returns Acad::eOk if successful. Returns Acad::eCopyNameExists if newname already exists in the database. Returns Acad::eMaxLayouts if there are already the maximum number of layouts in the database. Returns Acad::eCopyIsModelSpace if pLBTR is the ModelSpace layout.</returns>
#if EMSDK
    virtual Acad::ErrorStatus emsdk_cloneLayout(DbLayout* pLBTR, const emstring& newname);
    virtual Acad::ErrorStatus emsdk_cloneLayout(DbLayout* pLBTR, const emstring& newname, int newTabOrder);
    virtual Acad::ErrorStatus emsdk_cloneLayout(DbLayout* pLBTR, const emstring& newname, int newTabOrder, DbDatabase* pDb);
#endif
    virtual Acad::ErrorStatus cloneLayout(DbLayout* pLBTR, const ACHAR* newname);
    virtual Acad::ErrorStatus cloneLayout(DbLayout* pLBTR, const ACHAR* newname, int newTabOrder);
    virtual Acad::ErrorStatus cloneLayout(DbLayout* pLBTR, const ACHAR* newname, int newTabOrder, DbDatabase* pDb);

    /// <sumary>
    /// This function returns the DbObjectId for the non rectangular viewport ID associated with the given clipId. Returns NULL if given entity isn't a valid clip entity.
    /// </sumary>
    /// <param name="clipId">Input DbObjectId of clip entity to get the nonrectangular viewport ID from</param>
    virtual DbObjectId getNonRectVPIdFromClipId(DbObjectId clipId);

    /// <sumary>
    /// For Autodesk Internal Use.
    /// </sumary>
    /// <param name="index">For Autodesk Internal Use.</param>
    /// <param name="pDb">For Autodesk Internal Use.</param>
    virtual bool isVpnumClipped(int index);
    virtual bool isVpnumClipped(int index, const DbDatabase* pDb);

    /// <sumary>
    /// This function returns the number of items in the layout dictionary which should represent the number of DbLayout objects in the drawing. This includes the Model tab, which is always present.
    /// </sumary>
    /// <param name="pDb">Input pointer of DbDatabase to use. Default is the current database.</param>
    virtual int countLayouts();
    virtual int countLayouts(DbDatabase* pDb);

    /// <sumary>
    /// This method adds the newObj reactor object to the layout manager's reactor list.
    /// </sumary>
    /// <param name="newObj">Input pointer to a reactor object</param>
    virtual void addReactor(DbLayoutManagerReactor* newObj);

    /// <sumary>
    /// This method removes the newObj reactor object from the layout manager's reactor list. If newObj is not in the layout manager's reactor list, then this function does nothing.
    /// </sumary>
    /// <param name="delObj">Input pointer to a reactor object</param>
    virtual void removeReactor(DbLayoutManagerReactor* delObj);
};




#endif