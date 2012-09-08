#include <ppapi/cpp/core.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/input_event.h>
#include <ppapi/c/ppb_gamepad.h>
#include <SDL/SDL.h>
#include <SDL_nacl.h>

class GameInstance;
/**
 * ほぼ
 * https://developers.google.com/native-client/community/porting/SDLgames
 * のまんまー
 */

class GameInstance : public pp::Instance {
 private:
	static int num_instances_;			 // Ensure we only create one instance.
	pthread_t game_main_thread_;		 // This thread will run game_main().
	int num_changed_view_;					 // Ensure we initialize an instance only once.
	int width_; int height_;				 // Dimension of the SDL video screen.

	// Launches the actual game, e.g., by calling game_main().
	static void* LaunchGame(void* data);

	// This function allows us to delay game start until all
	// resources are ready.
	void StartGameInNewThread(int32_t dummy);

 public:

	explicit GameInstance(pp::Module* module, PP_Instance instance)
		: pp::Instance(instance),
			game_main_thread_(NULL),
			num_changed_view_(0),
			width_(640), height_(480) {
		// Game requires mouse and keyboard events; add more if necessary.
		RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE|PP_INPUTEVENT_CLASS_KEYBOARD);
		++num_instances_;
		assert (num_instances_ == 1);
	}

	virtual ~GameInstance() {
		// Wait for game thread to finish.
		if (game_main_thread_) { pthread_join(game_main_thread_, NULL); }
	}

	// This function is called with the HTML attributes of the embed tag,
	// which can be used in lieu of command line arguments.
	virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]) {
//		[Process arguments and set width_ and height_]
//		[Initiate the loading of resources]
		return true;
	}

	// This crucial function forwards PPAPI events to SDL.
	virtual bool HandleInputEvent(const pp::InputEvent& event) {
		SDL_NACL_PushEvent(event);
		return true;
	}

	// This function is called for various reasons, e.g. visibility and page
	// size changes. We ignore these calls except for the first
	// invocation, which we use to start the game.
	virtual void DidChangeView(const pp::Rect& position, const pp::Rect& clip) {
		++num_changed_view_;
		if (num_changed_view_ > 1) return;
		// NOTE: It is crucial that the two calls below are run here
		// and not in a thread.
		SDL_NACL_SetInstance(pp_instance(), width_, height_);
		// This is SDL_Init call which used to be in game_main()
		SDL_Init(SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_VIDEO);
		StartGameInNewThread(0);
	}
};
int GameInstance::num_instances_ = 0;

void GameInstance::StartGameInNewThread(int32_t dummy) {
//		if ([All Resourced Are Ready]) {
		pthread_create(&game_main_thread_, NULL, &LaunchGame, this);
//		} else {
//				Wait some more (here: 100ms).
//						pp::Module::Get()->core()->CallOnMainThread(
//										100, cc_factory_.NewCallback(&GameInstance::StartGameInNewThread), 0);
//		}
}

extern "C"{
#include "noiz2sa.h"
}
void* GameInstance::LaunchGame(void* data) {
	// Use "thiz" to get access to instance object.
	GameInstance* thiz = reinterpret_cast<GameInstance*>(data);
	// Craft a fake command line.
	char* argv[] = { "noiz2sa", };
	noiz2sa_main(sizeof(argv) / sizeof(argv[0]), argv);
	return 0;
}

//---------------------------------------------------------------------------
// 以下あんまり重要じゃない
//---------------------------------------------------------------------------

class GameModule : public pp::Module {
 public:
	GameModule() : pp::Module() {}
	virtual ~GameModule() {}

	virtual pp::Instance* CreateInstance(PP_Instance instance) {
		return new GameInstance(this, instance);
	}
};

namespace pp {
Module* CreateModule() {
	return new GameModule();
}
}	// namespace pp

