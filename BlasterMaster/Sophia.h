#pragma once
#include "AnimatableObject.h"
class CSophia :
    public CAnimatableObject
{
    void HandleKeys(DWORD dt);
    void HandleKeyUp(DWORD dt, int keyCode);
    void HandleKeyDown(DWORD dt, int keyCode);
    void HandleKeysHold(DWORD dt);


public:
	CSophia() {};
	CSophia(int classId, int x, int y, int animsId);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjs);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CSophia* LPSophia;
