#include "kernel.h"
#include "GiObjectManager.h"
#include "GiMaterial.h"
#include "GiPointMaterial.h"
#include "GiLineMaterial.h"
#include "GiLinePixelDashedMaterial.h"
#include "GiMeshMaterial.h"
#include "GiMaterialReactor.h"
#include "GiMaterialIterator.h"
#include "GiMaterialManager.h"
#include "EvEventManager.h"
#include "CmColor.h"
#include "GiImpl.h"


GiMaterialManager::GiMaterialManager()
{
	this->m_pImpl = new GiMaterialManagerImpl();
}
GiMaterialManager::~GiMaterialManager()
{

}
void GiMaterialManager::onBeginAnimationFrame(GiMaterialManager* pointer) {
	if (GI_IMP_MATERIALMANAGER(pointer->m_pImpl)->createMaterials.length() > 0) {
		for (int i = 0; i < GI_IMP_MATERIALMANAGER(pointer->m_pImpl)->createMaterials.length(); i++) {
			GiMaterial* pMaterial = GI_IMP_MATERIALMANAGER(pointer->m_pImpl)->createMaterials[i];
			for (int u = 0; u < GI_IMP_MATERIALMANAGER(pointer->m_pImpl)->pMaterialReactors->length(); u++) {
				GI_IMP_MATERIALMANAGER(pointer->m_pImpl)->pMaterialReactors->at(u)->createMaterial(pMaterial);
			}
		}
		GI_IMP_MATERIALMANAGER(pointer->m_pImpl)->createMaterials.removeAll();
	}
}

