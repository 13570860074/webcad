# CMakeList.txt: Test 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)


# 指定工程名
project(kernel_app)



# 指定C++ 版本
set(CMAKE_CXX_STANDARD 11)


# 设置是否开启调试模式
set(DEBUG_MODE 0)


# 设置环境变量
SET(ERROR_ON_UNDEFINED_SYMBOLS 0)
SET(WARN_ON_UNDEFINED_SYMBOLS 0)
SET(_CONSOLE)
SET(ALLOW_TABLE_GROWTH 1)



# winodws下设置编译选项
if(MSVC)

	# 源码统一使用 UTF-8 无 BOM，显式告诉 MSVC 按 UTF-8 解析源文件。
	add_compile_options(/utf-8)

    # 屏蔽C4828警告
    string(REPLACE "/W3" "/W3 /wd4828" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

endif()


# 注:wasm在PC端允许的最大内存为2147483648,而移动端是2147483648 / 2


# 设置编译模式
if(WIN32)

set(BUILD_USE_64BITS on)
set(WIN32_LEAN_AND_MEAN)

endif()


# 调试模式
if(DEBUG_MODE)
	set(CMAKE_BUILD_TYPE "Debug" )
else()
	set(CMAKE_BUILD_TYPE "Release" )
endif()
if(WIN32)
	set(CMAKE_BUILD_TYPE "Debug" )
endif()
if(APPLE)
	set(CMAKE_BUILD_TYPE "Debug" )
endif()



# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR}/Include)
include_directories(${PROJECT_SOURCE_DIR}/Include/public)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Ac)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Ge)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Db)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Ap)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Gi)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Gs)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Rx)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Ed)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Pr)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Ui)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Ev)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/Emscripten)
include_directories(${PROJECT_SOURCE_DIR}/Include/public/App)
include_directories(${PROJECT_SOURCE_DIR}/Include/private)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/kernel)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Ac)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Ge)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Ap)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Rx)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Db)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Gi)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Re)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Gs)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Ed)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Ui)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Ut)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Ev)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/Emscripten)
include_directories(${PROJECT_SOURCE_DIR}/Include/private/App)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty)





# 编译库文件
add_library(
	Ac
	STATIC
	src/Ac/AcDebug.cpp
	src/Ac/AcString.cpp
	src/Ac/AcJsonObject.cpp
	# 系统
	src/Ac/AcSystem.cpp
	# 事件
	src/Ac/AcEventBase.cpp
	src/Ac/AcEvent.cpp
	src/Ac/AcEmsdkEvent.cpp
	src/Ac/AcEventTrigger.cpp
	# 序列化
	src/Ac/AcBinary.cpp
	src/Ac/AcBinaryFiler.cpp
)


