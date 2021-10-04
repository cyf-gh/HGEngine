//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_SCENE_H
#define HONEYGAME_SCENE_H

#include "../Engine/HGObject.h"
#include "../Engine/HGBehaviour.h"
#include "GameObject.h"
#include <unordered_map>

namespace __HGImpl {
namespace V1SDL {
class Camera;
/// \brief ��Ϸ���� ��Scene��
/// \note 
/// * Scene::OnAttach��EngineImpl::NavigateScene������ ��Scene::OnAttach will be invoked in EngineImpl::NavigateScene��
/// * ���еĳ�����Ӧ����Engine�Ĺ���֮ǰ��� ��all scene should be constructed before the construction of Engine��
class Scene : public HG::HGObject<Scene> {
protected:
	std::unordered_map<std::string, GameObject*> umGameObjectsByName;
	Camera *m_pMainCamera;

public:
	Camera *GetMainCamera() const { return m_pMainCamera; }
	void SetMainCamera( Camera * pCamera ) { m_pMainCamera = pCamera; }
	/// \brief ��һ��GameObject���볡�� ��attach a new game object to scene��
	/// \note 
	/// * ͬ��GameObject�����Ḳ��ԭ�е�GameObject ��game object which has a duplicated name will not recover the old one��
	/// * �÷�����Ӧ�����û����ã�GameObject�Ĺ��콫�Զ����ø÷��� �� this method should NOT be invoked by user; it will be invoked in the constructor of GameObject��
	/// \sa GameObject::GameObject
	void AttachGameObject( GameObject* pGameObject );
	/// \brief ͨ��NameѰ��GameObject ��find game object by name��
	/// \return ��δ�ҵ���Ӧ���ֵ�GameObjectʱ������nullptr ��nullptr when the specified game object does not exist��
	GameObject* FindGameObject( const char* strName );

	void Update( void* pEvent );
	void Render( void* pRenderer );

	HG::pEvent OnAttach;

	explicit Scene( const char* strName );
	virtual ~Scene();
};
}
}

#endif //HONEYGAME_SCENE_H
