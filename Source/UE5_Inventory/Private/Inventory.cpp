// Fill out your copyright notice in the Description page of Project Settings.

#include "UE5_Inventory/Public/Inventory.h"

/*--------------------------------------------- PUBLIC ---------------------------------------------------------------*/

void UInventory::AddItemNew(FItemBase Item, int32 Amount, bool& IsSuccess, int32& Rest, int32& CellIndex)
{
	int32 iFoundIndex = NULL;
	bool bIsSuccess;
	int32 iIndex;
	int32 iRest;

	if (Item.IsStackable)
	{
		SearchFreeStack(Item, bIsSuccess, iIndex);

		// if free stack was found
		if (bIsSuccess)
		{
			iFoundIndex = iIndex;

			// if overstack
			if (GetAmountAtIndex(iFoundIndex) + Amount > MaxStackSize)
			{
				int32 iAmountLocal = GetAmountAtIndex(iFoundIndex) + Amount - MaxStackSize;

				SetCell(iFoundIndex, Item, MaxStackSize);

				AddItemNew(Item, iAmountLocal, bIsSuccess, iRest, iFoundIndex);

				IsSuccess = true;
				Rest = iRest;
				CellIndex = iFoundIndex;
			}
			// if no overstack
			else
			{
				SetCell(iFoundIndex, Item, GetAmountAtIndex(iFoundIndex) + Amount);

				IsSuccess = true;
				Rest = 0;
				CellIndex = iFoundIndex;
			}
		}

		// if free stack was not found
		if (!bIsSuccess)
		{
			SearchEmptyCell(bIsSuccess, iIndex);

			// if empty cell was found
			if (bIsSuccess)
			{
				iFoundIndex = iIndex;

				if (Amount > MaxStackSize)
				{
					SetCell(iFoundIndex, Item, MaxStackSize);

					AddItemNew(Item, Amount - MaxStackSize, bIsSuccess, iRest, iFoundIndex);

					IsSuccess = true;
					Rest = iRest;
					CellIndex = iFoundIndex;
				}
				else
				{
					SetCell(iFoundIndex, Item, Amount);

					IsSuccess = true;
					Rest = 0;
					CellIndex = iFoundIndex;
				}
			}

			// if empty cell was not found
			if (!bIsSuccess)
			{
				IsSuccess = false;
				Rest = Amount;
				CellIndex = iFoundIndex;
			}
		}
	}

	if (!Item.IsStackable)
	{
		SearchEmptyCell(bIsSuccess, iIndex);

		if (bIsSuccess)
		{
			iFoundIndex = iIndex;

			SetCell(iFoundIndex, Item, 1);

			if (Amount > 1)
			{
				AddItemNew(Item, Amount - 1, bIsSuccess, iRest, iFoundIndex);

				IsSuccess = true;
				Rest = iRest;
				CellIndex = iFoundIndex;
			}
			else
			{
				IsSuccess = true;
				Rest = 0;
				CellIndex = iFoundIndex;
			}
		}

		if (!bIsSuccess)
		{
			IsSuccess = false;
			Rest = Amount;
			CellIndex = iFoundIndex;
		}
	}
}

void UInventory::RemoveItemAtIndex(int32 Index, int32 Amount, bool& IsSuccess)
{
	IsSuccess = false;

	if (!IsCellEmpty(Index))
	{
		if (Amount >= GetAmountAtIndex(Index))
		{
			Cells[Index].Item.ID = NULL;
			Cells[Index].Item.Name = nullptr;
			Cells[Index].Item.Description = nullptr;
			Cells[Index].Item.IsStackable = nullptr;
			Cells[Index].Item.Icon = nullptr;

			Cells[Index].Amount = NULL;
		}
		else
		{
			Cells[Index].Amount -= Amount;
		}

		OnCellUpdated.Broadcast(Index);
		
		IsSuccess = true;
	}
}

bool UInventory::IsCellEmpty(int32 CellIndex)
{
	if (GetAmountAtIndex(CellIndex) == 0)
	{
		return true;
	}

	return false;
}

void UInventory::SearchEmptyCell(bool& IsSuccess, int32& Index)
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

void UInventory::SearchFreeStack(FItemBase Item, bool& IsSuccess, int32& Index)
{
	IsSuccess = false;

	for (int i = 0; i < Cells.Num(); i++)
	{
		if (!IsCellEmpty(i))
		{
			// if given item has the same type with founded item,
			// and amount of those items lower than maximum size of stack
			if (Cells[i].Item.ID == Item.ID && GetAmountAtIndex(i) < MaxStackSize)
			{
				IsSuccess = true;
				Index = i;
				break;
			}
		}
	}
}

