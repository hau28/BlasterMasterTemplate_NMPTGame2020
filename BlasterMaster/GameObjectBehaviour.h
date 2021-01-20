#pragma once
#include <functional>
#include "GameObject.h"

#include "Bullet.h"
#include "Enemy.h"

/// <summary>
/// A special class to encapsulate some GameObjects common behaviour.
/// So that we dont have to write the same functions again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again
/// </summary>
class CGameObjectBehaviour
{
public:
	/// <summary>
	/// <para> Solve the behaviour that an object is blocked another object on a collision event </para>
	/// <para> WARNING: mutate directly on movable object! </para>
	/// <para> coEvent->sourceObj: movable object that get blocked </para>
	/// <para> coEvent->otherObj: blockable object </para>
	/// </summary>
	static void BlockObject(DWORD dt, LPCOLLISIONEVENT coEvent);

	/// <summary>
	/// Calculate the center of a game object
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	static void CalcBoundingBoxCenter(LPGAMEOBJECT obj, float& x, float& y);

	/// <summary>
	/// Set the position of the top-left corner of a gameobject's bounding box
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	static void SetBoundingBoxPos(LPGAMEOBJECT obj, float x, float y);

	static void SetBoundingBoxCenter(LPGAMEOBJECT obj, float x, float y);

	static void CalcDirection(LPGAMEOBJECT fromObj, LPGAMEOBJECT toObj, float& x, float& y);

	/// <summary>
	/// <para> Calculate the direction from a game object to current player. </para>	
	/// <para> Simply takes the difference between 2 posision vectors </para>
	/// </summary>
	static void CalcDirectionToPlayer(LPGAMEOBJECT obj, float& x, float& y);

	static float CalcMagnitudeVector2(float x, float y);

	/// <summary>
	/// <para> Normalizes the vector (x,y), then stores the result in the vector (nx,ny) </para>
	/// <para> ie (x,y) and (nx,ny) has the same direction but |(nx,ny)| = 0 </para>
	/// </summary>
	static void NormalizeVector2(float x, float y, float& nx, float& ny);

	static void TransformBoundBox(float offsetX, float offsetY, float width, float height, float spriteWidth, float spriteHeight, float& newOffsetX, float& newOffsetY, float& newWidth, float& newHeight, bool flipX, bool flipY);

	/// <summary>
	/// Create an explosion object and add it to the current section in the next frame
	/// </summary>
	/// <param name="explosionClassId"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="sectionId"></param>
	static void CreateExplosion(int explosionClassId, int x, int y, int sectionId);

	/// <summary>
	/// Destroy the current object and create an explosion. (x,y) is the top-left corner position of game object
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="explosionClassId"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	static void Explode(LPGAMEOBJECT obj, int explosionClassId, int x, int y);

	/// <summary>
	/// Destroy the current object and create an explosion at the center of it
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="explosionClassId"></param>
	static void ExplodeAtCenter(LPGAMEOBJECT obj, int explosionClassId);

	/// <summary>
	/// Create a new game object and add it to its section in the next frame
	/// </summary>
	/// <param name="obj"></param>
	static void CreateObject(LPGAMEOBJECT obj);

	static void CreateObjectAtCenterOfAnother(LPGAMEOBJECT newObj, LPGAMEOBJECT fromObj);
	static void CreateObjectAtCenterOfBoss(LPGAMEOBJECT newObj, LPGAMEOBJECT fromObj);
	/// <summary>
	/// Remove a game object from its section in the next frame.
	/// Deallocate the object memory if isDestroyAfterRemove is true
	/// </summary>
	/// <param name="obj"></param>
	static void RemoveObject(LPGAMEOBJECT obj, bool isDestroyAfterRemove = true);

	/// <summary>
	/// Penetrable: specify if the bullet should explode when colliding with enemy
	/// </summary>
	/// <param name="bullet"></param>
	/// <param name="enemy"></param>
	/// <param name="penetrable"></param>
	static void HandleFriendlyBulletHitsEnemy(CBullet* bullet, CEnemy* enemy, bool penetrable = false);

	static bool IsMovableObject(LPGAMEOBJECT obj);
};

/// <summary>
/// returns true if obj1 must be behind obj2
/// </summary>
/// <param name="obj1"></param>
/// <param name="obj2"></param>
/// <returns></returns>
bool CompareRenderOrderSideview(LPGAMEOBJECT obj1, LPGAMEOBJECT obj2);
bool CompareRenderOrderOverhead(LPGAMEOBJECT obj1, LPGAMEOBJECT obj2);
