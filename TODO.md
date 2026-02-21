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

## 中优先级

### 构建系统
- [ ] **修复 CMake 构建**
  - README 提到 CMake 目前不工作
  - 位置: `src/CMakeLists.txt`

### 测试
- [ ] **集成单元测试框架**
  - utest.h 已集成但无自动化测试
  - 添加 CMake 测试目标
  - 位置: `src/Core/Test.hpp`

### 日志系统
- [ ] **实现分级日志 (DEBUG/INFO/WARN/ERROR)**
  - 当前日志系统过于简单
  - 添加日志级别过滤和输出目标控制
  - 位置: `src/EngineImpl/Log.hpp`

### 配置管理
- [ ] **添加配置文件支持**
  - 当前硬编码值过多 (如窗口大小、帧率等)
  - 使用 JSON 格式的配置文件
  - 新建: `src/Core/Config.h`

### 资源管理
- [ ] **实现资源池/缓存**
  - 避免重复加载相同资源
  - 添加资源引用计数
  - 位置: `src/EngineImpl/Asset.h`

---

## 代码质量

### 类型安全
- [ ] **用 enum class 替换 enum**
  - 避免隐式类型转换
  - 位置: `src/Engine/HGComponent.h`, `src/EngineImpl/Collision.h`

### 常量正确性
- [ ] **标记成员函数 const**
  - 许多 getter 方法应标记为 const
  - 检查: `GetName()`, `GetParent()`, `IsEnable()` 等

### 字符串处理
- [ ] **用 std::string_view 减少拷贝**
  - 当前大量使用 const char*
  - 位置: `src/Engine/HGObject.h`

### 命名规范
- [ ] **统一命名风格**
  - 部分成员变量用 m_ 前缀，部分不用
  - 如: `mIsEnable` vs `m_isFixedToCamera`

---

## 架构改进

### 事件系统
- [ ] **用 std::function 替代函数指针**
  - 当前使用 pEvent (函数指针)
  - 支持 lambda 和绑定器
  - 位置: `src/Engine/HGEvent.hpp`

### 组件系统
- [ ] **添加运行时动态组件注册**
  - 支持运行时添加/移除组件
  - 位置: `src/Engine/HGComponent.h`

### 实体组件系统 (ECS)
- [ ] **评估 ECS 架构**
  - 当前是 OOP 模式
  - 考虑迁移到 ECS 以提高性能

### 脚本系统
- [ ] **添加脚本绑定支持**
  - 可选: Lua 或 Python
  - 新建: `src/Script/`

---

## 性能优化

### 渲染优化
- [ ] **实现批渲染 (Batching)**
  - 减少 DrawCall
  - 位置: `src/EngineImpl/Renderer2D.h`

### 碰撞检测优化
- [ ] **实现空间分区 (四叉树/网格)**
  - 加速碰撞检测
  - 位置: `src/EngineImpl/Collision.h`

### 计算缓存
- [ ] **缓存变换矩阵计算结果**
  - 避免每帧重复计算
  - 位置: `src/EngineImpl/Transform.hpp`

---

## 文档/工具

### 文档
- [ ] **配置 Doxygen**
  - 生成 API 文档
  - 新建: `docs/Doxyfile`

### 编辑器功能
- [ ] **扩展 Editor 功能**
  - 当前仅有 LogViewer
  - 添加场景编辑器、属性面板等

### 运行时调试
- [ ] **扩展 ImGui 调试功能**
  - 对象层级查看
  - 性能分析器
  - 位置: `src/EngineImpl/Editor/`

---

## 缺少的功能

### 核心功能
- [ ] 音频系统 (Sound/Audio)
- [ ] 粒子系统 (Particle System)
- [ ] 动画状态机 (Animation State Machine)

### UI 系统
- [ ] Label 组件
- [ ] Input 组件
- [ ] Slider 组件
- [ ] Listbox 组件
- [ ] 布局系统 (Layout)

### 其他
- [ ] 地图/瓦片编辑器支持
- [ ] 网络/多人支持
- [ ] 热重载 (代码/资源)

---

## 备注

- 当前 C++ 标准: C++11
- 编译器: MSVC (推荐), MinGW (实验性)
- 依赖: SDL2, SDL2_image, SDL2_ttf, RapidJSON, nameof, UDRefl, imgui
