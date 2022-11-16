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

//TODO: Create a system for binding the corresponding item state to the item
/* Note:
 *	We also need to keep track of item states inside the inventory.
 *	The main thing to implement is to bind a unique state to an individual item.
 *	It is unclear to us how it will look in the current state of the project.
 */

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
UCLASS(Blueprintable, BlueprintType)
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
