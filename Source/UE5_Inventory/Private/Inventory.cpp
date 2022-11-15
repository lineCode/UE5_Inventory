// Fill out your copyright notice in the Description page of Project Settings.


// #include "Inventory.h"
#include "UE5_Inventory/Public/Inventory.h"

void UInventory::AddItem(FItemBase item)
{
	// If inventory already contains item of the same type, add it to the corresponding cell
	if (auto const FoundedCell = this->FindCellByItem(item); FoundedCell != nullptr)
	{
		//TODO: mock push to states
		// FoundedCell->item_states.Push(state);
		
		FoundedCell->quantity += 1;
	}

	// If it is not exists, then create new cell and add an item
	else
	{
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
	// Try to find ad item in the inventory
	if (auto const FoundedCell = this->FindCellByItem(item); FoundedCell != nullptr)
	{
		// Reduce quantity by one if there are already multiple items of the same type
		if (FoundedCell->quantity > 1)
		{
			FoundedCell->quantity -= 1;
		}
		
		// Remove cell if is is the only one item of corresponding type
		else
		{
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