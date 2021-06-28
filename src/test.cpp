#include "catch.hpp"
#include "lib.hpp"

///создано для того чтобы извлечь часные переменные
auto Game::testSpawn()
{
    Game::spawnCircle();
    Game::spawnCircle();
    return enemies.size();
}

///создано для того чтобы извлечь часные переменные
auto Game::testUpdateE()
{
    Game::spawnCircle();
    for (int i = 0; i < this->enemies.size(); i++)
    {
        bool deleted = false;
        ///Moving enemy lower
        enemies[i].move(0.f, 801.f);
        ///Check out of window
        if (enemies[i].getPosition().y > 800.f)
        {
            enemies.erase(enemies.begin() + i);
            health -= 1;
            std::cout << "Health: " << health << "\n";
        }
    }
    return health;
}

auto Game::testE()
{
    Game::update();
    return endGame;
}

///проверка того что цели появляются
TEST_CASE("Spawn enemy tests") {
    Game g;
    sf::RectangleShape enemy;
    CHECK(g.testSpawn() == 2);
    CHECK(g.testSpawn() == 4);
}

///проверка того что цели появляются и остаются для остальных операций с ними(таких как движение) и исчезают нанося урон игроку
TEST_CASE("DeSpawn enemy tests") {
    Game g;
    sf::RectangleShape enemy;
    CHECK(g.testUpdateE() == 19);
    CHECK(g.testUpdateE() == 18);
}

///проверка того что игра заканчивается при отрицательном здоровье
TEST_CASE("EndGame") {
    Game g;
    sf::RectangleShape enemy;
    for (int i = 0; i < 20; i++) {
        CHECK(g.testUpdateE() == 19-i);
    }
    CHECK(g.testE() == true);
}