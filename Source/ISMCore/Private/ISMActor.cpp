// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// Created by Axtel Sturnclaw

#include "ISMCore.h"
#include "ISMActor.h"

AISMActor::AISMActor()
{
  ISMComp = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh Component"));
  SetRootComponent(ISMComp);
}
