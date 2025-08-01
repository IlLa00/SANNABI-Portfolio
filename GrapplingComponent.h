#pragma once

#include "Component.h"

class GrapplingHookProjectilePool;
class GrapplingHookProjectile;
class SpriteRenderComponent;

class GrapplingComponent : public Component
{
	using Super = Component;

public:
	void Init(Actor* _owner) override;
	void Update(float deltaTime) override;
	void Render(HDC _hdcBack) override;
	void Destroy() override;

	void FireGrapple(Vector direction);

	void OnGrappling();
	void OffGrappling();

private:
	GrapplingHookProjectilePool* poolInstance = nullptr;
	GrapplingHookProjectile* curProjectile = nullptr;

	float maxChainLength = 300.0f;
	float pullSpeed = 2000.0f;
	float fireCooldown = 0.5f;

	bool bFiring = false;
	bool bAttached = false;
	Vector attachedPoint;
};

