#include "AnimatableObject.h"

#include "TileArea.h"
#include "CollisionSolver.h"
#include "BreakableBlock.h"

vector<Color> CAnimatableObject::flashingColors =
{
	{255,0,100},
	{10,255,10},
	{255, 255, 255},
	{255,0,100},
	{10,255,10},
	{255, 255, 255},
	{255,0,100},
	{10,255,10},
	{255, 255, 255}
};

CAnimatableObject::CAnimatableObject(int classId, int x, int y, int sectionId, int objAnimsId)
{
	this->classId = classId;
	SetPosition(x, y);
	this->currentSectionId = sectionId;

	LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(objAnimsId);
	animationHandlers = objAnims->GenerateAnimationHanlders();

	// set the initial state to avoid stupid allocation bug
	state = animationHandlers.begin()->first;
}

CAnimatableObject::CAnimatableObject(int classId, int x, int y, int sectionId) : CAnimatableObject::CAnimatableObject(classId, x, y, sectionId, LookUpAnimationsId(classId))
{
}

void CAnimatableObject::SetModifyColor(int a, int r, int g, int b)
{
	modifyA = a;
	SetModifyColor(r, g, b);
}

int CAnimatableObject::LookUpAnimationsId(int classId)
{
	return CObjectAnimationsLib::GetInstance()->GetAnimaionsId(classId);
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

void CAnimatableObject::SetModifyColor(int r, int g, int b)
{
	modifyR = r;
	modifyG = g;
	modifyB = b;
}

void CAnimatableObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}



void CAnimatableObject::DeoverlapWithBlockableTiles(vector<LPGAMEOBJECT>* coObjs)
{
	for(auto obj : *coObjs)
		if (IsBlockableObject(obj))
		{
			float dx, dy;
			dx = dy = 0;

			CCollisionSolver::DeOverlap(this, obj, dx, dy);

			this->x += dx;
			this->y += dy;
		}
}

bool CAnimatableObject::IsBlockableObject(LPGAMEOBJECT obj)
{
	if (obj == nullptr)
		return false;

	if (   obj->classId == CLASS_TILE_BLOCKABLE 
		|| obj->classId == CLASS_TILE_BLOCKABLE_O)
		return true;

	if (dynamic_cast<CBreakableBlock*>(obj))
	{
		CBreakableBlock* bb = dynamic_cast<CBreakableBlock*>(obj);
		if (bb->IsDestroyed())
			return false;
		else
			return true;
	}

	return false;
}

void CAnimatableObject::CheckCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjs, vector<LPCOLLISIONEVENT>& coEvents)
{
	CCollisionSolver::CalcPotentialCollisions(this, coObjs, coEvents, dt);
}

void CAnimatableObject::HandleCollisionWithWalls(DWORD dt, vector<LPCOLLISIONEVENT>* coEvents)
{
	for (auto event_ : *coEvents)
	{
		if (IsBlockableObject(event_->otherObject) && event_->nx != 0)
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
		if (IsBlockableObject(event_->otherObject))
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
	if (!allowOverlapWithBlocks)
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
	if (!animationHandlers[state])
	{
		DebugOut(L"[ERROR] Missing animation handler of state %d\n", state);
		return; //sanh - fix reset game
	}

	animationHandlers[state]->Render(x + offsetX, y + offsetY, modifyA, modifyR, modifyG, modifyB);
	animationHandlers[state]->Update();
}

void CAnimatableObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	UpdateVelocity(dt);

	ResolveInteractions(dt, coObjs);

	UpdatePosition(dt);
}

CAnimatableObject::~CAnimatableObject()
{
	for (auto x : animationHandlers)
		delete x.second;

	animationHandlers.clear();
}

