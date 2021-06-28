cmake_minimum_required(VERSION 3.17)

add_executable(game_test test.cpp lib.cpp)
# ^ Взять test.cpp и lib.cpp, собрать вместе, назвать game_test (game_test.exe)
target_link_libraries(game_test PUBLIC doctest::doctest default sfml-graphics sfml-audio)
# ^ Использовать в game_test библиотеки doctest::doctest и default
doctest_discover_tests(game_test)
# ^ Прочитать из game_test список тестов

add_executable(game main.cpp lib.cpp)
target_link_libraries(game PUBLIC default CONAN_PKG::fmt sfml-graphics sfml-audio)
target_link_libraries(game PUBLIC default sfml-graphics sfml-audio)