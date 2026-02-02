#include "OverheadWidget.h"

#include "Components/TextBlock.h"

void UOverheadWidget::UpdateHPText(float Health, float MaxHealth)
{
	if (OverHeadHp)
	{
		OverHeadHp->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}
