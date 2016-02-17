// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
//   Contributed by Rama
//   Converted and improved by Axtel Sturnclaw.

#include "ISMPluginPrivatePCH.h"
#include "UnrealEd.h"
#include "ISMCreator.h"
#include "ISMActor.h"
#include "ScopedTransaction.h"

#define LOCTEXT_NAMESPACE "ISMCreator"

AActor* FISMCreator::CreateInstancedStaticMeshActor( const TArray<UStaticMeshComponent*>& StaticMeshComponents )
{
	if(StaticMeshComponents.Num() < 1)
	{
		//No Components
		return nullptr;
	}

	UStaticMeshComponent* RootSMC = StaticMeshComponents[0];
	if(!RootSMC)
	{
		//Validity check
		return nullptr;
	}

	//Gather transforms
	TArray<FTransform> WorldTransforms;
	for(const UStaticMeshComponent* EachComponent : StaticMeshComponents )
	{
		if(!EachComponent)
		{
			//Validity check
			continue;
		}
		WorldTransforms.Add(EachComponent->GetComponentTransform());
	}

	//Create!
	return CreateInstancedStaticMeshActor(RootSMC,WorldTransforms,true); //World Space
}

AActor* FISMCreator::CreateInstancedStaticMeshActor( UStaticMeshComponent* StaticMeshComponent, const TArray<FTransform>& InstanceTransforms,bool bWorldSpace )
{
	if(!StaticMeshComponent)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.bDeferConstruction 	= false;

	AISMActor* NewISMA = StaticMeshComponent->GetWorld()->SpawnActor<AISMActor>(
		AISMActor::StaticClass(),
		StaticMeshComponent->GetComponentLocation(),
		StaticMeshComponent->GetComponentRotation(),
		SpawnInfo
	);

	check(NewISMA);
	check(NewISMA->ISMComp);

	//Get Component
	UInstancedStaticMeshComponent* NewISMC = NewISMA->ISMComp;

	//Mesh
	NewISMC->SetStaticMesh(StaticMeshComponent->StaticMesh);

	//Materials
	const int32 MatTotal = StaticMeshComponent->GetNumMaterials();
	for(int32 v = 0; v < MatTotal; v++)
	{
		NewISMC->SetMaterial(v,StaticMeshComponent->GetMaterial(v));
	}

	//Add each instance to the Instancd Static Mesh Component!
	for(const FTransform& EachInstanceTransform : InstanceTransforms)
	{
		if(bWorldSpace)
		{
			NewISMC->AddInstanceWorldSpace(EachInstanceTransform);
		}
		else
		{
			NewISMC->AddInstance(EachInstanceTransform);
		}
	}

	NewISMC->SetMobility(StaticMeshComponent->Mobility);
	NewISMC->Modify();

	return NewISMA;
}

