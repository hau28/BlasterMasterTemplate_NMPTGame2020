#pragma once
#include "AnimatableObject.h"
#include <queue>

// 8 23 31 39

const int SOPHIA_BOUNDBOX_WIDTH = 21;
const int SOPHIA_BOUNDBOX_HEIGHT = 16;
const int SOPHIA_BOUNDBOX_OFFSETX = 9;
const int SOPHIA_BOUNDBOX_OFFSETY = 23;
const float SOPHIA_MAX_SPEED = 0.1;
const float SOPHIA_ENGINE = 0.0005;
const float FRICTION = 0.00025;

const float SOPHIA_GRAVITY = 0.0005f;
const float SOPHIA_MAX_FALL_SPEED = 0.175f;
const float SOPHIA_JUMP_FORCE = 0.26f;


/// <summary>
/// SOPHIA is also animatable, however, it has a completely different kind of animation system
/// so we have to somehow override the render method
/// </summary>
class CSophia : public CAnimatableObject
{
private:
    void HandleKeys(DWORD dt);
    void HandleKeyUp(DWORD dt, int keyCode);
    void HandleKeyDown(DWORD dt, int keyCode);
    void HandleKeysHold(DWORD dt);

    int directionState, gunState, bodyState, wheelState;

    bool flagOnAir;
    bool flagDead=0;
    float ground;
    bool turnRight;

	CSophia() {};
	CSophia(int classId, int x, int y);

    static CSophia* __instance;

public:
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjs);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void updateWheel();
    virtual void updateDirection();
    virtual void updateBody();
    virtual void updateGun();
    virtual void Render();
    int wheel = 0;
    DWORD lastTimeupdateWheel;
    DWORD lastTimeupdateDirection;
    DWORD lastTimeupdateGun;
    DWORD lastTimeUpdateBody;
    static CSophia* GetInstance();
    static CSophia* InitInstance(int classId, int x, int y, int sectionId);
};

typedef CSophia* LPSOPHIA;
