#include "Actor.h"
#include "TextureManager.h"
#include "JsonFrameParser.h"
#include "Utility.h"
#include "DataTables.h"
#include "TextNode.h"
#include "FontManager.h"

namespace GEX
{
	namespace
	{
		const std::map<Actor::Type, ActorData> TABLE = initalizeActorData();
	}

	Actor::Actor(Type type, const GEX::TextureManager& textures)
		: Entity(100)
		, type_(type)
		, state_(State::Walk)
		, sprite_(textures.get(TABLE.at(type).texture))
		, direction_(Direction::Right)
		, travelDistance_(0.f)
		, directionIndex_(0)
		, attack_(false)
		, isForceField_(false)
		, forceFieldDuration_(sf::Time::Zero)
		, forceFieldCooldown_(sf::Time::Zero)

	{
		for (auto a : TABLE.at(type).animations)
		{
			animations_[a.first] = a.second;
		}

		if (Actor::getCategory() == Category::Zombie)
			state_ = State::Rise;  // zombies spawn in rise state

		centerOrigin(sprite_);

		// set up health display text
		std::unique_ptr<TextNode> health(new TextNode(""));
		healthDisplay_ = health.get();
		attachChild(std::move(health));

		// set up force field cooldown text
		std::unique_ptr<TextNode> forceFieldCooldown(new TextNode(""));
		forceFieldCooldownText_ = forceFieldCooldown.get();
		attachChild(std::move(forceFieldCooldown));

		std::unique_ptr<TextNode> forceFieldDuration(new TextNode(""));
		forceFieldDurationText_ = forceFieldDuration.get();
		attachChild(std::move(forceFieldDuration));

		forceFieldCircle_.setFillColor(sf::Color(54, 243, 255));
		forceFieldCircle_.setOutlineColor(sf::Color::Black);
		forceFieldCircle_.setOutlineThickness(2.f);
		forceFieldCircle_.setRadius(50);

		centerOrigin(forceFieldCircle_);

		updateTexts();
	}



	unsigned int Actor::getCategory() const
	{
		switch (type_)
		{
		case Type::Hero2:
			return Category::Hero;
			break;
		case Type::Zombie1:
		case Type::Zombie2:
		case Type::Zombie3:
		case Type::Zombie4:
		case Type::Zombie5:
			return Category::Zombie;
			break;
		}
	}

	void Actor::updateMovementPattern(sf::Time dt)
	{
		if (!isGuided())
		{
			// movement pattern
			const std::vector<GEX::Direction> directions = TABLE.at(type_).directions;

			if (!directions.empty())
			{
				if (travelDistance_ > (directions[directionIndex_].distance))
				{
					directionIndex_ = (++directionIndex_) % directions.size();
					travelDistance_ = 0;
				}

				float radians = toRadian(directions[directionIndex_].angle + 90.f);
				float vx = getMaxSpeed() * std::cos(radians);
				float vy = getMaxSpeed() * std::sin(radians);

				setVelocity(vx, vy);
				travelDistance_ += getMaxSpeed() * dt.asSeconds();
			}
		}
	}

	sf::FloatRect Actor::getBoundingBox() const
	{
		auto box = getWorldTransform().transformRect(sprite_.getGlobalBounds());
		box.width -= 30; // tighten up bounding box for more realistic collisions
		box.left += 15;
		return box;
	}

	float Actor::getMaxSpeed() const
	{
		return TABLE.at(type_).speed;
	}

	void Actor::accelerate(sf::Vector2f velocity)
	{
		switch (state_)
		{
		case State::Attack:
			Entity::accelerate(velocity / 4.f);
			break;
		case State::Walk:
			Entity::accelerate(velocity);
			break;
		case State::Run:
			Entity::accelerate(velocity * 2.f);
		}
	}

	void Actor::attack()
	{
		attack_ = true;
	}

	void Actor::setState(State state)
	{
		state_ = state;
		animations_[state_].restart();
	}

	int Actor::attackPoints() const
	{
		if (type_ == Type::Hero2 && state_ == State::Attack)
			return TABLE.at(type_).damageDone;
		else if (type_ != Type::Hero2)
			return TABLE.at(type_).damageDone;
		else
			return 0;
	}

	bool Actor::isMarkedForRemoval() const
	{
		return isDestroyed() && animations_[State::Dead].isFinished();
	}

