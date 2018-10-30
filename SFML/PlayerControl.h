///
/// Doxy comments go here
#pragma once

#include <map>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "Command.h"

namespace GEX {
	
	// forward declarion
	class CommandQueue;

	enum class Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		LaunchMissile,
		RR,
		RL
	};

	enum MissionStatus
	{
		MissionRunning,
		MissionFailure,
		MissionSuccess
	};
	class PlayerControl
	{
	public:
		PlayerControl();

		void			handleEvent(const sf::Event& event, CommandQueue& commands);
		void			handleRealtimeInput(CommandQueue& commands);

		void			setMissionStatus(MissionStatus status);
		MissionStatus	getMissionStatus() const;

	private:
		void			initalizeActions();
		static bool		isRealTimeAction(Action action);


	private:
		std::map<sf::Keyboard::Key, Action>		keyBindings_;
		std::map<Action, Command>				actionBindings_;

		MissionStatus							currentMissionStatus_;
	};
}