void FISMCreator::CreateInstancedStaticMeshFromSelection()
{
	const FScopedTransaction Transaction(LOCTEXT("CreateInstancedStaticMeshFromSelection", "Create Instanced Static Mesh Actor From Selected Actors"));

	if(GEditor->GetSelectedActorCount() < 1){
		return;
	}

	TArray<FTransform> InstanceTransforms;
	TArray<AActor*> ToDestroy;

	//Root Selected Static Mesh Actor's Component
	// Determines which static mesh asset is used and also the Materials used
	// in the Instanced Static Mesh Component
	UStaticMeshComponent* RootSMC 	= nullptr;

	for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
	{
		AStaticMeshActor* EachSMA = Cast<AStaticMeshActor>( *It );
		if(!EachSMA)
		{
			//Not an SMA
			continue;
		}

		//Has root SMC been found yet? (Component of first of selection that is an SMA)
		if(!RootSMC)
		{
			RootSMC = EachSMA->GetStaticMeshComponent();
		}

		else if(EachSMA->GetStaticMeshComponent()->StaticMesh != RootSMC->StaticMesh){
			//Not the same mesh.
			continue;
		}

		//Add
		InstanceTransforms.Add(EachSMA->GetTransform());

		//Will be destroying the original SMAs
		ToDestroy.Add(EachSMA);
	}

	//No Static Mesh Actors in current Selection?
	if(!RootSMC)
	{
		return;
	}

	//World must be valid
	if(!RootSMC->GetWorld())
	{
		return;
	}

	//Create new empty actor with Instanced Static Mesh Component
	AActor* NewISMA = CreateInstancedStaticMeshActor(RootSMC,InstanceTransforms);
	if(!NewISMA)
	{
		return;
	}

	//Copy Layers
	NewISMA->Layers = RootSMC->GetOwner()->Layers;

	//Set Folder
	NewISMA->SetFolderPath(RootSMC->GetOwner()->GetFolderPath());

	FActorLabelUtilities::SetActorLabelUnique(NewISMA, FString::Printf(TEXT("ISM_%s"), *RootSMC->StaticMesh->GetName()));

	//ISM fully created
	NewISMA->Modify();

	for(AActor* EachActorToDestroy : ToDestroy)
	{
		//Deselect actor to properly reset the tranform gizmo.
		GEditor->SelectActor(EachActorToDestroy, false, true);
		//This SMA is being destroyed, so save undo
		EachActorToDestroy->Modify();
		EachActorToDestroy->Destroy();
	}

	//Select new ISMA!
	GEditor->SelectActor( NewISMA, true, true );

}

void FISMCreator::RevertSelectedInstancedStaticMeshes()
{
	const FScopedTransaction Transaction(LOCTEXT("RevertSelectedInstancedStaticMesh", "Revert Instanced Static Mesh Actor to individual Static Mesh Actors"));

	for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
	{
		AActor* EachSelectedActor = static_cast<AActor*>( *It );

		TArray<UInstancedStaticMeshComponent*> Comps;
		EachSelectedActor->GetComponents(Comps);
		if(Comps.Num() < 1)
		{
			//No ISM components
			continue;
		}

		//For each Instanced Static Mesh Component of current selected actor
		for(UInstancedStaticMeshComponent* EachISMC :Comps)
		{
			//World must be valid
			if(!EachSelectedActor->GetWorld())
			{
				continue;
			}

			TArray<FTransform> InstanceTransforms;
			int32 Total = EachISMC->GetInstanceCount();
			for(int32 v = 0; v < Total; v++)
			{
				FTransform Transform;
				EachISMC->GetInstanceTransform(v,Transform,true); //world space!
				InstanceTransforms.Add(Transform);
			}

			//for each Transform in world space
			for(FTransform& EachInstanceTransform : InstanceTransforms)
			{
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnInfo.bDeferConstruction 	= false;

				AStaticMeshActor* EachSMA = EachSelectedActor->GetWorld()->SpawnActor<AStaticMeshActor>(
					AStaticMeshActor::StaticClass(),
					EachISMC->GetComponentLocation() , EachISMC->GetComponentRotation(), SpawnInfo
				);

				//Mesh Comp
				UStaticMeshComponent* Mesh = EachSMA->GetStaticMeshComponent();

				//Movable!
				Mesh->SetMobility(EComponentMobility::Movable);

				//Mesh
				Mesh->SetStaticMesh(EachISMC->StaticMesh);

				//Materials
				const int32 MatTotal = EachISMC->GetNumMaterials();
				for(int32 v = 0; v < MatTotal; v++)
				{
					Mesh->SetMaterial(v,EachISMC->GetMaterial(v));
				}

				//Transform
				EachSMA->SetActorTransform(EachInstanceTransform);

				//Copy Layers
				EachSMA->Layers = EachSelectedActor->Layers;

				//Set Folder
				EachSMA->SetFolderPath(EachSelectedActor->GetFolderPath());

				//Done
				EachSMA->Modify();

				//Select new SMA!
				GEditor->SelectActor( EachSMA, true, true );
			}

			//Deselect actor to properly reset the tranform gizmo.
			GEditor->SelectActor(EachSelectedActor, false, true);

			//Prep for destroy
			EachSelectedActor->Modify();
			EachSelectedActor->Destroy();
		}
	}
}



#undef LOCTEXT_NAMESPACE
