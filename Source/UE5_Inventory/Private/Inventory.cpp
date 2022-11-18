// Fill out your copyright notice in the Description page of Project Settings.


// #include "Inventory.h"
#include "UE5_Inventory/Public/Inventory.h"

#include "IPropertyTable.h"

void UInventory::AddItem(FItemBase Item)
{
	if (auto const FoundedCell = this->FindCellByItem(Item); FoundedCell != nullptr)
	{
		// If inventory already contains items of the same type, increase quantity in the corresponding cell

		//TODO: Create a system for binding the corresponding item state to the item
		// FoundedCell->item_states.Push(state);

		FoundedCell->quantity += 1;
	}
	else
	{
		// If it isn't exists, then create new cell and add an item
		FInvCell NewCell;

		NewCell.item = Item;
		// NewCell.item_states.Push(state);
		NewCell.quantity = 1;

		const auto CellPtr = MakeShared<FInvCell>(NewCell);

		this->Cells.Push(CellPtr);
	}
}

void UInventory::RemoveItem(FItemBase Item)
{
	// Try to find an item in the inventory
	if (auto const FoundedCell = this->FindCellByItem(Item); FoundedCell != nullptr)
	{
		if (FoundedCell->quantity > 1)
		{
			// Reduce quantity by one if there are already multiple items of the same type
			FoundedCell->quantity -= 1;
		}
		else
		{
			// Remove cell if it contains only the one item of corresponding type
			this->Cells.Remove(FoundedCell);
		}
	}
}

void UInventory::SwapItems(FItemBase ls_item, FItemBase Rs_Item)
{
}

TSharedPtr<FInvCell> UInventory::FindCellByItem(const FItemBase &Item)
{
	for (auto Cell : this->Cells)
	{
		if (Cell->item.id == Item.id)
		{
			return Cell;
		}
	}
	return nullptr;
}

// FItemBase UInventory::GetItem(int32 Item_ID)
// {
// }

void UInventory::PrintInventory()
{
	UE_LOG(LogTemp, Log, TEXT("Number Of Cells: %d"), Cells.Num());
	UE_LOG(LogTemp, Log, TEXT("\n\n"));

	for (auto InvCell : Cells)
	{
		UE_LOG(LogTemp, Log, TEXT("ID: %d"), InvCell->item.id);
		UE_LOG(LogTemp, Log, TEXT("Name: %s"), *(InvCell->item.name));
		UE_LOG(LogTemp, Log, TEXT("Description: %s"), *(InvCell->item.description));
		UE_LOG(LogTemp, Log, TEXT("Quantity: %d"), InvCell->quantity);
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

bool UInventory::IsCellEmpty(FInvCell Cell)
{
	if (Cell.quantity == 0)
	{
		return true;
	}

	return false;
}

void UInventory::GetItemAtIndex(int32 index, bool &IsCellEmpty, FItemBase &Item, int32 &Amount)
{
	// check if a cell by this index is empty
	IsCellEmpty = UInventory::IsCellEmpty(*this->Cells[index].Get());
	// IsCellEmpty = false;

	// if not empty...
	if (!IsCellEmpty)
	{
		Item = this->Cells[index].Get()->item;
		Amount = this->Cells[index].Get()->quantity;
	}
}

void UInventory::SearchEmptyCell(bool &IsSuccess, int32 &Index)
{
	IsSuccess = false;

	for (int i = 0; i < Cells.Num(); i++)
	{
		if (IsCellEmpty(*Cells[i]))
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
		if (!IsCellEmpty(*Cells[i]))
		{
			// if given item has the same type with founded item, and amount of those items lower than maximum size of stack
			if (Cells[i].Get()->item.id == Item.id && Cells[i].Get()->quantity < MaxStackSize)
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
			if (Cells[foundIndex].Get()->quantity + Amount > MaxStackSize)
			{
				Cells[foundIndex].Get()->item = Item;
				Cells[foundIndex].Get()->quantity = MaxStackSize;

				AddItemNew(Item, Cells[foundIndex].Get()->quantity + Amount - MaxStackSize, isSuccess, rest, foundIndex);

				// return values
				IsSuccess = true;
				Rest = rest;
				CellIndex = foundIndex;
			}

			// if no overstack
			else
			{
				Cells[foundIndex].Get()->item = Item;
				Cells[foundIndex].Get()->quantity = Cells[foundIndex].Get()->quantity + Amount;
				
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
					Cells[foundIndex].Get()->item = Item;
					Cells[foundIndex].Get()->quantity = MaxStackSize;
					
					AddItemNew(Item, Amount - MaxStackSize, isSuccess, rest, foundIndex);

					IsSuccess = true;
					Rest = rest;
					CellIndex = foundIndex;
				}

				else
				{
					Cells[foundIndex].Get()->item = Item;
					Cells[foundIndex].Get()->quantity = Amount;

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

			Cells[foundIndex].Get()->item = Item;
			Cells[foundIndex].Get()->quantity = 1;

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

int32 UInventory::GetAmountAtIndex(int32 Index)
{
	return Cells[Index].Get()->quantity;
}