add_library(
	Ge
	STATIC
	src/Ge/GeLibVersion.cpp
	src/Ge/GeMath.cpp
	src/Ge/GeGbl.cpp
	src/Ge/GeInterval.cpp
	src/Ge/GeQuaternion.cpp
	src/Ge/GeVector2d.cpp
	src/Ge/GeVector3d.cpp
	src/Ge/GePoint2d.cpp
	src/Ge/GePoint3d.cpp
	src/Ge/GeScale2d.cpp
	src/Ge/GeScale3d.cpp
	src/Ge/GeEntity2d.cpp
	src/Ge/GeSegmentChain2d.cpp
	src/Ge/GeSplineEnt2d.cpp
	src/Ge/GeEntity3d.cpp
	src/Ge/GeSplineEnt3d.cpp
	src/Ge/GePointEnt2d.cpp
	src/Ge/GePointEnt3d.cpp
	src/Ge/GePointOnCurve2d.cpp
	src/Ge/GePointOnCurve3d.cpp
	src/Ge/GePointOnSurface.cpp
	src/Ge/GeBoundBlock2d.cpp
	src/Ge/GeBoundBlock3d.cpp
	src/Ge/GeCurve2d.cpp
	src/Ge/GeCurve3d.cpp
	src/Ge/GeCircArc2d.cpp
	src/Ge/GeCircArc3d.cpp
	src/Ge/GeLine2d.cpp
	src/Ge/GeClipBoundary2d.cpp
	src/Ge/GeLine3d.cpp
	src/Ge/GeLinearEnt2d.cpp
	src/Ge/GeLinearEnt3d.cpp
	src/Ge/GeLineSeg2d.cpp
	src/Ge/GeLineSeg3d.cpp
	src/Ge/GeMatrix2d.cpp
	src/Ge/GeMatrix3d.cpp
	src/Ge/GeRay2d.cpp
	src/Ge/GeRay3d.cpp
	src/Ge/GeSurface.cpp
	src/Ge/GePlanarEnt.cpp
	src/Ge/GePlane.cpp
	src/Ge/GeBoundedPlane.cpp
	src/Ge/GeEllipArc2d.cpp
	src/Ge/GeEllipArc3d.cpp
	src/Ge/GeCubicSplineCurve2d.cpp
	src/Ge/GeCubicSplineCurve3d.cpp
	src/Ge/GeExtents2d.cpp
	src/Ge/GeExtents3d.cpp
	src/Ge/GePosition2d.cpp
	src/Ge/GePosition3d.cpp
	src/Ge/GeCompositeCurve2d.cpp
	src/Ge/GeCompositeCurve3d.cpp
	src/Ge/GeAugPolyline3d.cpp
	src/Ge/GePolyline2d.cpp
	src/Ge/GePolyline3d.cpp
	src/Ge/GeCurveSurfInt.cpp
	src/Ge/GeSurfSurfInt.cpp
	src/Ge/GeCurveCurveInt2d.cpp
	src/Ge/GeCurveCurveInt3d.cpp
	src/Ge/GeOffsetCurve2d.cpp
	src/Ge/GeCurveBoundary.cpp
	src/Ge/GeExternalCurve2d.cpp
	src/Ge/GeExternalCurve3d.cpp
	src/Ge/GeSurfaceCurve2dTo3d.cpp
	src/Ge/GeSurfaceCurve3dTo2d.cpp
	src/Ge/GeExternalSurface.cpp
	src/Ge/GeExternalBoundedSurface.cpp
	src/Ge/GeNurbCurve2d.cpp
	src/Ge/GeNurbCurve3d.cpp
	src/Ge/GeNurbSurface.cpp
	src/Ge/GeOffsetCurve3d.cpp
	src/Ge/GeOffsetSurface.cpp
	src/Ge/GeRevolvedSurface.cpp
	src/Ge/GeSpunSurf.cpp
	src/Ge/GeSphere.cpp
	src/Ge/GeVertexGenerator.cpp
	src/Ge/GeUvBox.cpp
)


add_library(
	Ev
	STATIC
	src/Ev/EvEventManager.cpp
	src/Ev/EvRendererReactor.cpp
	src/Ev/EvDocManagerReactor.cpp
	src/Ev/EvDatabaseReactor.cpp
)


add_library(
	Rx
	STATIC
	src/Rx/RxObjectImpl.cpp
	src/Rx/RxObject.cpp
	src/Rx/RxCopyOnWriteObject.cpp
	# 管理器
	src/Rx/RxDeclareMembers.cpp
	src/Rx/RxManagerDictionary.cpp
	src/Rx/RxManager.cpp
	# 类型管理器
	src/Rx/RxClassManager.cpp
	# 多线程
	src/Rx/RxWorkerThread.cpp
	src/Rx/RxWorkerThreadManager.cpp
	# 系统变量
	src/Rx/RxSystemVariable.cpp
	src/Rx/RxSystemVariableManager.cpp
	# 配置变量
	src/Rx/RxConfigurationVariable.cpp
	src/Rx/RxConfigurationVariableManager.cpp
	# 环境变量
	src/Rx/RxEnvironmentVariable.cpp
	src/Rx/RxEnvironmentVariableManager.cpp
	# 系统注册表
	src/Rx/RxSysRegistryManager.cpp
	src/Rx/RxSysRegistryDictionaryIterator.cpp
	# Rx反应器
	src/Rx/RxEventReactor.cpp
	# 异步等待管理器
	src/Rx/RxAsyncManager.cpp
)

add_library(
	Ut
	STATIC
	src/Ut/acutads.cpp
	src/Ut/acutmem.cpp
)


