#include "global.h"
#define WINDOWNAME xorstr_(L"Fortnite  ")

bool verify_game()
{
	m_driver_control = kernel_control_function();

	if (!m_driver_control)
		return false;

	m_pid = PIDManager::GetAowProcId();

	if (!m_pid)
		return false;


	usermode_pid = GetCurrentProcessId();

	printf(xorstr_("> usermode_pid: %d\n"), usermode_pid);

	m_base = GetBaseAddress();

	if (!m_base)
		return false;

	printf(xorstr_("> m_pid: %d base: %llx\n"), m_pid, m_base);

	ClearPiddb();
	FindMmDriverData();
	ClearMmUnloadedDrivers();

	return true;
}
static const char consoleNameAlphanum[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
int consoleNameLength = sizeof(consoleNameAlphanum) - 1;
char genRandomConsoleName()
{
	return consoleNameAlphanum[rand() % consoleNameLength];
}
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	create_console();
	HWND ConsWind = GetConsoleWindow();
	srand(time(0));
	std::wstring ConsoleNameStr;
	for (unsigned int i = 0; i < 20; ++i)
	{
		ConsoleNameStr += genRandomConsoleName();

	}
	SetConsoleTitle(ConsoleNameStr.c_str());
	while (TRUE)
	{
		SetupWindow();
		DirectXInit(MyWnd);
		verify_game();
		MainLoop();
	}

	return 0;
}
void create_console()
{
	if (!AllocConsole())
	{
		char buffer[1024] = { 0 };
		sprintf_s(buffer, xorstr_("Failed to AllocConsole( ), GetLastError( ) = %d"), GetLastError());
		MessageBox(HWND_DESKTOP, LPCWSTR(buffer), L"Error", MB_OK);

		return;
	}

	auto lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	auto hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	auto fp = _fdopen(hConHandle, xorstr_("w"));

	freopen_s(&fp, xorstr_("CONOUT$"), xorstr_("w"), stdout);

	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
}

