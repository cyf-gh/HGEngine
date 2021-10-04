//
// Created by cyf-m on 2020/12/12.
//

#ifndef HONEYGAME_CAMERA_H
#define HONEYGAME_CAMERA_H

#include "GameObject.h"
#include "Transform.hpp"

namespace __HGImpl {
namespace V1SDL {

class Camera : public GameObject {
public:
	GameObject* pTarget;
	/// \brief �Ƿ����ȾCamera��Ұ�е�GameObjects <br>whether render GameObjects in view only
	bool RenderInViewOnly;

	explicit Camera( const char* strName, Scene* pScene = nullptr ) : GameObject( strName ), RenderInViewOnly( true ), pTarget( nullptr ) {
		AddComponent( new Transform( "Transform" ) );
	}
	void Update( void* pEvent ) override;
	void Render( void* pRenderer ) override;
	GameObject* Clone() override { return new Camera( *this ); };
	/// \brief ���������С����ΪRenderer�����ͬ����С <br>set view size of camera the same size as renderer output size
	void SetCameraSizeToRendererSize();
	virtual ~Camera() { }
};
}
}
#endif //HONEYGAME_CAMERA_H
