#include "PlayerControl.h"
#include "Aircraft.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Category.h"
#include <functional>
#include "Actor.h"

namespace GEX {
	struct ActorMover
	{
	public:
		ActorMover(float vx, float vy)
			: velocity(vx, vy)
		{}
		void operator() (Actor& actor, sf::Time) const
		{
			actor.accelerate(velocity);
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
		keyBindings_[sf::Keyboard::Space] = Action::Attack;

		// set up action bindings
		initalizeActions();

		for (auto& pair : actionBindings_)
			pair.second.category = Category::Hero;


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

		actionBindings_[Action::MoveLeft].action = derivedAction<Actor>(ActorMover(-playerSpeed, 0.f));
		actionBindings_[Action::MoveRight].action = derivedAction<Actor>(ActorMover(+playerSpeed, 0.f));
		actionBindings_[Action::MoveUp].action = derivedAction<Actor>(ActorMover(0.f, -playerSpeed));
		actionBindings_[Action::MoveDown].action = derivedAction<Actor>(ActorMover(0.f, +playerSpeed));

		actionBindings_[Action::Attack].action = derivedAction<Actor>([](Actor& node, sf::Time dt) { node.attack(); });
		//actionBindings_[Action::Attack].category = Category::Type::Hero;

	}

	bool PlayerControl::isRealTimeAction(Action action)
	{
		switch (action)
		{
		case Action::MoveLeft:
		case Action::MoveRight:
		case Action::MoveDown:
		case Action::MoveUp:
			return true;
			break;
		default:
			return false;
			break;
		}
	}


}
