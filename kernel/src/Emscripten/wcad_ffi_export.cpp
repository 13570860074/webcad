/**
 * WebCAD Flutter FFI 导出实现
 *
 * 此文件实现 wcad_ffi_export.h 声明的所有纯C接口。
 * 内部调用现有的C++内核对象 (kernel(), GiEntityManager, etc.)
 *
 * 编译条件: 仅在非Emscripten环境下编译 (Windows/Android/iOS/macOS/Linux)
 */

#if !defined(__EMSCRIPTEN__)

#include "wcad_ffi_export.h"
#include "kernel.h"
#include "RxManagerDictionary.h"
#include "EvEventManager.h"
#include "ApDocManager.h"
#include "GiEntityManager.h"
#include "GiEntity.h"
#include "GiLineMaterial.h"
#include "GiEntityIterator.h"
#include "GiMaterial.h"
#include "GiMaterialManager.h"
#include "GiObjectId.h"
#include "GiEntityReactor.h"
#include "GiMaterialReactor.h"
#include "ReRendererManager.h"
#include "ReRendererReactor.h"
#include "ReCamera.h"
#include "EdUserInputManager.h"
#include "GeMatrix3d.h"
#include "AcSystem.h"

#include <string>
#include <cstring>

// ============================================================================
// FFI Reactor 适配器
// 将纯C回调函数包装成C++对象的虚函数回调
// ============================================================================

/// 实体Reactor适配器 - 将C函数指针桥接到GiEntityReactor
class FfiEntityReactor : public GiEntityReactor {
public:
    wcad_entity_callback_t m_onCreate;
    wcad_entity_callback_t m_onUpdateVertexs;
    wcad_entity_callback_t m_onTransformation;
    wcad_entity_callback_t m_onVisibility;

    FfiEntityReactor(
        wcad_entity_callback_t onCreate,
        wcad_entity_callback_t onUpdateVertexs,
        wcad_entity_callback_t onTransformation,
        wcad_entity_callback_t onVisibility)
        : m_onCreate(onCreate)
        , m_onUpdateVertexs(onUpdateVertexs)
        , m_onTransformation(onTransformation)
        , m_onVisibility(onVisibility)
    {}

    void createEntity(GiEntity* entity) override {
        if (m_onCreate) m_onCreate(entity);
    }
    void updateVertexs(GiEntity* entity) override {
        if (m_onUpdateVertexs) m_onUpdateVertexs(entity);
    }
    void transformation(GiEntity* entity) override {
        if (m_onTransformation) m_onTransformation(entity);
    }
    void setVisibility(GiEntity* entity, bool /*isVisibility*/) override {
        if (m_onVisibility) m_onVisibility(entity);
    }
};

/// 渲染Reactor适配器
class FfiRendererReactor : public ReRendererReactor {
public:
    wcad_renderer_callback_t m_onBegin;
    wcad_renderer_callback_t m_onEnd;
    wcad_renderer_callback_t m_onViewport;

    FfiRendererReactor(
        wcad_renderer_callback_t onBegin,
        wcad_renderer_callback_t onEnd,
        wcad_renderer_callback_t onViewport)
        : m_onBegin(onBegin)
        , m_onEnd(onEnd)
        , m_onViewport(onViewport)
    {}

    void beginRenderer() override {
        if (m_onBegin) m_onBegin(nullptr);
    }
    void endRenderer() override {
        if (m_onEnd) m_onEnd(nullptr);
    }
    void rendererViewport(ReRenderer* renderer) override {
        if (m_onViewport) m_onViewport(renderer);
    }
};

/// 材质Reactor适配器
class FfiMaterialReactor : public GiMaterialReactor {
public:
    wcad_material_callback_t m_onCreate;
    wcad_material_callback_t m_onRemove;

    FfiMaterialReactor(
        wcad_material_callback_t onCreate,
        wcad_material_callback_t onRemove)
        : m_onCreate(onCreate)
        , m_onRemove(onRemove)
    {}

