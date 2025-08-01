#include "pch.h"
#include "PlayerController.h"
#include "InputManager.h"
#include "Player.h"

void PlayerController::Init()
{
	inputInstance = InputManager::GetInstance();
}

void PlayerController::Update(float deltaTime)
{
	if (!inputInstance) return;
	if (!PossedCharacter) return;

	bool isMoving = false;

    if (inputInstance->GetButtonPressed(KeyType::A))
    {
        PossedCharacter->MoveLeft(deltaTime);
        isMoving = true;
    }
    else if (inputInstance->GetButtonPressed(KeyType::D))
    {
        PossedCharacter->MoveRight(deltaTime);
        isMoving = true;
    }

    if (inputInstance->GetButtonDown(KeyType::SpaceBar))
    {
        PossedCharacter->Jump();
    }
    if (inputInstance->GetButtonDown(KeyType::LeftMouse))
    {
        PossedCharacter->GrappleFire();
    }

    if (!isMoving)
        PossedCharacter->Idle(deltaTime);
}

void PlayerController::Destroy()
{
	PossedCharacter = nullptr;
	inputInstance = nullptr;
}

void PlayerController::Posses(Player* character)
{
	if (character)
		PossedCharacter = character;
}