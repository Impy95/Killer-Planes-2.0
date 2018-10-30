#include "PlayerControl.h"
#include "Aircraft.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Category.h"
#include <functional>

namespace GEX {
	struct AircraftMover
	{
	public:
		AircraftMover(float vx, float vy)
			: velocity(vx, vy)
		{}
		void operator() (Aircraft& aircraft, sf::Time) const
		{
			aircraft.accelerate(velocity);
		}

		sf::Vector2f velocity;
	};
	PlayerControl::PlayerControl()
		: currentMissionStatus_(MissionStatus::MissionRunning)
	{
		// set up bindings
		keyBindings_[sf::Keyboard::A] = Action::MoveLeft;
		keyBindings_[sf::Keyboard::D] = Action::MoveRight;
		keyBindings_[sf::Keyboard::W] = Action::MoveUp;
		keyBindings_[sf::Keyboard::S] = Action::MoveDown;

		keyBindings_[sf::Keyboard::Space] = Action::Fire;
		keyBindings_[sf::Keyboard::Q] = Action::LaunchMissile;
		//keyBindings_[sf::Keyboard::J] = Action::RR;
		//keyBindings_[sf::Keyboard::L] = Action::RL;


		// set up action bindings
		initalizeActions();

		for (auto& pair : actionBindings_)
			pair.second.category = Category::PlayerAircraft;

		//actionBindings_[Action::RR].category = Category::EnemyAircraft;
		//actionBindings_[Action::RL].category = Category::EnemyAircraft;


	}

	void PlayerControl::handleEvent(const sf::Event & event, CommandQueue & commands)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			auto found = keyBindings_.find(event.key.code);
			if (found != keyBindings_.end())
			{
				if (!isRealTimeAction(found->second))
					commands.push(actionBindings_[found->second]);
			}
		}
	}

	void PlayerControl::handleRealtimeInput(CommandQueue & commands)
	{
		// itraverse all assigned keys, look up action, generate command
		for (auto pair : keyBindings_)
		{
			if (sf::Keyboard::isKeyPressed(pair.first) && isRealTimeAction(pair.second))
				commands.push(actionBindings_[pair.second]);
		}
	}

	void PlayerControl::setMissionStatus(MissionStatus status)
	{
		currentMissionStatus_ = status;
	}

	MissionStatus PlayerControl::getMissionStatus() const
	{
		return currentMissionStatus_;
	}

	void PlayerControl::initalizeActions()
	{
		const float playerSpeed = 200.f;

		actionBindings_[Action::MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
		actionBindings_[Action::MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f));
		actionBindings_[Action::MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
		actionBindings_[Action::MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, +playerSpeed));
		// rotate raptors
		//actionBindings_[Action::RR].action = derivedAction<Aircraft>([](SceneNode& node, sf::Time dt) {node.rotate(+1.f); });
		//actionBindings_[Action::RL].action = derivedAction<Aircraft>([](SceneNode& node, sf::Time dt) {node.rotate(-1.f); });

		actionBindings_[Action::Fire].action = derivedAction<Aircraft>(std::bind(&Aircraft::fire, std::placeholders::_1));
		actionBindings_[Action::Fire].category = Category::PlayerAircraft;

		actionBindings_[Action::LaunchMissile].action = derivedAction<Aircraft>(std::bind(&Aircraft::launchMissile, std::placeholders::_1));
		actionBindings_[Action::LaunchMissile].category = Category::PlayerAircraft;
	}

	bool PlayerControl::isRealTimeAction(Action action)
	{
		switch (action)
		{
		case Action::MoveLeft:
		case Action::MoveRight:
		case Action::MoveDown:
		case Action::MoveUp:
		case Action::RR:
		case Action::RL:
		case Action::Fire:
			return true;
			break;
		default:
			return false;
			break;
		}
	}


}
