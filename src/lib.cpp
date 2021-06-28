#include "lib.hpp"

///Private functions
void Game::initVariables()
{
    this->window = nullptr;

    ///Game logic
    this->start = false;
    this->endGame = false;
    this->isMenu = true;
    this->points = 0;
    this->health = 20;
    this->enemySpawnTimerMax = 20.f;
    this->enemySpawnTimer = 10.f;
    this->maxEnemies = 5;
    this->mouseHeld = false;
}

///Window setting
void Game::initWindow()
{
    this->videoMode = sf::VideoMode(1600, 900);

    this->window = new sf::RenderWindow(this->videoMode, "Re: training", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(60);
}

///Font connection
void Game::initFonts()
{
    if (this->font.loadFromFile("C:/Users/msi-pc/CLionProjects/Re-training/assets/fonts/Dosis-Light.ttf"))
    {
        std::cout << "ERROR::GAME::INITFONTS::Failed to load font!" << "\n";
    }
}

///Font setting
void Game::initText()
{
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(24);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NONE");
}

///Enemies setting
void Game::initEnemies()
{
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setFillColor(sf::Color::Cyan);
}

///Level setting
void Game::initMap()
{
    if (!this->mapBackgroundTex.loadFromFile("C:/Users/msi-pc/CLionProjects/Re-training/assets/images/background1.jpg"))
    {
        std::cout << "ERROR::GAME::COULD NOT LOAD MAP BACKGROUND TEXTURE" << "\n";
    }

    this->mapBackground.setTexture(this->mapBackgroundTex);
}

///Constructors / Destructors
Game::Game()
{
    this->initVariables();
    this->initWindow();
    this->initFonts();
    this->initText();
    this->initEnemies();
    this->initMap();
}

Game::~Game()
{
    delete this->window;
}

///Accessors
const bool Game::running() const
{
    return this->window->isOpen();
}

const bool Game::getEndGame() const
{
    return this->endGame;
}

///Functions
void Game::spawnCircle()
{
    /**
        @return void

        Spawns enemies and sets their types and colors. Spawns them at random positions.
        -Sets a random type (diff).
        -Sets a random position.
        -Sets a random color.
        -Adds enemy to the vector.
    */


    this->enemy.setPosition(
            static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
            0.f
    );

    ///Randomize enemy type
    int type = rand() % 4;

    switch (type)
    {
        case 0:
            this->enemy.setSize(sf::Vector2f(30.f, 30.f));
            this->enemy.setFillColor(sf::Color::Blue);
            break;
        case 1:
            this->enemy.setSize(sf::Vector2f(50.f, 50.f));
            this->enemy.setFillColor(sf::Color::Cyan);
            break;
        case 2:
            this->enemy.setSize(sf::Vector2f(70.f, 70.f));
            this->enemy.setFillColor(sf::Color::Red);
            break;
        case 3:
            this->enemy.setSize(sf::Vector2f(100.f, 100.f));
            this->enemy.setFillColor(sf::Color::Green);
            break;
        default:
            this->enemy.setSize(sf::Vector2f(100.f, 100.f));
            this->enemy.setFillColor(sf::Color::Yellow);
            break;
    }

    ///Spawn the enemy
    this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
    ///Event polling
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::KeyPressed:
                if (this->ev.key.code == sf::Keyboard::Escape)
                    this->window->close();
                break;
        }
    }
}

void Game::updateMousePositions()
{
    /**
        @ return void

        Updates the mouse positions:
        - Mouse position relative to window (Vector2i)
    */

    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
    std::stringstream ss;

    ss << "Points: " << this->points << "\n"
       << "Health: " << this->health << "\n";

    this->uiText.setString(ss.str());
}

void Game::updateEnemies()
{
    /**
        @return void

        Updates the enemy spawn timer and spawns enemies
        When the total amount of enemies is smaller than the maximum.
        Moves the enemies downwards.
        Removes the enemies at the edge of the screen. //TODO
    */

    ///Updating the timer for enemy spawning
    if (this->enemies.size() < this->maxEnemies)
    {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            ///Spawn the enemy and reset the timer
            this->spawnCircle();
            this->enemySpawnTimer = 0.f;
        }
        else
            this->enemySpawnTimer += 1.f;
    }

    ///Moving and updating enemies
    for (int i = 0; i < this->enemies.size(); i++)
    {
        bool deleted = false;

        this->enemies[i].move(0.f, 5.f);

        if (this->enemies[i].getPosition().y > this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
            std::cout << "Health: " << this->health << "\n";
        }
    }

    ///Check if clicked upon
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (!this->mouseHeld)
        {
            this->mouseHeld = true;
            bool deleted = false;
            for (size_t i = 0; i < this->enemies.size() && !deleted; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {
                    //Gain points
                    this->points += 10;

                    std::cout << "Points: " << this->points << "\n";

                    //Delete the enemy
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);
                }
            }
        }
    }
    else
    {
        this->mouseHeld = false;
    }
}

