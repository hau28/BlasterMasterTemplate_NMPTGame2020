#include "JasonOverhead.h"

CJasonOverhead* CJasonOverhead::__instance = nullptr;

CJasonOverhead::CJasonOverhead()
{
    Init();
}

void CJasonOverhead::Init()
{
    classId = CLASS_JASONOVERHEAD;
    SetState(JASONOVERHEAD_STATE_IDLE_DOWN);

    // CuteTN Note: DIRTY AF
    LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(JASON_OVERHEAD_ANIMATIONS);
    animationHandlers = objAnims->GenerateAnimationHanlders();

    this->allowOverlapWithBlocks = true;

    //invulnerableTimer = new CTimer(this, INVULNERABLE_DURATION, 1);
    //invulnerableTimer->Stop();

    //dyingEffectTimer = new CTimer(this, DYING_EFFECT_DURATION, 1);
    //dyingEffectTimer->Stop();

    //flagInvulnerable = false;

    //vulnerableFlashingEffect = new CObjectFlashingEffectPlayer(this, &flashingColors, JASONSIDEVIEW_VULNERABLE_EFFECT_FLASHING_DURATION);
}

CJasonOverhead* CJasonOverhead::GetInstance()
{
    if (__instance == nullptr)
    {
        __instance = new CJasonOverhead();
    }

    return __instance;
}

CJasonOverhead* CJasonOverhead::InitInstance(int x, int y, int sectionId)
{
    GetInstance();
    __instance->SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    __instance->SetPosition(x, y);
    __instance->currentSectionId = sectionId;

    return __instance;
}

void CJasonOverhead::UpdateVelocity(DWORD dt)
{
}

void CJasonOverhead::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
}

void CJasonOverhead::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}

void CJasonOverhead::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x + JASONOVERHEAD_BOUNDBOX_OFFSETX;
    top = y + JASONOVERHEAD_BOUNDBOX_OFFSETY;
    right = left + JASONOVERHEAD_BOUNDBOX_WIDTH;
    bottom = top + JASONOVERHEAD_BOUNDBOX_HEIGHT;
}
