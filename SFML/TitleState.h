#pragma once

#include "State.h"

class TitleState : public GEX::State
{
public:
	TitleState(GEX::StateStack& stack, Context context);

	void					draw() override;
	bool					update(sf::Time dt) override;
	bool					handleEvent(const sf::Event& event) override;

private:
	sf::Sprite				backgroundSprite_;
	sf::Text				text_;
	bool					showText_;
	sf::Time				textEffectTime_;
};

