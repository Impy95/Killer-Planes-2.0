#include "TextureManager.h"
#include <stdexcept>
#include <cassert>

namespace GEX {
	TextureManager::TextureManager()
	{
	}


	TextureManager::~TextureManager()
	{
	}

	void TextureManager::load(TextureID id, const std::string & path)
	{
		std::unique_ptr<sf::Texture> texture(new sf::Texture());

		if (!texture->loadFromFile(path))
		{
			throw std::runtime_error("Texture load failed " + path);
		}

		auto rc = textures_.insert(std::make_pair(id, std::move(texture)));
		assert(rc.second);
	}

	sf::Texture& TextureManager::get(TextureID id) const
	{
		auto found = textures_.find(id);

		assert(found != textures_.end());

		return *(found->second);
	}
}
