#include "StateStack.h"
#include <cassert>

GEX::StateStack::StateStack(State::Context context)
	: stack_()
	, pendingList_()
	, context_(context)
	, factories_()
{}

GEX::StateStack::~StateStack()
{
}

void GEX::StateStack::update(sf::Time dt)
{
	for (auto itr = stack_.rbegin(); itr != stack_.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}
}

void GEX::StateStack::draw()
{
	for (State::Ptr& state : stack_)
		state->draw();
}

void GEX::StateStack::handleEvent(const sf::Event & event)
{
	for (auto itr = stack_.rbegin(); itr != stack_.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void GEX::StateStack::pushState(GEX::StateID stateID)
{
	pendingList_.push_back(PendingChange(Action::Push, stateID));
}

void GEX::StateStack::popState()
{
	pendingList_.push_back(PendingChange(Action::Pop));
}

void GEX::StateStack::clearStates()
{
	pendingList_.push_back(PendingChange(Action::Clear));
}

bool GEX::StateStack::isEmpty()
{
	return stack_.empty();
}

GEX::State::Ptr GEX::StateStack::createState(GEX::StateID stateID)
{
	auto found = factories_.find(stateID);
	assert(found != factories_.end());

	return found->second();
}

void GEX::StateStack::applyPendingChanges()
{
	for (PendingChange change : pendingList_)
	{
		switch (change.action)
		{
		case Action::Push:
			stack_.push_back(createState(change.stateID));
			break;

		case Action::Pop:
			stack_.pop_back();
			break;

		case Action::Clear:
			stack_.clear();
			break;
		}
	}

	pendingList_.clear();
}

GEX::StateStack::PendingChange::PendingChange(Action action, StateID stateID)
	: action(action)
	, stateID(stateID)
{
}
