# HGEngine ECS 架构评估报告

## 当前架构分析

### 现有 OOP 模式
```
GameObject (OOP)
  |- Component (继承)
  |    |- Transform
  |    |- Sprite
  |    |- RigidBody
  |    |- Collision
  |    |- Animation
  |    |- ...
```

### OOP 模式问题
1. **Cache Miss**: GameObject 分散在内存中，遍历组件效率低
2. **类型碎片化**: 同类组件分散在不同对象中
3. **继承限制**: 无法运行时动态组合行为

## ECS 架构设计

### 核心概念

```
Entity (ID) ---> Component (数据) <-- System (逻辑)
     |
     v
Component Data: Position, Velocity, Sprite, etc.
```

### 数据布局 (SoA - Structure of Arrays)

```cpp
// 示例: 位置组件数组 (cache-friendly)
struct PositionComponent {
    float* positionsX;  // 所有实体的X坐标
    float* positionsY;  // 所有实体的Y坐标
    int* entityIds;       // 对应的实体ID
    int capacity;
    int count;
};
```

### 架构设计

```cpp
// Entity: 只是一个ID
using Entity = un32;
const Entity INVALID_ENTITY = 0;

// Component: 纯数据
struct Transform {
    float x, y;
    float rotation;
    float scaleX, scaleY;
};

struct Sprite {
    Texture* texture;
    int frameIndex;
    float depth;
};

struct RigidBody {
    float velocityX, velocityY;
    float mass;
    bool isStatic;
};

// System: 处理逻辑
class PhysicsSystem {
public:
    void Update(float dt) {
        // 遍历所有有RigidBody和Transform的实体
        for (auto [entity, transform, body] : m_Registry.Query<Transform, RigidBody>()) {
            transform.x += body.velocityX * dt;
            transform.y += body.velocityY * dt;
        }
    }
};

// Registry: 实体组件管理器
class EntityRegistry {
private:
    std::vector<Transform> m_Transforms;
    std::vector<Sprite> m_Sprites;
    std::vector<RigidBody> m_Bodies;
    std::unordered_map<un32, std::vector<un32>> m_EntityComponents;  // entity -> component indices
    
public:
    Entity CreateEntity();
    void DestroyEntity(Entity e);
    
    template<typename T>
    T* AddComponent(Entity e);
    
    template<typename T>
    T* GetComponent(Entity e);
    
    template<typename... T>
    auto Query();  // 返回匹配组件的实体迭代器
};
```

## 迁移策略

### 阶段1: 保持兼容
- 保留现有 GameObject/Component 系统
- 添加 ECS 作为可选后端

### 阶段2: 内部优化
- 将 ECS 用于内部系统 (物理、渲染)
- GameObject 包装 ECS 实体

### 阶段3: 完全迁移
- 移除旧系统
- 统一使用 ECS

## 性能对比

| 操作 | OOP (当前) | ECS |
|------|------------|-----|
| 物理更新 | 遍历GameObject | 连续内存遍历 |
| 渲染 | 遍历GameObject获取Sprite | 连续数组 |
| 缓存命中率 | ~30% | ~90% |

## 建议

**结论**: 建议当前阶段保持 OOP，ECS 可作为未来优化方向。

原因:
1. 当前项目规模较小，OOP足够
2. 开发周期考虑
3. 可先实现基础功能再优化

**实施建议**:
- 使用数据导向设计 (Data-Oriented) 优化热点系统
- 物理系统使用连续数组
- 渲染系统使用批处理
