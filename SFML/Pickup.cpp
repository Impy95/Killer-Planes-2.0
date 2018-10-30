#include "Pickup.h"
#include "DataTables.h"
#include "Utility.h"

namespace GEX
{
	namespace
	{
		const std::map<Pickup::Type, PickupData> TABLE = initalizePickupData();
	}

	Pickup::Pickup(Type type, const TextureManager& textures)
		: Entity(1)
		, type_(type)
		, sprite_(textures.get(TABLE.at(type).texture), TABLE.at(type).textureRect)
	{
		centerOrigin(sprite_);
	}

	unsigned Pickup::getCategory() const
	{
		return Category::Pickup;
	}

	sf::FloatRect Pickup::getBoundingBox() const
	{
		return getWorldTransform().transformRect(sprite_.getGlobalBounds());
	}

	void Pickup::apply(Aircraft& player)
	{
		TABLE.at(type_).action(player);
	}

	void Pickup::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		Entity::updateCurrent(dt, commands);
	}

	void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}
}