    void createMaterial(GiMaterial* material) override {
        if (m_onCreate) m_onCreate(material);
    }
    void removeMaterial(GiObjectId /*id*/) override {
        if (m_onRemove) m_onRemove(nullptr);
    }
};


// ============================================================================
// 内核生命周期
// ============================================================================

WCAD_API void wcad_kernel_instantiate() {
    ::acrxManagerDictionary()->instantiate();
}

WCAD_API void wcad_kernel_interval() {
    ::kernel()->acevEventManager()->_emit(Ev::Interval::kInterval);
}


// ============================================================================
// 管理器获取
// ============================================================================

WCAD_API void* wcad_get_renderer_manager() {
    return static_cast<void*>(::kernel()->acreRendererManager());
}

WCAD_API void* wcad_get_material_manager() {
    return static_cast<void*>(::kernel()->acgiMaterialManager());
}

WCAD_API void* wcad_get_entity_manager() {
    return static_cast<void*>(::kernel()->acgiEntityManager());
}

WCAD_API void* wcad_get_user_input_manager() {
    return static_cast<void*>(::kernel()->acedUserInputManager());
}

WCAD_API void* wcad_get_doc_manager() {
    return static_cast<void*>(::kernel()->acapDocManager());
}


// ============================================================================
// ReRendererManager
// ============================================================================

WCAD_API void wcad_renderer_animation_frame(void* manager) {
    static_cast<ReRendererManager*>(manager)->animationFrame();
}

WCAD_API void wcad_renderer_add_reactor(void* manager, void* reactor) {
    static_cast<ReRendererManager*>(manager)->addReactor(
        static_cast<ReRendererReactor*>(reactor));
}


// ============================================================================
// Reactor 创建
// ============================================================================

WCAD_API void* wcad_create_entity_reactor(
    wcad_entity_callback_t on_create,
    wcad_entity_callback_t on_update_vertexs,
    wcad_entity_callback_t on_transformation,
    wcad_entity_callback_t on_visibility)
{
    return new FfiEntityReactor(on_create, on_update_vertexs,
                                on_transformation, on_visibility);
}

WCAD_API void* wcad_create_renderer_reactor(
    wcad_renderer_callback_t on_begin,
    wcad_renderer_callback_t on_end,
    wcad_renderer_callback_t on_viewport)
{
    return new FfiRendererReactor(on_begin, on_end, on_viewport);
}

WCAD_API void* wcad_create_material_reactor(
    wcad_material_callback_t on_create,
    wcad_material_callback_t on_remove)
{
    return new FfiMaterialReactor(on_create, on_remove);
}


// ============================================================================
// GiEntityManager
// ============================================================================

WCAD_API void wcad_entity_manager_add_reactor(void* manager, void* reactor) {
    static_cast<GiEntityManager*>(manager)->addReactor(
        static_cast<GiEntityReactor*>(reactor));
}

WCAD_API void* wcad_entity_manager_new_iterator(void* manager) {
    return static_cast<void*>(
        static_cast<GiEntityManager*>(manager)->newIterator());
}

// 静态缓冲区存储 information JSON
static std::string s_entityInfoBuffer;

WCAD_API const char* wcad_entity_manager_information(void* /*manager*/) {
    // GiEntityManager 暂无 information() 方法，返回空 JSON
    s_entityInfoBuffer = "{}";
    return s_entityInfoBuffer.c_str();
}


// ============================================================================
// GiEntity
// ============================================================================

WCAD_API int wcad_entity_type(void* entity) {
    return static_cast<int>(static_cast<GiEntity*>(entity)->type());
}

WCAD_API int wcad_entity_element(void* entity) {
    return static_cast<int>(static_cast<GiEntity*>(entity)->element());
}

WCAD_API unsigned int wcad_entity_vertex_length(void* entity) {
    return static_cast<GiEntity*>(entity)->vertexLength();
}

WCAD_API double* wcad_entity_vertexs(void* entity) {
    return static_cast<GiEntity*>(entity)->vertexs();
}

WCAD_API int wcad_entity_visibility(void* entity) {
    return static_cast<GiEntity*>(entity)->visibility() ? 1 : 0;
}

