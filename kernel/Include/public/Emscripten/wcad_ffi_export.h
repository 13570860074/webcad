/**
 * WebCAD Flutter FFI 导出接口
 *
 * 此文件为C++内核提供纯C接口，供Flutter dart:ffi调用。
 * 对应 Emscripten 绑定层 (GiExport.cpp, ReExport.cpp, EdExport.cpp, kernel.cpp)，
 * 但使用 extern "C" 导出而非 Emscripten embind。
 *
 * 命名规则: wcad_<module>_<function>
 *
 * 编译方式:
 *   Windows: 编译为 kernel.dll (使用 MSVC)
 *   Android: 编译为 libkernel.so (使用 NDK)
 *   macOS:   编译为 libkernel.dylib
 *   iOS:     静态链接到 App
 *   Linux:   编译为 libkernel.so
 *
 * 使用方法:
 *   在内核的 CMakeLists.txt 中增加 SHARED 库目标，
 *   包含此文件作为源文件之一。
 */

#ifndef WCAD_FFI_EXPORT_H
#define WCAD_FFI_EXPORT_H

#ifdef _WIN32
  #define WCAD_API extern "C" __declspec(dllexport)
#else
  #define WCAD_API extern "C" __attribute__((visibility("default")))
#endif


/* ================================================================
 * 回调函数类型定义 (用于 Reactor 模式)
 * 对应 web 端的 ArxEvent.GiEntityReactor_onXxx() 模式
 * ================================================================ */

typedef void (*wcad_entity_callback_t)(void* entity);
typedef void (*wcad_renderer_callback_t)(void* renderer);
typedef void (*wcad_material_callback_t)(void* material);


/* ================================================================
 * 内核生命周期
 * 对应 main.cpp: acrxManagerDictionary()->instantiate()
 * ================================================================ */

/** 初始化内核所有子系统 */
WCAD_API void wcad_kernel_instantiate();

/** 内核定时 tick (对应 onInterval -> emit kInterval) */
WCAD_API void wcad_kernel_interval();


/* ================================================================
 * 管理器获取 (对应 Emscripten kernel.cpp 导出的全局函数)
 * ================================================================ */

WCAD_API void* wcad_get_renderer_manager();
WCAD_API void* wcad_get_material_manager();
WCAD_API void* wcad_get_entity_manager();
WCAD_API void* wcad_get_user_input_manager();
WCAD_API void* wcad_get_doc_manager();


/* ================================================================
 * ReRendererManager
 * 对应 web: application.kernelManager().rendererManager()
 * ================================================================ */

WCAD_API void wcad_renderer_animation_frame(void* manager);
WCAD_API void wcad_renderer_add_reactor(void* manager, void* reactor);


/* ================================================================
 * Reactor 创建 (对应 web: new Gi.GiEntityReactor() + ArxEvent.xxx)
 * ================================================================ */

/**
 * 创建实体 Reactor，传入4个回调函数分别对应:
 * onCreateEntity, onUpdateVertexs, onTransformation, onVisibility
 */
WCAD_API void* wcad_create_entity_reactor(
    wcad_entity_callback_t on_create,
    wcad_entity_callback_t on_update_vertexs,
    wcad_entity_callback_t on_transformation,
    wcad_entity_callback_t on_visibility
);

/**
 * 创建渲染 Reactor，传入3个回调函数分别对应:
 * onBeginRenderer, onEndRenderer, onRendererViewport
 */
WCAD_API void* wcad_create_renderer_reactor(
    wcad_renderer_callback_t on_begin,
    wcad_renderer_callback_t on_end,
    wcad_renderer_callback_t on_viewport
);

/**
 * 创建材质 Reactor，传入2个回调函数分别对应:
 * onCreateMaterial, onRemoveMaterial
 */
WCAD_API void* wcad_create_material_reactor(
    wcad_material_callback_t on_create,
    wcad_material_callback_t on_remove
);


/* ================================================================
 * GiEntityManager
 * 对应 web: application.kernelManager().entityManager()
 * ================================================================ */

WCAD_API void  wcad_entity_manager_add_reactor(void* manager, void* reactor);
WCAD_API void* wcad_entity_manager_new_iterator(void* manager);
WCAD_API const char* wcad_entity_manager_information(void* manager);


/* ================================================================
 * GiEntity 属性访问
 * 对应 web: entity.type(), entity.vertexs(), entity.material() 等
 * ================================================================ */

WCAD_API int          wcad_entity_type(void* entity);
WCAD_API int          wcad_entity_element(void* entity);
WCAD_API unsigned int wcad_entity_vertex_length(void* entity);
WCAD_API double*      wcad_entity_vertexs(void* entity);
WCAD_API int          wcad_entity_visibility(void* entity);
WCAD_API void*        wcad_entity_material(void* entity);
WCAD_API void*        wcad_entity_matrix(void* entity);
WCAD_API long long    wcad_entity_object_id(void* entity);


/* ================================================================
 * GiMaterial 属性访问
 * 对应 web: material.type(), material.color() 等
 * ================================================================ */

WCAD_API int           wcad_material_type(void* material);
WCAD_API unsigned int  wcad_material_color(void* material);
WCAD_API unsigned char wcad_material_transparency(void* material);
WCAD_API double        wcad_material_line_width(void* material);
WCAD_API long long     wcad_material_object_id(void* material);


/* ================================================================
 * GiMaterialManager
 * 对应 web: application.kernelManager().materialManager()
 * ================================================================ */

WCAD_API void  wcad_material_manager_add_reactor(void* manager, void* reactor);


/* ================================================================
 * GiEntityIterator / GiMaterialIterator
 * 对应 web: manager.newIterator() -> start/done/step/getEntity
 * ================================================================ */

WCAD_API void  wcad_iterator_start(void* iterator);
WCAD_API int   wcad_iterator_done(void* iterator);
WCAD_API void  wcad_iterator_step(void* iterator);
WCAD_API void* wcad_iterator_get_entity(void* iterator);


/* ================================================================
 * EdUserInputManager
 * 对应 web: FUserInputManager -> EdUserInputManager
 * ================================================================ */

WCAD_API void wcad_input_set_device_pixel_ratio(void* manager, double ratio);
WCAD_API void wcad_input_set_rang_rect(void* manager,
    double top, double bottom, double left, double right);
WCAD_API void wcad_input_set_screen_size(void* manager,
    double width, double height);
WCAD_API void wcad_input_user_input(void* manager,
    const char* event_type, const char* json_data);


/* ================================================================
 * GeMatrix3d
 * 对应 web: entity.matrix().get(row, col)
 * ================================================================ */

WCAD_API double wcad_matrix3d_get(void* matrix, int row, int col);


/* ================================================================
 * ReCamera
 * 对应 web: renderer.camera().information()
 * ================================================================ */

WCAD_API const char* wcad_camera_information(void* camera);


#endif /* WCAD_FFI_EXPORT_H */
