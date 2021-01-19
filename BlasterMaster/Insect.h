#pragma once
#include "Enemy.h"

class CInsect : public CEnemy, public ITimeTrackable
{

    const int INSECT_BOUNDBOX_WIDTH = 18;
    const int INSECT_BOUNDBOX_HEIGHT = 19;
    const int INSECT_BOUNDBOX_OFFSETX = 7;
    const int INSECT_BOUNDBOX_OFFSETY = 5;

    const float INSECT_MOVE_SPEED_X = 0.019f;
    const float INSECT_MOVEDOWN_SPEED_Y = 0.013f;
    const float INSECT_MOVEUP_SPEED_Y = 0.05f;
    const float INSECT_FALL_SPEED_Y = 0.180f; // Vì k để 108 dc thì ghi 180
    
    const int TIME_UPDATE_MOVEUP = 400;
    const int TIME_UPDATE_MOVEDOWN = 1000;

    const int INSECT_HEALTHPOINT = 6;

private:

    const int BUZZ_DURATION = 300;
    bool flagtouchwall = false;
    LPTIMER buzzTimer;
    int timeUpdateVelocite;
    void UpdateState();
    bool turnRight = true;
    bool flagDown = false;
public:
    CInsect() {};
    CInsect(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void HandleTimerTick(LPTIMER sender);
};
typedef CInsect* LPINSECT;
