#include "export.h"
#include "RxExport.h"
#include "AcIterator.h"
#include "RxObject.h"
#include "Gi.h"
#include "GiObjectId.h"
#include "GiMaterial.h"
#include "GiDrawable.h"
#include "GiLineMaterial.h"
#include "GiLineDashedMaterial.h"
#include "GiLinePixelDashedMaterial.h"
#include "GiMeshMaterial.h"
#include "GiPointMaterial.h"
#include "GiMaterialReactor.h"
#include "GiMaterialManager.h"
#include "GiMaterialIterator.h"
#include "GiPoints.h"
#include "GiLineSegment.h"
#include "GiMesh.h"
#include "GiEntityReactor.h"
#include "GiEntityManager.h"
#include "GiEntityIterator.h"


#if EMSDK




EMSCRIPTEN_BINDINGS(Gi) {


	//MaterialType
	enum_<Gi::MaterialType>("MaterialType")
		.value("kUnknownMaterial", Gi::kUnknownMaterial)
		.value("kPointBasic", Gi::kPointBasic)
		.value("kLineBasic", Gi::kLineBasic)
		.value("kLineDashed", Gi::kLineDashed)
		.value("kLinePixelDashed", Gi::kLinePixelDashed)
		.value("kMeshBasic", Gi::kMeshBasic);

	//GiObjectId
	emscripten::class_<GiObjectId>("GiObjectId")
		.constructor()
		.function("isNull", &GiObjectId::isNull)
		.function("setNull", &GiObjectId::setNull)
		.function("asOldId", &GiObjectId::asOldId)
		.function("setFromOldId", &GiObjectId::setFromOldId)
		.function("isValid", &GiObjectId::isValid);

	//GiObject
	emscripten::class_<GiObject, base<RxObject>>("GiObject")
		RX_EMSDK_EXPORT_METHODS(GiObject)
		.function("objectId", &GiObject::objectId);

	//GiDrawable
	emscripten::class_<GiDrawable, base<RxObject>>("GiDrawable")
		RX_EMSDK_EXPORT_METHODS(GiDrawable);

	//GiMaterial
	emscripten::class_<GiMaterial, base<GiObject>>("GiMaterial")
		RX_EMSDK_EXPORT_METHODS(GiMaterial)
		.function("type", &GiMaterial::type)
		.function("color", &GiMaterial::color)
		.function("colorIndex", &GiMaterial::colorIndex)
		.function("transparency", &GiMaterial::transparency)
		.function("setColor", &GiMaterial::setColor)
		.function("setColorIndex", &GiMaterial::setColorIndex)
		.function("setTransparency", &GiMaterial::setTransparency);

	//GiPointMaterial
	emscripten::class_<GiPointMaterial, base<GiMaterial>>("GiPointMaterial")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(GiPointMaterial)
		.function("type", &GiPointMaterial::type)
		.function("size", &GiPointMaterial::size)
		.function("setSize", &GiPointMaterial::setSize);

	//GiLineMaterial
	emscripten::class_<GiLineMaterial, base<GiMaterial>>("GiLineMaterial")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(GiLineMaterial)
		.function("type", &GiLineMaterial::type)
		.function("lineWidth", &GiLineMaterial::lineWidth)
		.function("setLineWidth", &GiLineMaterial::setLineWidth);

	//GiLineDashedMaterial
	emscripten::class_<GiLineDashedMaterial, base<GiMaterial>>("GiLineDashedMaterial")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(GiLineDashedMaterial)
		.function("type", &GiLineDashedMaterial::type)
		.function("lineWidth", &GiLineDashedMaterial::lineWidth)
		.function("setLineWidth", &GiLineDashedMaterial::setLineWidth)
		.function("dashSize", &GiLineDashedMaterial::dashSize)
		.function("setDashSize", &GiLineDashedMaterial::setDashSize)
		.function("gapSize", &GiLineDashedMaterial::gapSize)
		.function("setGapSize", &GiLineDashedMaterial::setGapSize);

	//GiLinePixelDashedMaterial
	emscripten::class_<GiLinePixelDashedMaterial, base<GiMaterial>>("GiLinePixelDashedMaterial")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(GiLinePixelDashedMaterial)
		.function("type", &GiLinePixelDashedMaterial::type)
		.function("lineWidth", &GiLinePixelDashedMaterial::lineWidth)
		.function("setLineWidth", &GiLinePixelDashedMaterial::setLineWidth)
		.function("dashSize", &GiLinePixelDashedMaterial::dashSize)
		.function("setDashSize", &GiLinePixelDashedMaterial::setDashSize)
		.function("gapSize", &GiLinePixelDashedMaterial::gapSize)
		.function("setGapSize", &GiLinePixelDashedMaterial::setGapSize)
		.function("pixelRatio", &GiLinePixelDashedMaterial::pixelRatio)
		.function("setPixelRatio", &GiLinePixelDashedMaterial::setPixelRatio);

	//GiMeshMaterial
	emscripten::class_<GiMeshMaterial, base<GiMaterial>>("GiMeshMaterial")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(GiMeshMaterial)
		.function("type", &GiMeshMaterial::type);

	//GiMaterialManager
	emscripten::class_<GiMaterialManager, base<RxObject>>("GiMaterialManager")
		.constructor<>()
		.function("newIterator", &GiMaterialManager::newIterator, allow_raw_pointers())
		.function("addReactor", &GiMaterialManager::addReactor, allow_raw_pointers());

	//GiMaterialReactor
	emscripten::class_<GiMaterialReactor, base<RxObject>>("GiMaterialReactor")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(GiMaterialReactor)
		.function("onCreateMaterial", select_overload<void(int)>(&GiMaterialReactor::onCreateMaterial), allow_raw_pointers())
		.function("onRemoveMaterial", select_overload<void(int)>(&GiMaterialReactor::onRemoveMaterial), allow_raw_pointers());

	//GiMaterialIterator
	emscripten::class_<GiMaterialIterator, base<RxObject>>("GiMaterialIterator")
		RX_EMSDK_EXPORT_METHODS(GiMaterialIterator)
		.function("start", &GiMaterialIterator::start)
		.function("getMaterial", select_overload<GiMaterial * ()>(&GiMaterialIterator::getMaterial), allow_raw_pointers())
		.function("step", &GiMaterialIterator::step)
		.function("done", &GiMaterialIterator::done);


	//EntityType
	enum_<Gi::EntityType>("EntityType")
		.value("kUnknownEntity", Gi::kUnknownEntity)
		.value("kPoints", Gi::kPoints)
		.value("kLineSegment", Gi::kLineSegment)
		.value("kMesh", Gi::kMesh)
		.value("kLinear", Gi::kLinear)
		.value("kRay", Gi::kRay);

	//ElementType
	enum_<Gi::ElementType>("ElementType")
		.value("kUnknownElement", Gi::kUnknownElement)
		.value("kElement", Gi::kElement)
		.value("kDragging", Gi::kDragging)
		.value("kHighlight", Gi::kHighlight)
		.value("kCursor", Gi::kCursor)
		.value("kCoordSystem", Gi::kCoordSystem)
		.value("kGrip", Gi::kGrip)
		.value("kOsnap", Gi::kOsnap)
		.value("kGridline", Gi::kGridline)
		.value("kDynDim", Gi::kDynDim);

	//GiEntity
	emscripten::class_<GiEntity, base<GiObject>>("GiEntity")
		RX_EMSDK_EXPORT_METHODS(GiEntity)
		.function("type", &GiEntity::type)
		.function("element", &GiEntity::element)
		.function("setElement", &GiEntity::setElement)
		.function("vertexLength", &GiEntity::vertexLength)
		.function("vertexs", &GiEntity::vertexs)
		.function("visibility", &GiEntity::visibility)
		.function("material", &GiEntity::material, allow_raw_pointers())
		.function("matrix", &GiEntity::matrix)
		.function("setMatrix", &GiEntity::setMatrix)
		.function("applyMatrix", &GiEntity::applyMatrix)
		.function("quaternion", &GiEntity::quaternion)
		.function("position", &GiEntity::position)
		.function("setVisibility", &GiEntity::setVisibility)
		.function("setMaterial", &GiEntity::setMaterial, allow_raw_pointers())
		.function("setPosition", &GiEntity::setPosition);

	//GiPoints
	emscripten::class_<GiPoints, base<GiEntity>>("GiPoints")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(GiPoints)
		.function("vertexs", &GiPoints::vertexs)
		.function("type", &GiPoints::type);

	//GiLineSegment
	emscripten::class_<GiLineSegment, base<GiEntity>>("GiLineSegment")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(GiLineSegment)
		.function("vertexs", &GiLineSegment::vertexs)
		.function("type", &GiLineSegment::type);

	//GiMesh
	emscripten::class_<GiMesh, base<GiEntity>>("GiMesh")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(GiMesh)
		.function("vertexs", &GiMesh::vertexs)
		.function("type", &GiMesh::type);

	//GiEntityManager
	emscripten::class_<GiEntityManager, base<RxObject>>("GiEntityManager")
		.constructor<>()
		.function("newIterator", &GiEntityManager::newIterator, allow_raw_pointers())
		.function("addEntity", &GiEntityManager::addEntity, allow_raw_pointers())
		.function("addReactor", &GiEntityManager::addReactor, allow_raw_pointers());

	//GiEntityReactor
	emscripten::class_<GiEntityReactor, base<RxObject>>("GiEntityReactor")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(GiEntityReactor)
		.function("onCreateEntity", select_overload<void(int)>(&GiEntityReactor::onCreateEntity), allow_raw_pointers())
		.function("onUpdateVertexs", select_overload<void(int)>(&GiEntityReactor::onUpdateVertexs), allow_raw_pointers())
		.function("onTransformation", select_overload<void(int)>(&GiEntityReactor::onTransformation), allow_raw_pointers())
		.function("onVisibility", select_overload<void(int)>(&GiEntityReactor::onVisibility), allow_raw_pointers());

	//GiEntityIterator
	emscripten::class_<GiEntityIterator, base<RxObject>>("GiEntityIterator")
		RX_EMSDK_EXPORT_METHODS(GiEntityIterator)
		.function("start", &GiEntityIterator::start)
		.function("getEntity", select_overload<GiEntity* ()>(&GiEntityIterator::getEntity), allow_raw_pointers())
		.function("step", &GiEntityIterator::step)
		.function("done", &GiEntityIterator::done);
}



#endif
