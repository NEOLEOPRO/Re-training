#pragma once
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <time.h>
#include <vector>
#include <sstream>
#include <map>

///SFML
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>


/**
	Class that acts as the game engine.
	Wrapper class.
*/

class Game
{
private:
    ///Variables
    ///Window and menu
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;
    sf::Text gameOverText;
    sf::Text pointText;

    ///Map
    sf::Texture mapBackgroundTex;
    sf::Sprite mapBackground;

    ///Mouse positions
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    ///Resources
    sf::Font font;
    std::map<std::string, sf::Texture*> textures;

    ///Text
    sf::Text uiText;

    ///Game logic
    bool endGame;
    bool start;
    bool isMenu;
    unsigned points;
    int health;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    int maxEnemies;
    bool mouseHeld;

    ///Game objects
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape enemy;

    ///Private functions
    void initVariables();
    void initWindow();
    void initFonts();
    void initText();
    void initEnemies();
    void initMap();

public:
    ///Constructors / Destructors
    Game();
    virtual ~Game();

    ///Accessors
    const bool running() const;
    const bool getEndGame() const;

    ///Functions
    void spawnCircle();
    void pollEvents();

    ///changing parameters
    void updateMousePositions();
    void updateText();
    void updateEnemies();
    void update();

    ///rendering
    void renderText(sf::RenderTarget& target);
    void renderEnemies(sf::RenderTarget& target);
    void renderMap();
    void renderMenu();
    void render();

    ///tests
    auto testSpawn();
    auto testUpdateE();
    auto testE();
};

///Class that cuts the image into animation
class Animation
{
public:
    float Frame, speed;
    sf::Sprite sprite;
    std::vector<sf::IntRect> frames;

    Animation(){}
    /**
     \param t - loaded animation-image(2 or more sprites)
     \param x - top left corner of image
     \param y - top left corner of image
     \param w - weight
     \param h - height
     \param count - number of pieces
     \param Speed - speed
    */

    Animation (sf::Texture &t, int x, int y, int w, int h, int count, float Speed)
    {
        Frame = 0;
        speed = Speed;

        for (int i=0;i<count;i++)
            frames.push_back(sf::IntRect(x+i*w, y, w, h)  );

        sprite.setTexture(t);
        sprite.setOrigin(w/2,h/2);
        sprite.setTextureRect(frames[0]);
    }

    ///goes to the next piece
    void update()
    {
        Frame += speed;
        int n = frames.size();
        if (Frame >= n) Frame -= n;
        if (n>0) sprite.setTextureRect( frames[int(Frame)] );
    }

    ///End or not
    bool isEnd()
    {
        return Frame+speed>=frames.size();
    }

};