///Update window
void Game::update()
{
    this->pollEvents();

    if (!this->isMenu)
    {

    }
    if (!this->endGame)
    {
        this->updateMousePositions();

        this->updateText();

        this->updateEnemies();
    }

    ///End game condition
    if (this->health <= 0)
        this->endGame = true;
}

void Game::renderText(sf::RenderTarget& target)
{
    target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
    ///Rendering all the enemies
    for (auto &e : this->enemies)
    {
        target.draw(e);
    }
}

void Game::renderMap()
{
    this->window->draw(this->mapBackground);
}

void Game::renderMenu()
{
    ///Load font
    if (!this->font.loadFromFile("C:/Users/msi-pc/CLionProjects/Re-training/assets/fonts/PixellettersFull.ttf"))
        std::cout << "ERROR::GAME::Failed to load font" << "\n";

    ///Init point text
    this->pointText.setPosition(700.f, 25.f);
    this->pointText.setFont(this->font);
    this->pointText.setCharacterSize(20);
    this->pointText.setFillColor(sf::Color::White);
    this->pointText.setString("test");

    ///Init GameOver text
    this->gameOverText.setFont(this->font);
    this->gameOverText.setCharacterSize(60);
    this->gameOverText.setFillColor(sf::Color::Red);
    this->gameOverText.setString("Game Over!");
    this->gameOverText.setPosition(
            this->window->getSize().x / 2.f - this->gameOverText.getGlobalBounds().width / 2.f,
            this->window->getSize().y / 2.f - this->gameOverText.getGlobalBounds().height / 2.f);

    sf::Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
    menuTexture1.loadFromFile(R"(C:\Users\msi-pc\CLionProjects\Re-training\assets\images\menuwhite.png)", sf::IntRect (0, 0, 300, 50));
    menuTexture2.loadFromFile(R"(C:\Users\msi-pc\CLionProjects\Re-training\assets\images\menuwhite.png)", sf::IntRect (0, 50, 300, 50));
    menuTexture3.loadFromFile(R"(C:\Users\msi-pc\CLionProjects\Re-training\assets\images\menuwhite.png)", sf::IntRect (0, 100, 300, 50));
    menuBackground.loadFromFile(R"(C:\Users\msi-pc\CLionProjects\Re-training\assets\images\background.jpg)");
    sf::Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
    int menuNum = 0;

    menu1.setPosition(100, 30);
    menu2.setPosition(100, 90);
    menu3.setPosition(100, 150);
    menuBg.setPosition(0, 0);

    menu1.setColor(sf::Color::White);
    menu2.setColor(sf::Color::White);
    menu3.setColor(sf::Color::White);
    menuNum = 0;
    this->window->clear(sf::Color(129, 181, 221));
    if (sf::IntRect(100, 30, 300, 50).contains(sf::Mouse::getPosition(*this->window))) { menu1.setColor(sf::Color::Blue); menuNum = 1; }
    if (sf::IntRect(100, 90, 300, 50).contains(sf::Mouse::getPosition(*this->window))) { menu2.setColor(sf::Color::Blue); menuNum = 2; }
    if (sf::IntRect(100, 150, 300, 50).contains(sf::Mouse::getPosition(*this->window))) { menu3.setColor(sf::Color::Blue); menuNum = 3; }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (menuNum == 1) { isMenu = false; start = true;}//если нажали первую кнопку, то выходим из меню и играем
        if (menuNum == 2) { this->window->draw(about); }
        if (menuNum == 3)  { isMenu = false; this->window->close(); }
    }

    this->window->draw(menuBg);
    this->window->draw(menu1);
    this->window->draw(menu2);
    this->window->draw(menu3);
}

void Game::render()
{
    /**
        @return void

        - clear old frame
        - render objects
        - display frame in window

        Renders the game objects.
    */

    this->window->clear();

    if (this->isMenu) {
        this->renderMenu();
    }

    if (this->start) {
        ///Draw level
        this->renderMap();

        ///Draw game objects
        this->renderEnemies(*this->window);
        this->renderText(*this->window);
    }

    ///Game over screen
    if (this->health <= 0)
    {
        this->window->draw(this->gameOverText);
    }

    this->window->display();
}
