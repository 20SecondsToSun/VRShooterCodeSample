// The Family

#include "InteractableInterface.h"
#include "Components/MeshComponent.h"

// Add default functionality here for any IIInteractible functions that are not pure virtual.

void IInteractableInterface::AddHighlightMesh_Implementation(const FMeshStencilStruct& Value)
{
	HighlightMeshes.Add(Value);
}

TArray<FMeshStencilStruct> IInteractableInterface::GetHighlightMeshes_Implementation() const
{
	return HighlightMeshes;
}

bool IInteractableInterface::IsHighlighted_Implementation() const
{
	return Highlighted;
}

void IInteractableInterface::HighLightMeshes_Implementation()
{	
	for (int32 i = 0; i< HighlightMeshes.Num(); i++)
	{
		auto Mesh = HighlightMeshes[i].Mesh;
		Mesh->bRenderCustomDepth = true;

		auto Bit = HighlightMeshes[i].Bit;
		Mesh->SetCustomDepthStencilValue(Bit);

		auto StencilMask = HighlightMeshes[i].StencilMask;		
		Mesh->SetCustomDepthStencilWriteMask(StencilMask);
	}

	Highlighted = false;
	OnHighlightInteractableChanged_Implementation(Highlighted);
}

void IInteractableInterface::SnuffOutMeshes_Implementation()
{
	for (auto& Highlight : HighlightMeshes)
	{
		Highlight.Mesh->SetCustomDepthStencilValue(0);
	}

	Highlighted = true;
	OnHighlightInteractableChanged_Implementation(Highlighted);
}

void IInteractableInterface::OnHighlightInteractableChanged_Implementation(bool Value)
{

}

