#include "Texture.h"

using namespace HG::Math;

const HGSize<int>& HGEngine::V1SDL::Texture::GetSize() {
	SDL_QueryTexture( pHandle, NULL, NULL, &m_tSize.W, &m_tSize.H );
	return m_tSize;
}