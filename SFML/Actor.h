#pragma once
#include "Entity.h"
#include "Animation2.h"
#include <SFML/Graphics/Sprite.hpp>
#include <map>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace GEX
{
	class TextureManager;
	class TextNode;

	class Actor : public Entity
	{
	public:
		enum class Type
		{
			Hero2,
			Zombie1,
			Zombie2,
			Zombie3,
			Zombie4,
			Zombie5
		};

		enum class State
		{
			Attack,
			Dead,
			Idle,
			Rise,
			Walk,
			Run,
			Jump,
			count
		};

		enum class Direction
		{
			Left,
			Right,
			Up,
			Down
		};
	public:
		Actor(Type type, const TextureManager& textures);;
		~Actor() = default;

		unsigned int	getCategory() const override;
		sf::FloatRect	getBoundingBox() const override;
		float			getMaxSpeed() const;
		virtual void	accelerate(sf::Vector2f velocity) override;

		void			attack();
		void			setState(State state);
		int				attackPoints() const;

		bool			isMarkedForRemoval() const override;
		void			guidedTowards(sf::Vector2f position);
		bool			isGuided() const;
		void			activateForceField();
		bool			isForceFieldActive() const;

	private:
		void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		void			updateStates();
		void			updateCurrent(sf::Time dt, CommandQueue& commands) override;
		void			updateMovementPattern(sf::Time dt);
		void			updateTexts();

	private:
		Type									type_;
		State									state_;
		mutable sf::Sprite						sprite_;
		mutable std::map<State, Animation2>		animations_;
		Direction								direction_;
		TextNode*								healthDisplay_;

		float									travelDistance_;
		std::size_t								directionIndex_;
		bool									attack_;
		sf::Vector2f							targetDirection_;

		bool									isForceField_;
		sf::Time								forceFieldDuration_;
		sf::Time								forceFieldCooldown_;
		TextNode*								forceFieldCooldownText_;
		TextNode*								forceFieldDurationText_;
		sf::CircleShape							forceFieldCircle_;
	};
}

