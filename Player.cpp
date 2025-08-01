#include "pch.h"
#include "Player.h"
#include "SpriteRenderComponent.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "GrapplingComponent.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "TextureResource.h"
#include "ResourceManager.h"

void Player::Init()
{
	Super::Init();

	position = Vector(GWinSizeX / 2 - 300 , GWinSizeY / 2);
	
	bodyRenderComponent = new SpriteRenderComponent;
	bodyRenderComponent->Init(this);
	bodyRenderComponent->AddAnimation("Idle", "SNB_Idle", 8, 1.f);
	bodyRenderComponent->AddAnimation("Run", "SNB_Run", 22, 3.f);
	bodyRenderComponent->PlayAnimation("Idle");
	AddComponent(bodyRenderComponent);

	armRenderComponent = new SpriteRenderComponent;
	armRenderComponent->Init(this);
	armRenderComponent->AddAnimation("ArmIdle", "SNBArm_Idle", 8, 1.f);
	armRenderComponent->PlayAnimation("ArmIdle");
	AddComponent(armRenderComponent);

	grapplingComponent = new GrapplingComponent;
	grapplingComponent->Init(this);
	AddComponent(grapplingComponent);

	collisionComponent = new CollisionComponent;
	collisionComponent->Init(this);
	collisionComponent->SetCollisionSize(50, 75);
	collisionComponent->SetCollisionChannel(ECollisionChannel::Character);
	AddComponent(collisionComponent);

	physicsComponent = new PhysicsComponent;
	physicsComponent->Init(this);
	AddComponent(physicsComponent);

	chainPen = CreatePen(PS_DOT, 2, RGB(255, 255, 0)); 
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);

	CameraManager::GetInstance()->Update(position,Vector(GWinSizeX,GWinSizeY));
}

void Player::Render(HDC _hdcBack)
{
	Super::Render(_hdcBack);

	if (showAimingLine) // 일단 임시
	{
		Vector mousePos = InputManager::GetInstance()->GetMousePos();

		// 거리 계산
		float distance = (mousePos - position).Length();
		const float MAX_GRAPPLE_DISTANCE = 400.0f; // 최대 그래플링 거리

		// 거리에 따른 색상 변경
		COLORREF lineColor;
		if (distance <= MAX_GRAPPLE_DISTANCE)
			lineColor = RGB(0, 255, 0);    // 초록색 (사용 가능)
		else
			lineColor = RGB(255, 0, 0);    // 빨간색 (사용 불가)

		// 펜 설정
		HPEN oldPen = (HPEN)SelectObject(_hdcBack, CreatePen(PS_DOT, 2, lineColor));

		// 사슬 조준선 그리기 (점선)
		MoveToEx(_hdcBack, (int)position.x, (int)position.y, NULL);
		LineTo(_hdcBack, (int)mousePos.x, (int)mousePos.y);

		// 마우스 커서 위치에 작은 원 그리기 (조준점)
		HBRUSH oldBrush = (HBRUSH)SelectObject(_hdcBack, CreateSolidBrush(lineColor));
		Ellipse(_hdcBack,
			(int)mousePos.x - 5, (int)mousePos.y - 5,
			(int)mousePos.x + 5, (int)mousePos.y + 5);

		// 리소스 정리
		DeleteObject(SelectObject(_hdcBack, oldPen));
		DeleteObject(SelectObject(_hdcBack, oldBrush));
	}
		
}

void Player::Destroy()
{
	Super::Destroy();
}

void Player::Idle(float deltaTime)
{
	if (physicsComponent)
		physicsComponent->Idle();

	UpdateMovementState(EPlayerMovementState::Idle);

	bodyRenderComponent->PlayAnimation("Idle");
	armRenderComponent->PlayAnimation("ArmIdle");
}

void Player::MoveLeft(float deltaTime)
{
	direction = -1;

	if (physicsComponent)
		physicsComponent->Move();
	
	// 나중에 액션 고려..
	UpdateMovementState(EPlayerMovementState::Run);

	bodyRenderComponent->PlayAnimation("Run");
	//armRenderComponent->PlayAnimation("ArmRun");
}

void Player::MoveRight(float deltaTime)
{
	direction = 1;

	if (physicsComponent)
		physicsComponent->Move();

	// 나중에 액션 고려..
	UpdateMovementState(EPlayerMovementState::Run);

	bodyRenderComponent->PlayAnimation("Run");
	//armRenderComponent->PlayAnimation("ArmRun");
}

void Player::Jump()
{
	if (physicsComponent)
		physicsComponent->Jump();

	UpdateMovementState(EPlayerMovementState::Jump);
	// armRenderComponent->PlayAnimation("ArmJump");
}

void Player::GrappleFire()
{
	if (grapplingComponent)
	{
		Vector mousePos = InputManager::GetInstance()->GetMousePos();
		Vector mouse_direction = mousePos - position;
		mouse_direction.Normalize();

		grapplingComponent->FireGrapple(mouse_direction);
	}

	UpdateActionState(EPlayerActionState::GrappleFire);

	// 애니메이션
}

void Player::OnGrappling()
{
	UpdateActionState(EPlayerActionState::GrappleReelIn);
}

void Player::OffGrappling()
{
	UpdateActionState(EPlayerActionState::None);
}

void Player::UpdateMovementState(EPlayerMovementState state)
{
	movementState = state;
}

void Player::UpdateActionState(EPlayerActionState state)
{
	actionState = state;
}