add_library(
	Ap
	STATIC
	src/Ap/ApDocument.cpp
	src/Ap/ApWorkDocument.cpp
	src/Ap/ApDocumentIterator.cpp
	src/Ap/ApDocManagerReactor.cpp
	src/Ap/ApDocManager.cpp
	src/Ap/ApWorkDocManager.cpp
)


add_library(
	Db 
	STATIC
	src/Db/Db.cpp
	src/Db/DbHandle.cpp
	src/Db/DbObject.cpp
	src/Db/DbObjectId.cpp
	src/Db/DbObjectManager.cpp
	src/Db/DbHostApplicationServices.cpp
	# 序列化
	src/Db/DbDwgFiler.cpp
	src/Db/DbDxfFiler.cpp
	src/Db/DbDwgUndoFiler.cpp
	src/Db/DbDwgFileFiler.cpp
	src/Db/DbDwgCopyFiler.cpp
	# 计算捕捉
	src/Db/DbOsnapPointCompute.cpp
	# 范围
	src/Db/DbExtents2d.cpp
	src/Db/DbExtents.cpp
	# 夹点
	src/Db/DbGripData.cpp
	# 句柄
	src/Db/DbHandleTableRecord.cpp
	src/Db/DbHandleTable.cpp
	src/Db/DbHandleManage.cpp
	# 数据库
	src/Db/DbDatabaseImpl.cpp
	src/Db/DbDatabase.cpp
	src/Db/DbDatabaseReactor.cpp
	# 符号表/记录
	src/Db/DbSymbolTable.cpp
	src/Db/DbSymbolTableRecord.cpp
	src/Db/DbSymbolTableIterator.cpp
	# 视图表/记录
	src/Db/DbViewTable.cpp
	src/Db/DbViewTableRecord.cpp
	src/Db/DbViewTableIterator.cpp
	# 视口表/记录
	src/Db/DbViewportTable.cpp
	src/Db/DbAbstractViewTableRecord.cpp
	src/Db/DbViewportTableRecord.cpp
	src/Db/DbViewportTableIterator.cpp
	# 坐标系表/记录
	src/Db/DbUCSTable.cpp
	src/Db/DbUCSTableRecord.cpp
	src/Db/DbUCSTableIterator.cpp
	# 线型表/记录
	src/Db/DbLinetypeDashe.cpp
	src/Db/DbLinetypeTable.cpp
	src/Db/DbLinetypeTableRecord.cpp
	src/Db/DbLinetypeTableIterator.cpp
	# 图层表/记录
	src/Db/DbLayerTable.cpp
	src/Db/DbLayerTableRecord.cpp
	src/Db/DbLayerTableIterator.cpp
	# 应用表/记录
	src/Db/DbRegAppTable.cpp
	src/Db/DbRegAppTableRecord.cpp
	src/Db/DbRegAppTableIterator.cpp
	# 块表/记录
	src/Db/DbBlockTable.cpp
	src/Db/DbBlockTableRecord.cpp
	src/Db/DbBlockTableIterator.cpp
	src/Db/DbBlockTableRecordIterator.cpp
	# 文字样式表/记录
	src/Db/DbTextStyleTable.cpp
	src/Db/DbTextStyleTableRecord.cpp
	src/Db/DbTextStyleTableIterator.cpp
	# 标注样式表/记录
	src/Db/DbDimStyleTable.cpp
	src/Db/DbDimStyleTableRecord.cpp
	src/Db/DbDimStyleTableIterator.cpp
	# 词典表/记录
	src/Db/DbDictionaryIterator.cpp
	src/Db/DbDictionary.cpp
	# 布局
	src/Db/DbPlotSettings.cpp
	src/Db/DbLayout.cpp
	src/Db/DbLayoutManagerReactor.cpp
	src/Db/DbLayoutManager.cpp
	# 扩展记录
	src/Db/DbXrecord.cpp
	# 实体
	src/Db/DbEntity.cpp
	src/Db/DbEntityStake.cpp
	src/Db/DbViewport.cpp
	src/Db/DbBlockReference.cpp
	src/Db/DbBlockReferenceIdIterator.cpp
	src/Db/DbBlockBegin.cpp
	src/Db/DbBlockEnd.cpp
	src/Db/DbCurve.cpp
	src/Db/DbPoint.cpp
	src/Db/DbLine.cpp
	src/Db/DbRay.cpp
	src/Db/DbXline.cpp
	src/Db/DbLeader.cpp
	src/Db/DbCircle.cpp
	src/Db/DbArc.cpp
	src/Db/DbEllipse.cpp
	src/Db/DbPolyline.cpp
	src/Db/DbSpline.cpp
	src/Db/DbSolid.cpp
	src/Db/DbHatch.cpp
	src/Db/DbText.cpp
	src/Db/DbMText.cpp
	# 标注
	src/Db/DbDimension.cpp
	src/Db/DbArcDimension.cpp
	src/Db/DbRotatedDimension.cpp
	src/Db/DbAlignedDimension.cpp
	src/Db/DbDiametricDimension.cpp
	src/Db/DbRadialDimension.cpp
	src/Db/DbOrdinateDimension.cpp
	src/Db/Db2LineAngularDimension.cpp
	src/Db/Db3PointAngularDimension.cpp
	src/Db/DbRadialDimensionLarge.cpp
)


