#include "Platform/Platform.hpp"

class Player : public sf::Drawable, public sf::Transformable
{
private:
	sf::CircleShape body;
	sf::RectangleShape sword;
	const float PI = 3.141592654f;

public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(body, states);
		target.draw(sword, states);
	}

	void setColor(sf::Color color)
	{
		body.setFillColor(color);
		sword.setFillColor(color);
	}

	void handleKey(sf::Keyboard::Key forward, sf::Keyboard::Key back, sf::Keyboard::Key left, sf::Keyboard::Key right)
	{
		if (sf::Keyboard::isKeyPressed(left))
			rotate(-5.f);
		if (sf::Keyboard::isKeyPressed(right))
			rotate(5.f);
		if (sf::Keyboard::isKeyPressed(forward))
		{
			float r = getRotation() / 180 * PI;
			move(sin(r), -cos(r));
		}
		if (sf::Keyboard::isKeyPressed(back))
		{
			float r = getRotation() / 180 * PI;
			move(-sin(r), cos(r));
		}
	}

	Player()
	{
		body.setRadius(10.f);
		body.setPosition(-10.f, -10.f);
		sword.setSize(sf::Vector2f(4.f, 10.f));
		sword.setPosition(-2.f, -20.f);
	}
};

int main()
{
	util::Platform platform;

#if defined(_DEBUG)
	std::cout << "Hello World!" << std::endl;
#endif

	sf::RenderWindow window;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	window.create(sf::VideoMode(600.0f * screenScalingFactor, 600.0f * screenScalingFactor), "SFML works!");
	platform.setIcon(window.getSystemHandle());

	Player p1, p2;
	p1.setColor(sf::Color::Red);
	p2.setColor(sf::Color::Blue);
	p1.setPosition(200.f, 100.f);
	p2.setPosition(400.f, 100.f);
	p2.setRotation(90.f);

	sf::Event event;
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		p1.handleKey(sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D);
		p2.handleKey(sf::Keyboard::I, sf::Keyboard::K, sf::Keyboard::J, sf::Keyboard::L);

		window.clear();
		window.draw(p1);
		window.draw(p2);
		window.display();
	}

	return 0;
}
