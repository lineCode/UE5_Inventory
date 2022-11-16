// Fill out your copyright notice in the Description page of Project Settings.


// #include "Inventory.h"
#include "UE5_Inventory/Public/Inventory.h"

void UInventory::AddItem(FItemBase item)
{
	if (auto const FoundedCell = this->FindCellByItem(item); FoundedCell != nullptr)
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
		
		NewCell.item = item;
		// NewCell.item_states.Push(state);
		NewCell.quantity = 1;

		const auto CellPtr = MakeShared<FInvCell>(NewCell);
		
		this->Cells.Push(CellPtr);
	}
}

void UInventory::RemoveItem(FItemBase item)
{
	// Try to find an item in the inventory
	if (auto const FoundedCell = this->FindCellByItem(item); FoundedCell != nullptr)
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

TSharedPtr<FInvCell> UInventory::FindCellByItem(const FItemBase& Item)
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