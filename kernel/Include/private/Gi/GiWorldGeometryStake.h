#ifndef GIWORLDGEOMETRYSTAKE_H
#define GIWORLDGEOMETRYSTAKE_H


class DbEntity;
class GiMaterial;
class GiEntityGeometry;
class GiWorldGeometryStake
{
public:
    GiWorldGeometryStake();
    virtual ~GiWorldGeometryStake();

private:
    DbEntity* m_entity;
    GiMaterial* m_material;
    GiEntityGeometry* m_geometry;
    

public:
    GiMaterial* material()const;
    void setMaterial(GiMaterial * material);

    DbEntity* entity()const;
    void setEntity(DbEntity* ent);

    GiEntityGeometry* geometry()const;
    void setGeometry(GiEntityGeometry* g);

    /** 归还对象池前重置状态 */
    void poolReset();
};

#endif