void AIms(DWORD_PTR entity,Vector Localcam)
{
	float max = 100.f;

	uint64_t currentactormesh = READ64(entity + Mesh);

	Vector rootHead = GetBoneWithRotation(currentactormesh, 66);
	Vector rootHeadOut = ProjectWorldToScreen(rootHead, Vector(Localcam.y, Localcam.x, Localcam.z));

	if (GetClosestPlayerToCrossHair(rootHeadOut, max, AimFOV, entity))
		entityx = entity;
}
void drawLoop() {

	float radiusx = AimFOV * (ScreenCenterX / 100);
	float radiusy = AimFOV * (ScreenCenterY / 100);

	float calcradius = (radiusx + radiusy) / 2;
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(ScreenCenterX, ScreenCenterY), calcradius, 0xFFFFFFFF, 25);

	float radarWidth = 230;
	float PosDx = 1030;
	float PosDy = 150;

	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(PosDx, PosDy), ImVec2(PosDx + radarWidth, PosDy + radarWidth), IM_COL32(255, 255, 255, 130), 6.0f);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(PosDx + (radarWidth / 2), PosDy + (radarWidth / 2)), ImVec2(PosDx, PosDy), IM_COL32(0, 0, 0, 90));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(PosDx + (radarWidth / 2), PosDy + (radarWidth / 2)), ImVec2(PosDx + radarWidth, PosDy), IM_COL32(0, 0, 0, 90));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(PosDx + (radarWidth / 2), PosDy + (radarWidth / 2)), ImVec2(PosDx + (radarWidth / 2), PosDy + radarWidth), IM_COL32(0, 0, 0, 90));

	Uworld = READ64(m_base + UWorld);
	DWORD_PTR Gameinstance = READ64(Uworld + GInstance);

	if (Gameinstance == (DWORD_PTR)nullptr)
		return;

	DWORD_PTR LocalPlayers = READ64(Gameinstance + UlocalPlayer);

	if (LocalPlayers == (DWORD_PTR)nullptr)
		return;

	Localplayer = READ64(LocalPlayers);

	if (Localplayer == (DWORD_PTR)nullptr)
		return;

	PlayerController = READ64(Localplayer + playerController);

	if (PlayerController == (DWORD_PTR)nullptr)
		return;

	LocalPawn = READ64(PlayerController + localp4wn);

	if (LocalPawn == (DWORD_PTR)nullptr)
		return;

	Rootcomp = READ64(LocalPawn + rootComp);

	if (Rootcomp == (DWORD_PTR)nullptr)
		return;

	if (LocalPawn != 0) {
		localplayerID = ReadInteger(LocalPawn + ObjectID);
	}

	Ulevel = READ64(Uworld + UlevelX);

	if (Ulevel == (DWORD_PTR)nullptr)
		return;

	DWORD ActorCount = READ64(Ulevel + actorCount);

	DWORD_PTR AActors = READ64(Ulevel + actorArray);

	DWORD64 PlayerState = READ64(LocalPawn + PlayerStateX);

	if (AActors == (DWORD_PTR)nullptr)
		return;

	Vector Localcam = Camera(Rootcomp);

	int revise = 0;
	int radius = 0;
	float LowestDistance = FLT_MAX;
	DWORD64 toApuntar;
	for (int i = 0; i < ActorCount; i++)
	{
		if (ActorCount > 1)
		{
			uint64_t CurrentActor = READ64(AActors + i * 0x8);

			if (CurrentActor == (uint64_t)nullptr || CurrentActor == -1 || CurrentActor == NULL)
				continue;

			int curactorid = ReadInteger(CurrentActor + ObjectID);

			if (curactorid == localplayerID || curactorid == BOT_ACTORID)
			{
				uint64_t CurrentActorRootComponent = READ64(CurrentActor + rootComp);

				if (CurrentActorRootComponent == (uint64_t)nullptr || CurrentActorRootComponent == -1 || CurrentActorRootComponent == NULL)
					continue;

				uint64_t currentactormesh = READ64(CurrentActor + Mesh);

				if (currentactormesh == (uint64_t)nullptr || currentactormesh == -1 || currentactormesh == NULL)
					continue;

				Vector Headpos = GetBoneWithRotation(currentactormesh, 66);
				Vector rootOut = GetBoneWithRotation(currentactormesh, 0);
				Vector Localcam = Camera(Rootcomp);
				Vector Localpos = READV(Rootcomp + rootPos);

				int MyTeamId = ReadInteger(PlayerState + TeamID);
				DWORD64 otherPlayerState = READ64(CurrentActor + PlayerStateX);
				int ActorTeamId = ReadInteger(otherPlayerState + TeamID);


				float distance = Localpos.Distance(Headpos) / 100.f;

				if (distance < 1.5f)
					continue;

				Vector vHeadBone = GetBoneWithRotation(currentactormesh, 96);
				Vector vHip = GetBoneWithRotation(currentactormesh, 2);
				Vector vNeck = GetBoneWithRotation(currentactormesh, 65);
				Vector vUpperArmLeft = GetBoneWithRotation(currentactormesh, 34);
				Vector vUpperArmRight = GetBoneWithRotation(currentactormesh, 91);
				Vector vLeftHand = GetBoneWithRotation(currentactormesh, 35);
				Vector vRightHand = GetBoneWithRotation(currentactormesh, 63);
				Vector vLeftHand1 = GetBoneWithRotation(currentactormesh, 33);
				Vector vRightHand1 = GetBoneWithRotation(currentactormesh, 60);
				Vector vRightThigh = GetBoneWithRotation(currentactormesh, 74);
				Vector vLeftThigh = GetBoneWithRotation(currentactormesh, 67);
				Vector vRightCalf = GetBoneWithRotation(currentactormesh, 75);
				Vector vLeftCalf = GetBoneWithRotation(currentactormesh, 68);
				Vector vLeftFoot = GetBoneWithRotation(currentactormesh, 69);
				Vector vRightFoot = GetBoneWithRotation(currentactormesh, 76);

				Vector vHeadBoneOut = ProjectWorldToScreen(vHeadBone, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vHipOut = ProjectWorldToScreen(vHip, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vNeckOut = ProjectWorldToScreen(vNeck, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vUpperArmLeftOut = ProjectWorldToScreen(vUpperArmLeft, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vUpperArmRightOut = ProjectWorldToScreen(vUpperArmRight, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vLeftHandOut = ProjectWorldToScreen(vLeftHand, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vRightHandOut = ProjectWorldToScreen(vRightHand, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vLeftHandOut1 = ProjectWorldToScreen(vLeftHand1, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vRightHandOut1 = ProjectWorldToScreen(vRightHand1, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vRightThighOut = ProjectWorldToScreen(vRightThigh, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vLeftThighOut = ProjectWorldToScreen(vLeftThigh, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vRightCalfOut = ProjectWorldToScreen(vRightCalf, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vLeftCalfOut = ProjectWorldToScreen(vLeftCalf, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vLeftFootOut = ProjectWorldToScreen(vLeftFoot, Vector(Localcam.y, Localcam.x, Localcam.z));
				Vector vRightFootOut = ProjectWorldToScreen(vRightFoot, Vector(Localcam.y, Localcam.x, Localcam.z));

				if (MyTeamId != ActorTeamId)
				{

					Vector HeadposW2s = ProjectWorldToScreen(Headpos, Vector(Localcam.y, Localcam.x, Localcam.z));
					Vector Out = ProjectWorldToScreen(rootOut, Vector(Localcam.y, Localcam.x, Localcam.z));

					float boxsize = (float)(Out.y - HeadposW2s.y);
					float boxwidth = boxsize / 3.0f;

					float dwpleftx = (float)Out.x - boxwidth / 2.0f;
					float dwplefty = (float)Out.y;

					ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(dwpleftx, dwplefty), ImVec2(HeadposW2s.x + boxwidth, HeadposW2s.y + 5.0f), IM_COL32(255, 255, 255, 50));
					ImGui::GetOverlayDrawList()->AddRect(ImVec2(dwpleftx, dwplefty), ImVec2(HeadposW2s.x + boxwidth, HeadposW2s.y + 5.0f), IM_COL32(128, 224, 0, 200));

					char cName[64];
					if (ActorTeamId == -1)
					{
						sprintf_s(cName, xorstr_(" [ BOT ] %dm "), (int)distance);
					}
					else
					{
						sprintf_s(cName, xorstr_(" [%.2d] EnemyType [ Player ] %dm "), ActorTeamId, (int)distance);
					}
					revise = strlen(cName) * 7 + 28;

					DrawTriangleFilled(Out.x + (Rect.w / 2) - 5, Out.y + Out.z + 25, Out.x + (Rect.w / 2), Out.y + Out.z + 20, Out.x + (Rect.w / 2) + 5, Out.y + Out.z + 25, &Col.blue_);
					DrawFilledRect(Out.x - (revise / 2) + (Rect.w / 2) - 1, Out.y + Out.z + 25, revise + 1, 3, &Col.blue_);
					DrawFilledRect(Out.x - (revise / 2) + (Rect.w / 2) - 1, Out.y + Out.z + 28, revise + 1, 16, &Col.blue_);

					int textX = (int)(Out.x - (revise / 2) + (Rect.w / 2) + 21);
					DrawNewText(textX, Out.y + Out.z + 27, &Col.white_, cName);

					//

					if (distance > 0) radius = 50 / distance;
					DrawCircle(HeadposW2s.x, HeadposW2s.y - radius / 2, radius, &Col.white_, radius * 2);

					DrawLine(vHipOut.x, vHipOut.y, vNeckOut.x, vNeckOut.y, &Col.red_, 1.5);

					DrawLine(vUpperArmLeftOut.x, vUpperArmLeftOut.y, vNeckOut.x, vNeckOut.y, &Col.red_, 1.5);
					DrawLine(vUpperArmRightOut.x, vUpperArmRightOut.y, vNeckOut.x, vNeckOut.y, &Col.red_, 1.5);

					DrawLine(vLeftHandOut.x, vLeftHandOut.y, vUpperArmLeftOut.x, vUpperArmLeftOut.y, &Col.red_, 1.5);
					DrawLine(vRightHandOut.x, vRightHandOut.y, vUpperArmRightOut.x, vUpperArmRightOut.y, &Col.red_, 1.5);

					DrawLine(vLeftHandOut.x, vLeftHandOut.y, vLeftHandOut1.x, vLeftHandOut1.y, &Col.red_, 1.5);
					DrawLine(vRightHandOut.x, vRightHandOut.y, vRightHandOut1.x, vRightHandOut1.y, &Col.red_, 1.5);

					DrawLine(vLeftThighOut.x, vLeftThighOut.y, vHipOut.x, vHipOut.y, &Col.red_, 1.5);
					DrawLine(vRightThighOut.x, vRightThighOut.y, vHipOut.x, vHipOut.y, &Col.red_, 1.5);

					DrawLine(vLeftCalfOut.x, vLeftCalfOut.y, vLeftThighOut.x, vLeftThighOut.y, &Col.red_, 1.5);
					DrawLine(vRightCalfOut.x, vRightCalfOut.y, vRightThighOut.x, vRightThighOut.y, &Col.red_, 1.5);

					DrawLine(vLeftFootOut.x, vLeftFootOut.y, vLeftCalfOut.x, vLeftCalfOut.y, &Col.red_, 1.5);
					DrawLine(vRightFootOut.x, vRightFootOut.y, vRightCalfOut.x, vRightCalfOut.y, &Col.red_, 1.5);

					float radarWidth = 230;
					float PosDx = 1030;
					float PosDy = 150;

					Vector Coord;
					Vector rootPos, headPos;

					FLOAT fYaw = PlayerController + Yaw;

					FLOAT cY = cos(fYaw * (UCONST_Pi / 180.0f));
					FLOAT sY = sin(fYaw * (UCONST_Pi / 180.0f));

					FLOAT dX = rootPos.x - rootOut.x;
					FLOAT dY = rootPos.y - rootOut.y;

					Coord.x = (dY * cY - dX * sY) / 250.0f;
					Coord.y = (dX * cY + dY * sY) / 250.0f;

					Vector RetRadar;

					RetRadar.x = Coord.x + PosDx + (radarWidth / 2.0f);
					RetRadar.y = -Coord.y + PosDy + (radarWidth / 2.0f);

					if (RetRadar.x > (PosDx + radarWidth))
						RetRadar.x = (PosDx + radarWidth);
					else if (RetRadar.x < (PosDx))
						RetRadar.x = PosDx;

					if (RetRadar.y > (PosDy + radarWidth))
						RetRadar.y = (PosDy + radarWidth);
					else if (RetRadar.y < (PosDy))
						RetRadar.y = PosDy;

					DrawCircleFilled(RetRadar.x, RetRadar.y, 4.0f, &Col.red_);

					if (distance > 2.f)
					{
						if (LowestDistance > distance)
						{
							AIms(CurrentActor, Localcam);
						}
					}
				}
			}
		}
	}
	Aimbot(Localcam);
}

HRESULT DirectXInit(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
	{
		p_Object->Release();
		exit(4);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(p_Device);

	ImGui::StyleColorsClassic();
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	style->WindowTitleAlign.x = 0.50f;
	style->FrameRounding = 2.0f;

	p_Object->Release();
	return S_OK;
}
void SetupWindow()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);

	WNDCLASSEX wClass =
	{
		sizeof(WNDCLASSEX),
		0,
		WinProc,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		M_Name,
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	if (!RegisterClassEx(&wClass))
		exit(1);

	GameWnd = FindWindow(0, WINDOWNAME);
	if (GameWnd)
	{
		GetClientRect(GameWnd, &GameRect);
		POINT xy;
		ClientToScreen(GameWnd, &xy);
		GameRect.left = xy.x;
		GameRect.top = xy.y;

		Width = GameRect.right;
		Height = GameRect.bottom;
	}
	else exit(2);

	MyWnd = CreateWindowEx(NULL, M_Name, M_Name, WS_POPUP | WS_VISIBLE, GameRect.left, GameRect.top, Width, Height, NULL, NULL, 0, NULL);
	DwmExtendFrameIntoClientArea(MyWnd, &Margin);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);

	ShowWindow(MyWnd, SW_SHOW);
	UpdateWindow(MyWnd);

}
void render() {

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	drawLoop();

	ImGui::EndFrame();
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (p_Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		p_Device->EndScene();
	}
	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		p_Device->Reset(&p_Params);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}
WPARAM MainLoop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();
		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		if (hwnd_active == GameWnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;
		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{

			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		render();
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	CleanuoD3D();
	DestroyWindow(MyWnd);

	return Message.wParam;
}
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		CleanuoD3D();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (p_Device != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_Params.BackBufferWidth = LOWORD(lParam);
			p_Params.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = p_Device->Reset(&p_Params);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}
void CleanuoD3D()
{
	if (p_Device != NULL)
	{
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL)
	{
		p_Object->Release();
	}
}
int isTopwin()
{
	HWND hWnd = GetForegroundWindow();
	if (hWnd == GameWnd)
		return TopWindowGame;
	if (hWnd == MyWnd)
		return TopWindowMvoe;

	return 0;
}
void SetWindowToTarget()
{
	while (true)
	{
		GameWnd = FindWindow(0, WINDOWNAME);
		if (GameWnd)
		{
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(GameWnd, &GameRect);
			Width = GameRect.right - GameRect.left;
			Height = GameRect.bottom - GameRect.top;
			DWORD dwStyle = GetWindowLong(GameWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(MyWnd, GameRect.left, GameRect.top, Width, Height, true);
		}
	}
}