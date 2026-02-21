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
	/// @brief ���ظ�����Ƿ��ܱ���Ⱦ
	/// @return �Ƿ��ܱ���Ⱦ
	virtual bool IsRenderable() { return false; }
	/// @brief �������������Ⱦ
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
	explicit HGComponent( const char* strName ) : nRefCount( 0 ), nRenderIndex( HGRenderableComponentSeq::UNRENDERABLE ), m_pGameObject( nullptr ), HGObject<HGComponent>( strName ) { }
	explicit HGComponent() : nRefCount( 0 ), nRenderIndex( HGRenderableComponentSeq::UNRENDERABLE ), m_pGameObject( nullptr ), HGObject<HGComponent>() { }
	virtual ~HGComponent() = default;
};
}