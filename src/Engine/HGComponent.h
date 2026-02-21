#pragma once

#include <typeinfo>
#include <unordered_map>
#include <string>
#include "HGObject.h"

namespace HGEngine { namespace V1SDL { class GameObject; } }

namespace HGEngine {
namespace V1SDL {
class Renderer2D;
class Texture;
}}

namespace HG {

/// \brief Component type info for runtime registration
struct ComponentTypeInfo {
    const char* name;
    size_t typeId;
    HGComponent* (*creator)();
};

/// \brief Component registry for runtime type management
class ComponentRegistry {
private:
    static std::unordered_map<std::string, ComponentTypeInfo>* s_Registry;
    static std::unordered_map<size_t, ComponentTypeInfo>* s_TypeIdRegistry;

public:
    /// \brief Register a component type
    static void Register(const char* name, size_t typeId, HGComponent* (*creator)()) {
        if (s_Registry == nullptr) {
            s_Registry = new std::unordered_map<std::string, ComponentTypeInfo>();
            s_TypeIdRegistry = new std::unordered_map<size_t, ComponentTypeInfo>();
        }
        ComponentTypeInfo info = { name, typeId, creator };
        (*s_Registry)[name] = info;
        (*s_TypeIdRegistry)[typeId] = info;
    }

    /// \brief Create component by name
    static HGComponent* Create(const char* name) {
        if (s_Registry == nullptr) return nullptr;
        auto it = s_Registry->find(name);
        if (it != s_Registry->end() && it->second.creator) {
            return it->second.creator();
        }
        return nullptr;
    }

    /// \brief Create component by type id
    static HGComponent* Create(size_t typeId) {
        if (s_TypeIdRegistry == nullptr) return nullptr;
        auto it = s_TypeIdRegistry->find(typeId);
        if (it != s_TypeIdRegistry->end() && it->second.creator) {
            return it->second.creator();
        }
        return nullptr;
    }

    /// \brief Check if component type is registered
    static bool IsRegistered(const char* name) {
        if (s_Registry == nullptr) return false;
        return s_Registry->find(name) != s_Registry->end();
    }

    /// \brief Get type info by name
    static const ComponentTypeInfo* GetTypeInfo(const char* name) {
        if (s_Registry == nullptr) return nullptr;
        auto it = s_Registry->find(name);
        return (it != s_Registry->end()) ? &it->second : nullptr;
    }
};

enum class HGRenderableComponentSeq : int {
	UNRENDERABLE = -999,
	DEFAULT = 0,
	SPRITE,
	GEOMETRY,
	LABEL,
	EFFECT,
	HGRENDERABLECOMPONENTSEQ_LENTH
};

/// \brief 
/// ������࣬�伯����GameObjectӵ��<br>
/// Component base class, the set of components is owned by GameObject
/// \note 
/// ԭ���ϲ���������л�����õ����
/// \see HGEngine::V1SDL::GameObject
class HGComponent : public HGObject<HGComponent> {
protected:
	HGEngine::V1SDL::GameObject *m_pGameObject;
	
public:
	/// @brief ������ü���
	/// @see 
	///	* GameObject::~GameObject()
	/// * GameObject::RemoveComponent()
	/// * GameObject::AddComponent( HG::HGComponent* pComp )
	int nRefCount;
	/// @brief ��Ⱦ������ֵԽСԽ��Ⱦ���²�
	int nRenderIndex;

	/// @brief ��ȡ����Ⱦ���λ�� GameObject ���������ƫ��
	/// @return 
	virtual HG::Math::HGRect* GetLocalRectOffset() { return nullptr; }
	/// @brief ������ȾĿ��
	/// @note ���� IsRenderable() == true ʱ���ؾ��
	/// @return ��Ⱦ���
	virtual HGEngine::V1SDL::Texture* GetRenderTarget( HGEngine::V1SDL::Renderer2D* pRd ) { return nullptr; }
	/// @brief ���ظ�����Ƿ��ül�Ⱦ
	/// @return �Ƿ��ül�Ⱦ
	virtual bool IsRenderable() { return false; }
	/// @brief ������������Ⱦ
	#define HG_COMPONENT_RENDERABLE bool IsRenderable() override { return true; }
	/// @brief �������GameObject�Ƿ���Ψһ��
	/// @return �Ƿ�Ψһ
	virtual bool IsOneOnlyPerGameObject() { return true; }
	/// @brief ������������ظ�������ͬһGameObject
	#define HG_COMPONENT_MUTILABLE bool IsOneOnlyPerGameObject() override { return false; }
	/// @brief ���ظ�GameObject
	/// @return ��GameObject
	HGEngine::V1SDL::GameObject *GetGameObject() const { return m_pGameObject; }
	void SetGameObject( HGEngine::V1SDL::GameObject *pGameObject ) { m_pGameObject = pGameObject; }

	/// \brief Get component type name
	virtual const char* GetTypeName() const { return "HGComponent"; }

	/// \brief Get component type id
	virtual size_t GetTypeId() const { return typeid(HGComponent).hash_code(); }

	/// \brief Register this component type (call in static initializer)
	template<typename T>
	static void RegisterComponent(const char* name) {
		ComponentRegistry::Register(name, typeid(T).hash_code(), []() -> HGComponent* { return new T(); });
	}

	explicit HGComponent( const char* strName ) : nRefCount( 0 ), nRenderIndex( (int)HGRenderableComponentSeq::UNRENDERABLE ), m_pGameObject( nullptr ), HGObject<HGComponent>( strName ) { }
	explicit HGComponent() : nRefCount( 0 ), nRenderIndex( (int)HGRenderableComponentSeq::UNRENDERABLE ), m_pGameObject( nullptr ), HGObject<HGComponent>() { }
	virtual ~HGComponent() = default;
};
}