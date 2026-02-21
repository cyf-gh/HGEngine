#include "Texture.h"

using namespace HG::Math;

void HGEngine::V1SDL::Texture::SetAlphaMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(pHandle, blending);
}

void HGEngine::V1SDL::Texture::SetAlpha(un8 a) {
	SDL_SetTextureAlphaMod(pHandle, a);
}

HGEngine::V1SDL::Texture::Texture(const char* strTexture, const char* pStrFileName)
	: HG::HGObject<Texture>(strTexture), m_strFileName(pStrFileName) {
	pHandle = EngineImpl::GetEngine()->GetRenderer2D()->CreateTextureFromFile(pStrFileName);
}

const HGSize<int>& HGEngine::V1SDL::Texture::GetSize() {
	SDL_QueryTexture( pHandle, NULL, NULL, &m_tSize.W, &m_tSize.H );
	return m_tSize;
}

inline HGEngine::V1SDL::Texture::~Texture() {
	SDL_DestroyTexture(pHandle);
}
