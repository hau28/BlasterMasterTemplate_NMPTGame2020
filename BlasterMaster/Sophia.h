#pragma once
#include "AnimatableObject.h"

// 8 23 31 39

const int SOPHIA_BOUNDBOX_WIDTH = 21;
const int SOPHIA_BOUNDBOX_HEIGHT = 16;
const int SOPHIA_BOUNDBOX_OFFSETX = 9;
const int SOPHIA_BOUNDBOX_OFFSETY = 23;
const float SOPHIA_MAX_SPEED = 0.1;

const float SOPHIA_GRAVITY = 0.01f;

class CSophia :
    public CAnimatableObject
{
private:
    void HandleKeys(DWORD dt);
    void HandleKeyUp(DWORD dt, int keyCode);
    void HandleKeyDown(DWORD dt, int keyCode);
    void HandleKeysHold(DWORD dt);
    void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);

    bool flagOnAir;
    bool flagDead = 0;
    bool flagStop = false;
    bool stopLeft = false;
    bool isLeft = false;
    bool doneTurn = true;
    int wheel = 0;
    DWORD lastTimeMoveWheel;
    vector<int> idleRightStates = { SOPHIA_STATE_IDLE_RIGHT,SOPHIA_STATE_IDLE1_RIGHT,SOPHIA_STATE_IDLE2_RIGHT,SOPHIA_STATE_IDLE3_RIGHT };
    vector<int> idleLeftStates = { SOPHIA_STATE_IDLE_LEFT,SOPHIA_STATE_IDLE1_LEFT,SOPHIA_STATE_IDLE2_LEFT,SOPHIA_STATE_IDLE3_LEFT };
    vector<int> gunTurnRight = { SOPHIA_STATE_TURN_RIGHT,SOPHIA_STATE_TURN_RIGHT1 ,SOPHIA_STATE_TURN_RIGHT2 ,SOPHIA_STATE_TURN_RIGHT3 };
    vector<int> gunTurnLeft = { SOPHIA_STATE_TURN_LEFT,SOPHIA_STATE_TURN_LEFT1, SOPHIA_STATE_TURN_LEFT2, SOPHIA_STATE_TURN_LEFT3};
    vector<int> jumpRight = { SOPHIA_STATE_JUMP_RIGHT,SOPHIA_STATE_JUMP_RIGHT1 ,SOPHIA_STATE_JUMP_RIGHT2 ,SOPHIA_STATE_JUMP_RIGHT3 };
    vector<int> jumpLeft = { SOPHIA_STATE_JUMP_LEFT,SOPHIA_STATE_JUMP_LEFT1, SOPHIA_STATE_JUMP_LEFT2, SOPHIA_STATE_JUMP_LEFT3 };
    vector<int> fallLeft = { SOPHIA_STATE_FALL_LEFT,SOPHIA_STATE_FALL1_LEFT1 ,SOPHIA_STATE_FALL2_LEFT2 ,SOPHIA_STATE_FALL3_LEFT3 };
    vector<int> fallRight = { SOPHIA_STATE_FALL_RIGHT,SOPHIA_STATE_FALL1_RIGHT1,SOPHIA_STATE_FALL2_RIGHT2,SOPHIA_STATE_FALL3_RIGHT3 };
public:
    CSophia() {};
    CSophia(int classId, int x, int y, int animsId);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);     
    virtual void updateWheel();
    virtual void setIdleRight();
    virtual void setIdleLeft();
    virtual void setTurnRight();
    virtual void setTurnLeft();
    virtual void setJumpRight();
    virtual void setJumpLeft();
    virtual void setFallLeft();
    virtual void setFallRight();
};

typedef CSophia* LPSophia;