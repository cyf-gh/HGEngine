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
/// \brief 游戏场景 （Scene）
/// \note 
/// * Scene::OnAttach于EngineImpl::NavigateScene被调用 （Scene::OnAttach will be invoked in EngineImpl::NavigateScene）
/// * 所有的场景都应当在Engine的构造之前完成 （all scene should be constructed before the construction of Engine）
class Scene : public HG::HGObject<Scene> {
protected:
	std::unordered_map<std::string, GameObject*> umGameObjectsByName;
	Camera *m_pMainCamera;

public:
	Camera *GetMainCamera() const { return m_pMainCamera; }
	void SetMainCamera( Camera * pCamera ) { m_pMainCamera = pCamera; }
	/// \brief 将一个GameObject加入场景 （attach a new game object to scene）
	/// \note 
	/// * 同名GameObject并不会覆盖原有的GameObject （game object which has a duplicated name will not recover the old one）
	/// * 该方法不应该由用户调用，GameObject的构造将自动调用该方法 （ this method should NOT be invoked by user; it will be invoked in the constructor of GameObject）
	/// \sa GameObject::GameObject
	void AttachGameObject( GameObject* pGameObject );
	/// \brief 通过Name寻找GameObject （find game object by name）
	/// \return 当未找到相应名字的GameObject时，返回nullptr （nullptr when the specified game object does not exist）
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