	void Actor::guidedTowards(sf::Vector2f position)
	{
		assert(isGuided());
		targetDirection_ = unitVector(position - getWorldPosition());
	}

	bool Actor::isGuided() const
	{
		//return type_ == Type::Zombie1;
		return TABLE.at(type_).follows;
	}

	void Actor::activateForceField()
	{
		isForceField_ = true;
		forceFieldDuration_ = sf::seconds(5.f);
		forceFieldCooldown_ = sf::seconds(15.f);
	}

	bool Actor::isForceFieldActive() const
	{
		return isForceField_;
	}

	void Actor::updateStates()
	{
		if (state_ == State::Attack && animations_[state_].isFinished())
			state_ = State::Walk;

		if (attack_ && state_ != State::Attack)
		{
			state_ = State::Attack;
			animations_[state_].restart();
			attack_ = false;
		}

		if (state_ == State::Rise && animations_[state_].isFinished())
			state_ = State::Idle;

		if (state_ == State::Idle && length(getVelocity()) > 0.1f)
			state_ = State::Walk;

		if (isDestroyed())
			state_ = State::Dead;
	}

	void Actor::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		if (isGuided())
		{
			const float APPROACH_RATE = 400.f;

			auto newVelocity = unitVector(APPROACH_RATE * dt.asSeconds() * targetDirection_ + getVelocity());

			newVelocity *= getMaxSpeed();
			setVelocity(newVelocity);

			//auto angle = std::atan2(newVelocity.y, newVelocity.x);
			//setRotation(toDegree(angle) + 90);
		}

		if (forceFieldDuration_ > sf::Time::Zero)
			forceFieldDuration_ -= dt;

		if (forceFieldDuration_ <= sf::Time::Zero)
			isForceField_ = false;

		if (forceFieldCooldown_ > sf::Time::Zero)
			forceFieldCooldown_ -= dt;

		updateStates();

		auto rec = animations_.at(state_).update(dt);

		if (direction_ == Direction::Left && getVelocity().x > 10)
			direction_ = Direction::Right;
		if (direction_ == Direction::Right && getVelocity().x < 0)
			direction_ = Direction::Left;

		// flip image left right
		if (direction_ == Direction::Left)
			rec = flip(rec);

		sprite_.setTextureRect(rec);
		centerOrigin(sprite_);

		if (type_ == Type::Hero2)
		{
			forceFieldCircle_.setPosition(sprite_.getPosition());
			centerOrigin(forceFieldCircle_);
		}

		if (state_ != State::Dead) // don't move it while dying
			Entity::updateCurrent(dt, commands);

		updateMovementPattern(dt);
		updateTexts();
	}

	void Actor::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		if (type_ == Type::Hero2 && isForceFieldActive())
		{
			target.draw(forceFieldCircle_, states);
		}
		target.draw(sprite_, states);
	}

	void Actor::updateTexts()
	{
		healthDisplay_->setText(std::to_string(getHitPoints()) + "HP");
		healthDisplay_->setPosition(0.f, 70.f);
		healthDisplay_->setRotation(-getRotation());

		if (type_ == Type::Hero2 && forceFieldCooldown_ > sf::Time::Zero)
		{
			forceFieldCooldownText_->setText("FF CD: " + std::to_string(static_cast<int>(forceFieldCooldown_.asSeconds())));
			forceFieldCooldownText_->setPosition(0.f, -70.f);
			forceFieldCooldownText_->setRotation(-getRotation());
		}
		else if (type_ == Type::Hero2 && forceFieldCooldown_ <= sf::Time::Zero)
		{
			forceFieldCooldownText_->setText("FF Ready!");
			forceFieldCooldownText_->setPosition(0.f, -70.f);
			forceFieldCooldownText_->setRotation(-getRotation());
		}

		if (type_ == Type::Hero2 && forceFieldDuration_ > sf::Time::Zero)
		{
			forceFieldDurationText_->setText("FF: " + std::to_string(static_cast<int>(forceFieldDuration_.asSeconds())));
			forceFieldDurationText_->setPosition(0.f, 90.f);
			forceFieldDurationText_->setRotation(-getRotation());
		}
		else if (type_ == Type::Hero2 && forceFieldDuration_ <= sf::Time::Zero)
		{
			forceFieldDurationText_->setText("");
			forceFieldDurationText_->setPosition(0.f, 90.f);
			forceFieldDurationText_->setRotation(-getRotation());
		}
	}
}
