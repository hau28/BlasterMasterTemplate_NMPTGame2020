#include "AnimatableObject.h"

#include "TileArea.h"
#include "CollisionSolver.h"

CAnimatableObject::CAnimatableObject(int classId, int x, int y, int sectionId, int objAnimsId)
{
	this->classId = classId;
	SetPosition(x, y);
	this->currentSectionId = sectionId;

	LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(objAnimsId);
	animationHandlers = objAnims->GenerateAnimationHanlders();
}

void CAnimatableObject::CheckOverlaps(vector<LPGAMEOBJECT>* coObjs, vector<LPGAMEOBJECT>& overlappedObjs)
{
	overlappedObjs.clear();

	for (auto obj : *coObjs)
	{
		if (CCollisionSolver::IsOverlapped(this, obj))
		{
			overlappedObjs.push_back(obj);
		}
	}
}

void CAnimatableObject::HandleOverlaps(vector<LPGAMEOBJECT>* overlappedObjs)
{
	for (auto obj : *overlappedObjs)
		HandleOverlap(obj);
}

void CAnimatableObject::DeoverlapWithBlockableTiles(vector<LPGAMEOBJECT>* coObjs)
{
	for(auto obj : *coObjs)
		if (dynamic_cast<LPTILE_AREA>(obj))
		{
			LPTILE_AREA tiles = dynamic_cast<LPTILE_AREA>(obj);

			if (tiles->classId == CLASS_TILE_BLOCKABLE)
			{
				float dx, dy;
				dx = dy = 0;

				CCollisionSolver::DeOverlap(this, obj, dx, dy);

				this->x += dx;
				this->y += dy;
			}
		}
}

void CAnimatableObject::CheckCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjs, vector<LPCOLLISIONEVENT>& coEvents)
{
	CCollisionSolver::CalcPotentialCollisions(this, coObjs, coEvents, dt);
}

void CAnimatableObject::HandleCollisionWithWalls(DWORD dt, vector<LPCOLLISIONEVENT>* coEvents)
{
	for (auto event_ : *coEvents)
	{
		if (event_->otherObject->classId == CLASS_TILE_BLOCKABLE && event_->nx != 0)
		{
			event_->ny = 0;
			event_->rdx = 0;
			HandleCollision(dt, event_);
		}
	}
}

void CAnimatableObject::HandleCollisionWithBlockableTiles(DWORD dt, vector<LPCOLLISIONEVENT>* coEvents)
{
	for (auto event_ : *coEvents)
	{
		if (event_->otherObject->classId == CLASS_TILE_BLOCKABLE)
		{
			HandleCollision(dt, event_);
		}
	}
}

void CAnimatableObject::HandleCollisions(DWORD dt, vector<LPCOLLISIONEVENT>* coEvents)
{
	for (auto event_ : *coEvents)
	{
		HandleCollision(dt, event_);
	}
}

void CAnimatableObject::ResolveInteractions(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	DeoverlapWithBlockableTiles(coObjs);

	vector<LPCOLLISIONEVENT>* colEvents = new vector<LPCOLLISIONEVENT>();

	// CuteTN note: handle collision with walls first to avoid a AABB bug (the bad way)
	CheckCollision(dt, coObjs, *colEvents);
	HandleCollisionWithWalls(dt, colEvents);

	CheckCollision(dt, coObjs, *colEvents);
	HandleCollisionWithBlockableTiles(dt, colEvents);

	CheckCollision(dt, coObjs, *colEvents);
	HandleCollisions(dt, colEvents);

	vector<LPGAMEOBJECT>* overlappedObjs = new vector<LPGAMEOBJECT>();
	CheckOverlaps(coObjs, *overlappedObjs);
	HandleOverlaps(overlappedObjs);
}

void CAnimatableObject::UpdatePosition(DWORD dt)
{
	this->x += this->vx * dt;
	this->y += this->vy * dt;
}

void CAnimatableObject::Render(float offsetX, float offsetY)
{
	animationHandlers[state]->Render(x + offsetX, y + offsetY);
	animationHandlers[state]->Update();
}

void CAnimatableObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	UpdateVelocity(dt);

	ResolveInteractions(dt, coObjs);

	UpdatePosition(dt);
}
