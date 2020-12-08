#include "Bullet.h"

CBullet::CBullet(int x, int y, int sectionId, int animsId, bool isFriendly) : CAnimatableObject::CAnimatableObject(-1, x, y, sectionId, animsId)
{
	this->isFriendly = isFriendly;
}
