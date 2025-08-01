#include "pch.h"
#include "GrapplingComponent.h"
#include "GrapplingHookProjectilePool.h"
#include "GrapplingHookProjectile.h"
#include "CollisionComponent.h"
#include "Actor.h"
#include "Player.h"

void GrapplingComponent::Init(Actor* _owner)
{
	Super::Init(_owner);

	poolInstance = GrapplingHookProjectilePool::GetInstance();
	if(poolInstance)
		poolInstance->CreatePool(10);
}

void GrapplingComponent::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (curProjectile)
		curProjectile->Update(deltaTime);
	// 여기선 아마 플레이어의 중력을 담당할 것임.
}

void GrapplingComponent::Render(HDC _hdcBack)
{
	Super::Render(_hdcBack);

	if (curProjectile)
		curProjectile->Render(_hdcBack);
}

void GrapplingComponent::Destroy()
{
	Super::Destroy();
}

void GrapplingComponent::FireGrapple(Vector direction)
{
	if (poolInstance && !curProjectile) // 발사!
	{
		curProjectile = poolInstance->GetProjectile(owner->GetPosition(), direction, pullSpeed);
		if (curProjectile)
		{
			curProjectile->SetOwner(owner);

			CollisionComponent* comp = curProjectile->GetComponent<CollisionComponent>();
			if (comp)
			{
				comp->OnComponentBeginOverlap = [this](CollisionComponent* self, CollisionComponent* other)
					{
						if (other && other->GetOwner() == owner)
							return;

						curProjectile->SetFlying(false);
						OnGrappling();
					};
				comp->OnComponentEndOverlap = [this](CollisionComponent* self, CollisionComponent* other)
					{
						// 뭔가를 또 해야함
						OffGrappling();
					};
			}
		}
	}
}

void GrapplingComponent::OnGrappling()
{
	Player* player = dynamic_cast<Player*>(owner);
	if (player)
		player->OnGrappling();
}

void GrapplingComponent::OffGrappling()
{
	Player* player = dynamic_cast<Player*>(owner);
	if (player)
		player->OffGrappling();
}

