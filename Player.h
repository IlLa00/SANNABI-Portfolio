#pragma once

#include "Actor.h"

enum class EPlayerMovementState
{
	Idle,
	Run,
	Jump,
	Fall,
	Land,
	WallGrab,
	Grappleing,
};

enum class EPlayerActionState
{
	None,
	Attack,
	GrappleFire,
	GrappleSwing,
	GrappleReelIn,
	TakeDamage,
	Die
};

class SpriteRenderComponent;
class GrapplingComponent;
class CollisionComponent;
class PhysicsComponent;
class TextureResource;

class Player : public Actor
{
	using Super = Actor;

public:
	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC _hdcBack) override;
	void Destroy() override;

	void Idle(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void Jump();
	void GrappleFire();
	void OnGrappling();
	void OffGrappling();

	void UpdateMovementState(EPlayerMovementState state);
	void UpdateActionState(EPlayerActionState state);

private:
	SpriteRenderComponent* bodyRenderComponent = nullptr;
	SpriteRenderComponent* armRenderComponent = nullptr;
	GrapplingComponent* grapplingComponent = nullptr;
	CollisionComponent* collisionComponent = nullptr;
	PhysicsComponent* physicsComponent = nullptr;  

	EPlayerMovementState movementState;
	EPlayerActionState actionState;

	bool showAimingLine = true;  
	HPEN chainPen;               
};

