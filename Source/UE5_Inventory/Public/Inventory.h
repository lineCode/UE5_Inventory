// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

USTRUCT(BlueprintType)
struct FItemBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isStackable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D *Icon;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
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
	int32 index;

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
class UE5_INVENTORY_API UInventory : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	// UInventoryComponent();

	// Adds an item to the inventory
	UFUNCTION(BlueprintCallable)
	void AddItem(FItemBase Item);

	// Removes an item from the inventory
	UFUNCTION(BlueprintCallable)
	void RemoveItem(FItemBase Item);

	// Swaps two items in inventory
	void SwapItems(FItemBase Ls_Item, FItemBase Rs_Item);

	// Gets an item by id
	// FItemBase GetItem(int32 Item_ID);

	// Debug
	UFUNCTION(BlueprintCallable)
	void PrintInventory();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetAmountOfCells();

	UFUNCTION(BlueprintCallable)
	void SetAmountOfCells(int32 NewValue);

	UFUNCTION(BlueprintCallable)
	bool IsCellEmpty(int32 CellIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetItemAtIndex(int32 index, bool &IsCellEmpty, FItemBase &Item, int32 &Amount);

	UFUNCTION(BlueprintCallable)
	void SearchEmptyCell(bool &IsSuccess, int32 &Index);

	UFUNCTION(BlueprintCallable)
	void SearchFreeStack(FItemBase Item, bool &IsSuccess, int32 &Index);

	UFUNCTION(BlueprintCallable)
	void AddItemNew(FItemBase Item, int32 Amount, bool &IsSuccess, int32 &Rest, int32 &CellIndex);

	UFUNCTION(BlueprintCallable)
	int32 GetAmountAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetCells(TArray<FInvCell> &Result);
	
	// Array of cells
	UPROPERTY(BlueprintReadWrite)
	TArray<FInvCell> Cells;
	
private:
	// Finds a cell by item
	TSharedPtr<FInvCell> FindCellByItem(const FItemBase &Item);

	// Amount of cells
	uint32 AmountOfCells = 20;
	
	// Max size of stack
	int32 MaxStackSize = 99;

};
