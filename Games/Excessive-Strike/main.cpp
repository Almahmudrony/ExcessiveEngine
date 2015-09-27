// Mai feladat : Gondolkodni kene Core feldarabolasan, Physics, Sound, Graphics, Network
// A feloszt�s m�k�d�k�pes ha individu�lis modul r�szeket �lelnek fel a felosztott r�szek
// De mi van olyan funckionalit�ssal amihez pl graphicsEngine meg phyicsEngine is kell, ez egy�rtelm�en a Core dolga
// N�zz�k meg, hogy n�zn�nek ki ezek az oszt�lyok

// Input
// Network
// Graphics
// Physics
// Sound
// World
// Core Amikor Core p�ld�nyosul, akkor a t�bbi is
// Core p�ld�nyos�t�s�t nem vethetj�k el editoron bel�l, de j�t�kon bel�l lesz egy Core

// Egyszer� eset:
// Akarok egy GuiImage : GuiControl a k�perny� k�zep�re amin megjelenik az alma.jpg

// Editorbol ?? "New Gui Page(mainMenu)", lepakolom a control - t, majd dragelek alma.jpg oda
// K�db�l be�ll�tom a GuiLayer - t mainMenu - re
// Canvas
// - GuiPages
//		- GuiLayers
//			- GuiControls

// A GuiLibrary felel�ss�ge
// - Megjelen�t�shez sz�ks�ges absztrakci� hordoz�sa
// Vannak layerek, pl inventory, hp and mana
// Van layer, pl MainMenu
// Layerek sokas�g�t GuiPage - nek h�vjuk
// Minden stackelhet�, <GuiPage>, <GuiLayer>, <GuiControl>

// Ne bonyol�tsuk t�l. Pl van egy inventory nyitva level 0 - n�l
// level1 - re pusholok egy teljes mainMenu - t

// M�sik eset, az editoron bel�l bizonyos viewport r�szre akarom renderelni az eg�sz vil�got
// gui lib renderel�s lefut azt�n cs�kolom, ezut�n Core - n kereszt�l be�ll�tom hogy az ablak mely r�sz�re k�ne renderelni a GuiControl alapj�n, majd megh�vom az update - t
// Ezt mind az editor mainLoop - j�ban

// Core eddig hangot, .dae stb f�jlokat tud z�zni

#include "Core\EngineCore.h"
#include "Core\CameraComponent.h"
#include "PlatformLibrary\Window.h"
#include "PlatformLibrary\Timer.h"
#include "SupportLibrary\VisualCpuProfiler.h"
#include "PlayerScript.h"
#include "TestLevelScript.h"
#include "Core\InputCore.h"
#include "PlatformLibrary\File.h"
#include "Core\WorldCore.h"

void InitScript();

int main()
{
	// Full screen popup window for our game
	rWindow d;
		d.clientW = Sys::GetScreenSize().x; //FULL SCREEN props
		d.clientH = Sys::GetScreenSize().y; //FULL SCREEN props
		d.style = eWindowStyle::FULLSCREEN; //FULL SCREEN props
	Window* window = new Window(d);

	// Hide hardware cursor for our game on window
	window->SetCursorVisible(false);

	// Init Engine core
	//EngineCore::InstantiateSingleton();
	Core.InitSoundEngineSFML();
	rGraphicsEngineRaster graphicsDesc;
		graphicsDesc.gapiType = eGapiType::OPENGL_4_5;
		graphicsDesc.targetWindow = window;
	Core.InitGraphicsEngineRaster(graphicsDesc);
	rPhysicsEngineBullet physicsDesc;
		physicsDesc.gravity = mm::vec3(0, 0, -9.81f);
	Core.InitPhysicsEngineBullet(physicsDesc);	

	InitScript(); // Manual bullshit, TODO !!!

	Timer* timer = new Timer();
	timer->Start();

	while (window->IsOpen())
	{
		// Prepare for input processing
		Input.ClearFrameData();

		// Process input events coming from O.S.-> Window
		rWindowEvent evt;
		while(window->PopEvent(evt))
		{
			switch(evt.msg)
			{
			case eWindowMsg::KEY_PRESS:
				if (evt.key != eKey::INVALID)
					Input.KeyPress(evt.key);
				break;

			case eWindowMsg::KEY_RELEASE:
				if (evt.key != eKey::INVALID)
					Input.KeyRelease(evt.key);
				break;

			case eWindowMsg::MOUSE_MOVE:
			{
				assert(evt.x >= 0 && evt.y >= 0);
				Input.MouseMove(mm::ivec2(evt.deltaX, evt.deltaY), mm::uvec2((u32)evt.x, (u32)evt.y));
				break;
			}

			case eWindowMsg::MOUSE_PRESS:
				switch (evt.mouseBtn)
				{
				case eMouseBtn::LEFT:  Input.MouseLeftPress();  break;
				case eMouseBtn::MID:   Input.MouseMidPress();	 break;
				case eMouseBtn::RIGHT: Input.MouseRightPress(); break;
				}
				break;

			case eWindowMsg::MOUSE_RELEASE:
				switch (evt.mouseBtn)
				{
				case eMouseBtn::LEFT:  Input.MouseLeftRelease();  break;
				case eMouseBtn::MID:   Input.MouseMidRelease();   break;
				case eMouseBtn::RIGHT: Input.MouseRightRelease(); break;
				}
				break;
			}
		}

		Input.Update();

		float deltaSeconds = timer->GetSecondsPassed();
		timer->Reset();

		Core.Update(deltaSeconds);
	}

	return 0;
}

void InitScript()
{
	World.AddScript<TestLevelScript>();
}