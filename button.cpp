#include "Button.h"

Button::Button()
{
	rect.setFillColor(COLOR_IDLE);
	rect.setSize(Vector2f(64.f, 64.f));
}

Button::Button(string s, Font* f)
{
	text.setFont(*f);
	text.setString(s);
	text.setCharacterSize(28);
	rect.setFillColor(COLOR_IDLE);
	rect.setSize(Vector2f(text.getGlobalBounds().width * 2.0f, text.getGlobalBounds().height * 2.0f));
	text.setPosition(rect.getPosition() + Vector2f(rect.getSize().x - text.getGlobalBounds().width, 8) * 0.5f);
}

Button::Button(Texture* t, IntRect r, Vector2f size)
{
	sprite.setTexture(*t);
	sprite.setTextureRect(r);
	rect.setFillColor(COLOR_IDLE);
	rect.setSize(size);
	sprite.setPosition(rect.getPosition() + Vector2f(rect.getSize().x - sprite.getGlobalBounds().width, rect.getSize().y - sprite.getGlobalBounds().height) * 0.5f);
}

void Button::setPosition(Vector2f vec)
{
	position = vec;
	rect.setPosition(vec);
	text.setPosition(rect.getPosition() + Vector2f(rect.getSize().x - text.getGlobalBounds().width, 8) * 0.5f);
	sprite.setPosition(rect.getPosition() + Vector2f(rect.getSize().x - sprite.getGlobalBounds().width, rect.getSize().y - sprite.getGlobalBounds().height) * 0.5f);
}

void Button::setSize(Vector2f size)
{
	rect.setSize(size);
	text.setPosition(rect.getPosition() + Vector2f(rect.getSize().x - text.getGlobalBounds().width, 8) * 0.5f);
	sprite.setPosition(rect.getPosition() + Vector2f(rect.getSize().x - sprite.getGlobalBounds().width, rect.getSize().y - sprite.getGlobalBounds().height) * 0.5f);
}

void Button::setTexture(Texture* tex)
{
	sprite.setTexture(*tex);
}

void Button::setTextureRect(IntRect rect)
{
	sprite.setTextureRect(rect);
}

void Button::setDisabled(bool disabled)
{
	isDisabled = disabled;
}

bool Button::getOnClick()
{
	if (onClick)
	{
		onClick = false;
		return true;
	}
	return false;
}

void Button::update(Event& e, RenderWindow& window)
{
	if (isDisabled)
	{
		rect.setFillColor(COLOR_DISABLED);
		return;
	}

	Vector2i mousePos = Mouse::getPosition(window);
	bool isMouseInside = rect.getGlobalBounds().contains(Vector2f(mousePos));

	switch (e.type)
	{
	case Event::MouseMoved:
		if (isMouseInside)
		{
			if (state == State::IDLE) {
				state = State::HOVERED;
			}
		}
		else state = State::IDLE;
		break;
	case Event::MouseButtonPressed:
		if (e.mouseButton.button == Mouse::Left)
		{
			state = (isMouseInside) ? State::CLICKED : State::IDLE;
		}
		break;
	case Event::MouseButtonReleased:
		if (e.mouseButton.button == Mouse::Left)
		{
			if (isMouseInside)
			{
				if (state == State::CLICKED)
				{
					onClick = true;
				}
				state = State::HOVERED;
			}
			else state = State::IDLE;
		}
		break;
	}

	switch (state)
	{
	case State::IDLE:
		rect.setFillColor(COLOR_IDLE);
		break;
	case State::HOVERED:
		rect.setFillColor(COLOR_HOVERED);
		break;
	case State::CLICKED:
		rect.setFillColor(COLOR_CLICKED);
		break;
	}
}

void Button::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(rect, states);
	target.draw(sprite, states);
	target.draw(text, states);
}