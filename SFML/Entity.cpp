#include "Entity.h"
#include <cassert>


namespace GEX {
	Entity::Entity(int points)
		: hitPoints_(points)
	{

	}

	void Entity::setVelocity(sf::Vector2f velocity)
	{
		velocity_ = velocity;
	}

	void Entity::setVelocity(float vx, float vy)
	{
		velocity_.x = vx;
		velocity_.y = vy;
	}

	sf::Vector2f Entity::getVelocity() const
	{
		return velocity_;
	}

	void Entity::accelerate(sf::Vector2f velocity)
	{
		velocity_ += velocity;
	}

	void Entity::accelerate(float vx, float vy)
	{
		velocity_.x += vx;
		velocity_.y += vy;
	}

	int Entity::getHitPoints() const
	{
		return hitPoints_;
	}

	void Entity::damage(int points)
	{
		assert(points > 0);
		hitPoints_ -= points;
	}

	void Entity::repair(int points)
	{
		assert(points > 0);
		hitPoints_ += points;
	}

	void Entity::destroy()
	{
		hitPoints_ = 0;
	}

	bool Entity::isDestroyed() const
	{
		return (hitPoints_ <= 0);
	}

	void Entity::remove()
	{
		destroy();
	}

	void Entity::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		move(velocity_ * dt.asSeconds());
	}

}
