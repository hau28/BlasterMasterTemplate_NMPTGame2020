#include "JasonSideview.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include "Sophia.h"
#include "PlayScene.h"

#include <algorithm>
#include <assert.h>
#include "JasonJumpInEvent.h"
CJasonSideview* CJasonSideview::__instance = nullptr;

CJasonSideview::CJasonSideview()
{
    classId = CLASS_JASONSIDEVIEW;
    SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    // CuteTN Note: DIRTY AF
    LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(JASONSIDEVIEW_ANIMATIONS);
    animationHandlers = objAnims->GenerateAnimationHanlders();
}

CJasonSideview::CJasonSideview(int classId, int x, int y, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, -1, animsId)
{
    SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    init_camBox();
};

void CJasonSideview::init_camBox()
{
    //Init CamBox Jason must not to lag camera (original position camera)   
    float cameraX, cameraY;
    CGame::GetInstance()->GetCamPos(cameraX, cameraY);
    float centerPointX = cameraX + CGame::GetInstance()->GetScreenWidth() / 2;
    float centerPointY = cameraY + CGame::GetInstance()->GetScreenHeight() / 2;

    camBoxLeft = centerPointX - 16*2;
    camBoxRight  = camBoxLeft + 16* 4;
    camBoxBottom = centerPointY + 16*2;
    camBoxTop = camBoxBottom - 16 * 6;
    
    vx *= 3;
    vy = 0;
}

#pragma region key events handling

void CJasonSideview::HandleKeys(DWORD dt)
{
    if (CGame::GetInstance()->GetState() == GameState::PLAY_SIDEVIEW_JASON)
    {
        HandleKeysHold(dt);

        auto keyEvents = NewKeyEvents();

        for (auto e : keyEvents)
        {
            int keyCode = e->GetKeyCode();
            if (e->IsDown())
                HandleKeyDown(dt, keyCode);
            else
                HandleKeyUp(dt, keyCode);
        }
    }
    else 
        if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT_JASON ||
            CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT_JASON)
        {
            auto keyEvents = NewKeyEvents();

            for (auto e : keyEvents)
            {
                int keyCode = e->GetKeyCode();
                if (!e->IsDown())
                    HandleKeyUp(dt, keyCode);
            }
        }
}

void CJasonSideview::HandleKeysHold(DWORD dt)
{
    
    if (IsKeyDown(DIK_RIGHT))
    {
        Jason_turnRight = true;

        if (flag_jasoncrawl)
        {
            SetState(JASONSIDEVIEW_STATE_CRAWL_RIGHT);
            animationHandlers[state]->startLoopIndex = 0;
        }
        else
        {
            SetState(JASONSIDEVIEW_STATE_WALK_RIGHT);
            animationHandlers[state]->startLoopIndex = 0;
        }

        if (flag_jasoncrawl && vy == 0 && !flagOnAir)
            vx = JASONSIDEVIEW_VX / 2;
        else
            vx = JASONSIDEVIEW_VX;

    };

    if (IsKeyDown(DIK_LEFT))
    {
        Jason_turnRight = false; 

        if (flag_jasoncrawl)
        {
            SetState(JASONSIDEVIEW_STATE_CRAWL_LEFT);
            animationHandlers[state]->startLoopIndex = 0;
        }
        else
        {
            SetState(JASONSIDEVIEW_STATE_WALK_LEFT);
            animationHandlers[state]->startLoopIndex = 0;
        }

        if (flag_jasoncrawl && vy == 0 && !flagOnAir)
            vx = -JASONSIDEVIEW_VX / 2;
        else
            vx = -JASONSIDEVIEW_VX;

    }
}

