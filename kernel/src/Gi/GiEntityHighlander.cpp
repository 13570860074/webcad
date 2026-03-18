
#include "GiEntityHighlander.h"
#include "GiMaterialManager.h"
#include "GiEntityManager.h"
#include "GiLineSegment.h"
#include "GiMesh.h"
#include "DbEntity.h"
#include "ApDocManager.h"
#include "kernel.h"
#include "DbImpl.h"
#include "GiImpl.h"

GiEntityHighlander::GiEntityHighlander()
{
	this->m_pImpl = new GiEntityHighlanderImpl();
}
GiEntityHighlander::~GiEntityHighlander()
{
}

void GiEntityHighlander::highlight(DbEntity* entity)
{
	DB_IMP_ENTITY(entity->m_pImpl)->highlight = true;

	/** 获得几何对象 */
	AcArray<GiEntityGeometry*> geometrys = DB_IMP_ENTITY(entity->m_pImpl)->entitys;

	/* 获得当前文档 */
	ApDocument* document = ::curDoc();

	/** 创建高亮几何对象 */
	for (int i = 0; i < geometrys.length(); i++)
	{
		/** 获得几何对象的对应的实体 */
		GiEntity* giEntity = geometrys[i]->giEntity();
		if (giEntity == NULL) {
			continue;
		}
		if (giEntity->material() == NULL) {
			continue;
		}

		/* 获得对应的高亮材质 */
		GiMaterial* material = NULL;
		if (giEntity->type() == Gi::EntityType::kLineSegment)
		{
			int colorIndex = giEntity->material()->colorIndex();
			material = ::kernel()->acgiMaterialManager()->highlightLineSegments(colorIndex, 0);
		}
		else if (giEntity->type() == Gi::EntityType::kMesh)
		{
			int colorIndex = giEntity->material()->colorIndex();
			material = ::kernel()->acgiMaterialManager()->highlightMeshBasic(colorIndex);
		}
		if (material == NULL) {
			continue;
		}

		/* 获得实体 */
		if (material == GI_IMP_ENTITYHIGHLANDER(this->m_pImpl)->lastMaterial)
		{
			giEntity = GI_IMP_ENTITYHIGHLANDER(this->m_pImpl)->lastEntity;
		}
		else
		{
			/* 找到实体 */
			if (material->type() == Gi::MaterialType::kLinePixelDashed) {
				giEntity = ::kernel()->acgiEntityManager()->queryEntity(Gi::EntityType::kLineSegment, Gi::ElementType::kHighlight, material);
			}
			else if (material->type() == Gi::MaterialType::kMeshBasic) {
				giEntity = ::kernel()->acgiEntityManager()->queryEntity(Gi::EntityType::kMesh, Gi::ElementType::kHighlight, material);
			}

			/* 如果未找到则创建 */
			if (giEntity == NULL && material->type() == Gi::MaterialType::kLinePixelDashed)
			{
				giEntity = new GiLineSegment();
				giEntity->setElement(Gi::ElementType::kHighlight);
				giEntity->setMaterial(material);
				::kernel()->acgiEntityManager()->addEntity(giEntity);
			}
			else if (giEntity == NULL && material->type() == Gi::MaterialType::kMeshBasic)
			{
				giEntity = new GiMesh();
				giEntity->setElement(Gi::ElementType::kHighlight);
				giEntity->setMaterial(material);
				::kernel()->acgiEntityManager()->addEntity(giEntity);
			}
		}

		/* 添加几何实体 */
		if (material->type() == Gi::MaterialType::kLinePixelDashed)
		{
			GiEntityGeometry* geometry = geometrys[i]->clone();
			((GiLineSegment*)giEntity)->appendLineSegment((GiLineSegmentGeometry*)geometry);
			DB_IMP_ENTITY(entity->m_pImpl)->highlightEntitys.append(geometry);
		}
		else if (material->type() == Gi::MaterialType::kMeshBasic)
		{
			GiEntityGeometry* geometry = geometrys[i]->clone();
			((GiMesh*)giEntity)->appendTriangleMeshGeometry((GiTriangleMeshGeometry*)geometry);
			DB_IMP_ENTITY(entity->m_pImpl)->highlightEntitys.append(geometry);
		}

		GI_IMP_ENTITYHIGHLANDER(this->m_pImpl)->lastMaterial = material;
		GI_IMP_ENTITYHIGHLANDER(this->m_pImpl)->lastEntity = giEntity;
	}

	/** 隐藏 */
	for (int i = 0; i < geometrys.length(); i++) {
		geometrys[i]->setVisibility(false);
	}
}
void GiEntityHighlander::unhighlight(DbEntity* entity)
{
	DB_IMP_ENTITY(entity->m_pImpl)->highlight = false;

	AcArray<GiEntityGeometry*> geometrys = DB_IMP_ENTITY(entity->m_pImpl)->entitys;
	for (int i = 0; i < geometrys.length(); i++)
	{
		geometrys[i]->setVisibility(true);
	}
	geometrys = DB_IMP_ENTITY(entity->m_pImpl)->highlightEntitys;
	for (int i = 0; i < geometrys.length(); i++)
	{
		geometrys[i]->erased();
	}
	DB_IMP_ENTITY(entity->m_pImpl)->highlightEntitys.removeAll();
}