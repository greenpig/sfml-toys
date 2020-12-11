#include "Platform/Platform.hpp"

// See README.md for an introduction.

// Use a class to encapsulate player logic and data.
// If this gets too long, then it's a good idea to put this class into its separate .cpp and .hpp files.
class Player : public sf::Drawable, public sf::Transformable
{
private:
	sf::CircleShape body;
	sf::RectangleShape sword;
	const float PI = 3.141592654f;
	const float R = 10.f; // Body radius. Use constant so it is easy to change.
	const float L = 10.f; // Sword length

public:
	// This overrides the drawing behavior, defined in sf::Drawable, our super-class.
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

	// check if we are hit by another player (attacker)
	bool isHit(Player& attacker)
	{
		// Our body: a circle at getPosition().x, getPosition().y, with radius R (10)
		// The sword: start: x2 + R * sin(r2), y2 - R * cos(r2)
		//            tip:   x2 + (L+R) * sin(r2), y2 - (L+R) * sin(r2)
		float x = getPosition().x;
		float y = getPosition().y;

		float r2 = attacker.getRotation() / 180 * PI;
		float xt = attacker.getPosition().x + (L + R) * sin(r2);
		float yt = attacker.getPosition().y - (L + R) * cos(r2);

		// Is the tip inside my body?
		if ((x - xt) * (x - xt) + (y - yt) * (y - yt) < R * R)
		{
			return true;
		}
		return false;
	}

	// Constructor
	Player()
	{
		body.setRadius(R);
		body.setPosition(-R, -R);
		sword.setSize(sf::Vector2f(4.f, L));
		sword.setPosition(-2.f, -R - L);
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
	// setFramerateLimit basically adds a short delay after each call to window.display()
	// Without it our players will move too fast.
	window.setFramerateLimit(60);
	unsigned frame = 0;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		p1.handleKey(sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D);
		p2.handleKey(sf::Keyboard::I, sf::Keyboard::K, sf::Keyboard::J, sf::Keyboard::L);

		// Flicker every 3 frames if we are hit
		if (p1.isHit(p2) && frame % 3 == 0)
			p1.setColor(sf::Color::Cyan);
		else
			p1.setColor(sf::Color::Red);
		if (p2.isHit(p1) && frame % 3 == 0)
			p2.setColor(sf::Color::Yellow);
		else
			p2.setColor(sf::Color::Blue);

		// We redraw everything every frame
		window.clear();
		window.draw(p1);
		window.draw(p2);
		window.display();
		frame++;
	}

	return 0;
}
