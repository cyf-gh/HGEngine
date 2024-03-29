//
// Created by cyf-m on 2020/12/12.
//

#ifndef HONEYGAME_CAMERA_H
#define HONEYGAME_CAMERA_H

#include "GameObject.h"

namespace HGEngine {
namespace V1SDL {
class Effect;
class Camera : public GameObject {
public:
	GameObject* pTarget;
	/// \brief 是否仅渲染Camera视野中的GameObjects <br>whether render GameObjects in view only
	bool RenderInViewOnly;

	explicit Camera( const char* strName, Scene* pScene = nullptr );
	void Update( void* pEvent ) override;
	void Render( void* pRenderer ) override;
	GameObject* Clone() override { return new Camera( *this ); };
	Effect *GetEffect() const;
	/// \brief 将摄像机大小设置为Renderer输出的同样大小 <br>set view size of camera the same size as renderer output size
	void SetCameraSizeToRendererSize();
	virtual ~Camera() { }
};
}
}
#endif //HONEYGAME_CAMERA_H
