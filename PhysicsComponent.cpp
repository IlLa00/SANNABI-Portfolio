#include "pch.h"
#include "PhysicsComponent.h"
#include "TimerManager.h"
#include "Actor.h"
#include "CollisionComponent.h"

void PhysicsComponent::Init(Actor* _owner)
{
	Super::Init(_owner);

	if (owner)
	{
		CollisionComponent* collisionComp = owner->GetComponent<CollisionComponent>();
		if (collisionComp)
		{
			
			collisionComp->OnComponentBeginOverlap = [this](CollisionComponent* self, CollisionComponent* other)
				{
					OnGroundBeginOverlap(self, other);
				};
			collisionComp->OnComponentEndOverlap = [this](CollisionComponent* self, CollisionComponent* other)
				{
					// 사슬팔인데 플레이어와 부딪히면 처리 안함
					if (other->GetCollisionChannel() == ECollisionChannel::Projectile && other->GetOwner()->GetOwner() == owner)
						return;

					OnGroundEndOverlap(self, other);
				};
		}
	}
}

void PhysicsComponent::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (!owner) return;

	// 중력 업데이트
	Vector acceleration = owner->GetAcceleration();

	if (!bOnGround) // 공중에 있을 때는 중력 적용
		acceleration.y = gravity;
	else  // 지면에 있을 때는 중력 제거
		acceleration.y = 0;

	owner->SetAcceleration(acceleration);
	
	// 속도 업데이트
	Vector velocity = owner->GetVelocity();
	velocity += acceleration * deltaTime;

	// 지면에 있고 아래로 향하는 속도가 있으면 제거
	if (bOnGround && velocity.y > 0)
		velocity.y = 0;

	// 최대 낙하 속도 제한
	if (velocity.y > maxFallSpeed)
		velocity.y = maxFallSpeed;

	owner->SetVelocity(velocity);

	// 위치 업데이트
	Vector currentPos = owner->GetPosition();
	Vector newPos = currentPos + velocity * deltaTime;
	owner->SetPosition(newPos);

	// 점프 상태 체크
	if (bJumping && velocity.y >= 0)  
		bJumping = false;
}

void PhysicsComponent::Destroy()
{
	Super::Destroy();

}

void PhysicsComponent::Idle()
{
	if (!owner) return;

	Vector velocity = owner->GetVelocity();
	velocity.x = 0;
	owner->SetVelocity(velocity);
}

void PhysicsComponent::Move()
{
	if (!owner) return;

	float direction = owner->GetDirection();

	Vector velocity = owner->GetVelocity();
	velocity.x = owner->GetSpeed() * direction;

	owner->SetVelocity(velocity);
}


void PhysicsComponent::Jump()
{
	if (!owner || !CanJump()) return;

	Vector velocity = owner->GetVelocity();
	velocity.y = -jumpForce;
	owner->SetVelocity(velocity);

	bOnGround = false;
	bJumping = true;
}

void PhysicsComponent::OnGroundBeginOverlap(CollisionComponent* self, CollisionComponent* other)
{
	if (bOnGround) return;

	if (other && other->GetCollisionChannel() == ECollisionChannel::WorldStatic)
	{
		Vector velocity = owner->GetVelocity();
		if (velocity.y >= 0)  
			bOnGround = true;
	}
}

void PhysicsComponent::OnGroundEndOverlap(CollisionComponent* self, CollisionComponent* other)
{
	if (!bOnGround) return;

	bOnGround = false;
}

void PhysicsComponent::AddForce(Vector force)
{
	
}