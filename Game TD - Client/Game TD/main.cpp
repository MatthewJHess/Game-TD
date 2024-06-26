#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <memory>
#include "Game.h"

enum class Screen { Menu, Settings, Game };
Screen currentScreen = Screen::Menu;

const std::string SERVER_IP = "127.0.0.1";  // Change to your server's IP address
const unsigned short SERVER_PORT = 53000;

int main() {
    std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "FreakyTD - Client");

    window->setFramerateLimit(144);

    // Connect to server
    sf::TcpSocket socket;
    if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Done) {
        std::cerr << "Error connecting to server!" << std::endl;
        return -1;
    }
    std::cout << "Connected to server!" << std::endl;
    sf::Packet packet;
    int type = 1;
    if (socket.receive(packet) == sf::Socket::Done) { //for which player this client is
        packet >> type;
    }
    // Menu screen elements
    sf::Font font;
    font.loadFromFile("Fonts/OLDENGL.TTF");

    sf::Text title("FreakyTD", font, 80);
    title.setPosition(window->getSize().x / 2 - title.getGlobalBounds().width / 2, 100);

    sf::Text playButtonText("Play", font, 50);
    playButtonText.setPosition(window->getSize().x / 2 - playButtonText.getGlobalBounds().width / 2, 300);

    sf::Text settingsButtonText("Settings", font, 50);
    settingsButtonText.setPosition(window->getSize().x / 2 - settingsButtonText.getGlobalBounds().width / 2, 400);

    sf::Text exitButtonText("Exit", font, 50);
    exitButtonText.setPosition(window->getSize().x / 2 - exitButtonText.getGlobalBounds().width / 2, 500);

    // Settings screen elements
    sf::Text musicText("Music Volume", font, 50);
    musicText.setPosition(100, 100);

    sf::RectangleShape musicSlider(sf::Vector2f(300, 10));
    musicSlider.setPosition(100, 200);

    sf::CircleShape musicSliderHandle(10);
    musicSliderHandle.setPosition(250, 195);  // Example position

    sf::Text soundText("Sound Volume", font, 50);
    soundText.setPosition(100, 300);

    sf::RectangleShape soundSlider(sf::Vector2f(300, 10));
    soundSlider.setPosition(100, 400);

    sf::CircleShape soundSliderHandle(10);
    soundSliderHandle.setPosition(250, 395);  // Example position

    sf::Text backButtonText("Back", font, 50);
    backButtonText.setPosition(100, 500);

    // Initialize game
    Game game(window, socket, type);

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }

            if (currentScreen == Screen::Menu) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

                    if (playButtonText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        std::cout << "Play button clicked!" << std::endl;  // Debug message
                        currentScreen = Screen::Game;
                    }
                    else if (settingsButtonText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        currentScreen = Screen::Settings;
                    }
                    else if (exitButtonText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        window->close();
                    }
                }
            }
            else if (currentScreen == Screen::Settings) {
                // Add slider functionality
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

                    if (backButtonText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        currentScreen = Screen::Menu;
                    }
                }
            }
            else if (currentScreen == Screen::Game) {
                game.pollEvents();
            }
        }

        window->clear();

        if (currentScreen == Screen::Game) {
            game.update();
            game.render();
        }
        else if (currentScreen == Screen::Menu) {
            window->draw(title);
            window->draw(playButtonText);
            window->draw(settingsButtonText);
            window->draw(exitButtonText);
        }
        else if (currentScreen == Screen::Settings) {
            window->draw(musicText);
            window->draw(musicSlider);
            window->draw(musicSliderHandle);
            window->draw(soundText);
            window->draw(soundSlider);
            window->draw(soundSliderHandle);
            window->draw(backButtonText);
        }

        window->display();
    }

    return 0;
}
