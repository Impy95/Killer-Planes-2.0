#include "Application.h"
#include "StateIdentifier.h"
#include "GameState.h"
#include "TitleState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "FontManager.h"
#include "GEXState.h"
#include "GameOverState.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.0f / 60.f); // seconds per frame for 60 fps

Application::Application()
	: window_(sf::VideoMode(1280, 960), "Killer Planes", sf::Style::Close)
	, player_()
	, textures_()
	, stateStack_(GEX::State::Context(window_, textures_, player_))
	, statisticsText_()
	, statisticsUpdateTime_()
	, statisticsNumFrames_(0)
{
	window_.setKeyRepeatEnabled(false);

	GEX::FontManager::getInstance().load(GEX::FontID::Main, "Media/Sansation.ttf");

	textures_.load(GEX::TextureID::TitleScreen, "Media/Textures/TitleScreenBig.png");
	textures_.load(GEX::TextureID::Face, "Media/Textures/face.png");

	statisticsText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	statisticsText_.setPosition(5.0f, 5.0f);
	statisticsText_.setCharacterSize(15.0f);
	statisticsText_.setString("Frames / Second = \n Time / Update =");

	registerStates();
	stateStack_.pushState(GEX::StateID::Title);

}

void Application::run()
{


	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (window_.isOpen())
	{
		timeSinceLastUpdate += clock.restart(); // not adding now, resetting

		while (timeSinceLastUpdate > TimePerFrame)
		{
			processEvents();
			update(TimePerFrame);

			if (stateStack_.isEmpty())
				window_.close();
			timeSinceLastUpdate -= TimePerFrame;
		}

		updateStatistics(timeSinceLastUpdate);
		//updateStatistics(timeSinceLastUpdate);
		//update(timeSinceLastUpdate);				// update remaining time

		render();
	}
}

void Application::processEvents()
{
	sf::Event event;
	while (window_.pollEvent(event))
	{
		stateStack_.handleEvent(event);

		if (event.type == sf::Event::Closed)
			window_.close();
	}
}

void Application::update(sf::Time dt)
{
	stateStack_.update(dt);
}

void Application::render()
{
	window_.clear();
	stateStack_.draw();

	window_.setView(window_.getDefaultView());
	window_.draw(statisticsText_);
	window_.display();
}

void Application::updateStatistics(sf::Time dt)
{
	statisticsUpdateTime_ += dt;
	statisticsNumFrames_ += 1;

	if (statisticsUpdateTime_ > sf::seconds(1))
	{
		statisticsText_.setString(
			"Frames / Second = " + std::to_string(statisticsNumFrames_) + "\n" +
			"Time / Update = " + std::to_string(statisticsUpdateTime_.asMicroseconds() / statisticsNumFrames_)
		);
		statisticsUpdateTime_ -= sf::seconds(1);
		statisticsNumFrames_ = 0;
	}
}

void Application::registerStates()
{
	stateStack_.registerState<TitleState>(GEX::StateID::Title);
	stateStack_.registerState<MenuState>(GEX::StateID::Menu);
	stateStack_.registerState<GameState>(GEX::StateID::Game);
	stateStack_.registerState<PauseState>(GEX::StateID::Pause);
	stateStack_.registerState<GexState>(GEX::StateID::GEX);
	stateStack_.registerState<GameOverState>(GEX::StateID::GameOver);
}

