/**
* @file
* World.cpp
* @author
* Vaughn Rowse 2018
* @version 1.0
*
* @section DESCRIPTION
*
* @section LICENSE
*
* Copyright 2018
* Permission to use, copy, modify, and/or distribute this software for
* any purpose with or without fee is hereby granted, provided that the
* above copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
* @section Academic Integrity
* I certify that this work is solely my own and complies with
* NBCC Academic Integrity Policy (policy 1111)
*/
#include "World.h"
#include "Aircraft.h"
#include "Category.h"
#include "Pickup.h"
#include "DataTables.h"
#include "ParticleNode.h"

namespace GEX {
	World::World(sf::RenderWindow& window) : window_(window),
		worldView_(window.getDefaultView()),
		textures_(),
		sceneGraph_(),
		sceneLayer_(),
		worldBounds_(0.f, 0.f, worldView_.getSize().x, 5000.f),
		spawnPosition_(worldView_.getSize().x / 2.f, worldBounds_.height - worldView_.getSize().y / 2.f),
		scrollSpeeds_(-50.f),
		count_(0),
		playerActor_(nullptr)
	{
		loadTextures();
		buildScene();

		// prepare the view
		worldView_.setCenter(spawnPosition_);
	}

	void World::update(sf::Time dt, CommandQueue& commands)
	{
		// scroll the world
		worldView_.move(0.f, scrollSpeeds_ * dt.asSeconds());

		playerActor_->setVelocity(0.f, 0.f);

		destroyEntitiesOutOfView();
		guideZombie();

		// run all the commands in the command queue
		while (!commandQueue_.isEmpty())
			sceneGraph_.onCommand(commandQueue_.pop(), dt);

		handleCollisions();
		sceneGraph_.removeWrecks();

		adaptPlayerVelocity();
		sceneGraph_.update(dt, commands);
		adaptPlayerPosition();

		spawnEnemies();

	}

	void World::adaptPlayerPosition()
	{
		const float BORDER_DISTANCE = 40.f;
		sf::FloatRect viewBounds(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());

		sf::Vector2f position = playerActor_->getPosition();
		position.x = std::max(position.x, viewBounds.left + BORDER_DISTANCE);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - BORDER_DISTANCE);

