// The Family

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/PrimitiveComponent.h"
#include "InteractableInterface.generated.h"

USTRUCT(BlueprintType)
struct FMeshStencilStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
	class UMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
		int32 Bit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
		ERendererStencilMask StencilMask;
};

class UMeshComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class VRSHOOTER_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void StartInteract(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void StopInteract(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		TArray<FMeshStencilStruct> GetHighlightMeshes() const;
	virtual TArray<FMeshStencilStruct> GetHighlightMeshes_Implementation() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void AddHighlightMesh(const FMeshStencilStruct& Value);
	void AddHighlightMesh_Implementation(const FMeshStencilStruct& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void HighLightMeshes();
	virtual void HighLightMeshes_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void SnuffOutMeshes();
	virtual void SnuffOutMeshes_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		bool IsHighlighted() const;
	virtual bool IsHighlighted_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void OnHighlightInteractableChanged(bool Value);
	virtual void OnHighlightInteractableChanged_Implementation(bool Value);

private:
	bool Highlighted = false;
	TArray<FMeshStencilStruct> HighlightMeshes;
};
