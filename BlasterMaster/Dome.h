#pragma once
#include "Enemy.h"
#include "ITimeTrackable.h"
#include "Timer.h"

const int ROTATION_UP = 0;
const int ROTATION_RIGHT = 1;
const int ROTATION_DOWN = 2;
const int ROTATION_LEFT = 3;

class CDome : public CEnemy, public ITimeTrackable
{
private:
    // bounding box
	// 7 7 

    const int DOME_BOUNDBOX_WIDTH = 15;
    const int DOME_BOUNDBOX_HEIGHT = 15;
    const int DOME_BOUNDBOX_OFFSETX = 7;
    const int DOME_BOUNDBOX_OFFSETY = 7;
    
    const float DOME_STICKY_FORCE = 0.1008f; // => Thycute's birthday is not magic number, Thycute is magic tho
    const float DOME_MOVING_SPEED = 0.045f; 
    const float DOME_SHOOTING_SPEED = 0.2f; 

    int rotation = 0;
    bool isClockwise = true;
    bool flagShooting = false;
    bool flagFirstLand = false;

    bool flagTouchVerticalBlock = false;
    bool flagTouchHorizontalBlock = false;

    const int DOME_HEALTHPOINT = 8;

    void UpdateState();

    /// <summary>
    /// Actually add on modulo 4. Does NOT mutate the rotation field
    /// </summary>
    /// <returns></returns>
    static int AddToRotation(int rotate, int moreRotate);
    static void CalcDirectionVector(int rotation, int& x, int& y);

    void AddStickyForce();
    void AddMovingSpeed();
    void HandleRotateOnBlockCorner();

    LPTIMER PrepareShootingTimer = nullptr;
    bool CheckShootingCondition();
    void PrepareShooting();
    void StartShooting();
    void EndShooting();
    virtual void HandleTimerTick(CTimer* sender);

public:
    CDome() {};
    CDome(int classId, int x, int y, int sectionId, int rotation, bool isClockwise, int animsId);
    
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CDome* LPDOME;