void CJasonSideview::HandleKeyUp(DWORD dt, int keyCode)
{
    if (keyCode == DIK_RIGHT || keyCode == DIK_LEFT)
    {

        if (flagOnAir)
        {
            ax = JASONSIDEVIEW_AX;
            flag_jumpwalk = true;
            flag_jasoncrawl = false;
        }
        else 
            vx = 0; 

        if (!flag_jasoncrawl && !IsKeyDown(DIK_X))
        {
            if (keyCode == DIK_RIGHT)
            {
                Jason_turnRight == true;
                SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
            }
            else
            {
                Jason_turnRight == false;
                SetState(JASONSIDEVIEW_STATE_IDLE_LEFT);
            };
        }

        if (flag_jasoncrawl && !flagOnAir)
        {
            if (keyCode == DIK_RIGHT)
            {
                Jason_turnRight == true;

                /*animationHandlers[state]->currentFrameIndex = 1;
                animationHandlers[state]->startLoopIndex = 1;*/
            }
            else
            {
                Jason_turnRight == false;
                animationHandlers[state]->startLoopIndex = 1;
            }
        }
    }

	if (keyCode == DIK_UP || keyCode == DIK_DOWN)
		vy = 0;
}

void CJasonSideview::HandleKeyDown(DWORD dt, int keyCode)
{
    if (keyCode == DIK_RIGHT || keyCode == DIK_LEFT)
        vx = 0;

    if (keyCode == DIK_DOWN && vy == 0)
    {
        flag_jasoncrawl = true;

        if (Jason_turnRight)
        {
            SetState(JASONSIDEVIEW_STATE_CRAWL_RIGHT);
            animationHandlers[state]->startLoopIndex = 1;
        }
        else
        {
            SetState(JASONSIDEVIEW_STATE_CRAWL_LEFT);
            animationHandlers[state]->startLoopIndex = 1;
        }
    }

    if (keyCode == DIK_UP && flag_jasoncrawl)
    {
        vy = 0;
        flag_jasoncrawl = false;

        if (Jason_turnRight)
            SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
        else
            SetState(JASONSIDEVIEW_STATE_IDLE_LEFT);
    } 

    if (keyCode == DIK_X && !flagOnAir && !flag_jasoncrawl)
    {
        vx = 0;
        vy -= JASONSIDEVIEW_JUMP_SPEED_Y;

        if (Jason_turnRight)
            SetState(JASONSIDEVIEW_STATE_JUMP_RIGHT);
        else
            SetState(JASONSIDEVIEW_STATE_JUMP_LEFT);
        
    }

    if (keyCode == DIK_RSHIFT)
    {
        if (CCollisionSolver::IsOverlapped(CSophia::GetInstance(), __instance)) 
        {
            CJasonJumpInEvent* jasonJumpInEvent = new CJasonJumpInEvent(x, y, currentSectionId);
            CGame::AddGameEvent(jasonJumpInEvent);
           // jasonJumpInEvent->deleteJasonSideview();
        }
    }
}

void CJasonSideview::UpdateVelocity(DWORD dt)
{

}

void CJasonSideview::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
    if (coEvent == nullptr)
        return;
    if (coEvent->otherObject == this)
        return;

    LPGAMEOBJECT obj = coEvent->otherObject;

    if (dynamic_cast<LPTILE_AREA>(obj))
    {
        LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(obj);

        switch (tileArea->classId)
        {
        case CLASS_TILE_BLOCKABLE:
        {
            CGameObjectBehaviour::BlockObject(dt, coEvent);

            if (coEvent->ny < 0)
                flagOnAir = false;

            break;
        }

        case CLASS_TILE_PORTAL:
        {
            LPPORTAL fromPortal = dynamic_cast<LPPORTAL>(obj);
            LPPORTAL toPortal = CPortalLib::GetInstance()->Get(fromPortal->associatedPortalId);

            // Sanh code from here!
            LPGAME_EVENT newEvent = new CWalkInPortalEvent("WalkInPortalEvent", fromPortal, toPortal);
            CGame::GetInstance()->AddGameEvent(newEvent);
            // to do: create an event to CGame, let CGame handle switching section
            DebugOut(L"Jason to portal %d of section %d, tick %d\n", toPortal->associatedPortalId, toPortal->currentSectionId, GetTickCount());
        }
        }
    }
}

