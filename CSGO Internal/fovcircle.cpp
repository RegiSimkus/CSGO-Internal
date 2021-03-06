#pragma once
#include "esp.h"

void ESP::DrawFOVCircle(C_BasePlayer* localplayer)
{
	if (Settings::Aim::FOV == 0.f || !Settings::Aim::Enabled)
		return;

	if (!localplayer) return;

	float radius = GetFOVRadius(Settings::Aim::FOV, localplayer);

	// Validating FOVLength in case the FOV was set while the localplayer was null, such as in the main menu
	if (Settings::Aim::FOVLength == 0.f && Settings::Aim::FOV != 0.f)
		Settings::Aim::FOVLength = radius * radius;

	DrawCircle(scrw / 2, scrh / 2, radius, ImColor_White);
}