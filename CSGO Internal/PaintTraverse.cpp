#include "PaintTraverse.h"
#include "esp.h"

tPaintTraverse oPaintTraverse = 0;
void __stdcall hkPaintTraverse(void* panel, bool forcerepaint, bool allowforce)
{
	oPaintTraverse(I::IPanel, panel, forcerepaint, allowforce);
	if (!(panel == I::IEngineVGUI->GetPanel(PANEL_TOOLS) && I::IEngineTool->m_bIsInGame))
		return;

	int scrw, scrh;
	I::ISurface->GetScreenSize(&scrw, &scrh);

	ESP::scrw = scrw;
	ESP::scrh = scrh;

	int localIndex = I::IEngineClient->GetLocalPlayer();
	if (!localIndex) return;

	C_BasePlayer* localplayer = I::IEntityList->GetClientEntity(localIndex);
	if (!localplayer) return;

	player_info_t playerinfo;
	for (int i = 0; i < g_Globals->maxClients; i++)
	{
		C_BasePlayer* player = I::IEntityList->GetClientEntity(i);
		if (!player)
			continue;

		if (i == localIndex)
			continue;

		if (player->IsDormant())
			continue;

		if (player->GetHealth() <= 0)
			continue;

		if (!(I::IEngineClient->GetPlayerInfo(i, &playerinfo)))
			continue;

		ESP::DrawPlayer(player, localplayer, playerinfo);
	}
	
	ESP::DrawFOVCircle(localplayer);
}