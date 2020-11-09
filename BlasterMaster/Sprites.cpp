#include "Sprites.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

CSpriteLib* CSpriteLib::__instance = nullptr;

CSpriteLib* CSpriteLib::GetInstance()
{
	if (__instance == nullptr) __instance = new CSpriteLib();
	return __instance;
}

void CSprite::Draw(float x, float y, int alpha, bool flipX, int rotate)
{
	CGame * game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, alpha, flipX, rotate);
}

void CSpriteLib::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, right, bottom);
}

LPSPRITE CSpriteLib::Get(int id)
{
	return sprites[id];
}

/*
	Clear all loaded textures
*/
void CSpriteLib::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}