void GiMaterialManager::onInit() {

	::kernel()->acevEventManager()->on(Ev::RendererReactor::kBeginAnimationFrame, (void*)onBeginAnimationFrame, this);

	// 创建普通线材质
	GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultLineBeginIndex = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length();
	for (int i = 1; i < 256; i++) {
		GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->append(NULL);
	}

	// 创建高亮虚线材质
	GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultHighlightLineBeginIndex = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length();
	for (int i = 1; i < 256; i++) {

		GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->append(NULL);
	}

	// 创建锁定线材质
	GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultLockLineBeginIndex = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length();
	for (int i = 1; i < 256; i++) {
		GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->append(NULL);
	}

	// 创建普通网格材质
	GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultMeshBeginIndex = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length();
	for (int i = 1; i < 256; i++) {
		GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->append(NULL);
	}

	// 创建高亮网格材质
	GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultHighlightMeshBeginIndex = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length();
	for (int i = 1; i < 256; i++) {

		GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->append(NULL);
	}

	// 创建点材质
	GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultPointBeginIndex = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length();
	for (int i = 1; i < 256; i++) {
		GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->append(NULL);
	}

	GI_IMP_MATERIALMANAGER(this->m_pImpl)->maxInitMaterial = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length();
}
GiObjectId GiMaterialManager::addMaterial(GiMaterial* pMaterial, const bool list) const {
	::kernel()->acgiObjectManager()->addGiObject(pMaterial);
	GI_IMP_MATERIALMANAGER(this->m_pImpl)->createMaterials.append(pMaterial);
	if (list == true) {
		GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->append(pMaterial);
	}
	return pMaterial->objectId();
}
GiObjectId GiMaterialManager::addMaterial(GiMaterial* pMaterial)const {
	return this->addMaterial(pMaterial, true);
}
void GiMaterialManager::addReactor(GiMaterialReactor* pReactor) {

	GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterialReactors->append(pReactor);
	for (int i = 0; i < GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length(); i++) {
		if (GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i) != NULL) {
			pReactor->createMaterial(GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i));
		}
	}
}
GiMaterialIterator* GiMaterialManager::newIterator() {
	GiMaterialIterator* iterator = new GiMaterialIterator();
	GI_IMP_MATERIALITERATOR(iterator->m_pImpl)->iterator = AcIterator<GiMaterial*>((AcArray<GiMaterial*>*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials);
	return iterator;
}
GiMaterial* GiMaterialManager::points(const unsigned char _colorIndex, const unsigned char _transparency, const unsigned char size) {
	GiPointMaterial* pMaterial = NULL;

	do
	{

		/* 默认材质 */
		if (_transparency == 255 && (size == 0 || size == 1)) {
			pMaterial = (GiPointMaterial*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultPointBeginIndex + _colorIndex - 1);
			if (pMaterial == NULL) {
				pMaterial = new GiPointMaterial();
				pMaterial->setTransparency(0);
				pMaterial->setColorIndex(_colorIndex);
				pMaterial->setSize(size);
				this->addMaterial(pMaterial, false);
				GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->setAt(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultPointBeginIndex + _colorIndex - 1, pMaterial);
			}
			break;
		}

		/* 循环查询,如果未找到则添加 */
		for (int i = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length() - 1; i >= GI_IMP_MATERIALMANAGER(this->m_pImpl)->maxInitMaterial; i--) {
			if (Gi::MaterialType::kPointBasic == GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i)->type()) {
				GiPointMaterial* tempMaterial = (GiPointMaterial*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i);
				if (tempMaterial->colorIndex() == _colorIndex &&
					int(_transparency / 255.0 * 100) == tempMaterial->transparency()) {
					pMaterial = tempMaterial;
					break;
				}
			}
		}
		if (pMaterial == NULL) {
			pMaterial = new GiPointMaterial();
			pMaterial->setColorIndex(_colorIndex);
			pMaterial->setTransparency(int(_transparency / 255.0 * 100));
			pMaterial->setSize(size);
			this->addMaterial(pMaterial, true);
		}

	} while (false);

	return pMaterial;
}
GiMaterial* GiMaterialManager::lineSegments(const unsigned char _colorIndex, const unsigned char _transparency, double _lineWidth) {
	GiLineMaterial* pMaterial = NULL;

	do
	{

		/* 默认材质 */
		if (_transparency == 255 && _lineWidth < 1.002) {
			pMaterial = (GiLineMaterial*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultLineBeginIndex + _colorIndex - 1);
			if (pMaterial == NULL) {
				pMaterial = new GiLineMaterial();
				pMaterial->setLineWidth(1.0);
				pMaterial->setTransparency(0);
				pMaterial->setColorIndex(_colorIndex);
				this->addMaterial(pMaterial, false);
				GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->setAt(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultLineBeginIndex + _colorIndex - 1, pMaterial);
			}
			break;
		}
		else if (_transparency == 127 && _lineWidth < 1.002) {
			pMaterial = (GiLineMaterial*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultLockLineBeginIndex + _colorIndex - 1);
			if (pMaterial == NULL) {
				pMaterial = new GiLineMaterial();
				pMaterial->setLineWidth(1.0);
				pMaterial->setTransparency(50);
				pMaterial->setColorIndex(_colorIndex);
				this->addMaterial(pMaterial, false);
				GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->setAt(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultLockLineBeginIndex + _colorIndex - 1, pMaterial);
			}
			break;
		}

		/* 循环查询,如果未找到则添加 */
		for (int i = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length() - 1; i >= GI_IMP_MATERIALMANAGER(this->m_pImpl)->maxInitMaterial; i--) {
			if (Gi::MaterialType::kLineBasic == GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i)->type()) {
				GiLineMaterial* tempMaterial = (GiLineMaterial*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i);
				if (abs(tempMaterial->lineWidth() - _lineWidth) < 0.02 &&
					tempMaterial->colorIndex() == _colorIndex &&
					int(_transparency / 255.0 * 100) == tempMaterial->transparency()) {
					pMaterial = tempMaterial;
					break;
				}
			}
		}
		if (pMaterial == NULL) {
			pMaterial = new GiLineMaterial();
			pMaterial->setColorIndex(_colorIndex);
			pMaterial->setTransparency(int(_transparency / 255.0 * 100));
			pMaterial->setLineWidth(_lineWidth);
			this->addMaterial(pMaterial);
		}

	} while (false);

	return pMaterial;
}
GiMaterial* GiMaterialManager::linePixelDashed(const unsigned char _colorIndex, double _dashSize, double _gapSize, const unsigned char _transparency, double _lineWidth) {

	GiLinePixelDashedMaterial* pMaterial = NULL;

	do
	{
		/* 循环查询,如果未找到则添加 */
		for (int i = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length() - 1; i >= GI_IMP_MATERIALMANAGER(this->m_pImpl)->maxInitMaterial; i--) {
			if (Gi::MaterialType::kLinePixelDashed == GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i)->type() &&
				_colorIndex == GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i)->colorIndex()) {
				GiLinePixelDashedMaterial* tempMaterial = (GiLinePixelDashedMaterial*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i);
				if (abs(tempMaterial->lineWidth() - _lineWidth) < 0.0002 &&
					abs(tempMaterial->dashSize() - _dashSize) < 0.0002 &&
					abs(tempMaterial->gapSize() - _gapSize) < 0.0002) {
					pMaterial = tempMaterial;
					break;
				}
			}
		}
		if (pMaterial == NULL) {
			pMaterial = new GiLinePixelDashedMaterial();
			pMaterial->setColorIndex(_colorIndex);
			pMaterial->setTransparency(_transparency);
			pMaterial->setLineWidth(_lineWidth);
			pMaterial->setDashSize(_dashSize);
			pMaterial->setGapSize(_gapSize);
			this->addMaterial(pMaterial);
		}

	} while (false);

	return pMaterial;
}
GiMaterial* GiMaterialManager::highlightLineSegments(const unsigned char _colorIndex, double _lineWidth) {

	GiLinePixelDashedMaterial* pMaterial = NULL;

	do
	{

		/* 默认材质 */
		if (_lineWidth < 1.002) {
			pMaterial = (GiLinePixelDashedMaterial*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultHighlightLineBeginIndex + _colorIndex - 1);
			if (pMaterial == NULL) {
				pMaterial = new GiLinePixelDashedMaterial();
				pMaterial->setLineWidth(1.0);
				pMaterial->setTransparency(20);
				pMaterial->setColorIndex(_colorIndex);
				pMaterial->setDashSize(2.0);
				pMaterial->setGapSize(2.0);
				this->addMaterial(pMaterial, false);
				GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->setAt(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultHighlightLineBeginIndex + _colorIndex - 1, pMaterial);
			}
			break;
		}

		/* 循环查询,如果未找到则添加 */
		for (int i = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length() - 1; i >= GI_IMP_MATERIALMANAGER(this->m_pImpl)->maxInitMaterial; i--) {
			if (Gi::MaterialType::kLinePixelDashed == GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i)->type()) {
				GiLinePixelDashedMaterial* tempMaterial = (GiLinePixelDashedMaterial*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i);
				if (_colorIndex == tempMaterial->colorIndex() &&
					abs(_lineWidth - tempMaterial->lineWidth()) < 0.002 &&
					tempMaterial->transparency() == 80)
				{
					pMaterial = tempMaterial;
					break;
				}
			}
		}
		if (pMaterial == NULL) {
			pMaterial = new GiLinePixelDashedMaterial();
			pMaterial->setColorIndex(_colorIndex);
			pMaterial->setTransparency(80);
			pMaterial->setLineWidth(_lineWidth);
			this->addMaterial(pMaterial);
		}

	} while (false);

	return pMaterial;
}
GiMaterial* GiMaterialManager::meshBasic(const unsigned char _colorIndex, const unsigned char _transparency) {
	GiMaterial* pMaterial = NULL;

	do
	{
		/* 默认材质 */
		if (_transparency == 255) {
			pMaterial = (GiMeshMaterial*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultMeshBeginIndex + _colorIndex - 1);
			if (pMaterial == NULL) {
				pMaterial = new GiMeshMaterial();
				pMaterial->setTransparency(0);
				pMaterial->setColorIndex(_colorIndex);
				this->addMaterial(pMaterial, false);
				GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->setAt(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultMeshBeginIndex + _colorIndex - 1, pMaterial);
			}
			break;
		}

		for (int i = GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->length() - 1; i >= GI_IMP_MATERIALMANAGER(this->m_pImpl)->maxInitMaterial; i--) {
			if (Gi::MaterialType::kMeshBasic == GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i)->type()) {
				GiMeshMaterial* tempMaterial = (GiMeshMaterial*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(i);
				if (tempMaterial->colorIndex() == _colorIndex &&
					tempMaterial->transparency() == _transparency) {
					pMaterial = tempMaterial;
					break;
				}
			}
		}
		if (pMaterial == NULL) {

			pMaterial = new GiMeshMaterial();
			pMaterial->setTransparency(_transparency);
			pMaterial->setColorIndex(_colorIndex);
			this->addMaterial(pMaterial);
		}

	} while (false);

	return pMaterial;
}
GiMaterial* GiMaterialManager::highlightMeshBasic(const unsigned char _colorIndex) {
	GiMaterial* pMaterial = NULL;

	do
	{
		/* 默认材质 */
		pMaterial = (GiMeshMaterial*)GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->at(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultHighlightMeshBeginIndex + _colorIndex - 1);
		if (pMaterial == NULL) {
			pMaterial = new GiMeshMaterial();
			pMaterial->setTransparency(50);
			pMaterial->setColorIndex(_colorIndex);
			this->addMaterial(pMaterial, false);
			GI_IMP_MATERIALMANAGER(this->m_pImpl)->pMaterials->setAt(GI_IMP_MATERIALMANAGER(this->m_pImpl)->defaultHighlightMeshBeginIndex + _colorIndex - 1, pMaterial);
		}
	} while (false);

	return pMaterial;
}