#pragma once

#include "Component.h"

enum class ECollisionChannel
{
	WorldStatic, // 정적인 지형
	WorldDynamic, // 동적인 플랫폼
	Character, // 캐릭터
	Projectile, // 발사체

	Max
};

class CollisionComponent : public Component
{
	using Super = Component;
	using FOnComponentBeginOverlap = function<void(CollisionComponent*, CollisionComponent*)>;
	using FOnComponentEndOverlap = function<void(CollisionComponent*, CollisionComponent*)>;

public:
	void Init(Actor* _owner) override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	void Destroy() override;

	void SetCollisionSize(int _offsetX, int _offsetY);

	void SetPendginKill(bool state) { bPendingKill = state; }
	bool IsPendingKill() { return bPendingKill; }

	void SetCollisionChannel(ECollisionChannel channel) { collisionChannel = channel; }
	ECollisionChannel GetCollisionChannel() { return collisionChannel; }

	RECT GetBoundingBox() const;

public:
	FOnComponentBeginOverlap OnComponentBeginOverlap;
	FOnComponentEndOverlap OnComponentEndOverlap;

private:
	Vector position;
	int offsetX;
	int offsetY;
	int width;  
	int height;  

	bool bPendingKill = false;

	ECollisionChannel collisionChannel;
};

