
#ifndef ACED_H
#define ACED_H

#include "adesk.h"
#include "acadstrc.h"
#include "GeMatrix3d.h"
#include "DbObjectIdArray.h"
#include "RxEvent.h"

class DbObjectId;
class DbViewport;
class DbViewTableRecord;
class RxEventReactor;


#define ACED_EDITOR_OBJ "AcEditor"


#define acedEditor Editor::cast(acrxSysRegistry()->at(ACED_EDITOR_OBJ))

class Editor : public RxEvent
{
public:
    RX_DECLARE_MEMBERS_VIRTUAL(Editor);
    void addReactor(RxEventReactor* newObj) override = 0;
    void removeReactor(RxEventReactor* delObj) override = 0;
};


Acad::ErrorStatus acedMspace();  // in a layout, switch to mspace
Acad::ErrorStatus acedPspace();  // in a layout, switch to pspace
Acad::ErrorStatus acedSetCurrentVPort(int vpnumber);
Acad::ErrorStatus acedSetCurrentVPort(const DbViewport* pVp);

short acedGetCurVportCircleSides();
Acad::ErrorStatus acedSetCurVportCircleSides(short value);

Acad::ErrorStatus acedSetCurrentUCS(const GeMatrix3d& mat);
Acad::ErrorStatus acedGetCurrentUCS(GeMatrix3d& mat);

//  AutoCAD color to RGB conversion function
//
Adesk::UInt32     acedGetRGB(int color);

Acad::ErrorStatus acedGetCurrentSelectionSet(DbObjectIdArray& sset);



// Get the current viewport id.  Returns null id if no vports open.
// The id returned will refer to a viewport entity (DbViewport) if
// the editor is in layout mode.  Otherwise, in tilemode, it will
// refer to an DbViewportTableRecord object.
//
DbObjectId acedActiveViewportId();

// Converts viewport number into an id.  The id refers to a Viewport
// entity (in layout mode) or to a ViewportTableRecord (in tile mode).
// The int number is one returned by the CVPORT sysvar or by the
// DbViewport::number() method.
//
DbObjectId acedViewportIdFromNumber(int nVpNum);

// Returns the number of viewports in the current editor window.
// Includes the paperspace viewport, when we're in layout mode.
//
int acedNumberOfViewports();

// Get the current viewport ObjectId for the current layout's viewport.
// Returns null id in tile mode.  Note that acedActiveViewportId()
// has the same functionality as this when in layout mode.
DbObjectId acedGetCurViewportObjectId();

#endif