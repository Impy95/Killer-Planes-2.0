#include "CommandQueue.h"



namespace GEX
{
	void CommandQueue::push(const Command & command)
	{
		queue_.push(command);
	}
	Command CommandQueue::pop()
	{
		auto cmd = queue_.front();
		queue_.pop();
		return cmd;
	}
	bool CommandQueue::isEmpty() const
	{
		return queue_.empty();
	}
}