add_library(
	Gi
	STATIC
	src/Gi/Gi.cpp
	src/Gi/GiObjectManager.cpp
	src/Gi/GiObject.cpp
	src/Gi/GiObjectId.cpp
	src/Gi/GiDrawable.cpp
	# 材质
	src/Gi/GiMaterialManager.cpp
	src/Gi/GiMaterialIterator.cpp	
	src/Gi/GiMaterialReactor.cpp	
	src/Gi/GiMaterial.cpp
	src/Gi/GiLineMaterial.cpp
	src/Gi/GiLineDashedMaterial.cpp
	src/Gi/GiLinePixelDashedMaterial.cpp
	src/Gi/GiMeshMaterial.cpp
	src/Gi/GiPointMaterial.cpp
	# 实体
	src/Gi/GiEntityMutex.cpp
	src/Gi/GiEntityManager.cpp
	src/Gi/GiEntityIterator.cpp	
	src/Gi/GiEntityReactor.cpp	
	src/Gi/GiEntity.cpp
	src/Gi/GiPoints.cpp
	src/Gi/GiLineSegment.cpp
	src/Gi/GiMesh.cpp
	# 实体几何对象
	src/Gi/GiEntityGeometryStake.cpp
	src/Gi/GiEntityGeometry.cpp
	src/Gi/GiPointGeometry.cpp
	src/Gi/GiLineSegmentGeometry.cpp
	src/Gi/GiTriangleMeshGeometry.cpp
	# 绘图器
	src/Gi/GiFill.cpp
	src/Gi/GiParameter.cpp
	src/Gi/GiTextStyle.cpp
	src/Gi/GiSubEntityTraits.cpp
	src/Gi/GiEntityTraits.cpp
	src/Gi/GiWorldGeometryStake.cpp
	src/Gi/GiWorldGeometry.cpp
	src/Gi/GiWorldDrawGeometry.cpp
	src/Gi/GiCommonDraw.cpp
	src/Gi/GiWorldDraw.cpp
	src/Gi/GiViewportDraw.cpp
	src/Gi/GiViewportGeometry.cpp
	src/Gi/GiEntDraw.cpp
	src/Gi/GiWorldDrawManager.cpp
	src/Gi/GiEntityHighlander.cpp
	# 几何范围
	src/Gi/GiComputeDisplay.cpp
	src/Gi/GiComputeExtent.cpp
	src/Gi/GiGeometryExtentView.cpp
	src/Gi/GiGeometryExtent.cpp
	src/Gi/GiGeometryExtentManager.cpp
)

add_library(
	Gs
	STATIC
	src/Gs/GsElementManager.cpp
	src/Gs/GsElement.cpp
	src/Gs/GsCursor.cpp
	src/Gs/GsCoordSystem.cpp
)

