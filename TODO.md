# HGEngine 开发计划

## 高优先级 ✅ (已完成)

### 内存管理
- [x] **实现智能指针 (std::unique_ptr/shared_ptr)**
  - 减少手动内存管理风险
  - 迁移 HGComponent、GameObject 等核心类的所有权管理
  - 位置: `src/Core/Memory.h`
  - 实现: `UniquePtr<T>`, `SharedPtr<T>` 封装类

- [x] **实现对象池 (Object Pool)**
  - 减少频繁分配/释放带来的开销
  - 特别适用于 GameObject、Component 等高频创建销毁的对象
  - 位置: `src/Core/Memory.h`
  - 实现: `HG::Memory::ObjectPool<T>` 线程安全对象池

### 线程安全
- [x] **为静态map添加互斥锁**
  - HGObject 的静态成员 `umTheseOnes` 和 `umTheseOnesById` 在多线程下不安全
  - 需要添加 std::mutex 保护
  - 位置: `src/Engine/HGObject.h`
  - 实现: 添加 `static std::mutex s_Mutex` 保护所有静态操作

### 错误处理
- [x] **添加异常处理机制**
  - 当前仅返回 HGResult，缺少详细错误信息
  - 实现 HGException 类
  - 位置: `src/Core/Error.h`
  - 实现: `HG::Exception` 类, `HG::Result` 类, `HG_THROW` 等宏

### 序列化
- [x] **完善 JSON 序列化支持**
  - RapidJSON 已集成但未充分利用
  - 实现 GameObject/Component 的 Save/Load 方法
  - 位置: `src/Core/Serialization.h`
  - 实现: `ToJson<T>()`, `FromJson<T>()`, `SaveToFile<T>()`, `LoadFromFile<T>()`

---

## 中优先级 ✅ (已完成)

### 构建系统
- [x] **修复 CMake 构建**
  - README 提到 CMake 目前不工作
  - 位置: `src/CMakeLists.txt`
  - 实现: 完整的CMake配置，支持SDL2/SDL2_image/SDL2_ttf从third_party加载

### 测试
- [x] **集成单元测试框架**
  - utest.h 已集成但无自动化测试
  - 添加 CMake 测试目标
  - 位置: `src/Core/Test.hpp`, `src/Core/test/main.cpp`
  - 实现: `TestSuite` 类, `HG_TEST_REGISTER`/`HG_TEST_RUN_ALL` 宏

### 日志系统
- [x] **实现分级日志 (DEBUG/INFO/WARN/ERROR)**
  - 当前日志系统过于简单
  - 添加日志级别过滤和输出目标控制
  - 位置: `src/EngineImpl/Log.hpp`
  - 实现: `LogLevel` 枚举, `HGLog` 改进, 线程安全, 文件和控制台输出

### 配置管理
- [x] **添加配置文件支持**
  - 当前硬编码值过多 (如窗口大小、帧率等)
  - 使用 JSON 格式的配置文件
  - 位置: `src/Core/Config.h` (新建)
  - 实现: `Config` 类, Get/Set方法, 文件加载/保存, 默认值宏

### 资源管理
- [x] **实现资源池/缓存**
  - 避免重复加载相同资源
  - 添加资源引用计数
  - 位置: `src/EngineImpl/AssetManager.h` (新建)
  - 实现: AssetManager 类, ResourceHandle, 缓存, 引用计数

---

## 代码质量 ✅ (已完成)

### 类型安全
- [x] **用 enum class 替换 enum**
  - 避免隐式类型转换
  - 位置: `src/Engine/HGComponent.h`, `src/EngineImpl/Collision.h`, `src/EngineImpl/Layer.h`, `src/EngineImpl/Animation.h`
  - 转换: `HGRenderableComponentSeq`, `HG_LAYER_INDEX`, `HG_ANIMATOR_PLAYMODE`

### 常量正确性
- [x] **标记成员函数 const**
  - 许多 getter 方法应标记为 const
  - 修复: `IsInCameraView()` 添加 const

### 字符串处理
- [ ] **用 std::string_view 减少拷贝** (跳过 - 需要C++17，与C-like风格冲突)
  - 当前大量使用 const char*
  - 位置: `src/Engine/HGObject.h`

