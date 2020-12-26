#include "SwitchSceneEvent.h"

SwitchSceneEvent::SwitchSceneEvent(LPPORTAL portal)
{
	this->port = portal->port;
	
	if (portal->port == PORTAL_TO_ENDSCENE_PORT)
		id_scene = ID_SCENE_END;
	else
	{
		// CuteTN Magic trick: switch between Overhead and Sideview scene
		int tempSum = ID_SCENE_OVERHEAD + ID_SCENE_SIDEVIEW;
		id_scene = tempSum - CGame::GetInstance()->GetCurrentSceneId();
	}
}
