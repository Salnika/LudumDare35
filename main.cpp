#include "data.hpp"
#include "back.hpp"
#include "ACharacter.hpp"
#include "Humain.hpp"
#include "Rabbit.hpp"
#include "PowerEvent.hpp"
#include "Hulk.hpp"
#include "Captain.hpp"
#include "JumpObstacle.hpp"
#include "JumpRabbitObstacle.hpp"
#include "JumpDuck.hpp"

#include "Duck.hpp"
#include <ctime>
#include <SFML/Audio.hpp>
#include "DestructibleObstacle.hpp"


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
  return (0);
}

int	newGame(sf::RenderWindow &window)
{
  std::string		score = "";
  sf::Font		font;
  bool			initObs = false;
  sf::Text		scoreText;
  int			currentObstacle;
  if (!font.loadFromFile("ressources/talldark.ttf"))
    return -1;
  scoreText.setFont(font);
  scoreText.setStyle(sf::Text::Bold);
  scoreText.setColor(sf::Color::Yellow);
  scoreText.setCharacterSize(34);
  float			scoreint = 0;
  Background          back1("ressources/background.png", "ressources/ground.png", sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(WIDTH, 64), 0);
  Background          back2("ressources/background.png", "ressources/ground.png", sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(WIDTH, 64), WIDTH);
  PowerEvent          events;

  AObstacle	*obstacle;
  AObstacle *destructibleObstacle = new DestructibleObstacle(1200, 120, "ressources/WallExplosion3.png");
  AObstacle *jumpObstacle = new JumpObstacle(120, 120, "ressources/Fire2.png");
  AObstacle *rabbitObstacle = new JumpRabbitObstacle(1000, 200, "ressources/RabbitJump.png");
  AObstacle *duckObstacle = new JumpDuck(0, 0, "ressources/DuckJump.png");
  //obstacle = jumpObstacle;
  ACharacter	*humain = new Humain();
  ACharacter	*rabbit = new Rabbit();
  ACharacter	*hulk = new Hulk();
  ACharacter	*duck = new Duck();
  ACharacter	*current = humain;
  sf::Time 	timee;
  sf::Clock   	total;
  sf::Clock 	frameClock;
  CHARTYPE      transformation;

  int last_time = 500000;
  window.setFramerateLimit(60);
  srand(time(NULL));
  jumpObstacle->init();
  while (window.isOpen())
    {
      timee = frameClock.restart();
      sf::Event   event;

      while (window.pollEvent(event))
	{
	  if (event.type == sf::Event::Closed ||  event.key.code == sf::Keyboard::Escape)
	    return 0;
	  if (events.getDisplay())
	    {
	      if (event.type == sf::Event::KeyPressed)
		{
		  if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Q)
		    transformation = events.getBlockType(1, window);
		  else if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::W)
		    transformation = events.getBlockType(2, window);
		  else if (event.key.code == sf::Keyboard::E)
		    transformation = events.getBlockType(3, window);
		  else if (event.key.code == sf::Keyboard::R)
		    transformation = events.getBlockType(4, window);

		  switch ((int)transformation)
		    {
		      case 0:
		      current = humain;
		      break;
		      case 1:
		      current = hulk;
		      break;
		      case 2:
		      current = rabbit;
		      break;
		      case 3:
		      current = duck;
		      break;
		    }
		}
	    }
	}
      handleEvents(current, timee, back1, back2);
      if (total.getElapsedTime().asMicroseconds() - last_time > 6000000 && !events.getDisplay())
	{
         currentObstacle = events.newEvent(window);
	  initObs = true;
           switch (currentObstacle) {
             case 0:
             obstacle = destructibleObstacle;
             obstacle->init();
             break;
             case 1:
             obstacle = duckObstacle;
             obstacle->init();
             break;
             case 3:
             obstacle = rabbitObstacle;
             obstacle->init();
             break;
	    }
	  last_time = total.getElapsedTime().asMicroseconds();
	}
      else if (jumpObstacle->getAnimatedSprite().getPosition().x < 0 && rand() % 200 == 30)
	jumpObstacle->init();

      window.clear();
      back1.update(window);
      back2.update(window);
      if (initObs)
	{
	  obstacle->update(window, timee);
          if (obstacle->checkPlayerCollision(*current) == true)
	    return 0;
	  if (obstacle->getAnimatedSprite().getPosition().x < current->getAnimatedSprite().getPosition().x - 300 )
	    current = humain;
	  if (obstacle->getAnimatedSprite().getPosition().x > jumpObstacle->getAnimatedSprite().getPosition().x - 500 && obstacle->getAnimatedSprite().getPosition().x < jumpObstacle->getAnimatedSprite().getPosition().x + 500)
	    jumpObstacle->getAnimatedSprite().setPosition(sf::Vector2f(-1, -1));
	}
      jumpObstacle->update(window, timee);
      if (jumpObstacle->checkPlayerCollision(*current) == true)
        return 0;
      //exit(0);
      score = "Score  " + patch::to_string((int)scoreint);
      scoreText.setString(score);
      window.draw(scoreText);
      events.update(total.getElapsedTime().asMicroseconds(), window);
      current->move(ACharacter::RIGHT, sf::Vector2f(0, 0), timee, back1.getGround(), back2.getGround());
      window.draw(current->getAnimatedSprite());
      window.display();
      scoreint += 0.2;
    }
  return 0;
}