		position.y = std::max(position.y, viewBounds.top + BORDER_DISTANCE);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - BORDER_DISTANCE);

		playerActor_->setPosition(position);
	}

	void World::adaptPlayerVelocity()
	{
		sf::Vector2f velocity = playerActor_->getVelocity();
		if (velocity.x != 0.f && velocity.y != 0.f)
			playerActor_->setVelocity(velocity / std::sqrt(2.f));
	}

	void World::addEnemies()
	{
		addEnemy(Actor::Type::Zombie1, -250.f, 200.f);
		addEnemy(Actor::Type::Zombie1, 0.f, 200.f);
		addEnemy(Actor::Type::Zombie1, 250.f, 200.f);

		addEnemy(Actor::Type::Zombie1, -250.f, 600.f);
		addEnemy(Actor::Type::Zombie2, 0.f, 600.f);
		addEnemy(Actor::Type::Zombie3, 250.f, 600.f);

		addEnemy(Actor::Type::Zombie2, -70.f, 400.f);
		addEnemy(Actor::Type::Zombie2, 70.f, 400.f);
		addEnemy(Actor::Type::Zombie2, -70.f, 800.f);

		addEnemy(Actor::Type::Zombie3, 70.f, 800.f);
		addEnemy(Actor::Type::Zombie3, -70.f, 850.f);
		addEnemy(Actor::Type::Zombie3, 70.f, 850.f);

		std::sort(enemySpawnPoints_.begin(), enemySpawnPoints_.end(), 
			[](SpawnPoint lhs, SpawnPoint rhs)
			{
				return lhs.y < rhs.y;
			});
	}

	void World::addEnemy(Actor::Type type, float relX, float relY)
	{
		SpawnPoint spawnPoint(type, spawnPosition_.x - relX, spawnPosition_.y - relY);
		enemySpawnPoints_.push_back(spawnPoint);
	}

	void World::spawnEnemies()
	{
		while (!enemySpawnPoints_.empty() &&
			enemySpawnPoints_.back().y > getBattlefieldBounds().top)
		{
			auto spawnPoint = enemySpawnPoints_.back();
			std::unique_ptr<Actor> enemy(new Actor(spawnPoint.type, textures_));
			enemy->setPosition(spawnPoint.x, spawnPoint.y);
			sceneLayer_[UpperAir]->attachChild(std::move(enemy));
			enemySpawnPoints_.pop_back();
		}
	}

	sf::FloatRect World::getViewBounds() const
	{
		return sf::FloatRect(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());
	}

	sf::FloatRect World::getBattlefieldBounds() const
	{
		sf::FloatRect bounds = getViewBounds();
		bounds.top -= 100.f;
		bounds.height += 100.f;
		return bounds;
	}

	void World::guideZombie()
	{
		// build a list of active Enemies
		Command heroCollector;
		heroCollector.category = Category::Hero;
		heroCollector.action = derivedAction<Actor>([this](Actor& hero, sf::Time dt)
		{
			if (!hero.isDestroyed())
				activeEnemies_.push_back(&hero);
		});

		Command zombieGuider;
		zombieGuider.category = Category::Type::Zombie;
		zombieGuider.action = derivedAction<Actor>([this](Actor& zombie, sf::Time dt)
		{
			// ignore not guided zombies
			if (!zombie.isGuided())
				return;

			float minDistance = std::numeric_limits<float>::max();
			Actor* hero = nullptr;

			for (Actor* e : activeEnemies_)
			{
				auto d = distance(zombie, *e);
				if (d < minDistance)
				{
					minDistance = d;
					hero = e;
				}
			}

			if (hero)
				zombie.guidedTowards(hero->getWorldPosition());
		});

		commandQueue_.push(heroCollector);
		commandQueue_.push(zombieGuider);
		activeEnemies_.clear();
	}

	void World::draw()
	{
		window_.setView(worldView_);
		window_.draw(sceneGraph_);
	}

	CommandQueue & World::getCommandQueue()
	{
		return commandQueue_;
	}

	bool World::hasAlivePlayer() const
	{
		return !playerActor_->isDestroyed();
	}

	bool World::hasPlayerReachedEnd() const
	{
		return !worldBounds_.contains(playerActor_->getPosition());
	}

	void World::handleCollisions()
	{
		// build a list of colliding pairs of SceneNodes
		std::set<SceneNode::Pair> collisionPairs;
		sceneGraph_.checkSceneCollision(sceneGraph_, collisionPairs);

		for (SceneNode::Pair pair : collisionPairs)
		{
			if (matchesCategories(pair, Category::Type::Hero, Category::Type::Zombie))
			{
				auto& hero  = static_cast<Actor&>(*(pair.first));
				auto& zombie = static_cast<Actor&>(*(pair.second));

				if (!hero.isForceFieldActive())
					hero.damage(zombie.attackPoints());
				zombie.damage(hero.attackPoints());

				auto zpos = zombie.getPosition();
				auto hpos = hero.getPosition();
				auto diffPos = zpos - hpos;
				zombie.setPosition(zpos + 0.2f * diffPos);
				hero.setPosition(hpos - 0.1f * diffPos);
			}

			else if (matchesCategories(pair, Category::Type::Zombie, Category::Type::Zombie))
			{
				auto& zombie1 = static_cast<Actor&>(*(pair.first));
				auto& zombie2 = static_cast<Actor&>(*(pair.second));

				auto zpos = zombie1.getPosition();
				auto hpos = zombie2.getPosition();
				auto diffPos = zpos - hpos;
				zombie1.setPosition(zpos + 0.2f * diffPos);
				zombie2.setPosition(hpos - 0.1f * diffPos);
			}
		}
	}

	bool World::matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
	{
		const unsigned int category1 = colliders.first->getCategory();
		const unsigned int category2 = colliders.second->getCategory();

		if (type1 & category1 && type2 & category2)
		{
			return true;
		}
		else if (type1 & category2 && type2 & category1)
		{
			std::swap(colliders.first, colliders.second);
			return true;
		}
		else
			return false;
	}

	void World::destroyEntitiesOutOfView()
	{
		Command command;
		command.category = Category::Type::Zombie;
		command.action = derivedAction<Entity>([this](Entity& e, sf::Time dt)
		{
			if (!getBattlefieldBounds().intersects(e.getBoundingBox()))
				e.remove();
		});

		commandQueue_.push(command);
	}

	void World::loadTextures()
	{
		textures_.load(GEX::TextureID::Entities, "Media/Textures/Entities.png");
		textures_.load(GEX::TextureID::Jungle, "Media/Textures/JungleBig.png");
		textures_.load(GEX::TextureID::Particle, "Media/Textures/Particle.png");
		textures_.load(GEX::TextureID::Explosion, "Media/Textures/Explosion.png");
		textures_.load(GEX::TextureID::FinishLine, "Media/Textures/FinishLine.png");
		textures_.load(GEX::TextureID::Hero2, "Media/Textures/hero2.png");
		textures_.load(GEX::TextureID::Zombie1, "Media/Textures/zombie1_sheet.png");
		textures_.load(GEX::TextureID::Zombie2, "Media/Textures/zombie2.png");
		textures_.load(GEX::TextureID::Zombie3, "Media/Textures/zombie3.png");
		
	}

	void World::buildScene()
	{
		// Initalize layers
		for (int i = 0; i < LayerCount; i++)
		{
			auto category = (i == UpperAir) ? Category::Type::AirSceneLayer : Category::Type::None;
			SceneNode::Ptr layer(new SceneNode(category));
			sceneLayer_.push_back(layer.get());
			sceneGraph_.attachChild(std::move(layer));
		}

		// Particle Systems
		std::unique_ptr<ParticleNode> smoke(new ParticleNode(Particle::Type::Smoke, textures_));
		sceneLayer_[LowerAir]->attachChild(std::move(smoke));

		std::unique_ptr<ParticleNode> fire(new ParticleNode(Particle::Type::Propellant, textures_));
		sceneLayer_[LowerAir]->attachChild(std::move(fire));

		// background
		sf::Texture& texture = textures_.get(TextureID::Jungle);
		sf::IntRect textureRect(worldBounds_);
		texture.setRepeated(true);

		std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
		backgroundSprite->setPosition(worldBounds_.left, worldBounds_.top);
		sceneLayer_[Background]->attachChild(std::move(backgroundSprite));

		// add player aircraft & game objects
		std::unique_ptr<Actor> leader(new Actor(Actor::Type::Hero2, textures_));
		leader->setPosition(spawnPosition_);
		leader->setVelocity(200.f, scrollSpeeds_);
		playerActor_ = leader.get();
		sceneLayer_[UpperAir]->attachChild(std::move(leader));

		// add enemy aircraft
		addEnemies();
	}


}