void UInventory::SwapCells(int32 Index1, int32 Index2, bool& IsSuccess)
{
	IsSuccess = false;

	// check if indices are not out of bounds
	if (Index1 <= Cells.Num() - 1 || Index2 <= Cells.Num() - 1)
	{
		FInvCell sCell = Cells[Index1];

		Cells[Index1] = Cells[Index2];
		Cells[Index2] = sCell;

		OnCellUpdated.Broadcast(Index1);
		OnCellUpdated.Broadcast(Index2);
		
		IsSuccess = true;
	}
}

void UInventory::SplitStack(int32 StackIndex, int32 Amount, bool& IsSuccess)
{
	IsSuccess = false;

	if (!IsCellEmpty(StackIndex))
	{
		if (Cells[StackIndex].Item.IsStackable && Cells[StackIndex].Amount > Amount)
		{
			bool bIsSuccess;
			int iIndex;

			SearchEmptyCell(bIsSuccess, iIndex);

			if (bIsSuccess)
			{
				SetCell(StackIndex, GetAmountAtIndex(StackIndex) - Amount);
				SetCell(iIndex, Cells[StackIndex].Item, Amount);

				OnCellUpdated.Broadcast(StackIndex);
				
				IsSuccess = true;
			}
		}
	}
}

void UInventory::AddToIndex(int32 FromIndex, int32 ToIndex, bool& IsSuccess)
{
	IsSuccess = false;

	if (Cells[FromIndex].Item.ID == Cells[ToIndex].Item.ID && GetAmountAtIndex(ToIndex) < MaxStackSize
		&& Cells[FromIndex].Item.IsStackable)
	{
		if (MaxStackSize - GetAmountAtIndex(ToIndex) >= GetAmountAtIndex(FromIndex))
		{
			bool bIsSuccess;

			SetCell(ToIndex, Cells[FromIndex].Item, GetAmountAtIndex(FromIndex) + GetAmountAtIndex(ToIndex));
			RemoveItemAtIndex(FromIndex, GetAmountAtIndex(FromIndex), bIsSuccess);
		}
		else
		{
			SetCell(FromIndex, GetAmountAtIndex(FromIndex) - (MaxStackSize - GetAmountAtIndex(ToIndex)));
			SetCell(ToIndex, Cells[FromIndex].Item, MaxStackSize);
		}

		OnCellUpdated.Broadcast(FromIndex);
		OnCellUpdated.Broadcast(ToIndex);

		IsSuccess = true;
	}
}

void UInventory::SplitStackToIndex(int32 FromIndex, int32 ToIndex, int32 Amount, bool& IsSuccess)
{
	IsSuccess = false;

	if (IsCellEmpty(ToIndex) && !IsCellEmpty(FromIndex) && Cells[FromIndex].Item.IsStackable
		&& GetAmountAtIndex(FromIndex) > 1 && GetAmountAtIndex(FromIndex) > Amount)
	{
		SetCell(FromIndex, GetAmountAtIndex(FromIndex) - Amount);
		SetCell(ToIndex, Cells[FromIndex].Item, Amount);

		OnCellUpdated.Broadcast(FromIndex);
		OnCellUpdated.Broadcast(ToIndex);
	}
}

void UInventory::GetItemAtIndex(int32 Index, bool& IsCellEmpty, FItemBase& Item, int32& Amount)
{
	// check if a cell by this index is empty
	IsCellEmpty = UInventory::IsCellEmpty(Index);

	// if not empty...
	if (!IsCellEmpty)
	{
		Item = Cells[Index].Item;
		Amount = Cells[Index].Amount;
	}
}

void UInventory::SetAmountOfCells(int32 NewValue)
{
	AmountOfCells = NewValue;
}

int32 UInventory::GetAmountOfCells()
{
	return AmountOfCells;
}

int32 UInventory::GetAmountAtIndex(int32 Index)
{
	return Cells[Index].Amount;
}

void UInventory::GetCells(TArray<FInvCell>& Result)
{
	Result = Cells;
}

/*--------------------------------------------- PRIVATE --------------------------------------------------------------*/

void UInventory::SetCell(int32 Index, FItemBase Item, int32 Amount)
{
	Cells[Index].Item = Item;
	Cells[Index].Amount = Amount;

	OnCellUpdated.Broadcast(Index);
}

void UInventory::SetCell(int32 Index, FItemBase Item)
{
	Cells[Index].Item = Item;

	OnCellUpdated.Broadcast(Index);
}

void UInventory::SetCell(int32 Index, int32 Amount)
{
	Cells[Index].Amount = Amount;

	OnCellUpdated.Broadcast(Index);
}