int	 creds_func(sf::RenderWindow & window)
{
  std::string		title = "Credits";
  sf::Font		font;
  sf::Text		Text;
  if (!font.loadFromFile("ressources/talldark.ttf"))
    return -1;

  sf::Text		quit("Back to menu", font, 60);

  quit.setPosition(sf::Vector2f(500, 490));

  Text.setFont(font);
  Text.setStyle(sf::Text::Bold);
  Text.setColor(sf::Color::Yellow);
  Text.setPosition(525, 50);

  int	textSize	= 64;
  bool	isGrowing = true;
  sf::Vector2f	textPos;
  sf::Vector2i	mousePos;
  textPos.x = 525;
  textPos.y = 50;
  Text.setCharacterSize(64);
  Text.setString(title);
  //  newGame(window);

  Background         back1("ressources/background.png", "ressources/ground.png", sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(WIDTH, 64), 0);
  Background         back2("ressources/background.png", "ressources/ground.png", sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(WIDTH, 64), WIDTH);
  while (window.isOpen())
    {
      sf::Event   event;

      while (window.pollEvent(event))
	{
	  if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
	    window.close();
	  mousePos = sf::Mouse::getPosition();
	  if (mousePos.x > 500 && mousePos.x < 650)
	    {
	      if (mousePos.y > 590 && mousePos.y < 650)
		{
		  quit.setColor(sf::Color::Red);
		  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		    return 0;
		}
	      else
		quit.setColor(sf::Color::White);
	    }
	}

      if (isGrowing)
	{
	  textPos.x--;
	  textSize++;
	}
      else
	{
	  textPos.x++;
	  textSize--;
	}
      if (textSize == 94)
	isGrowing = false;
      else if (textSize == 54)
	isGrowing = true;
      Text.setCharacterSize(textSize);
      Text.setPosition(textPos);

      window.clear();
      back1.update(window);
      back2.update(window);
      window.draw(Text);
      window.draw(quit);
      window.display();
    }
  return 0;
}


int main()
{
  sf::RenderWindow    window(sf::VideoMode(WIDTH,HEIGHT), "Endless Shifter");
  std::string		title = "<Insert Title here>";
  sf::Font		font;
  sf::Text		Text;
  if (!font.loadFromFile("ressources/talldark.ttf"))
    return -1;

  sf::Text		play("Play", font, 60);
  sf::Text		creds("Credit", font, 60);
  sf::Text		quit("Exit", font, 60);

  play.setPosition(sf::Vector2f(500, 200));
  creds.setPosition(sf::Vector2f(500, 270));
  quit.setPosition(sf::Vector2f(500, 340));

  Text.setFont(font);
  Text.setStyle(sf::Text::Bold);
  Text.setColor(sf::Color::Yellow);
  Text.setPosition(425, 50);

  int	textSize	= 64;
  bool	isGrowing = true;
  sf::Vector2f	textPos;
  sf::Vector2i	mousePos;
  textPos.x = 425;
  textPos.y = 50;
  Text.setCharacterSize(64);
  Text.setString(title);
  //  newGame(window);

  Background          back1("ressources/background.png", "ressources/ground.png", sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(WIDTH, 64), 0);
  Background          back2("ressources/background.png", "ressources/ground.png", sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(WIDTH, 64), WIDTH);
  sf::SoundBuffer buffer;
  sf::Sound sound;

  window.setFramerateLimit(60);
  srand(time(NULL));
  if (!buffer.loadFromFile("ressources/Avengers.wav"))
    return -1;
  sound.setBuffer(buffer);
  sound.setLoop(true);
  sound.play();
  while (window.isOpen())
    {
      sf::Event   event;

      while (window.pollEvent(event))
	{
	  if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
	    window.close();
	  mousePos = sf::Mouse::getPosition();
	  if (mousePos.x > 500 && mousePos.x < 650)
	    {
	      if (mousePos.y > 300 && mousePos.y < 360)
		{
		  play.setColor(sf::Color::Red);
		  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		    newGame(window);
		}
	      else
		play.setColor(sf::Color::White);
	      if (mousePos.y > 370 && mousePos.y < 430)
		{
		  creds.setColor(sf::Color::Red);
		  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                                        creds_func(window);
		}
	      else
		creds.setColor(sf::Color::White);
	      if (mousePos.y > 440 && mousePos.y < 500)
		{
		  quit.setColor(sf::Color::Red);
		  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		    window.close();
		}
	      else
                                quit.setColor(sf::Color::White);
	    }
	}

      if (isGrowing)
	{
	  textPos.x--;
	  textSize++;
	}
      else
                {
	  textPos.x++;
	  textSize--;
	}
      if (textSize == 94)
	isGrowing = false;
      else if (textSize == 54)
	isGrowing = true;
                Text.setCharacterSize(textSize);
      Text.setPosition(textPos);

      window.clear();
      back1.update(window);
      back2.update(window);
      window.draw(Text);
      window.draw(play);
      window.draw(creds);
      window.draw(quit);
      window.display();
      window.setFramerateLimit(60);
    }
        return 0;
}
