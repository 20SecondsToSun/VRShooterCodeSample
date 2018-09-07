// The Family

#include "SVRMixRealityPlayerController.h"

void ASVRMixRealityPlayerController::DefineBindControls()
{
	//Super::DefineBindControls();

	//InputComponent->BindAxis("MoveForward", this, &ASVRBasePlayerController::MoveForward);
	//InputComponent->BindAxis("MoveRight", this, &ASVRBasePlayerController::MoveRight);

	//InputComponent->BindAxis("LookUp", this, &ASVRBasePlayerController::LookUpHandler);
	//InputComponent->BindAxis("Turn", this, &ASVRBasePlayerController::TurnHandler);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ASVRBasePlayerController::PlayerStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ASVRBasePlayerController::PlayerStopFire);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ASVRBasePlayerController::Jump);

	InputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &ASVRBasePlayerController::ChangeWeapon);
	InputComponent->BindAction("DropFlag", IE_Pressed, this, &ASVRBasePlayerController::FlagDropHandler);
	InputComponent->BindAction("ChangeLevel", IE_Pressed, this, &ASVRBasePlayerController::ChangeLevelHandler);
	InputComponent->BindAction("Respawn", IE_Pressed, this, &ASVRBasePlayerController::ForceRespawnHandler);

	InputComponent->BindAction("MakeDead", IE_Pressed, this, &ASVRBasePlayerController::MakeDeadHandler);
}