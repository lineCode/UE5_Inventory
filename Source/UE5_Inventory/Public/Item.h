// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class UE5_INVENTORY_API UItem : public UObject
{
	GENERATED_BODY()

public:

	// performs item's ability
	UFUNCTION(BlueprintCallable)
	void UseItem();

	// unique id
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Description;

	// can be stacked?
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsStackable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;

	
};
