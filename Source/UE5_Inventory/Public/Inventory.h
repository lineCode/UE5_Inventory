// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

USTRUCT(BlueprintType)
struct FItemBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 id;

	UPROPERTY(BlueprintReadWrite)
	FString name;

	UPROPERTY(BlueprintReadWrite)
	FString description;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* ItemIcon;
};

// USTRUCT(BlueprintType)
// struct FItemState
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(BlueprintReadWrite)
// 	int32 some_state;
// };

USTRUCT(BlueprintType)
struct FInvCell
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FItemBase item;
	
	UPROPERTY(BlueprintReadWrite)
	int32 quantity;
	
	// UPROPERTY(BlueprintReadWrite)
	// TArray<FItemState> item_states;
};

/**
 * 
 */
UCLASS(Blueprintable)
class UE5_INVENTORY_API UInventory : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	// UInventoryComponent();

	// Adds an item to the inventory
	UFUNCTION(BlueprintCallable)
	void AddItem(FItemBase item);

	// Removes an item from the inventory
	UFUNCTION(BlueprintCallable)
	void RemoveItem(FItemBase item);

	// Swaps two items in inventory
	void SwapItems(FItemBase ls_item, FItemBase rs_item);

	// Gets an item by id
	FItemBase GetItem(int32 item_id);

	// Debug
	UFUNCTION(BlueprintCallable)
	void PrintInventory();
	
	
private:

	// Finds a cell by item
	TSharedPtr<FInvCell> FindCellByItem(const FItemBase& Item);

	// Quantity of cells
	uint32 Capacity;

	// Array of cells
	TArray<TSharedPtr<FInvCell>> Cells;
		
};
