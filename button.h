#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Button : public Drawable
{
public:
	const Color COLOR_IDLE = Color(85, 80, 71);
	const Color COLOR_HOVERED = Color(71, 67, 59);
	const Color COLOR_CLICKED = Color(58, 54, 48);
	const Color COLOR_DISABLED = Color(47, 44, 39);
	enum State
	{
		IDLE, HOVERED, CLICKED, DISABLED
	};
	RectangleShape rect;
	Sprite sprite;
	Text text;
	int state = State::IDLE;
	bool isDisabled = false;
	bool onClick = false;
	Vector2f position;
	Button();
	Button(string, Font*);
	Button(Texture*, IntRect, Vector2f size = Vector2f(70.f, 70.f));
	void setPosition(Vector2f);
	void setSize(Vector2f);
	void setTexture(Texture*);
	void setTextureRect(IntRect);
	void setDisabled(bool);
	void update(Event&, RenderWindow&);
	virtual void draw(RenderTarget&, RenderStates) const;
	bool getOnClick();
};
