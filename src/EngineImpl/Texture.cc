#include "Texture.h"

using namespace HG::Math;

HGSize<int> HGEngine::V1SDL::Texture::GetSize() {
	HGSize<int> size;
	SDL_QueryTexture( pHandle, NULL, NULL, &size.W, &size.H );
	return size;
}