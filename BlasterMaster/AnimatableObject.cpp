#include "AnimatableObject.h"

#include "TileArea.h"
#include "CollisionSolver.h"

CAnimatableObject::CAnimatableObject(int classId, int x, int y, int objAnimsId)
{
	this->classId = classId;
	SetPosition(x, y);

	LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(objAnimsId);
	animationHandlers = objAnims->GenerateAnimationHanlders();
}

void CAnimatableObject::Deoverlap(vector<LPGAMEOBJECT>* coObjs)
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

void CAnimatableObject::HandleCollisions(DWORD dt, vector<LPCOLLISIONEVENT>* coEvents)
{
	for (auto event_ : *coEvents)
	{
		HandleCollision(dt, event_);
	}
}

void CAnimatableObject::UpdatePosition(DWORD dt)
{
	this->x += this->vx * dt;
	this->y += this->vy * dt;
}

void CAnimatableObject::Render()
{
	animationHandlers[state]->Render(x, y);
	animationHandlers[state]->Update();
}

void CAnimatableObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	UpdateVelocity(dt);
	Deoverlap(coObjs);

	vector<LPCOLLISIONEVENT>* colEvents = new vector<LPCOLLISIONEVENT>();
	colEvents->clear();
	CheckCollision(dt, coObjs, *colEvents);
	HandleCollisions(dt, colEvents);

	UpdatePosition(dt);
}
