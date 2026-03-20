#ifndef GEMATRIX3DSTACK_H
#define GEMATRIX3DSTACK_H

#include "AcArray.h"
#include "GeMatrix3d.h"

class GeMatrix3dStack
{
public:
    GeMatrix3dStack() {}

    bool empty() const
    {
        return m_data.isEmpty();
    }

    int size() const
    {
        return m_data.length();
    }

    void clear()
    {
        m_data.removeAll();
    }

    void push(const GeMatrix3d& value)
    {
        m_data.append(value);
    }

    GeMatrix3d pop()
    {
        if (m_data.isEmpty()) {
            return GeMatrix3d::kIdentity;
        }

        GeMatrix3d value = m_data.last();
        m_data.removeLast();
        return value;
    }

    GeMatrix3d& top()
    {
        if (m_data.isEmpty()) {
            m_data.append(GeMatrix3d::kIdentity);
        }
        return m_data.last();
    }

    const GeMatrix3d& top() const
    {
        if (m_data.isEmpty()) {
            return GeMatrix3d::kIdentity;
        }
        return m_data.last();
    }

private:
    AcArray<GeMatrix3d> m_data;
};

#endif
