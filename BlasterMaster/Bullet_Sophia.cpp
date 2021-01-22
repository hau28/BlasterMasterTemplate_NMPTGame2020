#include "Bullet_Sophia.h"
#include "GameObjectBehaviour.h"

#include "TileArea.h"
#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"
#include "Sound.h"

CBullet_Sophia::CBullet_Sophia(int x, int y, int sectionId, int dirX, int dirY, bool enabledCrusherBeam) :
	CBullet(enabledCrusherBeam? CLASS_SOPHIA_WHITE_BULLET : CLASS_SOPHIA_BULLET, x, y, sectionId, true)
{
	this->enabledCrusherBeam = enabledCrusherBeam;

	damage = BULLET_SOPHIA_DAMAGE;

	// normalize the direction vector
	float nx, ny;
	CGameObjectBehaviour::NormalizeVector2(dirX, dirY, nx, ny);

	vx = nx * BULLET_SOPHIA_SPEED;
	vy = ny * BULLET_SOPHIA_SPEED;

	if (vx < 0)
		SetState(enabledCrusherBeam? BULLET_WHITE_SOPHIA_SIDEVIEW_STATE_LEFT : BULLET_SOPHIA_SIDEVIEW_STATE_LEFT);
	else if (vx > 0)
		SetState(enabledCrusherBeam ? BULLET_WHITE_SOPHIA_SIDEVIEW_STATE_RIGHT : BULLET_SOPHIA_SIDEVIEW_STATE_RIGHT);
	else
		SetState(enabledCrusherBeam ? BULLET_WHITE_SOPHIA_SIDEVIEW_STATE_UP : BULLET_SOPHIA_SIDEVIEW_STATE_UP);

	if (enabledCrusherBeam)
	{
		crusherBeamChangingColorEffectTimer = new CTimer(this, CRUSHER_BEAM_COLOR_DURATION);
		crusherBeamChangingColorEffectTimer->Start();
	}

	isHiddenByForeground = true;
	zIndex = ZINDEX_SOPHIA_BULLET;
}

void CBullet_Sophia::ChangeRandomColor()
{
	// set random bright color
	const int MIN_COLOR_R = 150;
	const int MIN_COLOR_G = 150;
	const int MIN_COLOR_B = 150;
	modifyR = MIN_COLOR_R + rand() % (256 - MIN_COLOR_R);
	modifyG = MIN_COLOR_G + rand() % (256 - MIN_COLOR_G);
	modifyB = MIN_COLOR_B + rand() % (256 - MIN_COLOR_B);
}

void CBullet_Sophia::UpdateVelocity(DWORD dt)
{
	// nothing
}

void CBullet_Sophia::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
		CGameObjectBehaviour::BlockObject(dt, coEvent);
		Explode(CLASS_SMALL_EXPLOSION_SIDEVIEW);
	}

	if (dynamic_cast<CEnemy*>(obj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(obj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy);
	}

	if (IsBlockableObject(obj) && !dynamic_cast<CEnemy*>(obj)) {
		Sound::getInstance()->play(SOPHIA_BULLET_EXPLOSION, false, 1);
	}
}

void CBullet_Sophia::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (dynamic_cast<CEnemy*>(overlappedObj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(overlappedObj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy);
	}
}

void CBullet_Sophia::CalcExplosionCenterPos(float& explosionX, float& explosionY)
{
	float l, r, t, b;
	GetBoundingBox(l, t, r, b);
	CGameObjectBehaviour::CalcBoundingBoxCenter(this, explosionX, explosionY);

	switch (state)
	{
	case BULLET_SOPHIA_SIDEVIEW_STATE_UP:
	case BULLET_WHITE_SOPHIA_SIDEVIEW_STATE_UP:
		explosionY = t;
		break;
	
	case BULLET_SOPHIA_SIDEVIEW_STATE_LEFT:
	case BULLET_WHITE_SOPHIA_SIDEVIEW_STATE_LEFT:
		explosionX = l;
		break;

	case BULLET_SOPHIA_SIDEVIEW_STATE_RIGHT:
	case BULLET_WHITE_SOPHIA_SIDEVIEW_STATE_RIGHT:
		explosionX = r;
		break;
	}
}

void CBullet_Sophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	if (enabledCrusherBeam)
	{
		crusherBeamChangingColorEffectTimer->Update(dt);
	}

	CBullet::Update(dt, coObjs);
}

void CBullet_Sophia::HandleTimerTick(LPTIMER sender)
{
	if (sender == crusherBeamChangingColorEffectTimer)
	{
		ChangeRandomColor();
	}
}

void CBullet_Sophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != BULLET_SOPHIA_SIDEVIEW_STATE_UP && state != BULLET_WHITE_SOPHIA_SIDEVIEW_STATE_UP)
	{
		float offsetX, offsetY, width, height;
		offsetX = BULLET_SOPHIA_BOUNDBOX_OFFSETX;
		offsetY = BULLET_SOPHIA_BOUNDBOX_OFFSETY;
		width = BULLET_SOPHIA_BOUNDBOX_WIDTH;
		height = BULLET_SOPHIA_BOUNDBOX_HEIGHT;

		if (state == BULLET_SOPHIA_SIDEVIEW_STATE_RIGHT || BULLET_WHITE_SOPHIA_SIDEVIEW_STATE_RIGHT)
		{
			CGameObjectBehaviour::TransformBoundBox(
				BULLET_SOPHIA_BOUNDBOX_OFFSETX, BULLET_SOPHIA_BOUNDBOX_OFFSETY, BULLET_SOPHIA_BOUNDBOX_WIDTH, BULLET_SOPHIA_BOUNDBOX_HEIGHT,
				BULLET_SOPHIA_SPRITE_WIDTH, BULLET_SOPHIA_SPRITE_HEIGHT,
				offsetX, offsetY, width, height, true, false
			);
		}

		left = x + offsetX;
		right = left + width;
		top = y + offsetY;
		bottom = top + height;
	}
	else
	{
		// flip diagonally
		left = x + BULLET_SOPHIA_BOUNDBOX_OFFSETY;
		right = left + BULLET_SOPHIA_BOUNDBOX_HEIGHT;
		top = y + BULLET_SOPHIA_BOUNDBOX_OFFSETX;
		bottom = top + BULLET_SOPHIA_BOUNDBOX_WIDTH;
	}
}