WCAD_API void* wcad_entity_material(void* entity) {
    return static_cast<void*>(static_cast<GiEntity*>(entity)->material());
}

WCAD_API void* wcad_entity_matrix(void* entity) {
    // 使用静态缓冲区存储矩阵（单线程安全）
    static GeMatrix3d s_matrixBuffer;
    s_matrixBuffer = static_cast<GiEntity*>(entity)->matrix();
    return static_cast<void*>(&s_matrixBuffer);
}

WCAD_API long long wcad_entity_object_id(void* entity) {
    return static_cast<long long>(
        static_cast<GiEntity*>(entity)->objectId().asOldId());
}


// ============================================================================
// GiMaterial
// ============================================================================

WCAD_API int wcad_material_type(void* material) {
    return static_cast<int>(static_cast<GiMaterial*>(material)->type());
}

WCAD_API unsigned int wcad_material_color(void* material) {
    return static_cast<GiMaterial*>(material)->color();
}

WCAD_API unsigned char wcad_material_transparency(void* material) {
    return static_cast<GiMaterial*>(material)->transparency();
}

WCAD_API double wcad_material_line_width(void* material) {
    // 安全转换：如果是 GiLineMaterial 类型才有 lineWidth
    GiMaterial* mat = static_cast<GiMaterial*>(material);
    if (mat->type() == Gi::kLineBasic || mat->type() == Gi::kLineDashed ||
        mat->type() == Gi::kLinePixelDashed) {
        return static_cast<GiLineMaterial*>(mat)->lineWidth();
    }
    return 1.0;
}

WCAD_API long long wcad_material_object_id(void* material) {
    return static_cast<long long>(
        static_cast<GiMaterial*>(material)->objectId().asOldId());
}


// ============================================================================
// GiMaterialManager
// ============================================================================

WCAD_API void wcad_material_manager_add_reactor(void* manager, void* reactor) {
    static_cast<GiMaterialManager*>(manager)->addReactor(
        static_cast<GiMaterialReactor*>(reactor));
}


// ============================================================================
// Iterator
// ============================================================================

WCAD_API void wcad_iterator_start(void* iterator) {
    static_cast<GiEntityIterator*>(iterator)->start();
}

WCAD_API int wcad_iterator_done(void* iterator) {
    return static_cast<GiEntityIterator*>(iterator)->done() ? 1 : 0;
}

WCAD_API void wcad_iterator_step(void* iterator) {
    static_cast<GiEntityIterator*>(iterator)->step();
}

WCAD_API void* wcad_iterator_get_entity(void* iterator) {
    return static_cast<void*>(
        static_cast<GiEntityIterator*>(iterator)->getEntity());
}


// ============================================================================
// EdUserInputManager
// ============================================================================

WCAD_API void wcad_input_set_device_pixel_ratio(void* manager, double ratio) {
    static_cast<EdUserInputManager*>(manager)->setDevicePixelRatio(ratio);
}

WCAD_API void wcad_input_set_rang_rect(void* manager,
    double top, double bottom, double left, double right)
{
    static_cast<EdUserInputManager*>(manager)->setRangRect(
        top, bottom, left, right);
}

WCAD_API void wcad_input_set_screen_size(void* manager,
    double width, double height)
{
    static_cast<EdUserInputManager*>(manager)->setScreenSize(width, height);
}

WCAD_API void wcad_input_user_input(void* manager,
    const char* event_type, const char* json_data)
{
    static_cast<EdUserInputManager*>(manager)->userInput(
        std::string(event_type), std::string(json_data));
}


// ============================================================================
// GeMatrix3d
// ============================================================================

WCAD_API double wcad_matrix3d_get(void* matrix, int row, int col) {
    return static_cast<GeMatrix3d*>(matrix)->operator()(row, col);
}


// ============================================================================
// ReCamera
// ============================================================================

static std::string s_cameraInfoBuffer;

WCAD_API const char* wcad_camera_information(void* camera) {
    s_cameraInfoBuffer = static_cast<ReCamera*>(camera)->information();
    return s_cameraInfoBuffer.c_str();
}


#endif // !__EMSCRIPTEN__
