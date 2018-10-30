#pragma once
#include "SceneNode.h"
#include "Particle.h"
#include "ParticleNode.h"

namespace GEX
{
	class EmitterNode : public SceneNode
	{
	public:
		explicit		EmitterNode(Particle::Type type);

	private:
		void			updateCurrent(sf::Time dt, CommandQueue& commands) override;
		void			emitParticle(sf::Time dt);

	private:
		sf::Time		accumlatedTime_;
		Particle::Type	type_;
		ParticleNode*	particleSystem_;

	};
}

