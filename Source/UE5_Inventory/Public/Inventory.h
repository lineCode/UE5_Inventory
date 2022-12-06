// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

// Base struct of items
USTRUCT(BlueprintType)
struct FItemBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsStackable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;
};

// Base struct of cells
USTRUCT(BlueprintType)
struct FInvCell
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FItemBase Item;

	UPROPERTY(BlueprintReadWrite)
	int32 Amount;
};

// List of item types
UENUM(BlueprintType)
enum EItemCategories
{
	Consumable,
	Equipment,
	QuestItems,
	Readables
};

// Delegate to update inventory cell UI if an item has been added / removed / edited / moved
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCellUpdated, int32, CellToUpdate);

// Delegate to entire update inventory UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(Blueprintable, BlueprintType)
class/*UE5_INVENTORY_API*/ UInventory : public UObject
{
	GENERATED_BODY()

public:
	/*--------------------------------------------- FUNCTIONS --------------------------------------------------------*/
	
	// Adds an item to the cell
	UFUNCTION(BlueprintCallable)
	void AddItemNew(FItemBase Item, int32 Amount, bool& IsSuccess, int32& Rest, int32& CellIndex);

	// Removes certain number of items by index
	UFUNCTION(BlueprintCallable)
	void RemoveItemAtIndex(int32 Index, int32 Amount, bool& IsSuccess);

	// Checks if cell by this index is empty
	UFUNCTION(BlueprintCallable)
	bool IsCellEmpty(int32 CellIndex);

	// To find empty cell
	UFUNCTION(BlueprintCallable)
	void SearchEmptyCell(bool& IsSuccess, int32& Index);

	// To find empty stack
	UFUNCTION(BlueprintCallable)
	void SearchFreeStack(FItemBase Item, bool& IsSuccess, int32& Index);

	// Swaps two cells
	UFUNCTION(BlueprintCallable)
	void SwapCells(int32 Index1, int32 Index2, bool& IsSuccess);

	// Splits stack into a certain amount
	UFUNCTION(BlueprintCallable)
	void SplitStack(int32 StackIndex, int32 Amount, bool& IsSuccess);

	// Gets amount of items from cell 'A' and adds it to cell 'B'
	// (item of cell 'A' should equals to item of cell 'B' / used for drag&drop operations)
	UFUNCTION(BlueprintCallable)
	void AddToIndex(int32 FromIndex, int32 ToIndex, bool& IsSuccess);

	// Gets certain amount of items from cell 'A' and adds it to cell 'B'
	// (cell 'B' should be empty / used for drag&drop operations)
	UFUNCTION(BlueprintCallable)
	void SplitStackToIndex(int32 FromIndex, int32 ToIndex, int32 Amount, bool& IsSuccess);

	// Creates a new cell
	UFUNCTION(BlueprintCallable)
	void AddCell();

	// Removes a cell by index
	UFUNCTION(BlueprintCallable)
	void RemoveCell(int32 CellIndex);

	/*--------------------------------------------- GETTERS / SETTERS ------------------------------------------------*/

	// Gets item info by index
	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetItemAtIndex(int32 Index, bool& IsCellEmpty, FItemBase& Item, int32& Amount);

	// Sets new amount of cells
	UFUNCTION(BlueprintCallable)
	void SetAmountOfCells(int32 NewValue);

	// Gets amount of cells
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetAmountOfCells();

	// Gets amount of items by index
	UFUNCTION(BlueprintCallable)
	int32 GetAmountAtIndex(int32 Index);

	// Gets cells
	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetCells(TArray<FInvCell>& Result);

	/*--------------------------------------------- PROPERTIES -------------------------------------------------------*/
	
	// Signature of FOnCellUpdated delegate
	UPROPERTY(BlueprintAssignable)
	FOnCellUpdated OnCellUpdated;

	// Signature of FOnInventoryUpdated delegate
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;

	// Array of cells
	UPROPERTY(BlueprintReadWrite)
	TArray<FInvCell> Cells;

	// Amount of cells
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn=true))
	int32 AmountOfCells = 20;
private:

	// Max size of stack
	int32 MaxStackSize = 99;

	// Sets new Item and Amount of the cell
	FORCEINLINE void SetCell(int32 Index, FItemBase Item, int32 Amount);

	// Sets new Item of the cell
	FORCEINLINE void SetCell(int32 Index, FItemBase Item);

	// Sets new Amount of the cell
	FORCEINLINE void SetCell(int32 Index, int32 Amount);
};
