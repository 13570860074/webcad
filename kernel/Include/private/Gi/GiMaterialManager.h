#ifndef GIMATERIALMANAGER_H
#define GIMATERIALMANAGER_H

#include "Gi.h"
#include "RxManager.h"
#include "CmColor.h"


class GiMaterialReactor;
class GiMaterialIterator;
class GiMaterialManager : public RxObject, public RxManager {
public:
	GiMaterialManager();
	virtual ~GiMaterialManager();
	RX_DECLARE_MEMBERS(GiMaterialManager);
	RX_MANAGER_DECLARE_MEMBERS(GiMaterialManager);

private:
	static void onBeginAnimationFrame(GiMaterialManager* pointer);

	/* 添加材质 */
	GiObjectId addMaterial(GiMaterial* pMaterial, const bool list)const;

public:

	/* 初始化 */
	virtual void onInit();

	/* 添加反应器 */
	void addReactor(GiMaterialReactor* pReactor);

	/* 添加材质 */
	GiObjectId addMaterial(GiMaterial*)const;

	/* 遍历材质 */
	GiMaterialIterator* newIterator();

	/* 获得点材质 */
	GiMaterial* points(const unsigned char _colorIndex, const unsigned char _transparency, const unsigned char size);

	/* 获得线材质 */
	GiMaterial* lineSegments(const unsigned char _colorIndex, const unsigned char _transparency, double _lineWidth);

	/* 获得高亮显示的线材质 */
	GiMaterial* highlightLineSegments(const unsigned char _colorIndex, double _lineWidth);

	/* 获得LinePixelDashed材质 */
	GiMaterial* linePixelDashed(const unsigned char _colorIndex, double _dashSize, double _gapSize, const unsigned char _transparency, double _lineWidth);

	/* 获得网格材质 */
	GiMaterial* meshBasic(const unsigned char _colorIndex, const unsigned char _transparency);

	/* 获得高亮网格材质 */
	GiMaterial* highlightMeshBasic(const unsigned char _colorIndex);
};




#endif