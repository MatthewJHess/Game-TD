#include <iostream>

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>

using namespace sf

int main()
{
	//Window
	RenderWindow window(VideoMode(600, 600), "TD Game", Style::Titlebar | Style::Close);
	Event ev;
	CircleShape shape(300.f);
	shape.setFillColor(Color::Green);

	//Game loop
	while (window.isOpen()) 
	{
		//Event polling
		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
			case Event::Closed:
				window.close();
				break;
				case Event::KeyPressed;
					if (ev.key.code == Keyboard::Escape)
						window.close();
					break;
			}
		}

		//Update

		//Render
		window.clear(); //clear frame

		//Draw game
		window.draw(shape);

		window.display(); //Tell app done drawing
	}
	return 0;
}