#pragma once

#include <typeinfo>
#include "HGObject.h"

namespace HGEngine { namespace V1SDL { class GameObject; } }

namespace HGEngine {
namespace V1SDL {
class Renderer2D;
class Texture;
}}

namespace HG {
enum HGRenderableComponentSeq : int {
	UNRENDERABLE = -999,
	DEFAULT = 0,
	SPRITE,
	GEOMETRY,
	LABEL,
	EFFECT,
	HGRENDERABLECOMPONENTSEQ_LENTH
};
/// \brief 
/// 组件基类，其集合由GameObject拥有<br>
/// Component base class, the set of components is owned by GameObject
/// \note 
/// 原则上不允许组件有互相调用的情况
/// \see HGEngine::V1SDL::GameObject
class HGComponent : public HGObject<HGComponent> {
protected:
	HGEngine::V1SDL::GameObject *m_pGameObject;
	
public:
	/// @brief 组件引用计数
	/// @see 
	///	* GameObject::~GameObject()
	/// * GameObject::RemoveComponent()
	/// * GameObject::AddComponent( HG::HGComponent* pComp )
	int nRefCount;
	/// @brief 渲染索引，值越小越渲染在下层
	int nRenderIndex;
	/// @brief 获取可渲染组件位于 GameObject 本地坐标的偏移
	/// @return 
	virtual HG::Math::HGRect* GetLocalRectOffset() { return nullptr; }
	/// @brief 返回渲染目标
	/// @note 仅在 IsRenderable() == true 时返回句柄
	/// @return 渲染句柄
	virtual HGEngine::V1SDL::Texture* GetRenderTarget( HGEngine::V1SDL::Renderer2D* pRd ) { return nullptr; }
	/// @brief 返回该组件是否能被渲染
	/// @return 是否能被渲染
	virtual bool IsRenderable() { return false; }
	/// @brief 声明该组件可渲染
	#define HG_COMPONENT_RENDERABLE bool IsRenderable() override { return true; }
	/// @brief 该组件在GameObject是否是唯一的
	/// @return 是否唯一
	virtual bool IsOneOnlyPerGameObject() { return true; }
	/// @brief 声明该组件可重复存在于同一GameObject
	#define HG_COMPONENT_MUTILABLE bool IsOneOnlyPerGameObject() override { return false; }
	/// @brief 返回父GameObject
	/// @return 父GameObject
	HGEngine::V1SDL::GameObject *GetGameObject() const { return m_pGameObject; }
	void SetGameObject( HGEngine::V1SDL::GameObject *pGameObject ) { m_pGameObject = pGameObject; }
	explicit HGComponent( const char* strName ) : nRefCount( 0 ), nRenderIndex( HGRenderableComponentSeq::UNRENDERABLE ), m_pGameObject( nullptr ), HGObject<HGComponent>( strName ) { }
	explicit HGComponent() : nRefCount( 0 ), nRenderIndex( HGRenderableComponentSeq::UNRENDERABLE ), m_pGameObject( nullptr ), HGObject<HGComponent>() { }
	virtual ~HGComponent() = default;
};
}