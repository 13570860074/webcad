#include "Db.h"
#include "export.h"
#include "EmObject.h"
#include "AcIterator.h"
#include "RxObject.h"
#include "arxHeaders.h"
#include "RxExport.h"
#include "AcArrayExport.h"

#if EMSDK

#define DB_EMSDK_EXPORT_METHODS(CLASSNAME) \
	RX_EMSDK_EXPORT_METHODS(CLASSNAME)

EMSCRIPTEN_BINDINGS(Db)
{

	// OpenMode
	enum_<Db::OpenMode>("OpenMode")
		.value("kNotOpen", Db::kNotOpen)
		.value("kForRead", Db::kForRead)
		.value("kForWrite", Db::kForWrite)
		.value("kForNotify", Db::kForNotify);

	enum_<Db::LineSpacingStyle>("LineSpacingStyle")
		.value("kAtLeast", Db::kAtLeast)
		.value("kExactly", Db::kExactly);

	enum_<Db::LineWeight>("LineWeight")
		.value("kLnWt000", Db::LineWeight::kLnWt000)
		.value("kLnWt005", Db::LineWeight::kLnWt005)
		.value("kLnWt009", Db::LineWeight::kLnWt009)
		.value("kLnWt013", Db::LineWeight::kLnWt013)
		.value("kLnWt015", Db::LineWeight::kLnWt015)
		.value("kLnWt018", Db::LineWeight::kLnWt018)
		.value("kLnWt020", Db::LineWeight::kLnWt020)
		.value("kLnWt025", Db::LineWeight::kLnWt025)
		.value("kLnWt030", Db::LineWeight::kLnWt030)
		.value("kLnWt035", Db::LineWeight::kLnWt035)
		.value("kLnWt040", Db::LineWeight::kLnWt040)
		.value("kLnWt050", Db::LineWeight::kLnWt050)
		.value("kLnWt053", Db::LineWeight::kLnWt053)
		.value("kLnWt060", Db::LineWeight::kLnWt060)
		.value("kLnWt070", Db::LineWeight::kLnWt070)
		.value("kLnWt080", Db::LineWeight::kLnWt080)
		.value("kLnWt090", Db::LineWeight::kLnWt090)
		.value("kLnWt100", Db::LineWeight::kLnWt100)
		.value("kLnWt106", Db::LineWeight::kLnWt106)
		.value("kLnWt120", Db::LineWeight::kLnWt120)
		.value("kLnWt140", Db::LineWeight::kLnWt140)
		.value("kLnWt158", Db::LineWeight::kLnWt158)
		.value("kLnWt200", Db::LineWeight::kLnWt200)
		.value("kLnWt211", Db::LineWeight::kLnWt211)
		.value("kLnWtByLayer", Db::LineWeight::kLnWtByLayer)
		.value("kLnWtByBlock", Db::LineWeight::kLnWtByBlock)
		.value("kLnWtByLwDefault", Db::LineWeight::kLnWtByLwDefault);

	// AttachmentPoint
	enum_<DbMText::AttachmentPoint>("AttachmentPoint")
		.value("kTopLeft", DbMText::AttachmentPoint::kTopLeft)
		.value("kTopCenter", DbMText::AttachmentPoint::kTopCenter)
		.value("kTopRight", DbMText::AttachmentPoint::kTopRight)
		.value("kMiddleLeft", DbMText::AttachmentPoint::kMiddleLeft)
		.value("kMiddleCenter", DbMText::AttachmentPoint::kMiddleCenter)
		.value("kMiddleRight", DbMText::AttachmentPoint::kMiddleRight)
		.value("kBottomLeft", DbMText::AttachmentPoint::kBottomLeft)
		.value("kBottomCenter", DbMText::AttachmentPoint::kBottomCenter)
		.value("kBottomRight", DbMText::AttachmentPoint::kBottomRight)
		.value("kBaseLeft", DbMText::AttachmentPoint::kBaseLeft)
		.value("kBaseCenter", DbMText::AttachmentPoint::kBaseCenter)
		.value("kBaseRight", DbMText::AttachmentPoint::kBaseRight)
		.value("kBaseAlign", DbMText::AttachmentPoint::kBaseAlign)
		.value("kBottomAlign", DbMText::AttachmentPoint::kBottomAlign)
		.value("kMiddleAlign", DbMText::AttachmentPoint::kMiddleAlign)
		.value("kTopAlign", DbMText::AttachmentPoint::kTopAlign)
		.value("kBaseFit", DbMText::AttachmentPoint::kBaseFit)
		.value("kBottomFit", DbMText::AttachmentPoint::kBottomFit)
		.value("kMiddleFit", DbMText::AttachmentPoint::kMiddleFit)
		.value("kTopFit", DbMText::AttachmentPoint::kTopFit)
		.value("kBaseMid", DbMText::AttachmentPoint::kBaseMid)
		.value("kBottomMid", DbMText::AttachmentPoint::kBottomMid)
		.value("kMiddleMid", DbMText::AttachmentPoint::kMiddleMid)
		.value("kTopMid", DbMText::AttachmentPoint::kTopMid);

	// DbHandle
	emscripten::class_<DbHandle>("DbHandle")
		.constructor()
		.constructor<unsigned int>()
		.function("isNull", &DbHandle::isNull)
		.function("getIntoAsciiBuffer", select_overload<std::string() const>(&DbHandle::emsdk_getIntoAsciiBuffer));

	// DbHostApplicationServices
	emscripten::class_<DbHostApplicationServices>("DbHostApplicationServices")
		.constructor()
		.function("workingDatabase", &DbHostApplicationServices::workingDatabase, allow_raw_pointers())
		.function("setWorkingDatabase", &DbHostApplicationServices::setWorkingDatabase, allow_raw_pointers())
		.function("load_dxf_data", &DbHostApplicationServices::emsdk_load_dxf_data, allow_raw_pointers())
		.function("layoutManager", &DbHostApplicationServices::layoutManager, allow_raw_pointers());

	// DbObjectId
	emscripten::class_<DbObjectId>("DbObjectId")
		.constructor()
		.class_function("cast", &DbObjectId::cast)
		.function("isNull", &DbObjectId::isNull)
		.function("setNull", &DbObjectId::setNull)
		.function("asOldId", &DbObjectId::asOldId)
		.function("setFromOldId", &DbObjectId::setFromOldId)
		.function("isValid", &DbObjectId::isValid)
		.function("isErased", &DbObjectId::isErased)
		.function("handle", &DbObjectId::handle);

	// DbObjectIdArray
	emscripten::class_<DbObjectIdArray>("DbObjectIdArray")
		.constructor()
		ACARRAY_EMSDK_EXPORT_METHODS(DbObjectIdArray, DbObjectId);

	// DbObject
	emscripten::class_<DbObject, base<GiDrawable>>("DbObject")
		.function("objectId", &DbObject::objectId)
		.function("ownerId", &DbObject::ownerId)
		.function("setOwnerId", &DbObject::setOwnerId)
		.function("getDbHandle", &DbObject::getDbHandle)
		.function("database", &DbObject::database, allow_raw_pointers())
		.function("subOpen", &DbObject::subOpen)
		.function("close", &DbObject::close)
		.function("subClose", &DbObject::subClose)
		.function("erase", select_overload<bool()>(&DbObject::erase), allow_raw_pointers())
		.function("erase", select_overload<bool(bool eraseIt)>(&DbObject::erase), allow_raw_pointers())
		.function("subErase", &DbObject::subErase)
		.function("isErased", &DbObject::isErased)
		.function("isReadEnabled", &DbObject::isReadEnabled)
		.function("isWriteEnabled", &DbObject::isWriteEnabled)
		.function("isNotifyEnabled", &DbObject::isNotifyEnabled)
		.function("isModified", &DbObject::isModified)
		.function("clone", &DbObject::clone, allow_raw_pointers())
		.function("mergeStyle", &DbObject::mergeStyle);

	// DbEntity
	emscripten::class_<DbEntity, base<DbObject>>("DbEntity")
		.function("blockId", &DbEntity::blockId)
		.function("color", &DbEntity::color)
		.function("setColor", select_overload<Acad::ErrorStatus(const CmColor&)>(&DbEntity::setColor))
		.function("setColor", select_overload<Acad::ErrorStatus(const CmColor&)>(&DbEntity::setColor))
		.function("colorIndex", &DbEntity::colorIndex)
		.function("setColorIndex", select_overload<Acad::ErrorStatus(UInt16)>(&DbEntity::setColorIndex))
		.function("setColorIndex", select_overload<Acad::ErrorStatus(UInt16, bool)>(&DbEntity::setColorIndex))
		.function("entityColor", &DbEntity::entityColor)
		.function("transparency", &DbEntity::transparency)
		.function("setTransparency", &DbEntity::setTransparency)
		.function("layer", select_overload<emstring() const>(&DbEntity::emsdk_layer))
		.function("layerId", &DbEntity::layerId)
		.function("setLayer", select_overload<Acad::ErrorStatus(const emstring&)>(&DbEntity::emsdk_setLayer))
		.function("setLayer", select_overload<Acad::ErrorStatus(const emstring&, Adesk::Boolean)>(&DbEntity::emsdk_setLayer))
		.function("setLayer", select_overload<Acad::ErrorStatus(const emstring&, Adesk::Boolean, bool)>(&DbEntity::emsdk_setLayer))
		.function("setLayer", select_overload<Acad::ErrorStatus(DbObjectId)>(&DbEntity::setLayer))
		.function("setLayer", select_overload<Acad::ErrorStatus(DbObjectId, Adesk::Boolean)>(&DbEntity::setLayer))
		.function("setLayer", select_overload<Acad::ErrorStatus(DbObjectId, Adesk::Boolean, bool)>(&DbEntity::setLayer))
		.function("linetype", select_overload<emstring() const>(&DbEntity::emsdk_linetype))
		.function("linetypeId", &DbEntity::linetypeId)
		.function("setLinetype", select_overload<Acad::ErrorStatus(const emstring&)>(&DbEntity::emsdk_setLinetype))
		.function("setLinetype", select_overload<Acad::ErrorStatus(const emstring&, Adesk::Boolean)>(&DbEntity::emsdk_setLinetype))
		.function("setLinetype", select_overload<Acad::ErrorStatus(DbObjectId)>(&DbEntity::setLinetype))
		.function("setLinetype", select_overload<Acad::ErrorStatus(DbObjectId, Adesk::Boolean)>(&DbEntity::setLinetype))
		.function("linetypeScale", &DbEntity::linetypeScale)
		.function("setLinetypeScale", &DbEntity::setLinetypeScale)
		.function("visibility", &DbEntity::visibility)
		.function("setVisibility", &DbEntity::setVisibility)
		.function("lineWeight", &DbEntity::lineWeight)
		.function("setLineWeight", &DbEntity::setLineWeight)
		.function("collisionType", &DbEntity::collisionType)
		.function("clone", &DbEntity::clone, allow_raw_pointers())
		.function("castShadows", &DbEntity::castShadows)
		.function("setCastShadows", &DbEntity::setCastShadows)
		.function("receiveShadows", &DbEntity::receiveShadows)
		.function("setReceiveShadows", &DbEntity::setReceiveShadows)
		.function("setPropertiesFrom", &DbEntity::setPropertiesFrom, allow_raw_pointers())
		.function("isPlanar", &DbEntity::isPlanar);

	// DbLine
	emscripten::class_<DbLine, base<DbEntity>>("DbLine")
		.constructor<>()
		DB_EMSDK_EXPORT_METHODS(DbLine)
		.function("startPoint", &DbLine::startPoint)
		.function("setStartPoint", &DbLine::setStartPoint)
		.function("endPoint", &DbLine::endPoint)
		.function("setEndPoint", &DbLine::setEndPoint);

	// DbDimension
	emscripten::class_<DbDimension, base<DbEntity>>("DbDimension")
		DB_EMSDK_EXPORT_METHODS(DbDimension)
		.function("textDefinedWidth", &DbDimension::textDefinedWidth)
		.function("textDefinedHeight", &DbDimension::textDefinedHeight)
		.function("setTextDefinedSize", &DbDimension::setTextDefinedSize)
		.function("resetTextDefinedSize", &DbDimension::resetTextDefinedSize)
		.function("textPosition", &DbDimension::textPosition)
		.function("setTextPosition", &DbDimension::setTextPosition)
		.function("isUsingDefaultTextPosition", &DbDimension::isUsingDefaultTextPosition)
		.function("useSetTextPosition", &DbDimension::useSetTextPosition)
		.function("useDefaultTextPosition", &DbDimension::useDefaultTextPosition)
		.function("setUsingDefaultTextPosition", &DbDimension::setUsingDefaultTextPosition)
		.function("normal", &DbDimension::normal)
		.function("setNormal", &DbDimension::setNormal)
		.function("isPlanar", &DbDimension::isPlanar)
		.function("getPlane", &DbDimension::emsdk_getPlane)
		.function("elevation", &DbDimension::elevation)
		.function("setElevation", &DbDimension::setElevation)
		.function("dimensionText", &DbDimension::emsdk_dimensionText)
		.function("setDimensionText", &DbDimension::emsdk_setDimensionText)
		.function("textRotation", &DbDimension::textRotation)
		.function("setTextRotation", &DbDimension::setTextRotation)
		.function("dimensionStyle", &DbDimension::dimensionStyle)
		.function("setDimensionStyle", &DbDimension::setDimensionStyle)
		.function("textAttachment", &DbDimension::textAttachment)
		.function("setTextAttachment", &DbDimension::setTextAttachment)
		.function("textLineSpacingStyle", &DbDimension::textLineSpacingStyle)
		.function("setTextLineSpacingStyle", &DbDimension::setTextLineSpacingStyle)
		.function("textLineSpacingFactor", &DbDimension::textLineSpacingFactor)
		.function("setTextLineSpacingFactor", &DbDimension::setTextLineSpacingFactor);

	// DbAlignedDimension
	emscripten::class_<DbAlignedDimension, base<DbDimension>>("DbAlignedDimension")
		.constructor<>()
		.constructor<const GePoint3d&, const GePoint3d&, const GePoint3d&>()
		DB_EMSDK_EXPORT_METHODS(DbAlignedDimension)
		.function("xLine1Point", &DbAlignedDimension::xLine1Point)
		.function("setXLine1Point", &DbAlignedDimension::setXLine1Point)
		.function("xLine2Point", &DbAlignedDimension::xLine2Point)
		.function("setXLine2Point", &DbAlignedDimension::setXLine2Point)
		.function("dimLinePoint", &DbAlignedDimension::dimLinePoint)
		.function("setDimLinePoint", &DbAlignedDimension::setDimLinePoint)
		.function("oblique", &DbAlignedDimension::oblique)
		.function("setOblique", &DbAlignedDimension::setOblique)
		.function("jogSymbolOn", &DbAlignedDimension::jogSymbolOn)
		.function("setJogSymbolOn", &DbAlignedDimension::setJogSymbolOn)
		.function("jogSymbolPosition", &DbAlignedDimension::jogSymbolPosition)
		.function("setJogSymbolPosition", &DbAlignedDimension::setJogSymbolPosition);

	// DbArcDimension
	emscripten::class_<DbArcDimension, base<DbDimension>>("DbArcDimension")
		.constructor<>()
		.constructor<const GePoint3d&, const GePoint3d&, const GePoint3d&, const GePoint3d&>()
		DB_EMSDK_EXPORT_METHODS(DbArcDimension)
		.function("arcPoint", &DbArcDimension::arcPoint)
		.function("setArcPoint", &DbArcDimension::setArcPoint)
		.function("xLine1Point", &DbArcDimension::xLine1Point)
		.function("setXLine1Point", &DbArcDimension::setXLine1Point)
		.function("xLine2Point", &DbArcDimension::xLine2Point)
		.function("setXLine2Point", &DbArcDimension::setXLine2Point)
		.function("centerPoint", &DbArcDimension::centerPoint)
		.function("setCenterPoint", &DbArcDimension::setCenterPoint)
		.function("isPartial", &DbArcDimension::isPartial)
		.function("setIsPartial", &DbArcDimension::setIsPartial)
		.function("arcStartParam", &DbArcDimension::arcStartParam)
		.function("setArcStartParam", &DbArcDimension::setArcStartParam)
		.function("arcEndParam", &DbArcDimension::arcEndParam)
		.function("setArcEndParam", &DbArcDimension::setArcEndParam)
		.function("hasLeader", &DbArcDimension::hasLeader)
		.function("setHasLeader", &DbArcDimension::setHasLeader)
		.function("leader1Point", &DbArcDimension::leader1Point)
		.function("setLeader1Point", &DbArcDimension::setLeader1Point)
		.function("leader2Point", &DbArcDimension::leader2Point)
		.function("setLeader2Point", &DbArcDimension::setLeader2Point)
		.function("arcSymbolType", &DbArcDimension::arcSymbolType)
		.function("setArcSymbolType", &DbArcDimension::setArcSymbolType);

	// DbOrdinateDimension
	emscripten::class_<DbOrdinateDimension, base<DbDimension>>("DbOrdinateDimension")
		.constructor<>()
		.constructor<Adesk::Boolean, const GePoint3d&, const GePoint3d&>()
		DB_EMSDK_EXPORT_METHODS(DbOrdinateDimension)
		.function("isUsingXAxis", &DbOrdinateDimension::isUsingXAxis)
		.function("isUsingYAxis", &DbOrdinateDimension::isUsingYAxis)
		.function("useXAxis", &DbOrdinateDimension::useXAxis)
		.function("useYAxis", &DbOrdinateDimension::useYAxis)
		.function("setUsingXAxis", &DbOrdinateDimension::setUsingXAxis)
		.function("setUsingYAxis", &DbOrdinateDimension::setUsingYAxis)
		.function("origin", &DbOrdinateDimension::origin)
		.function("definingPoint", &DbOrdinateDimension::definingPoint)
		.function("setDefiningPoint", &DbOrdinateDimension::setDefiningPoint)
		.function("leaderEndPoint", &DbOrdinateDimension::leaderEndPoint)
		.function("setLeaderEndPoint", &DbOrdinateDimension::setLeaderEndPoint);

	// DbRadialDimension
	emscripten::class_<DbRadialDimension, base<DbDimension>>("DbRadialDimension")
		.constructor<>()
		.constructor<const GePoint3d&, const GePoint3d&, double>()
		DB_EMSDK_EXPORT_METHODS(DbRadialDimension)
		.function("leaderLength", &DbRadialDimension::leaderLength)
		.function("setLeaderLength", &DbRadialDimension::setLeaderLength)
		.function("center", &DbRadialDimension::center)
		.function("setCenter", &DbRadialDimension::setCenter)
		.function("setChordPoint", &DbRadialDimension::setChordPoint)
		.function("extArcStartAngle", &DbRadialDimension::extArcStartAngle)
		.function("setExtArcStartAngle", &DbRadialDimension::setExtArcStartAngle)
		.function("setExtArcEndAngle", &DbRadialDimension::setExtArcEndAngle);

	// DbRadialDimensionLarge
	emscripten::class_<DbRadialDimensionLarge, base<DbDimension>>("DbRadialDimensionLarge")
		.constructor<>()
		.constructor<const GePoint3d&, const GePoint3d&, const GePoint3d&, const GePoint3d&, double>()
		DB_EMSDK_EXPORT_METHODS(DbRadialDimensionLarge)
		.function("center", &DbRadialDimensionLarge::center)
		.function("setCenter", &DbRadialDimensionLarge::setCenter)
		.function("chordPoint", &DbRadialDimensionLarge::chordPoint)
		.function("setChordPoint", &DbRadialDimensionLarge::setChordPoint)
		.function("overrideCenter", &DbRadialDimensionLarge::overrideCenter)
		.function("setOverrideCenter", &DbRadialDimensionLarge::setOverrideCenter)
		.function("jogPoint", &DbRadialDimensionLarge::jogPoint)
		.function("setJogPoint", &DbRadialDimensionLarge::setJogPoint)
		.function("jogAngle", &DbRadialDimensionLarge::jogAngle)
		.function("setJogAngle", &DbRadialDimensionLarge::setJogAngle)
		.function("setOverrideCenterPP", &DbRadialDimensionLarge::setOverrideCenterPP)
		.function("setJogPointPP", &DbRadialDimensionLarge::setJogPointPP)
		.function("setTextPositionPP", &DbRadialDimensionLarge::setTextPositionPP)
		.function("extArcStartAngle", &DbRadialDimensionLarge::extArcStartAngle)
		.function("setExtArcStartAngle", &DbRadialDimensionLarge::setExtArcStartAngle)
		.function("extArcEndAngle", &DbRadialDimensionLarge::extArcEndAngle)
		.function("setExtArcEndAngle", &DbRadialDimensionLarge::setExtArcEndAngle);

	// DbRotatedDimension
	emscripten::class_<DbRotatedDimension, base<DbDimension>>("DbRotatedDimension")
		.constructor<>()
		.constructor<double, const GePoint3d&, const GePoint3d&, const GePoint3d&>()
		DB_EMSDK_EXPORT_METHODS(DbRotatedDimension)
		.function("xLine1Point", &DbRotatedDimension::xLine1Point)
		.function("setXLine1Point", &DbRotatedDimension::setXLine1Point)
		.function("xLine2Point", &DbRotatedDimension::xLine2Point)
		.function("setXLine2Point", &DbRotatedDimension::setXLine2Point)
		.function("dimLinePoint", &DbRotatedDimension::dimLinePoint)
		.function("setDimLinePoint", &DbRotatedDimension::setDimLinePoint)
		.function("oblique", &DbRotatedDimension::oblique)
		.function("setOblique", &DbRotatedDimension::setOblique)
		.function("rotation", &DbRotatedDimension::rotation)
		.function("setRotation", &DbRotatedDimension::setRotation)
		.function("jogSymbolOn", &DbRotatedDimension::jogSymbolOn)
		.function("setJogSymbolOn", &DbRotatedDimension::setJogSymbolOn)
		.function("jogSymbolPosition", &DbRotatedDimension::jogSymbolPosition)
		.function("setJogSymbolPosition", &DbRotatedDimension::setJogSymbolPosition);

	// Db2LineAngularDimension
	emscripten::class_<Db2LineAngularDimension, base<DbDimension>>("Db2LineAngularDimension")
		.constructor<>()
		.constructor<const GePoint3d&, const GePoint3d&, const GePoint3d&, const GePoint3d&, const GePoint3d&>()
		DB_EMSDK_EXPORT_METHODS(Db2LineAngularDimension)
		.function("arcPoint", &Db2LineAngularDimension::arcPoint)
		.function("setArcPoint", &Db2LineAngularDimension::setArcPoint)
		.function("xLine1Start", &Db2LineAngularDimension::xLine1Start)
		.function("setXLine1Start", &Db2LineAngularDimension::setXLine1Start)
		.function("xLine1End", &Db2LineAngularDimension::xLine1End)
		.function("setXLine1End", &Db2LineAngularDimension::setXLine1End)
		.function("xLine2Start", &Db2LineAngularDimension::xLine2Start)
		.function("setXLine2Start", &Db2LineAngularDimension::setXLine2Start)
		.function("xLine2End", &Db2LineAngularDimension::xLine2End)
		.function("setXLine2End", &Db2LineAngularDimension::setXLine2End)
		.function("extArcOn", &Db2LineAngularDimension::extArcOn)
		.function("setExtArcOn", &Db2LineAngularDimension::setExtArcOn);

	// Db3PointAngularDimension
	emscripten::class_<Db3PointAngularDimension, base<DbDimension>>("Db3PointAngularDimension")
		.constructor<>()
		.constructor<const GePoint3d&, const GePoint3d&, const GePoint3d&, const GePoint3d&>()
		DB_EMSDK_EXPORT_METHODS(Db3PointAngularDimension)
		.function("arcPoint", &Db3PointAngularDimension::arcPoint)
		.function("setArcPoint", &Db3PointAngularDimension::setArcPoint)
		.function("xLine1Point", &Db3PointAngularDimension::xLine1Point)
		.function("setXLine1Point", &Db3PointAngularDimension::setXLine1Point)
		.function("xLine2Point", &Db3PointAngularDimension::xLine2Point)
		.function("setXLine2Point", &Db3PointAngularDimension::setXLine2Point)
		.function("centerPoint", &Db3PointAngularDimension::centerPoint)
		.function("setCenterPoint", &Db3PointAngularDimension::setCenterPoint)
		.function("extArcOn", &Db3PointAngularDimension::extArcOn)
		.function("setExtArcOn", &Db3PointAngularDimension::setExtArcOn);

	// DbDatabase
	emscripten::class_<DbDatabase, base<EmObject>>("DbDatabase")
		.constructor()
		.function("clayer", &DbDatabase::clayer)
		.function("setClayer", &DbDatabase::setClayer)
		.function("cecolor", &DbDatabase::cecolor)
		.function("setCecolor", &DbDatabase::setCecolor)
		.function("cetransparency", &DbDatabase::cetransparency)
		.function("setCetransparency", &DbDatabase::setCetransparency)
		.function("getLayerTable", select_overload<DbLayerTable * ()>(&DbDatabase::emsdk_getLayerTable), allow_raw_pointers())
		.function("getLayerTable", select_overload<DbLayerTable * (Db::OpenMode)>(&DbDatabase::emsdk_getLayerTable), allow_raw_pointers())
		.function("getLayoutDictionary", select_overload<DbDictionary * () const>(&DbDatabase::getLayoutDictionary), allow_raw_pointers())
		.function("getLayoutDictionary", select_overload<DbDictionary * (Db::OpenMode) const>(&DbDatabase::getLayoutDictionary), allow_raw_pointers());

	// DbSymbolTable
	emscripten::class_<DbSymbolTable, base<DbObject>>("DbSymbolTable")
		DB_EMSDK_EXPORT_METHODS(DbSymbolTable)
		.function("getAt", select_overload<DbSymbolTableRecord * (const emstring&) const>(&DbSymbolTable::emsdk_getAt), allow_raw_pointers())
		.function("getAt", select_overload<DbSymbolTableRecord * (const emstring&, Db::OpenMode) const>(&DbSymbolTable::emsdk_getAt), allow_raw_pointers())
		.function("getAt", select_overload<DbSymbolTableRecord * (const emstring&, Db::OpenMode, bool) const>(&DbSymbolTable::emsdk_getAt), allow_raw_pointers())
		.function("getIdAt", select_overload<DbObjectId(const emstring&) const>(&DbSymbolTable::emsdk_getIdAt))
		.function("getIdAt", select_overload<DbObjectId(const emstring&, bool) const>(&DbSymbolTable::emsdk_getIdAt))
		.function("has", select_overload<bool(const emstring&) const>(&DbSymbolTable::emsdk_has))
		.function("has", select_overload<bool(DbObjectId) const>(&DbSymbolTable::emsdk_has))
		.function("add", select_overload<Acad::ErrorStatus(DbSymbolTableRecord*)>(&DbSymbolTable::add), allow_raw_pointers())
		.function("newIterator", select_overload<DbSymbolTableIterator * () const>(&DbSymbolTable::emsdk_newIterator), allow_raw_pointers())
		.function("newIterator", select_overload<DbSymbolTableIterator * (bool) const>(&DbSymbolTable::emsdk_newIterator), allow_raw_pointers())
		.function("newIterator", select_overload<DbSymbolTableIterator * (bool, bool) const>(&DbSymbolTable::emsdk_newIterator), allow_raw_pointers());

	// DbLayerTable
	emscripten::class_<DbLayerTable, base<DbObject>>("DbLayerTable")
		.constructor<>()
		DB_EMSDK_EXPORT_METHODS(DbLayerTable)
		.function("getAt", select_overload<DbLayerTableRecord * (const emstring&) const>(&DbLayerTable::emsdk_getAt), allow_raw_pointers())
		.function("getAt", select_overload<DbLayerTableRecord * (const emstring&, Db::OpenMode) const>(&DbLayerTable::emsdk_getAt), allow_raw_pointers())
		.function("getAt", select_overload<DbLayerTableRecord * (const emstring&, Db::OpenMode, bool) const>(&DbLayerTable::emsdk_getAt), allow_raw_pointers())
		.function("newIterator", select_overload<DbLayerTableIterator * () const>(&DbLayerTable::emsdk_newIterator), allow_raw_pointers())
		.function("add", select_overload<Acad::ErrorStatus(DbLayerTableRecord*)>(&DbLayerTable::add), allow_raw_pointers());

	// DbSymbolTableIterator
	emscripten::class_<DbSymbolTableIterator, base<RxObject>>("DbSymbolTableIterator")
		.function("getRecordId", select_overload<DbObjectId() const>(&DbSymbolTableIterator::getRecordId))
		.function("getRecord", select_overload<DbSymbolTableRecord * () const>(&DbSymbolTableIterator::getRecord), allow_raw_pointers())
		.function("getRecord", select_overload<DbSymbolTableRecord * (bool) const>(&DbSymbolTableIterator::getRecord), allow_raw_pointers())
		.function("getRecord", select_overload<DbSymbolTableRecord * (Db::OpenMode, bool) const>(&DbSymbolTableIterator::getRecord), allow_raw_pointers())
		.function("done", &DbSymbolTableIterator::done)
		.function("start", select_overload<void()>(&DbSymbolTableIterator::start))
		.function("start", select_overload<void(bool)>(&DbSymbolTableIterator::start))
		.function("start", select_overload<void(bool, bool)>(&DbSymbolTableIterator::start))
		.function("step", select_overload<void()>(&DbSymbolTableIterator::step))
		.function("step", select_overload<void(bool)>(&DbSymbolTableIterator::step))
		.function("step", select_overload<void(bool, bool)>(&DbSymbolTableIterator::step));

	// DbLayerTableIterator
	emscripten::class_<DbLayerTableIterator, base<DbSymbolTableIterator>>("DbLayerTableIterator")
		.constructor<>()
		.function("getRecord", select_overload<DbLayerTableRecord * () const>(&DbLayerTableIterator::getRecord), allow_raw_pointers())
		.function("getRecord", select_overload<DbLayerTableRecord * (Db::OpenMode) const>(&DbLayerTableIterator::getRecord), allow_raw_pointers())
		.function("getRecord", select_overload<DbLayerTableRecord * (Db::OpenMode, bool) const>(&DbLayerTableIterator::getRecord), allow_raw_pointers());

	// DbSymbolTableRecord
	emscripten::class_<DbSymbolTableRecord, base<DbObject>>("DbSymbolTableRecord")
		DB_EMSDK_EXPORT_METHODS(DbSymbolTableRecord)
		.function("getName", &DbSymbolTableRecord::emsdk_getName)
		.function("setName", &DbSymbolTableRecord::emsdk_setName);

	// DbLayerTableRecord
	emscripten::class_<DbLayerTableRecord, base<DbSymbolTableRecord>>("DbLayerTableRecord")
		.constructor<>()
		DB_EMSDK_EXPORT_METHODS(DbLayerTableRecord)
		.function("isFrozen", &DbLayerTableRecord::isFrozen)
		.function("setIsFrozen", &DbLayerTableRecord::setIsFrozen)
		.function("isOff", &DbLayerTableRecord::isOff)
		.function("setIsOff", &DbLayerTableRecord::setIsOff)
		.function("VPDFLT", &DbLayerTableRecord::VPDFLT)
		.function("setVPDFLT", &DbLayerTableRecord::setVPDFLT)
		.function("isLocked", &DbLayerTableRecord::isLocked)
		.function("setIsLocked", &DbLayerTableRecord::setIsLocked)
		.function("color", &DbLayerTableRecord::color)
		.function("setColor", &DbLayerTableRecord::setColor)
		.function("entityColor", &DbLayerTableRecord::entityColor)
		.function("transparency", &DbLayerTableRecord::transparency)
		.function("setTransparency", &DbLayerTableRecord::setTransparency)
		.function("linetypeObjectId", &DbLayerTableRecord::linetypeObjectId)
		.function("setLinetypeObjectId", &DbLayerTableRecord::setLinetypeObjectId)
		.function("isPlottable", &DbLayerTableRecord::isPlottable)
		.function("setIsPlottable", &DbLayerTableRecord::setIsPlottable)
		.function("lineWeight", &DbLayerTableRecord::lineWeight)
		.function("setLineWeight", &DbLayerTableRecord::setLineWeight)
		.function("isInUse", &DbLayerTableRecord::isInUse)
		.function("description", &DbLayerTableRecord::emsdk_description)
		.function("setDescription", &DbLayerTableRecord::emsdk_setDescription)
		.function("isHidden", select_overload<bool() const>(&DbLayerTableRecord::isHidden))
		.function("setIsHidden", &DbLayerTableRecord::setIsHidden);

	// DbDictionary
	emscripten::class_<DbDictionary, base<DbObject>>("DbDictionary")
		.constructor<>()
		DB_EMSDK_EXPORT_METHODS(DbDictionary)
		.function("getAt", select_overload<DbObjectId(const emstring&)>(&DbDictionary::emsdk_getAt))
		.function("getAt", select_overload<DbObject * (const emstring&, Db::OpenMode)>(&DbDictionary::emsdk_getAt), allow_raw_pointers())
		.function("nameAt", select_overload<const emstring(DbObjectId)>(&DbDictionary::emsdk_nameAt))
		.function("has", select_overload<bool(const emstring&) const>(&DbDictionary::emsdk_has))
		.function("numEntries", &DbDictionary::numEntries)
		.function("remove", select_overload<DbObjectId(const emstring&)>(&DbDictionary::emsdk_remove))
		.function("remove", select_overload<Acad::ErrorStatus(DbObjectId)>(&DbDictionary::remove))
		.function("setName", select_overload<bool(const emstring&, const emstring&)>(&DbDictionary::emsdk_setName))
		.function("setAt", select_overload<DbObjectId(const emstring&, DbObject*)>(&DbDictionary::emsdk_setAt), allow_raw_pointers())
		.function("newIterator", &DbDictionary::newIterator, allow_raw_pointers());

	// DbDictionaryIterator
	emscripten::class_<DbDictionaryIterator, base<RxObject>>("DbDictionaryIterator")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(DbDictionaryIterator)
		.function("name", select_overload<std::string() const>(&DbDictionaryIterator::name))
		.function("object", select_overload<DbObject * ()>(&DbDictionaryIterator::object), allow_raw_pointers())
		.function("object", select_overload<DbObject * (Db::OpenMode)>(&DbDictionaryIterator::object), allow_raw_pointers())
		.function("objectId", select_overload<DbObjectId() const>(&DbDictionaryIterator::objectId))
		.function("done", &DbDictionaryIterator::done)
		.function("next", &DbDictionaryIterator::next);

	// DbPlotSettings
	emscripten::class_<DbPlotSettings, base<DbObject>>("DbPlotSettings")
		.constructor<>()
		DB_EMSDK_EXPORT_METHODS(DbPlotSettings);

	// DbLayout
	emscripten::class_<DbLayout, base<DbPlotSettings>>("DbLayout")
		.constructor<>()
		DB_EMSDK_EXPORT_METHODS(DbLayout)
		.function("getBlockTableRecordId", &DbLayout::getBlockTableRecordId)
		.function("setBlockTableRecordId", &DbLayout::setBlockTableRecordId)
		.function("getName", select_overload<std::string() const>(&DbLayout::emsdk_getName))
		.function("getLayoutName", select_overload<std::string() const>(&DbLayout::emsdk_getLayoutName))
		.function("setLayoutName", select_overload<Acad::ErrorStatus(const std::string&)>(&DbLayout::emsdk_setLayoutName))
		.function("getTabOrder", &DbLayout::getTabOrder)
		.function("setTabOrder", &DbLayout::setTabOrder)
		.function("getTabSelected", &DbLayout::getTabSelected)
		.function("setTabSelected", &DbLayout::setTabSelected)
		.function("getViewportArray", &DbLayout::getViewportArray);

	// DbLayoutManagerReactor
	emscripten::class_<DbLayoutManagerReactor, base<RxObject>>("DbLayoutManagerReactor")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(DbLayoutManagerReactor)
		.function("onLayoutCreated", select_overload<void(int)>(&DbLayoutManagerReactor::onLayoutCreated), allow_raw_pointers())
		.function("onLayoutToBeRemoved", select_overload<void(int)>(&DbLayoutManagerReactor::onLayoutToBeRemoved), allow_raw_pointers())
		.function("onLayoutRemoved", select_overload<void(int)>(&DbLayoutManagerReactor::onLayoutRemoved), allow_raw_pointers())
		.function("onAbortLayoutRemoved", select_overload<void(int)>(&DbLayoutManagerReactor::onAbortLayoutRemoved), allow_raw_pointers())
		.function("onLayoutToBeCopied", select_overload<void(int)>(&DbLayoutManagerReactor::onLayoutToBeCopied), allow_raw_pointers())
		.function("onLayoutCopied", select_overload<void(int)>(&DbLayoutManagerReactor::onLayoutCopied), allow_raw_pointers())
		.function("onAbortLayoutCopied", select_overload<void(int)>(&DbLayoutManagerReactor::onAbortLayoutCopied), allow_raw_pointers())
		.function("onLayoutToBeRenamed", select_overload<void(int)>(&DbLayoutManagerReactor::onLayoutToBeRenamed), allow_raw_pointers())
		.function("onLayoutRenamed", select_overload<void(int)>(&DbLayoutManagerReactor::onLayoutRenamed), allow_raw_pointers())
		.function("onAbortLayoutRename", select_overload<void(int)>(&DbLayoutManagerReactor::onAbortLayoutRename), allow_raw_pointers())
		.function("onLayoutSwitched", select_overload<void(int)>(&DbLayoutManagerReactor::onLayoutSwitched), allow_raw_pointers())
		.function("onPlotStyleTableChanged", select_overload<void(int)>(&DbLayoutManagerReactor::onPlotStyleTableChanged), allow_raw_pointers())
		.function("onLayoutsReordered", select_overload<void(int)>(&DbLayoutManagerReactor::onLayoutsReordered), allow_raw_pointers())
		.function("onRefreshLayoutTabs", select_overload<void(int)>(&DbLayoutManagerReactor::onRefreshLayoutTabs), allow_raw_pointers());

	// DbLayoutManager
	emscripten::class_<DbLayoutManager, base<RxObject>>("DbLayoutManager")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(DbLayoutManager)
		.function("setCurrentLayout", select_overload<Acad::ErrorStatus(const std::string&)>(&DbLayoutManager::emsdk_setCurrentLayout))
		.function("setCurrentLayout", select_overload<Acad::ErrorStatus(const std::string&, DbDatabase*)>(&DbLayoutManager::emsdk_setCurrentLayout), allow_raw_pointers())
		.function("setCurrentLayoutId", &DbLayoutManager::setCurrentLayoutId)
		.function("getActiveLayoutName", select_overload<std::string(bool)>(&DbLayoutManager::emsdk_getActiveLayoutName))
		.function("getActiveLayoutName", select_overload<std::string(bool, const DbDatabase*)>(&DbLayoutManager::emsdk_getActiveLayoutName), allow_raw_pointers())
		.function("findActiveLayout", select_overload<std::string(bool)>(&DbLayoutManager::emsdk_findActiveLayout))
		.function("findActiveLayout", select_overload<std::string(bool, const DbDatabase*)>(&DbLayoutManager::emsdk_findActiveLayout), allow_raw_pointers())
		.function("getActiveLayoutBTRId", select_overload<DbObjectId()>(&DbLayoutManager::getActiveLayoutBTRId))
		.function("getActiveLayoutBTRId", select_overload<DbObjectId(const DbDatabase*)>(&DbLayoutManager::getActiveLayoutBTRId), allow_raw_pointers())
		.function("findLayoutNamed", select_overload<DbObjectId(const std::string&)>(&DbLayoutManager::emsdk_findLayoutNamed))
		.function("findLayoutNamed", select_overload<DbObjectId(const std::string&, const DbDatabase*)>(&DbLayoutManager::emsdk_findLayoutNamed), allow_raw_pointers())
		.function("getLayoutNamed", select_overload<DbLayout * (const std::string&)>(&DbLayoutManager::emsdk_getLayoutNamed), allow_raw_pointers())
		.function("getLayoutNamed", select_overload<DbLayout * (const std::string&, Db::OpenMode, const DbDatabase*)>(&DbLayoutManager::emsdk_getLayoutNamed), allow_raw_pointers())
		.function("layoutExists", select_overload<bool(const std::string&)>(&DbLayoutManager::emsdk_layoutExists), allow_raw_pointers())
		.function("layoutExists", select_overload<bool(const std::string&, const DbDatabase*)>(&DbLayoutManager::emsdk_layoutExists), allow_raw_pointers())
		.function("copyLayout", select_overload<Acad::ErrorStatus(const std::string&, const std::string&)>(&DbLayoutManager::emsdk_copyLayout))
		.function("copyLayout", select_overload<Acad::ErrorStatus(const std::string&, const std::string&, DbDatabase*)>(&DbLayoutManager::emsdk_copyLayout), allow_raw_pointers())
		.function("deleteLayout", select_overload<Acad::ErrorStatus(const std::string&)>(&DbLayoutManager::emsdk_deleteLayout))
		.function("deleteLayout", select_overload<Acad::ErrorStatus(const std::string&, DbDatabase*)>(&DbLayoutManager::emsdk_deleteLayout), allow_raw_pointers())
		.function("createLayout", select_overload<DbObjectId(const std::string&)>(&DbLayoutManager::emsdk_createLayout))
		.function("createLayout", select_overload<DbObjectId(const std::string&, DbDatabase*)>(&DbLayoutManager::emsdk_createLayout), allow_raw_pointers())
		.function("renameLayout", select_overload<Acad::ErrorStatus(const std::string&, const std::string&)>(&DbLayoutManager::emsdk_renameLayout))
		.function("renameLayout", select_overload<Acad::ErrorStatus(const std::string&, const std::string&, DbDatabase*)>(&DbLayoutManager::emsdk_renameLayout), allow_raw_pointers())
		.function("cloneLayout", select_overload<Acad::ErrorStatus(DbLayout*, const std::string&)>(&DbLayoutManager::emsdk_cloneLayout), allow_raw_pointers())
		.function("cloneLayout", select_overload<Acad::ErrorStatus(DbLayout*, const std::string&, int)>(&DbLayoutManager::emsdk_cloneLayout), allow_raw_pointers())
		.function("cloneLayout", select_overload<Acad::ErrorStatus(DbLayout*, const std::string&, int, DbDatabase*)>(&DbLayoutManager::emsdk_cloneLayout), allow_raw_pointers())
		.function("getNonRectVPIdFromClipId", &DbLayoutManager::getNonRectVPIdFromClipId)
		.function("isVpnumClipped", select_overload<bool(int)>(&DbLayoutManager::isVpnumClipped))
		.function("isVpnumClipped", select_overload<bool(int, const DbDatabase*)>(&DbLayoutManager::isVpnumClipped), allow_raw_pointers())
		.function("countLayouts", select_overload<int()>(&DbLayoutManager::countLayouts))
		.function("countLayouts", select_overload<int(DbDatabase*)>(&DbLayoutManager::countLayouts), allow_raw_pointers())
		.function("addReactor", &DbLayoutManager::addReactor, allow_raw_pointers())
		.function("removeReactor", &DbLayoutManager::removeReactor, allow_raw_pointers());

	// DbDimData
	emscripten::class_<DbDimData>("DbDimData")
		.constructor()
		.function("ownerId", &DbDimData::ownerId)
		.function("setOwnerId", &DbDimData::setOwnerId)
		.function("dimension", &DbDimData::dimension, allow_raw_pointers())
		.function("setDimension", &DbDimData::setDimension, allow_raw_pointers())
		.function("bitFlags", &DbDimData::bitFlags)
		.function("setBitFlags", &DbDimData::setBitFlags)
		.function("isDimFocal", &DbDimData::isDimFocal)
		.function("setDimFocal", &DbDimData::setDimFocal)
		.function("isDimEditable", &DbDimData::isDimEditable)
		.function("setDimEditable", &DbDimData::setDimEditable)
		.function("isDimInvisible", &DbDimData::isDimInvisible)
		.function("setDimInvisible", &DbDimData::setDimInvisible)
		.function("isDimHideIfValueIsZero", &DbDimData::isDimHideIfValueIsZero)
		.function("setDimHideIfValueIsZero", &DbDimData::setDimHideIfValueIsZero)
		.function("isDimResultantLength", &DbDimData::isDimResultantLength)
		.function("setDimResultantLength", &DbDimData::setDimResultantLength)
		.function("isDimDeltaLength", &DbDimData::isDimDeltaLength)
		.function("setDimDeltaLength", &DbDimData::setDimDeltaLength)
		.function("isDimResultantAngle", &DbDimData::isDimResultantAngle)
		.function("setDimResultantAngle", &DbDimData::setDimResultantAngle)
		.function("isDimDeltaAngle", &DbDimData::isDimDeltaAngle)
		.function("setDimDeltaAngle", &DbDimData::setDimDeltaAngle)
		.function("isDimRadius", &DbDimData::isDimRadius)
		.function("setDimRadius", &DbDimData::setDimRadius)
		.function("isCustomDimValue", &DbDimData::isCustomDimValue)
		.function("setCustomDimValue", &DbDimData::setCustomDimValue)
		.function("isConstrained", &DbDimData::isConstrained)
		.function("setConstrain", &DbDimData::setConstrain)
		.function("isCustomString", &DbDimData::isCustomString)
		.function("setCustomString", &DbDimData::setCustomString);

	// DbDimDataPtrArray
	emscripten::class_<DbDimDataPtrArray>("DbDimDataPtrArray")
		.constructor()
		ACARRAY_PTR_EMSDK_EXPORT_METHODS(DbDimDataPtrArray, DbDimData*);

	// DbDimDataArray
	emscripten::class_<DbDimDataArray>("DbDimDataArray")
		.constructor()
		ACARRAY_EMSDK_EXPORT_METHODS(DbDimDataArray, DbDimData);
}

#endif
