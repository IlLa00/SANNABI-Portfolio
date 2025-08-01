#include "pch.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "Player.h"
#include "GrapplingHookProjectile.h"

void CollisionManager::Init(HWND hwnd)
{
	bBlock[(int)ECollisionChannel::Projectile][(int)ECollisionChannel::Character] = 1;
	bBlock[(int)ECollisionChannel::Projectile][(int)ECollisionChannel::WorldStatic] = 1;
	bBlock[(int)ECollisionChannel::Projectile][(int)ECollisionChannel::WorldDynamic] = 1;

	bBlock[(int)ECollisionChannel::Character][(int)ECollisionChannel::WorldStatic] = 1;
	bBlock[(int)ECollisionChannel::Character][(int)ECollisionChannel::WorldDynamic] = 1;
	bBlock[(int)ECollisionChannel::Character][(int)ECollisionChannel::Projectile] = 1;
	bBlock[(int)ECollisionChannel::Character][(int)ECollisionChannel::Character] = 1;

	bBlock[(int)ECollisionChannel::WorldStatic][(int)ECollisionChannel::Projectile] = 1;
	bBlock[(int)ECollisionChannel::WorldStatic][(int)ECollisionChannel::Character] = 1;

	bBlock[(int)ECollisionChannel::WorldDynamic][(int)ECollisionChannel::Projectile] = 1;
	bBlock[(int)ECollisionChannel::WorldDynamic][(int)ECollisionChannel::Character] = 1;

	bIgnore[(int)ECollisionChannel::Projectile][(int)ECollisionChannel::Projectile] = 1;

	bIgnore[(int)ECollisionChannel::WorldStatic][(int)ECollisionChannel::WorldStatic] = 1;
	bIgnore[(int)ECollisionChannel::WorldStatic][(int)ECollisionChannel::WorldDynamic] = 1;

	bIgnore[(int)ECollisionChannel::WorldDynamic][(int)ECollisionChannel::WorldDynamic] = 1;
	bIgnore[(int)ECollisionChannel::WorldDynamic][(int)ECollisionChannel::WorldStatic] = 1;
}

void CollisionManager::Update()
{
	for (int i = 0; i < collisionComponents.size(); i++)
	{
		if (!collisionComponents[i]->IsActive()) continue;

		for (int j = i+1; j < collisionComponents.size(); j++)
		{
			if (!collisionComponents[j]->IsActive()) continue;

			ECollisionChannel channel1 = collisionComponents[i]->GetCollisionChannel();
			ECollisionChannel channel2 = collisionComponents[j]->GetCollisionChannel();

			if (bIgnore[(int)channel1][(int)channel2] || !bBlock[(int)channel1][(int)channel2])
				continue;

			auto pair = make_pair(collisionComponents[i], collisionComponents[j]);
			bool isColliding = CheckAABBCollision(collisionComponents[i], collisionComponents[j]);
			bool wasColliding = collisionPairs[pair];

			if (isColliding && !wasColliding) // 새로운 충돌 시작
			{
				if (collisionComponents[i]->OnComponentBeginOverlap)
					collisionComponents[i]->OnComponentBeginOverlap(collisionComponents[i], collisionComponents[j]);

				if (collisionComponents[j]->OnComponentBeginOverlap)
					collisionComponents[j]->OnComponentBeginOverlap(collisionComponents[j], collisionComponents[i]);
			}
			else if (!isColliding && wasColliding) // 충돌 종료
			{
				if (collisionComponents[i]->OnComponentEndOverlap)
					collisionComponents[i]->OnComponentEndOverlap(collisionComponents[i], collisionComponents[j]);

				if (collisionComponents[j]->OnComponentEndOverlap)
					collisionComponents[j]->OnComponentEndOverlap(collisionComponents[j], collisionComponents[i]);
			}

			collisionPairs[pair] = isColliding;
		}
	}
}

bool CollisionManager::CheckAABBCollision(CollisionComponent* comp1, CollisionComponent* comp2)
{
	if (!comp1 || !comp2) return false;

	RECT rect1 = comp1->GetBoundingBox();
	RECT rect2 = comp2->GetBoundingBox();

	return !(rect1.right < rect2.left ||
		rect1.left > rect2.right ||
		rect1.bottom < rect2.top ||
		rect1.top > rect2.bottom);
}

void CollisionManager::Destroy()
{
	collisionComponents.clear();
}

void CollisionManager::RemovePendingKillComponents()
{
	collisionComponents.erase(
		remove_if(collisionComponents.begin(), collisionComponents.end(),
			[](CollisionComponent* comp) { return comp->IsPendingKill(); }),
		collisionComponents.end());
}

void CollisionManager::RegisterCollisionComponent(CollisionComponent* comp)
{
	if (!comp) return;

	collisionComponents.push_back(comp);
}

void CollisionManager::UnregisterCollisionComponent(CollisionComponent* comp)
{
	if (!comp) return;

	comp->SetPendginKill(true);
}
