#include "catch.hpp"
#include "lib.hpp"

///проверка того что цели появляются
TEST_CASE("Spawn enemy tests") {
    Game g;
    g.spawnCircle();
    CHECK(g.testSpawn() == 1);
    g.spawnCircle();
    CHECK(g.testSpawn() == 2);
}

///проверка того что цели остаются для остальных операций с ними(таких как движение) и исчезают нанося урон игроку
TEST_CASE("DeSpawn enemy tests") {
    Game g;
    for (int i = 0; i < 5; i++) {
        g.spawnCircle(); ///призывает 5 целей(5 единиц урона)
    }
    for (int i = 0; i < 161; i++) {
        g.updateEnemies();
        g.update();
    }
    CHECK(g.testUpdateE() == 15);
}

///проверка того что игра заканчивается при отрицательном здоровье
TEST_CASE("EndGame") {
    Game g;
    for (int i = 0; i < 20; i++) {
        g.spawnCircle();
    }
    for (int i = 0; i < 161; i++) {
        g.updateEnemies();
        g.update();
    }
    CHECK(g.testE() == true);
}