// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
//   Contributed by Rama
//   Converted and improved by Axtel Sturnclaw.

#pragma once

class FISMCreator
{

//Instanced Static Mesh Creator
//  Utility Functions
public:
	/**
	* Create an instanced static mesh actor from an array of static mesh components.
	*
	* @param 	StaticMeshComponents, The first component is used to determine the static nesh asset and materials for the new instanced static nesh component.
	*			Each other static mesh component becomes an instance in the instanced static mesh component.
	*
	* @return A new empty actor with a properly initialized instanced static nesh component,
	*			where the instances exactly matche the world transforms of the input static mesh components
	*/
	static AActor* CreateInstancedStaticMeshActor( const TArray<UStaticMeshComponent*>& StaticMeshComponents );

	/**
	* Create an instanced static nesh actor from a static mesh component and a series of Instance Transforms
	*
	* @param StaticMeshComponent, The static nesh asset and materials for the new instanced static nesh component are copied from the StaticMeshComponent.
	* @param InstanceTransforms, Each of the instances to be greated for the new Instanced Static Mesh Component.
	* @param bWorldSpace, Whether or not the Transforms are in world space. If false, new instances will be added in component space.
	* @return A new empty actor with a properly initialized instanced static nesh component.
	*/
	static AActor* CreateInstancedStaticMeshActor( UStaticMeshComponent* StaticMeshComponent, const TArray<FTransform>& InstanceTransforms,bool bWorldSpace=true );

//Level Editor Commands
//  Called from ISMActions.cpp
public:

	/**
	* Create an instanced static mesh actor from all selected static mesh actors
	* that share the same static mesh asset as the root selected actor.
	*/
	static void CreateInstancedStaticMeshFromSelection();

	/** Revert selected instanced static mesh actors into editable static mesh actors */
	static void RevertSelectedInstancedStaticMeshes();


};
