/**
* @file
* DataTables.cpp
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
#include "DataTables.h"
#include "JsonFrameParser.h"
#include "Actor.h"

namespace GEX
{
	std::map<Actor::Type, ActorData> initalizeActorData()
	{
		std::map<Actor::Type, ActorData> data;

		data[Actor::Type::Hero2].texture = TextureID::Hero2;
		data[Actor::Type::Hero2].hitpoints = 100;
		data[Actor::Type::Hero2].damageDone = 5;
		data[Actor::Type::Hero2].speed = 50;

		JsonFrameParser frames = JsonFrameParser("Media/Textures/hero2.json");

		data[Actor::Type::Hero2].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
		data[Actor::Type::Hero2].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
		data[Actor::Type::Hero2].animations[Actor::State::Idle].setRepeat(true);

		data[Actor::Type::Hero2].animations[Actor::State::Jump].addFrameSet(frames.getFramesFor("jump"));
		data[Actor::Type::Hero2].animations[Actor::State::Jump].setDuration(sf::seconds(1.f));
		data[Actor::Type::Hero2].animations[Actor::State::Jump].setRepeat(false);

		data[Actor::Type::Hero2].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
		data[Actor::Type::Hero2].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
		data[Actor::Type::Hero2].animations[Actor::State::Attack].setRepeat(false);

		data[Actor::Type::Hero2].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
		data[Actor::Type::Hero2].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
		data[Actor::Type::Hero2].animations[Actor::State::Walk].setRepeat(true);

		data[Actor::Type::Hero2].animations[Actor::State::Run].addFrameSet(frames.getFramesFor("run"));
		data[Actor::Type::Hero2].animations[Actor::State::Run].setDuration(sf::seconds(1.f));
		data[Actor::Type::Hero2].animations[Actor::State::Run].setRepeat(true);

		data[Actor::Type::Hero2].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
		data[Actor::Type::Hero2].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
		data[Actor::Type::Hero2].animations[Actor::State::Dead].setRepeat(false);


		data[Actor::Type::Zombie1].texture = TextureID::Zombie1;
		data[Actor::Type::Zombie1].hitpoints = 100;
		data[Actor::Type::Zombie1].damageDone = 1;
		data[Actor::Type::Zombie1].speed = 50;
		data[Actor::Type::Zombie1].follows = true;

		frames = JsonFrameParser("Media/Textures/zombie1_sheet.json");

		data[Actor::Type::Zombie1].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
		data[Actor::Type::Zombie1].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie1].animations[Actor::State::Idle].setRepeat(true);
		data[Actor::Type::Zombie1].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
		data[Actor::Type::Zombie1].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie1].animations[Actor::State::Attack].setRepeat(true);
		data[Actor::Type::Zombie1].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
		data[Actor::Type::Zombie1].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie1].animations[Actor::State::Walk].setRepeat(true);
		data[Actor::Type::Zombie1].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
		data[Actor::Type::Zombie1].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie1].animations[Actor::State::Dead].setRepeat(false);
		data[Actor::Type::Zombie1].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("rise"));
		data[Actor::Type::Zombie1].animations[Actor::State::Rise].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie1].animations[Actor::State::Rise].setRepeat(false);

		data[Actor::Type::Zombie1].directions.emplace_back(Direction(45.f, 50.f));
		data[Actor::Type::Zombie1].directions.emplace_back(Direction(-45.f, 100.f));
		data[Actor::Type::Zombie1].directions.emplace_back(Direction(45.f, 50.f));



		data[Actor::Type::Zombie2].texture = TextureID::Zombie2;
		data[Actor::Type::Zombie2].hitpoints = 100;
		data[Actor::Type::Zombie2].damageDone = 1;
		data[Actor::Type::Zombie2].speed = 50;
		data[Actor::Type::Zombie2].follows = false;

		frames = JsonFrameParser("Media/Textures/zombie2.json");

		data[Actor::Type::Zombie2].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
		data[Actor::Type::Zombie2].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie2].animations[Actor::State::Idle].setRepeat(true);
		data[Actor::Type::Zombie2].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
		data[Actor::Type::Zombie2].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie2].animations[Actor::State::Attack].setRepeat(true);
		data[Actor::Type::Zombie2].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
		data[Actor::Type::Zombie2].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie2].animations[Actor::State::Walk].setRepeat(true);
		data[Actor::Type::Zombie2].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
		data[Actor::Type::Zombie2].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie2].animations[Actor::State::Dead].setRepeat(false);
		data[Actor::Type::Zombie2].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("rise"));
		data[Actor::Type::Zombie2].animations[Actor::State::Rise].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie2].animations[Actor::State::Rise].setRepeat(false);


		data[Actor::Type::Zombie2].directions.emplace_back(Direction(45.f, 50.f));
		data[Actor::Type::Zombie2].directions.emplace_back(Direction(-45.f, 100.f));
		data[Actor::Type::Zombie2].directions.emplace_back(Direction(45.f, 50.f));




		data[Actor::Type::Zombie3].texture = TextureID::Zombie3;
		data[Actor::Type::Zombie3].hitpoints = 100;
		data[Actor::Type::Zombie3].damageDone = 1;
		data[Actor::Type::Zombie3].speed = 50;
		frames = JsonFrameParser("Media/Textures/zombie3.json");
		data[Actor::Type::Zombie3].follows = false;

		data[Actor::Type::Zombie3].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
		data[Actor::Type::Zombie3].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie3].animations[Actor::State::Idle].setRepeat(true);
		data[Actor::Type::Zombie3].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
		data[Actor::Type::Zombie3].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie3].animations[Actor::State::Attack].setRepeat(true);
		data[Actor::Type::Zombie3].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
		data[Actor::Type::Zombie3].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie3].animations[Actor::State::Walk].setRepeat(true);
		data[Actor::Type::Zombie3].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
		data[Actor::Type::Zombie3].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie3].animations[Actor::State::Dead].setRepeat(false);
		data[Actor::Type::Zombie3].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("walk"));
		data[Actor::Type::Zombie3].animations[Actor::State::Rise].setDuration(sf::seconds(1.f));
		data[Actor::Type::Zombie3].animations[Actor::State::Rise].setRepeat(false);

		data[Actor::Type::Zombie3].directions.emplace_back(Direction(45.f, 50.f));
		data[Actor::Type::Zombie3].directions.emplace_back(Direction(-45.f, 100.f));
		data[Actor::Type::Zombie3].directions.emplace_back(Direction(45.f, 50.f));


		return data;
	}

	std::map<Pickup::Type, PickupData> initalizePickupData()
	{
		std::map<Pickup::Type, PickupData> data;

		data[Pickup::Type::HealthRefill].texture = TextureID::Entities;
		data[Pickup::Type::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);
		data[Pickup::Type::HealthRefill].action = [](Aircraft& a) {a.repair(25); };

		data[Pickup::Type::MissileRefill].texture = TextureID::Entities;
		data[Pickup::Type::MissileRefill].textureRect = sf::IntRect(40, 64, 40, 40);
		data[Pickup::Type::MissileRefill].action = [](Aircraft& a) {a.collectMissile(3); };

		data[Pickup::Type::FireSpread].texture = TextureID::Entities;
		data[Pickup::Type::FireSpread].textureRect = sf::IntRect(80, 64, 40, 40);
		data[Pickup::Type::FireSpread].action = [](Aircraft& a) {a.increasedFireSpread(); };

		data[Pickup::Type::FireRate].texture = TextureID::Entities;
		data[Pickup::Type::FireRate].textureRect = sf::IntRect(120, 64, 40, 40);
		data[Pickup::Type::FireRate].action = [](Aircraft& a) {a.increasedFireRate(); };

		return data;
	}

	std::map<Projectile::Type, ProjectileData> initalizeProjectileData()
	{
		std::map<Projectile::Type, ProjectileData> data;

		data[Projectile::Type::AlliedBullet].damage = 10;
		data[Projectile::Type::AlliedBullet].speed = 300.f;
		data[Projectile::Type::AlliedBullet].texture = TextureID::Entities;
		data[Projectile::Type::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

		data[Projectile::Type::EnemyBullet].damage = 10;
		data[Projectile::Type::EnemyBullet].speed = 300.f;
		data[Projectile::Type::EnemyBullet].texture = TextureID::Entities;
		data[Projectile::Type::EnemyBullet].textureRect = sf::IntRect(175, 64, 3, 14);

		data[Projectile::Type::Missile].damage = 200;
		data[Projectile::Type::Missile].speed = 150.f;
		data[Projectile::Type::Missile].texture = TextureID::Entities;
		data[Projectile::Type::Missile].textureRect = sf::IntRect(160, 64, 15, 32);

		return data;
	}
	std::map<AircraftType, AircraftData> initalizeAircraftData()
	{
		std::map<GEX::AircraftType, AircraftData> data;

		// Eagle
		data[AircraftType::Eagle].hitpoints = 100;
		data[AircraftType::Eagle].speed = 200.f;
		data[AircraftType::Eagle].texture = TextureID::Entities;
		data[AircraftType::Eagle].textureRect = sf::IntRect(0, 0, 48, 64);
		data[AircraftType::Eagle].fireInterval = sf::seconds(1);
		data[AircraftType::Eagle].hasRollAnimation = true;

		// Raptor
		data[AircraftType::Raptor].hitpoints = 20;
		data[AircraftType::Raptor].speed = 80.f;
		data[AircraftType::Raptor].texture = TextureID::Entities;
		data[AircraftType::Raptor].textureRect = sf::IntRect(144, 0, 84, 64);

		data[AircraftType::Raptor].fireInterval = sf::Time::Zero;
		data[AircraftType::Raptor].directions.emplace_back(Direction(45.f, 80.f)); //Emplace back avooid create a temp object - saves resources
		data[AircraftType::Raptor].directions.emplace_back(Direction(-45.f, 160.f));
		data[AircraftType::Raptor].directions.emplace_back(Direction(45.f, 80.f));
		data[AircraftType::Raptor].hasRollAnimation = false;

		// Avenger
		data[AircraftType::Avenger].hitpoints = 40;
		data[AircraftType::Avenger].speed = 50.f;
		data[AircraftType::Avenger].texture = TextureID::Entities;
		data[AircraftType::Avenger].textureRect = sf::IntRect(228, 0, 60, 59);

		data[AircraftType::Avenger].fireInterval = sf::seconds(2);
		data[AircraftType::Avenger].directions.emplace_back(Direction(45.f, 50.f)); //Emplace back avooid create a temp object - saves resources
		data[AircraftType::Avenger].directions.emplace_back(Direction(0.f, 50.f));
		data[AircraftType::Avenger].directions.emplace_back(Direction(-45.f, 100.f));
		data[AircraftType::Avenger].directions.emplace_back(Direction(0.f, 50.f));
		data[AircraftType::Avenger].directions.emplace_back(Direction(45.f, 50.f));
		data[AircraftType::Avenger].hasRollAnimation = false;

		return data;
	}

	std::map<Particle::Type, ParticleData> initalizeParticleData()
	{
		std::map<Particle::Type, ParticleData> data;

		//data[Particle::Type::Propellant].color = sf::Color(255, 255, 50);
		data[Particle::Type::Propellant].color = sf::Color(160, 10, 20);
		data[Particle::Type::Propellant].lifetime = sf::seconds(0.6f);

		data[Particle::Type::Smoke].color = sf::Color(50, 50, 50);
		data[Particle::Type::Smoke].lifetime = sf::seconds(4.f);

		return data;
	}
}
