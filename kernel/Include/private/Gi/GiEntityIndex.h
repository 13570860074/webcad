#ifndef GIENTITYINDEX_H
#define GIENTITYINDEX_H

#include "GiEntity.h"


class GiEntityIndexPointerMaterial :public AcArray<GiEntity*> {
public:
	GiEntityIndexPointerMaterial() {
		this->materialId = 0;
	}

	unsigned int materialId;
	AcArray<GiEntity*> entitys;
};
class GiEntityIndexPointerElement :public AcArray<GiEntityIndexPointerMaterial*> {
public:
	GiEntityIndexPointerElement() {
		this->element = Gi::ElementType::kElement;
	}

	Gi::ElementType element;
};
class GiEntityIndexPointerType :public AcArray<GiEntityIndexPointerElement*> {
public:
	GiEntityIndexPointerType() {
		this->type = Gi::EntityType::kLineSegment;

		GiEntityIndexPointerElement* entityIndex = new GiEntityIndexPointerElement();
		entityIndex->element = Gi::ElementType::kElement;
		this->append(entityIndex);

		entityIndex = new GiEntityIndexPointerElement();
		entityIndex->element = Gi::ElementType::kDragging;
		this->append(entityIndex);

		entityIndex = new GiEntityIndexPointerElement();
		entityIndex->element = Gi::ElementType::kHighlight;
		this->append(entityIndex);

		entityIndex = new GiEntityIndexPointerElement();
		entityIndex->element = Gi::ElementType::kCursor;
		this->append(entityIndex);

		entityIndex = new GiEntityIndexPointerElement();
		entityIndex->element = Gi::ElementType::kCoordSystem;
		this->append(entityIndex);

		entityIndex = new GiEntityIndexPointerElement();
		entityIndex->element = Gi::ElementType::kGrip;
		this->append(entityIndex);

		entityIndex = new GiEntityIndexPointerElement();
		entityIndex->element = Gi::ElementType::kOsnap;
		this->append(entityIndex);

		entityIndex = new GiEntityIndexPointerElement();
		entityIndex->element = Gi::ElementType::kGridline;
		this->append(entityIndex);

		entityIndex = new GiEntityIndexPointerElement();
		entityIndex->element = Gi::ElementType::kDynDim;
		this->append(entityIndex);
	}

	Gi::EntityType type;
};
class GiEntityIndexPointerTypeArray :public AcArray<GiEntityIndexPointerType*> {
public:
	GiEntityIndexPointerTypeArray() {
		GiEntityIndexPointerType* entityIndexPointerType = new GiEntityIndexPointerType();
		entityIndexPointerType->type = Gi::EntityType::kPoints;
		this->append(entityIndexPointerType);

		entityIndexPointerType = new GiEntityIndexPointerType();
		entityIndexPointerType->type = Gi::EntityType::kLineSegment;
		this->append(entityIndexPointerType);

		entityIndexPointerType = new GiEntityIndexPointerType();
		entityIndexPointerType->type = Gi::EntityType::kMesh;
		this->append(entityIndexPointerType);

		entityIndexPointerType = new GiEntityIndexPointerType();
		entityIndexPointerType->type = Gi::EntityType::kLinear;
		this->append(entityIndexPointerType);

		entityIndexPointerType = new GiEntityIndexPointerType();
		entityIndexPointerType->type = Gi::EntityType::kRay;
		this->append(entityIndexPointerType);
	}
	virtual ~GiEntityIndexPointerTypeArray() {

	}
};


#endif