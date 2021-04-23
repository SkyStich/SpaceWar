// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSingleton.h"
#include "Engine/Engine.h"

UBaseSingleton& UBaseSingleton::Get()
{
    UBaseSingleton* Singleton = Cast<UBaseSingleton>(GEngine->GameSingleton);
    
    return Singleton ? *Singleton : *NewObject<UBaseSingleton>(UBaseSingleton::StaticClass());
}
