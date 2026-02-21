//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_SCENE_H
#define HONEYGAME_SCENE_H

#include <unordered_map>
#include "../Engine/HGObject.h"
#include "../Engine/HGBehaviour.h"
#include "GameObject.h"
#include "Layer.h"

namespace HGEngine {
namespace V1SDL {
class Camera;
class GUI;
/// \brief ��Ϸ���� <br>Scene
/// \note 
/// * Scene::OnAttach��EngineImpl::NavigateScene������ <br>Scene::OnAttach will be invoked in EngineImpl::NavigateScene
/// * ���еĳ�����Ӧ����Engine�Ĺ���֮ǰ��� <br>all scene should be constructed before the construction of Engine
class Scene : public HG::HGObject<Scene> {
protected:
	std::unordered_map<std::string, GameObject*> umGameObjectsByName;
	std::vector<Layer*> m_vecLayers;
	Camera *m_pMainCamera;
	std::unordered_map<std::string, GUI*> m_umGUIs;
	bool m_bIsStart;
public:
	#ifdef HGENGINE_GUI
	GUI* TryCreateGUI( const std::string& name, bool isVisiable = false );
	GUI* GetGUI( const std::string& name );
	#endif 
	Camera *GetMainCamera() const { return m_pMainCamera; }
	void SetMainCamera( Camera * pCamera );
	/// \brief ��һ��GameObject���볡��<br>attach a new game object to scene
	/// \note 
	/// * ͬ��GameObject�����Ḳ��ԭ�е�GameObject <br>game object which has a duplicated name will not recover the old one
	/// * �÷�����Ӧ�����û����ã�GameObject�Ĺ��콫�Զ����ø÷��� <br> this method should NOT be invoked by user; it will be invoked in the constructor of GameObject
	/// \sa GameObject::GameObject
	void AttachGameObject( GameObject* pGameObject, char LayerIndex = 0 );
	/// \brief ͨ��NameѰ��GameObject<br>find game object by name
	/// \return ��δ�ҵ���Ӧ���ֵ�GameObjectʱ������nullptr<br>nullptr when the specified game object does not exist
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
