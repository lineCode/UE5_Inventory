// Fill out your copyright notice in the Description page of Project Settings.


// #include "Inventory.h"
#include "UE5_Inventory/Public/Inventory.h"

void UInventory::PrintInventory()
{
	UE_LOG(LogTemp, Log, TEXT("Number Of Cells: %d"), Cells.Num());
	UE_LOG(LogTemp, Log, TEXT("\n\n"));

	for (auto InvCell : Cells)
	{
		UE_LOG(LogTemp, Log, TEXT("ID: %d"), InvCell.item.id);
		UE_LOG(LogTemp, Log, TEXT("Name: %s"), *(InvCell.item.name));
		UE_LOG(LogTemp, Log, TEXT("Description: %s"), *(InvCell.item.description));
		UE_LOG(LogTemp, Log, TEXT("Quantity: %d"), InvCell.quantity);
		UE_LOG(LogTemp, Log, TEXT("\n"));
	}

	UE_LOG(LogTemp, Log, TEXT("--------------------------------------------------\n"));
}

int32 UInventory::GetAmountOfCells()
{
	return this->AmountOfCells;
}

void UInventory::SetAmountOfCells(int32 NewValue)
{
	this->AmountOfCells = NewValue;
}

bool UInventory::IsCellEmpty(int32 CellIndex)
{
	if (Cells[CellIndex].quantity == 0) 
	{
		return true;
	}

	return false;
}

void UInventory::GetItemAtIndex(int32 index, bool &IsCellEmpty, FItemBase &Item, int32 &Amount)
{
	// check if a cell by this index is empty
	IsCellEmpty = UInventory::IsCellEmpty(index);
	// IsCellEmpty = false;

	// if not empty...
	if (!IsCellEmpty)
	{
		Item = this->Cells[index].item;
		Amount = this->Cells[index].quantity;
	}
}

void UInventory::SearchEmptyCell(bool &IsSuccess, int32 &Index)
{
	IsSuccess = false;

	for (int i = 0; i < Cells.Num(); i++)
	{
		if (IsCellEmpty(i))
		{
			IsSuccess = true;
			Index = i;
			break;
		}
	}
}

void UInventory::SearchFreeStack(FItemBase Item, bool &IsSuccess, int32 &Index)
{
	IsSuccess = false;

	for (int i = 0; i < Cells.Num(); i++)
	{
		if (!IsCellEmpty(i))
		{
			// if given item has the same type with founded item, and amount of those items lower than maximum size of stack
			if (Cells[i].item.id == Item.id && Cells[i].quantity < MaxStackSize)
			{
				IsSuccess = true;
				Index = i;
				break;
			}
		}
	}
}

void UInventory::AddItemNew(FItemBase Item, int32 Amount, bool &IsSuccess, int32 &Rest, int32 &CellIndex)
{
	int32 foundIndex = NULL;
	bool isSuccess;
	int32 index;
	int32 rest;

	if (Item.isStackable)
	{
		SearchFreeStack(Item, isSuccess, index);

		// if free stack was found
		if (isSuccess)
		{
			foundIndex = index;
			
			// if overstack
			if (Cells[foundIndex].quantity + Amount > MaxStackSize)
			{
				int32 amountLocal = Cells[foundIndex].quantity + Amount - MaxStackSize;
				
				Cells[foundIndex].item = Item;
				Cells[foundIndex].quantity = MaxStackSize;

				AddItemNew(Item, amountLocal, isSuccess, rest, foundIndex);

				// return values
				IsSuccess = true;
				Rest = rest;
				CellIndex = foundIndex;
			}

			// if no overstack
			else
			{
				Cells[foundIndex].item = Item;
				Cells[foundIndex].quantity = Cells[foundIndex].quantity + Amount;
				
				IsSuccess = true;
				Rest = 0;
				CellIndex = foundIndex;
			}
		}

		// if free stack was not found
		if (!isSuccess)
		{
			SearchEmptyCell(isSuccess, index);

			// if empty cell was found
			if (isSuccess)
			{
				foundIndex = index;
				
				if (Amount > MaxStackSize)
				{
					Cells[foundIndex].item = Item;
					Cells[foundIndex].quantity = MaxStackSize;
					
					AddItemNew(Item, Amount - MaxStackSize, isSuccess, rest, foundIndex);

					IsSuccess = true;
					Rest = rest;
					CellIndex = foundIndex;
				}

				else
				{
					Cells[foundIndex].item = Item;
					Cells[foundIndex].quantity = Amount;

					IsSuccess = true;
					Rest = 0;
					CellIndex = foundIndex;
				}
			}

			// if empty cell was not found
			if (!isSuccess)
			{
				IsSuccess = false;
				Rest = Amount;
				CellIndex = foundIndex;
			}
		}
	}

	if (!Item.isStackable)
	{
		SearchEmptyCell(isSuccess, index);

		if (isSuccess)
		{
			foundIndex = index;

			Cells[foundIndex].item = Item;
			Cells[foundIndex].quantity = 1;

			if (Amount > 1)
			{
				AddItemNew(Item, Amount - 1, isSuccess, rest, foundIndex);

				IsSuccess = true;
				Rest = rest;
				CellIndex = foundIndex;
			}
			else
			{
				IsSuccess = true;
				Rest = 0;
				CellIndex = foundIndex;
			}
		}

		if (!isSuccess)
		{
			IsSuccess = false;
			Rest = Amount;
			CellIndex = foundIndex;
		}
	}
}

void UInventory::RemoveItemAtIndex(int32 Index, int32 Amount, bool &IsSuccess)
{
	IsSuccess = false;
	
	if (!IsCellEmpty(Index))
	{
		if (Amount >= GetAmountAtIndex(Index))
		{
			Cells[Index].item.id = NULL;
			Cells[Index].item.name = nullptr;
			Cells[Index].item.description = nullptr;
			Cells[Index].item.isStackable = nullptr;
			Cells[Index].item.Icon = nullptr;

			Cells[Index].quantity = NULL;
			
			IsSuccess = true;
		}
		else
		{
			Cells[Index].quantity -= Amount;

			IsSuccess = true;
		}
	}
}

void UInventory::SwapCells(int32 Index1, int32 Index2, bool &IsSuccess)
{
	IsSuccess = false;
	
	// check if indices are not out of bounds
	if (Index1 <= Cells.Num() - 1 || Index2 <= Cells.Num() - 1)
	{
		FInvCell cellLocal = Cells[Index1];

		Cells[Index1] = Cells[Index2];
		Cells[Index2] = cellLocal;

		IsSuccess = true;
	}
}

void UInventory::SplitStack(int32 StackIndex, int32 Amount, bool &IsSuccess)
{
	IsSuccess = false;
	
	if (!IsCellEmpty(StackIndex))
	{
		if (Cells[StackIndex].item.isStackable && Cells[StackIndex].quantity > Amount)
		{
			bool isSuccess;
			int index;
			
			SearchEmptyCell(isSuccess, index);

			if (isSuccess)
			{
				Cells[StackIndex].quantity -= Amount;

				Cells[index].item = Cells[StackIndex].item;
				Cells[index].quantity = Amount;

				IsSuccess = true;
			}
		}
	}
}

int32 UInventory::GetAmountAtIndex(int32 Index)
{
	return Cells[Index].quantity;
}

void UInventory::GetCells(TArray<FInvCell> &Result)
{
	TArray<FInvCell> CellsLocal;
	
	for (int i = 0; i < this->Cells.Num(); i++)
	{
		CellsLocal[i] = this->Cells[i];
	}

	Result = CellsLocal;
}