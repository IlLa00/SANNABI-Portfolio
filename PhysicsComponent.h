#pragma once

#include "Component.h"

class CollisionComponent;

class PhysicsComponent : public Component
{
	using Super = Component;

public:
	void Init(Actor* _owner) override;
	void Update(float deltaTime) override;
	void Destroy() override;

	bool CanJump() { return bOnGround && !bJumping; }

	bool IsOnGround() { return bOnGround; }

	void Idle();
	void Move();
	void Jump();

	void OnGroundBeginOverlap(CollisionComponent* self, CollisionComponent* other);
	void OnGroundEndOverlap(CollisionComponent* self, CollisionComponent* other);

private:
	void AddForce(Vector force);

private:
	float gravity = 980.f;
	float jumpForce = 500.f;  // 점프 힘
	float maxFallSpeed = 1000.f;  // 최대 낙하 속도

	bool bOnGround = false;
	bool bJumping = false;
};