### 命名规范
- [x] **统一命名风格**
  - 部分成员变量用 m_ 前缀，部分不用
  - 如: `mIsEnable` vs `m_isFixedToCamera`
  - 修复: `mIsEnable` -> `m_bIsEnable`, `m_isFixedToCamera` -> `m_bIsFixedToCamera`, `m_isGUI` -> `m_bIsGUI`

---

## 架构改进 ✅ (已完成)

### 事件系统
- [x] **用 std::function 替代函数指针** (跳过 - 与C-like风格冲突，保持使用函数指针)
  - 当前使用 pEvent (函数指针)
  - 保持现有实现

### 组件系统
- [x] **添加运行时动态组件注册**
  - 支持运行时添加/移除组件
  - 位置: `src/Engine/HGComponent.h`
  - 实现: `ComponentRegistry` 类, `RegisterComponent<T>()` 模板方法

### 实体组件系统 (ECS)
- [ ] **评估 ECS 架构**
  - 当前是 OOP 模式
  - 考虑迁移到 ECS 以提高性能

### 脚本系统
- [x] **添加脚本绑定支持**
  - 使用C风格函数指针 (非std::function)
  - 位置: `src/Script/Script.hpp` (新建)
  - 实现: `ScriptManager`, `HG_SCRIPT_DECLARE`, `HG_SCRIPT_IMPLEMENT` 宏

---

## 性能优化 ✅ (已完成)

### 渲染优化
- [x] **实现批渲染 (Batching)**
  - 减少 DrawCall
  - 位置: `src/EngineImpl/Renderer2D.h`
  - 实现: `RenderBatch` 类, `FlushBatch()`, `SetBatchEnabled()` 方法

### 碰撞检测优化
- [x] **实现空间分区 (网格法)**
  - 加速碰撞检测
  - 位置: `src/EngineImpl/SpatialGrid.h` (新建)
  - 实现: `SpatialGrid` 类, 网格分区算法

### 计算缓存
- [x] **缓存变换矩阵计算结果**
  - 避免每帧重复计算
  - 位置: `src/EngineImpl/Transform.hpp`
  - 实现: 脏标记模式 (dirty flag), `m_bIsDirty`, `MarkDirty()`, `UpdateCache()`

---

## 文档/工具

### 文档
- [x] **配置 Doxygen**
  - 生成 API 文档
  - 位置: `docs/Doxyfile` (新建)

### 编辑器功能
- [ ] **扩展 Editor 功能**
  - 当前仅有 LogViewer
  - 添加场景编辑器、属性面板等

### 运行时调试
- [x] **扩展 ImGui 调试功能**
  - 对象层级查看
  - 性能分析器
  - 位置: `src/EngineImpl/Editor/DebugManager.h` (新建)
  - 实现: `DebugManager` 类, 层级/性能/场景/控制台面板

---

## 缺少的功能

### 核心功能
- [x] 粒子系统 (Particle System)
  - 位置: `src/EngineImpl/ParticleSystem.h` (新建)
- [x] 音频系统 (Sound/Audio)
  - 位置: `src/EngineImpl/Audio.h` (新建)
- [ ] 动画状态机 (Animation State Machine)

### UI 系统
- [x] Label 组件 (已存在，修复enum类型)
- [x] Input 组件 (Input Field)
  - 位置: `src/EngineImpl/GUI/UIComponents.h` (新建)
- [x] Button 组件
  - 位置: `src/EngineImpl/GUI/UIComponents.h` (新建)
- [x] Slider 组件
  - 位置: `src/EngineImpl/GUI/UIComponents.h` (新建)
- [x] Listbox 组件
  - 位置: `src/EngineImpl/GUI/UIComponents.h` (新建)
- [x] Checkbox 组件
  - 位置: `src/EngineImpl/GUI/UIComponents.h` (新建)
- [x] 布局系统 (Layout)
  - 位置: `src/EngineImpl/GUI/Layout.h` (新建)
  - 实现: HorizontalLayout, VerticalLayout, GridLayout, RelativeLayout, FlowLayout
- [x] 动画状态机 (Animation State Machine)
  - 位置: `src/EngineImpl/AnimationStateMachine.h` (新建)

### 其他
- [ ] 地图/瓦片编辑器支持
- [ ] 网络/多人支持
- [ ] 热重载 (代码/资源)

---

## 备注

- 当前 C++ 标准: C++11
- 编译器: MSVC (推荐), MinGW (实验性)
- 依赖: SDL2, SDL2_image, SDL2_ttf, RapidJSON, nameof, UDRefl, imgui
