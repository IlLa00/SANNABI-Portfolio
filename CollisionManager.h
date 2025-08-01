#pragma once

#include "Singleton.h"
#include "CollisionComponent.h"

class CollisionComponent;

class CollisionManager : public Singleton<CollisionManager>
{
	friend Singleton<CollisionManager>;

private:
	CollisionManager() {}

public:
	void Init(HWND hwnd);
	void Update();
	void Destroy();

	void RemovePendingKillComponents();

	void RegisterCollisionComponent(CollisionComponent* comp);
	void UnregisterCollisionComponent(CollisionComponent* comp);

	bool CheckAABBCollision(CollisionComponent* comp1, CollisionComponent* comp2);

private:
	bool bIgnore[(int)ECollisionChannel::Max][(int)ECollisionChannel::Max] = { 0 };
	bool bBlock[(int)ECollisionChannel::Max][(int)ECollisionChannel::Max] = { 0 };
	bool bOverlap[(int)ECollisionChannel::Max][(int)ECollisionChannel::Max] = { 0 };

	vector<CollisionComponent*> collisionComponents;
	unordered_map<pair<CollisionComponent*, CollisionComponent*>, bool, PairHash> collisionPairs;
};