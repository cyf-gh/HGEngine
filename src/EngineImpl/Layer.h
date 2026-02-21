#pragma once 

#include <vector>
#include <Memory.h>
#include "GameObject.h"

namespace HGEngine {
namespace V1SDL {

enum HG_LAYER_INDEX : char {
	HG_LAYER_0 = 0,
	HG_LAYER_1,
	HG_LAYER_2,
	HG_LAYER_3,
	HG_LAYER_4,
	HG_LAYER_5,
	HG_LAYER_6,
	HG_LAYER_7,
	HG_LAYER_8,
	HG_LAYER_9,
	HG_LAYER_10,
	HG_LAYER_11,
	HG_LAYER_12,
	HG_LAYER_13,
	HG_LAYER_14,
	HG_LAYER_15,
	HG_LAYER_16,
	HG_LAYER_GUI,
	HG_LAYER_CAMERA,
	HG_LAYER_LENGTH
};

class Layer : public HG::HGObject<Layer>, public HG::Memory::NonCopyable {
protected:
	std::vector<GameObject*> m_vecObjs;
	std::vector<GameObject*> m_vecX;
	std::vector<GameObject*> m_vecY;

public:
	/// \see HGEngine::V1SDL::HG_LAYER_INDEX
	const char LayerIndex;

	void DoCheck();

	/// \brief ������ڵĲ㼶�Ƿ�Ϸ�
	bool IsLayerIndexLegal() const { return LayerIndex >= HG_LAYER_0 && LayerIndex < HG_LAYER_LENGTH; }

	/// \brief
	/// ��һ�� GameObject ������ Layer
	void AttachGameObject( GameObject* pGameObject );
	/// \brief
	/// ��һ�� GameObject �� Layer �Ƴ�
	///	\return 
	/// �������ڸ� GameObject ʱ���� false
	bool DetachGameObject( GameObject* pGameObject );
	bool DetachGameObject( const char *strName );
	bool DetachGameObject( const un32 Id );
	
	/// \brief
	/// ���а���X��Y�������
	/// \note
	///	Ӧ�����ƶ�����ʱ���е���
	un32 SortXY( GameObject* pGameObject );

	explicit Layer( const char* strName, const char un8Index ) : HG::HGObject<Layer>( strName ), LayerIndex( un8Index ) { }
	virtual ~Layer() { }
};
}
}