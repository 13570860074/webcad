#ifndef GSOSNAPDATA_H
#define GSOSNAPDATA_H

#include "Db.h"
#include "DbObjectId.h"
#include "GePoint3d.h"
#include "GeScale3d.h"
#include "GeMatrix3d.h"

class DbEntity;
class DbOsnapData {

public:
	enum OsnapMode
	{
		kOsModeNone = -1,
		kOsModeEnd = Db::OsnapMode::kOsModeEnd,
		kOsModeMid = Db::OsnapMode::kOsModeMid,
		kOsModeCen = Db::OsnapMode::kOsModeCen,
		kOsModeNode = Db::OsnapMode::kOsModeNode,
		kOsModeQuad = Db::OsnapMode::kOsModeQuad,
		kOsModeIns = Db::OsnapMode::kOsModeIns,
		kOsModePerp = Db::OsnapMode::kOsModePerp,
		kOsModeTan = Db::OsnapMode::kOsModeTan,
		kOsModeNear = Db::OsnapMode::kOsModeNear,

		kOsModeAnchor,					// 锚点
		kOsModeCenReference,			// 圆心参照
		kOsModeExtendLineIntersec,		// 延长线交点

		kOsModeExtendLine,				// 延长直线
		kOsModeExtendLineSeg,			// 延长线段
		kOsModeExtendLineCen,			// 圆心延长直线
	};

public:
	DbOsnapData();
	virtual ~DbOsnapData();

	DbOsnapData& operator = (const DbOsnapData& _osnap) {
		this->m_entity = _osnap.m_entity;
		this->m_osnapPt = _osnap.m_osnapPt;
		this->m_dcsOsnapPt = _osnap.m_dcsOsnapPt;
		this->m_osnapMode = _osnap.m_osnapMode;
		this->m_osnapDir = _osnap.m_osnapDir;
		this->m_thisGsMarker = _osnap.m_thisGsMarker;
		this->m_otherGsMarker = _osnap.m_otherGsMarker;
		return *this;
	}
	bool operator == (const DbOsnapData& _osnap) {
		if (this->m_osnapPt.isEqualTo(_osnap.m_osnapPt) == false) {
			return false;
		}
		if (this->m_osnapDir.isEqualTo(_osnap.m_osnapDir) == false) {
			return false;
		}
		if (this->m_osnapMode != _osnap.m_osnapMode) {
			return false;
		}
		return true;
	}

public:

	DbOsnapData* clone();

	double distanceTo(const GePoint2d& _pt)const;
	double distanceTo(const GePoint3d& _pt)const;

	bool isOn(const GePoint2d& _pt)const;

	DbEntity *entity()const;
	void setEntity(DbEntity* _entity);

	Adesk::GsMarker thisGsMarker()const;
	void setThisGsMarker(Adesk::GsMarker marker);

	Adesk::GsMarker otherGsMarker()const;
	void setOtherGsMarker(Adesk::GsMarker marker);

	const DbOsnapData::OsnapMode& osnapMode() const;
	void setOsnapMode(const DbOsnapData::OsnapMode& mode);

	const GePoint2d& dcsOsnapPoint() const;
	void setDcsOsnapPoint(const GePoint2d& pt);

	const GePoint3d& osnapPoint() const;
	void setOsnapPoint(const GePoint3d& pt);

	const GeVector3d& osnapDir() const;
	void setOsnapDir(const GeVector3d& dir);


protected:
	DbEntity* m_entity;
	Adesk::GsMarker m_thisGsMarker;
	Adesk::GsMarker m_otherGsMarker;
	DbOsnapData::OsnapMode m_osnapMode;
	GePoint2d m_dcsOsnapPt;
	GePoint3d m_osnapPt;
	GeVector3d m_osnapDir;
};



#endif