# HGEngine 代码风格规范

## 概述
本规范参考DOOM (1993) 源代码风格，采用C-like C++风格，避免过度使用现代C++特性。

## 命名规范

### 常量
- 全大写+下划线: `MAX_BUFFER_SIZE`
- 枚举值: `kEnumValue` 或 `ENUM_VALUE`

### 变量
- 成员变量: `m_nValue` (m_前缀 + 匈牙利 notation)
- 全局变量: `g_nValue`
- 静态变量: `s_nValue`

### 函数
- 驼峰命名: `GetValue()`, `CalculatePosition()`

### 类/结构体
- 帕斯卡命名: `class GameObject`, `struct Vector2`

## 禁止使用的现代C++特性

| 特性 | 原因 | 替代方案 |
|------|------|----------|
| `auto` | 降低可读性 | 显式类型声明 |
| `lambda` | 运行时开销 | 函数指针 |
| `std::function` | 堆分配 | 函数指针 |
| `constexpr` (C++11+) | 兼容性 | `const` |
| `std::string_view` | C++17 | `const char*` |
| `std::optional` | C++17 | 指针或特殊值 |
| `std::variant` | C++17 | 联合或类型标志 |
| `auto` lambda | 降低可读性 | 显式类型 |
| 范围for循环 (auto&) | C++11 | 传统for循环 |

## 推荐使用

### 类型定义
使用已有的类型定义:
```cpp
typedef char        ch8;
typedef int8_t      n8;
typedef int16_t     n16;
typedef int32_t     n32;
typedef int64_t     n64;
typedef uint8_t     un8;
typedef uint16_t    un16;
typedef uint32_t    un32;
typedef uint64_t    un64;
typedef float       f32;
typedef double      f64;
```

### 内联函数
使用 `inline` 代替宏进行简单操作:
```cpp
inline int Max(int a, int b) { return a > b ? a : b; }
inline void SafeDelete(void*& p) { delete p; p = nullptr; }
```

### 指针
- 优先使用 `T*` 而非智能指针 (除非有所有权问题)
- 使用 `nullptr` 而非 `NULL`

## 宏的使用原则

### 允许使用宏的情况
1. **条件编译**: `#ifdef DEBUG`, `#pragma once`
2. **代码生成**: 序列化/反序列化宏
3. **复杂算法**: 无法用函数表达的算法 (如 `HG_ERASE_IN_VEC`)
4. **类型无关的通用操作**: 保留 `HG_SAFE_DEL`

### 应转换为内联函数的情况
1. 简单计算: `HG_EMPTY_STRING` → `inline const char* EmptyString()`
2. 函数调用封装: `HG_RANDOM_UN32` → `inline un32 RandomUInt()`
3. 访问器: `HG_ENGINE()` → `inline Engine* GetEngine()`

## 代码格式

### 括号风格 (DOOM风格)
```cpp
if (condition)
{
    DoSomething();
}
else
{
    DoOther();
}

while (condition)
{
    LoopBody();
}

for (i = 0; i < MAX; i++)
{
    // ...
}
```

### 函数返回类型
```cpp
// DOOM风格: 返回类型在单独行
int
CalculateSum(int a, int b)
{
    return a + b;
}

// 或者一行
int CalculateSum(int a, int b);
```

### 注释风格
```cpp
// 单行注释

/*
 * 块注释
 * 第二行
 */
```

## 调试相关

### 断言
```cpp
// 使用 assert 或自定义
#ifdef DEBUG
#define ASSERT(expr) ((expr) ? (void)0 : AssertFailed(#expr, __FILE__, __LINE__))
#else
#define ASSERT(expr) ((void)0)
#endif
```

## 示例对比

### 不推荐 (现代C++风格)
```cpp
auto GetComponent = [](auto& obj) { return obj.component; };
auto values = Filter(items, [](auto& i) { return i > 0; });
const auto& ref = obj;
```

### 推荐 (C-like风格)
```cpp
Component* GetComponent(GameObject* obj);
int FilterItems(const int* items, int count, int* output);
const int& GetRef(const Object& obj);
```

## 性能优先情况

以下情况可使用现代C++特性:
- 热点路径的优化
- 模板元编程
- 需要RAll的资源管理
- 特定库接口要求