add_library(
	Ed
	STATIC
	# 全局
	src/Ed/aced.cpp
	src/Ed/acedads.cpp
	src/Ed/acedTrans.cpp
	# 用户输入
	src/Ed/EdTouchEvent.cpp
	src/Ed/EdMouseEvent.cpp
	src/Ed/EdWheelEvent.cpp
	src/Ed/EdKeyboardEvent.cpp
	src/Ed/EdUserInputManager.cpp
	# 拖拽
	src/Ed/EdJig.cpp
	src/Ed/EdDimJig.cpp
	src/Ed/EdPickJig.cpp
	# 控制器
	src/Ed/EdController.cpp
	src/Ed/EdAcadController.cpp
	src/Ed/EdTouchController.cpp
	src/Ed/EdControllerManager.cpp
	# 命令
	src/Ed/EdCommand.cpp
	src/Ed/EdCommandReactor.cpp
	src/Ed/EdCommandStack.cpp
	src/Ed/EdCommandStackReactor.cpp
	src/Ed/EdRegistryCommand.cpp
	src/Ed/EdRegistryCommandIterator.cpp
	src/Ed/EdRegistryCommandStack.cpp
	# 编辑反应器
	src/Ed/EditorReactor.cpp
	src/Ed/EditorReactorManager.cpp
	# 选择集
	src/Ed/EdSelectionSetIndex.cpp
	src/Ed/EdSelectionSetManager.cpp
)

add_library(
	App
	STATIC
	src/App/Application.cpp
	src/App/ApplicationManager.cpp
)


add_library(
	Re 
	STATIC
	src/Re/ReCamera.cpp
	src/Re/ReRenderer.cpp
	src/Re/ReRendererManager.cpp
	src/Re/ReRendererReactor.cpp
	src/Re/ReOrthogonalCamera.cpp
)

add_library(
	Pr
	STATIC
	src/Pr/PrDXFParser_new.cpp
)


# 设置路径
if(EMSCRIPTEN)
	# 设定刻执行文件生成路径
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/wasm)
	# 设定bin生成路径
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/wasm/bin)
	# 设定bin生成路径
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/wasm/lib)
elseif(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/win)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/win/bin)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/win/lib)
elseif(APPLE)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/mac)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/mac/bin)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/mac/lib)
elseif(UNIX)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/unix)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/unix/bin)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/unix/lib)
endif()

# 设置引用库路径
if(EMSCRIPTEN)
	link_directories(${PROJECT_SOURCE_DIR}/wasm/bin)
elseif(WIN32)
	link_directories(${PROJECT_SOURCE_DIR}/win/bin)
elseif(APPLE)
	link_directories(${PROJECT_SOURCE_DIR}/mac/bin)
elseif(UNIX)
	link_directories(${PROJECT_SOURCE_DIR}/unix/bin)
endif()



add_library(
	Kernel
	STATIC
	src/kernel/kernel.cpp
)

add_library(KernelModules INTERFACE)



# 将源代码添加到此项目的可执行文件。
add_executable(
	main
	main.cpp
	src/Emscripten/Acad.cpp
	src/Emscripten/kernel.cpp
	src/Emscripten/EmExport.cpp
	src/Emscripten/GeExport.cpp
	src/Emscripten/DbExport.cpp
	src/Emscripten/RxExport.cpp
	src/Emscripten/GiExport.cpp
	src/Emscripten/ReExport.cpp
	src/Emscripten/ApExport.cpp
	src/Emscripten/EdExport.cpp
	src/Emscripten/CmExport.cpp
)

 
# CMAKE_CXX_FLAGS: 用于设置C++编译器的编译选项


if(EMSCRIPTEN)


if(DEBUG_MODE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -g")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -o3")
endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ALLOW_TABLE_GROWTH")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_DISABLE_EXCEPTION_CATCHING")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXTRA_EXPORTED_RUNTIME_METHODS=['addFunction']")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ASYNCIFY")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_PTHREADS")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s PROXY_TO_PTHREAD=1")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --bind")

elseif(WIN32)

	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

elseif(APPLE)


elseif(UNIX)


endif()



# 聚合单独模块，main 只链接这一组模块
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_libraries(KernelModules INTERFACE
		-Wl,--start-group
		Kernel Ac Ge Rx Ev Gi Re Ut Ap Db Gs Ed Pr App
		-Wl,--end-group
	)
else()
	target_link_libraries(KernelModules INTERFACE Kernel Ac Ge Rx Ev Gi Re Ut Ap Db Gs Ed Pr App)
endif()

target_link_libraries(main PRIVATE KernelModules)


