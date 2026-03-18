#ifndef GSGRIPDATA_H
#define GSGRIPDATA_H


#include "DbObjectId.h"
#include "GePoint3d.h"
#include "GeScale3d.h"


class DbGripData  {
public:
    DbGripData();
    virtual ~DbGripData();

    enum DbGripMode
    {
        kGripModeDrag,
        kGripModePolylineMid,
    };


public:
    DbGripData& operator = (const DbGripData& _grip) {
        this->m_id.setFromOldId(_grip.m_id.asOldId());
        this->m_gripPt = _grip.m_gripPt;
        this->m_index = _grip.m_index;
        this->m_gripMode = _grip.m_gripMode;
        this->m_scale = _grip.m_scale;
        this->m_rotationAxis = _grip.m_rotationAxis;
        return *this;
    }
    bool operator == (const DbGripData& _grip) {
        if (this->m_gripPt.isEqualTo(_grip.m_gripPt) == false) {
            return false;
        }
        if (this->m_index != _grip.m_index) {
            return false;
        }
        if (this->m_scale.isEqualTo(_grip.m_scale) == false) {
            return false;
        }
        if (this->m_rotationAxis.isEqualTo(_grip.m_rotationAxis) == false) {
            return false;
        }
        if (this->m_gripMode != _grip.m_gripMode) {
            return false;
        }
        return true;
    }

    DbGripData* clone()const {
        DbGripData* data = new DbGripData();
        data->m_id.setFromOldId(this->m_id.asOldId());
        data->m_gripPt = this->m_gripPt;
        data->m_index = this->m_index;
        data->m_gripMode = this->m_gripMode;
        data->m_scale = this->m_scale;
        data->m_rotationAxis = this->m_rotationAxis;
        return data;
    }

    const DbObjectId objectId()const;
    void setObjectId(const DbObjectId& _id);

    unsigned int index() const;
    void setIndex(const unsigned int i);

    const GePoint3d& gripPoint() const;
    void setGripPoint(const GePoint3d& pt);

    const DbGripMode& gripMode() const;
    void setGripMode(const DbGripMode& mode);

    const GeScale3d scale()const;
    void setScale(const GeScale3d& _scale);

    const GeVector3d direction()const;
    void setDirection(const GeVector3d& _direction);

    const double angle()const;
    const GeVector3d rotationAxis()const;
    void setRotation(const double angle, const GeVector3d& _axis);


private:
    DbObjectId m_id;
    unsigned int m_index;
    GePoint3d m_gripPt;
    DbGripMode m_gripMode;
    GeScale3d m_scale;
    GeVector3d m_direction;
    GeVector3d m_rotationAxis;
};




#endif