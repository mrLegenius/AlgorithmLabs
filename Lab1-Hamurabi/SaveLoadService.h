#pragma once

#include "YearProcessor.h"
#include "Resources.h"
#include "OrderProcessor.h"

bool TrySave(int& year, float& averageStarvedPercent, YearInfo& info, Resources& resources);
void TryLoad(int& year, float& averageStarvedPercent, YearInfo& info, Resources& resources);

