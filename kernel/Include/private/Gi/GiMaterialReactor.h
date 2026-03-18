#ifndef GIMATERIALREACTOR_H
#define GIMATERIALREACTOR_H


#include "export.h"
#include "GiMaterial.h"


class GiMaterialReactor : public RxObject {
public:
	RX_DECLARE_MEMBERS(GiMaterialReactor);

	virtual void createMaterial(GiMaterial* pMaterial);
	virtual void removeMaterial(GiObjectId id);

#if EMSDK
	GiMaterialReactor();
	virtual ~GiMaterialReactor();

	void onCreateMaterial(int ptr);
	void onRemoveMaterial(int ptr);
#endif
};



#endif