void CJasonSideview::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x + JASONSIDEVIEW_BOUNDBOX_OFFSETX;
    top = y + JASONSIDEVIEW_BOUNDBOX_OFFSETY;
    right = left + JASONSIDEVIEW_BOUNDBOX_WIDTH;
    bottom = top + JASONSIDEVIEW_BOUNDBOX_HEIGHT;
}

void CJasonSideview::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
    // CuteTN note: we may need to refactor this function
    if (CGame::GetInstance()->GetCurrentPlayer()->classId == CLASS_SOPHIA)
        return;

    HandleKeys(dt);

    // dơ lắm cơ mà fix sau đi :((( 
    if (vx == 0) flag_jumpwalk = false;

    if (flag_jumpwalk && !flagOnAir ) {
        if (Jason_turnRight) 
        {
            SetState(JASONSIDEVIEW_STATE_WALK_RIGHT);
            if (vx > 0)
            {
                vx += ax * dt;
            }
            else
                vx = 0;
        }
        else
        {
            SetState(JASONSIDEVIEW_STATE_WALK_LEFT);
            if (vx < 0)
            { 
                vx -=  ax * dt;
            }
            else
                vx = 0;
        }   
        
    }

    vy += JASONSIDEVIEW_GRAVITY * dt;
    flagOnAir = true;

    if (CGame::GetInstance()->GetState() != GameState::SECTION_SWITCH_LEFT_JASON &&
        CGame::GetInstance()->GetState() != GameState::SECTION_SWITCH_RIGHT_JASON)
    {
        UpdateVelocity(dt);
            
        ResolveInteractions(dt, coObjs);
    }

    // SANH update cambox camera
    if (x + 16 >= camBoxRight) {
        camBoxRight = x + 16;
        camBoxLeft = camBoxRight - 16 * 4;
    }
    else if (x <= camBoxLeft) {
        camBoxLeft = x;
        camBoxRight = x + 16 * 4;
    }
    if (y + 16 >= camBoxBottom) {
        camBoxBottom = y + 16;
        camBoxTop = camBoxBottom - 16 * 6;
    }
    if (y<= camBoxTop) {
        camBoxTop = y;
        camBoxBottom = camBoxTop + 16 * 6;
    }

    if (flagOnAir && Jason_turnRight)
        SetState(JASONSIDEVIEW_STATE_JUMP_RIGHT);

    if (flagOnAir && !Jason_turnRight)
        SetState(JASONSIDEVIEW_STATE_JUMP_LEFT);

    if (vy == 0 && vx == 0 && !flag_jasoncrawl)
    {
        if (Jason_turnRight)
            SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
        else
            SetState(JASONSIDEVIEW_STATE_IDLE_LEFT);
    }

    //Push vx when jason switch section --- Sanh
    if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT_JASON)
    {
        vx = -JASONSIDEVIEW_VX / 3;
        vy = 0;
        SetState(JASONSIDEVIEW_STATE_WALK_LEFT);
    }
    if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT_JASON)
    {
        vx = JASONSIDEVIEW_VX / 3;
        vy = 0;
        SetState(JASONSIDEVIEW_STATE_WALK_RIGHT);
    }

    UpdatePosition(dt);

}

void CJasonSideview::Render(float offsetX, float offsetY)
{
    animationHandlers[state]->Render(x + offsetX, y + offsetY);

    if (!IsKeyDown(DIK_LEFT) && state == JASONSIDEVIEW_STATE_CRAWL_LEFT)
        ;
    else if (!IsKeyDown(DIK_RIGHT) && state == JASONSIDEVIEW_STATE_CRAWL_RIGHT)
        ;
    else
        animationHandlers[state]->Update();
}

CJasonSideview* CJasonSideview::GetInstance()
{
    if (__instance == nullptr)
    {
        __instance = new CJasonSideview();
    }

    return __instance;
}

CJasonSideview* CJasonSideview::InitInstance(int x, int y, int sectionId)
{
    GetInstance();
    __instance->SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    __instance->SetPosition(x, y);
    __instance->currentSectionId = sectionId;
 
    return __instance;
}


