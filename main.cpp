#include "data.hpp"
#include "back.hpp"
#include "ACharacter.hpp"
#include "Humain.hpp"
#include "Rabbit.hpp"
#include "PowerEvent.hpp"
#include "Hulk.hpp"
#include "Captain.hpp"
#include <ctime>

int handleEvents(ACharacter *character, const sf::Time& frameTime, Background &back1, Background &back2)
{
    sf::Vector2f movement(0.f, 0.f);
    static int jumped;

    if (character->getAnimatedSprite().getPosition().y < character->getJumpHeight())
	jumped = 1;
    else if (jumped != 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && character->getAnimatedSprite().getPosition().y > character->getJumpHeight())
    {
	movement.y -= character->getSpeed();
	character->move(ACharacter::UP, movement, frameTime, back1.getGround(), back2.getGround());
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	jumped = 1;
    if (character->collide(back1.getGround(), back2.getGround()) == true)
	jumped = 0;
}

int main()
{
    sf::RenderWindow    window(sf::VideoMode(WIDTH,HEIGHT), "SFML");
    Background          back1("ressources/background.png", "ressources/ground.png", sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(WIDTH, 64), 0);
    Background          back2("ressources/background.png", "ressources/ground.png", sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(WIDTH, 64), WIDTH);
    PowerEvent          events;
    ACharacter *humain = new Rabbit();
    sf::Time timee;
    sf::Clock   total;
    sf::Clock frameClock;
    CHARTYPE   transformation;

    int last_time = 500000;

    window.setFramerateLimit(60);

    srand(time(NULL));
    while (window.isOpen())
	{
	    timee = frameClock.restart();
	    sf::Event   event;

	   while (window.pollEvent(event))
	    {
		if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
		    window.close();
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
		    events.newEvent();
	if (events.getDisplay())
	    {
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Num1)
			    transformation = events.getBlockType(1);
			else if (event.key.code == sf::Keyboard::Num2)
			    transformation = events.getBlockType(2);
			else if (event.key.code == sf::Keyboard::Num3)
			    transformation = events.getBlockType(3);
			else if (event.key.code == sf::Keyboard::Num4)
			    transformation = events.getBlockType(4);
		}
	    }
	    }
	    handleEvents(humain, timee, back1, back2);
	    if (total.getElapsedTime().asMicroseconds() - last_time > 6000000 && !events.getDisplay())
	{
	    events.newEvent();
	    last_time = total.getElapsedTime().asMicroseconds();
	}

	    window.clear();
	    back1.update(window);
	    back2.update(window);
	    events.update(total.getElapsedTime().asMicroseconds(), window);
	    humain->move(ACharacter::RIGHT, sf::Vector2f(0, 0), timee, back1.getGround(), back2.getGround());
	    window.draw(humain->getAnimatedSprite());
	    window.display();
	}
    return 0;
}
