#include "EmitterNode.h"
#include "Command.h"
#include "CommandQueue.h"


namespace GEX
{
	EmitterNode::EmitterNode(Particle::Type type)
		: SceneNode()
		, accumlatedTime_(sf::Time::Zero)
		, type_(type)
		, particleSystem_(nullptr)
	{
	}

	void EmitterNode::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		if (particleSystem_)
		{
			emitParticle(dt);
		}
		else
		{
			auto finder = [this](ParticleNode& container, sf::Time)
			{
				if (container.getParticleType() == type_)
					particleSystem_ = &container;
			};

			Command command;
			command.category = Category::ParticleSystem;
			command.action = derivedAction<ParticleNode>(finder);

			commands.push(command);
		}
	}

	void EmitterNode::emitParticle(sf::Time dt)
	{
		const float EMISSION_RATE = 30.f;
		const sf::Time interval = sf::seconds(1.f) / EMISSION_RATE;

		accumlatedTime_ += dt;

		while(accumlatedTime_ > interval)
		{
			accumlatedTime_ -= interval;
			particleSystem_->addParticle(getWorldPosition());
		}
	}
}
