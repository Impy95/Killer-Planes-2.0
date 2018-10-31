/**
* @file
* Animation2.h
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
#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

namespace sf
{
	class Time;
}

using Frame = sf::IntRect;

namespace GEX
{
	class Animation2
	{
	public:
							Animation2(bool repeat = true);

		void				addFrame(Frame frame);
		void				addFrameSet(const std::vector<Frame> frames);
		void				setDuration(sf::Time duration);
		sf::Time			getDuration() const;

		void				setRepeat(bool flag);
		bool				isRepeating() const;

		void				restart();
		bool				isFinished() const;

		Frame				getCurrentFrame() const;

		Frame				update(sf::Time dt);

	private:
		std::vector<Frame>	frames_;
		std::size_t			currentFrame_;
		sf::Time			duration_;
		sf::Time			elapsedTime_;
		bool				repeat_;
	};
}

