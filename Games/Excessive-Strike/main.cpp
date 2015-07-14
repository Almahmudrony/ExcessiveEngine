// Nagyon fontos !! 
// Ha Behavior mut�l Script nem felt�tlen !!
// Ha Script mut�l, Behavior nem felt�tlen


// Csin�lj egy olyan unit test - et, hogy az al�bbit k�nyelmesen meglehessen csin�lni !
// Adott Thing - ek
// Thing0 Thing1 Thing2 Thing3 Thing4
// Actor -> g�mb grafika
// Behavior -> mozg�s balra jobbra

// Gomb nyom�sra Thing1 �s Thing2 behaviorja al� tartoz� scripteket m�dos�tani akarom hogy nagyobb mozg�st v�gezzen, de ezt csak Thing1 �s Thing2 - re
// Sz�val azt csin�lom hogy m�solok egy �j behaviort be�ll�tom mindkett�nek, majd a behavior script - j�t m�dos�tom �gy Thing1 �s Thing2 gyorsabban fog mozogni
//


// Elm�let
//// Mib�l elhet Thing - et l�trehozni?
//// ActorBehavior, Actor, WorldComponent, ActorScript  // ezek k�z�l csak ActorScript lehet templateType
//
//// Editor oldalon �sszepakolok egy Thing - et, amiben van csom� komponens, �s r�aggatok n�h�ny script - et
//// Thing* myType ....
//
//Actor* constructedActor = gCore.AddActor();
//// Sok komponens hozz� csatol�sa....
//ActorBehavior* behavior = gCore.AddBehavior();
//// Script be�ll�t�sa
//
//// Elkezd�dik a level ->
//Thing* thingType = gCore.CreateThing();
//thingType->SetActor(constructedActor);
//thingType->AddBehavior(behavior);
//
//
//// Ide kell p�r flag hogy bDeepCopyActor, bDeepCopyBehavior, bDeepCopyScript
//// bUniqScript, bUniqActor, bUniqBehavior
//auto thing0 = gCore.SpawnThing(thingType, eCopy::ACTOR); // �s emiatt bel�l a thingType->actor - b�l deepCopy lesz az �j thing - ben, nem csak pointer csere
//auto thing1 = gCore.SpawnThing(thingType, eCopy::ACTOR);
//auto thing2 = gCore.SpawnThing(thingType, eCopy::ACTOR);
//
//thing0->setPos({ 0, 0, 0 });
//thing1->setPos({ 10, 10, 10 }); // Ez fogja �s k�zvetlen az actor RootComponent -j�nek a poz�ci�j�t �t�ll�tja
//// Sz�val amikor Thing - et spawnolok thing - b�l, akkor az Actor - t �gy ahogy van deep copyzom, de a Behavior - t azt nem


#include "Core\Core.h"
#include "PlatformLibrary\IWindow.h"
#include "Core\CameraComponent.h"
#include "PlatformLibrary\Window.h"
#include "PlatformLibrary\Timer.h"
#include "SupportLibrary\VisualCpuProfiler.h"
#include "PlayerScript.h"
#include "TestLevelScript.h"
#include "Core\Input.h"

void InitScript();

int main()
{
	// Full screen popup window for our game
	rWindow d;
		d.clientW = Sys::GetScreenSize().x; //FULL SCREEN props
		d.clientH = Sys::GetScreenSize().y; //FULL SCREEN props
		d.style = eWindowStyle::FULLSCREEN; //FULL SCREEN props
	IWindow* window = new Window(d);

	// Hide hardware cursor for our game on window
	window->HideCursor();

	// Init Engine core
	rGraphicsEngineRaster graphicsDesc;
		graphicsDesc.gapiType = eGapiType::OPENGL_4_5;
		graphicsDesc.targetWindow = window;
	gCore.InitGraphicsEngineRaster(graphicsDesc);
	gCore.InitPhysicsEngineBullet();
	gCore.InitSoundEngineSFML();
	
	ITimer* timer = new Timer();
	timer->Start();

	InitScript(); // Manual bullshit, TODO !!!

	while (window->IsOpen())
	{
		// Prepare for input processing
		gInput.ClearFrameData();

		// Process input events coming from O.S.-> Window
		rWindowEvent evt;
		while(window->PopEvent(evt))
		{
			switch(evt.msg)
			{
			case eWindowMsg::KEY_PRESS:
				gInput.KeyPress(evt.key);
				break;

			case eWindowMsg::KEY_RELEASE:
				gInput.KeyRelease(evt.key);
				break;

			case eWindowMsg::MOUSE_MOVE:
			{
				assert(evt.x >= 0 && evt.y >= 0);
				gInput.MouseMove(mm::ivec2(evt.deltaX, evt.deltaY), mm::uvec2((u32)evt.x, (u32)evt.y));
				break;
			}

			case eWindowMsg::MOUSE_PRESS:
				switch (evt.mouseBtn)
				{
				case eMouseBtn::LEFT:  gInput.MouseLeftPress();  break;
				case eMouseBtn::MID:   gInput.MouseMidPress();	 break;
				case eMouseBtn::RIGHT: gInput.MouseRightPress(); break;
				}
				break;

			case eWindowMsg::MOUSE_RELEASE:
				switch (evt.mouseBtn)
				{
				case eMouseBtn::LEFT:  gInput.MouseLeftRelease();  break;
				case eMouseBtn::MID:   gInput.MouseMidRelease();   break;
				case eMouseBtn::RIGHT: gInput.MouseRightRelease(); break;
				}
				break;
			}
		}

		gInput.Update();

		float deltaSeconds = timer->GetSecondsPassed();
		timer->Reset();

		gCore.Update(deltaSeconds);
	}
	return 0;
}

void InitScript()
{
	gCore.AddScript<TestLevelScript>();
}