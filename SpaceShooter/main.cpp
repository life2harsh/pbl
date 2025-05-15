#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include "player.h"
#include "enemy.h"
#include "asteroid.h"

enum class GameState { StartScreen, Playing, Paused, GameOver };

int main() {

    constexpr unsigned W = 1920, H = 1080;
    sf::RenderWindow window(sf::VideoMode({ W, H }), "Space Shooter");
    window.setFramerateLimit(60);

    bool gameOver = false;

    int score = 0, highscore = 0;
    {
        std::ifstream in("highscore.txt");
        if (in.is_open()) in >> highscore;
    }

    sf::Texture bgTex;
    if (!bgTex.loadFromFile("H:/space.png")) {
        std::cerr << "Failed to load space backdrop!\n";
        return -1;
    }
    bgTex.setRepeated(true);
    sf::Sprite background(bgTex);
    background.setTextureRect(sf::IntRect({ 0, 0 }, { static_cast<int>(W), static_cast<int>(H) }));

    float scrollOffset = 0.f;
    const float scrollSpeed = 320.f;

    Player player;
    if (!player.load("H:/shooter.png")) {
        std::cerr << "Failed to load player texture.\n";
        return -1;
    }

    sf::Texture enemyTex, asteroidTex;
    if (!enemyTex.loadFromFile("H:/enemy.png") ||
        !asteroidTex.loadFromFile("H:/asteroid.png"))
    {
        std::cerr << "Failed to load enemy or asteroid textures.\n";
        return -1;
    }

    sf::Font font;
    if (!font.openFromFile("H:/arial.ttf")) {
        std::cerr << "Failed to open font\n";
        return -1;
    }
    sf::Text scoreText(font, ""), highScoreText(font, "");
    scoreText.setCharacterSize(32);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({ 20.f, 20.f });
    highScoreText.setCharacterSize(28);
    highScoreText.setFillColor(sf::Color(200, 200, 200));
    highScoreText.setPosition({ 20.f, 60.f });

    sf::Text startScreenText(font, "SPACE SHOOTER\n\nPress Enter to Start");
    startScreenText.setCharacterSize(64);
    startScreenText.setFillColor(sf::Color::White);
    {
        auto b = startScreenText.getLocalBounds();
        startScreenText.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
        startScreenText.setPosition({ W / 2.f, H / 2.f });
    }

    sf::Text pauseText(font, "Game Paused\nPress Enter to Resume\nPress R to Restart\nPress Escape to Quit");
    pauseText.setCharacterSize(64);
    pauseText.setFillColor(sf::Color::White);
    {
        auto b = pauseText.getLocalBounds();
        pauseText.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
        pauseText.setPosition({ W / 2.f, H / 2.f });
    }

    std::vector<Enemy> enemies;
    std::vector<Asteroid> asteroids;

    sf::Clock clock;
    float dt = 0.f;

    float enemyTimer = 0.f;
    const float enemyInterval = 1.0f;
    int level = 1, spawnedThisWave = 0;
    const int enemiesPerWave = 5;

    float asteroidTimer = 0.f;
    const float asteroidInterval = 2.5f;

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> xDist(0.f, static_cast<float>(W));

    const float shootCooldown = 0.2f;
    float shootTimer = shootCooldown;

    GameState state = GameState::StartScreen;

    while (window.isOpen()) {
        dt = clock.restart().asSeconds();

        while (auto ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (auto kp = ev->getIf<sf::Event::KeyPressed>()) {
                if (state == GameState::GameOver) {
                    if (kp->code == sf::Keyboard::Key::Enter) {
                        player = Player();
                        if (!player.load("H:/shooter.png")) {
                            std::cerr << "Failed to load player texture on restart.\n";
                            return -1;
                        }
                        enemies.clear();
                        asteroids.clear();
                        score = 0;
                        level = 1;
                        spawnedThisWave = 0;
                        gameOver = false;
                        state = GameState::Playing;
                    }
                    else if (kp->code == sf::Keyboard::Key::Escape) {
                        window.close();
                    }
                }
                else if (state == GameState::Playing) {
                    if (kp->code == sf::Keyboard::Key::Escape) {
                        state = GameState::Paused;
                    }

                }
                else if (state == GameState::Paused) {
                    if (kp->code == sf::Keyboard::Key::Enter) {
                        state = GameState::Playing;
                    }
                    else if (kp->code == sf::Keyboard::Key::R) {

                        player = Player();
                        if (!player.load("H:/shooter.png")) {
                            std::cerr << "Failed to load player texture on restart.\n";
                            return -1;
                        }
                        enemies.clear();
                        asteroids.clear();
                        score = 0;
                        level = 1;
                        spawnedThisWave = 0;
                        gameOver = false;
                        state = GameState::Playing;
                    }
                    else if (kp->code == sf::Keyboard::Key::Escape) {
                        window.close();
                    }
                }
                else if (state == GameState::StartScreen) {
                    if (kp->code == sf::Keyboard::Key::Enter) {
                        state = GameState::Playing;
                    }
                }
            }
        }
        scrollOffset -= scrollSpeed * dt;
        if (scrollOffset <= 0.f)
            scrollOffset = std::fmod(scrollOffset + bgTex.getSize().y, bgTex.getSize().y);
        background.setTextureRect(sf::IntRect(
            { 0, static_cast<int>(scrollOffset) },
            { static_cast<int>(W), static_cast<int>(H) }
        ));

        if (state == GameState::Playing) {
            player.handleInput();
            shootTimer += dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)
                && shootTimer >= shootCooldown)
            {
                player.shoot();
                shootTimer = 0.f;
            }
            player.updateBullet();

            enemyTimer += dt;
            if (enemyTimer >= enemyInterval && spawnedThisWave < level * enemiesPerWave) {
                enemyTimer = 0.f;
                const int rows = 2, cols = 5;
                const float sx = 140.f, sy = 140.f;
                for (int r = 0; r < rows; ++r) {
                    for (int c = 0; c < cols; ++c) {
                        float x = (W / 2.f) - ((cols - 1) * sx) / 2.f + c * sx;
                        float y = -200.f - r * sy;
                        enemies.emplace_back(enemyTex, sf::Vector2f(x, y), 100.f + 20.f * level);
                        spawnedThisWave++;
                        if (spawnedThisWave >= level * enemiesPerWave) break;
                    }
                    if (spawnedThisWave >= level * enemiesPerWave) break;
                }
            }
            if (spawnedThisWave >= level * enemiesPerWave && enemies.empty()) {
                level++;
                spawnedThisWave = 0;
            }

            const size_t maxAsteroids = 5;
            asteroidTimer += dt;
            if (asteroidTimer >= asteroidInterval && asteroids.size() < maxAsteroids) {
                asteroidTimer = 0.f;
                float x = xDist(rng);
                float asteroidSpeed = 80.f + level * 5.f;
                asteroids.emplace_back(asteroidTex, sf::Vector2f(x, -60.f), asteroidSpeed);
            }

            for (auto& e : enemies)    e.update(dt);
            for (auto& a : asteroids)  a.update(dt);

            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                [&](auto& e) { return e.isOffScreen(H); }), enemies.end());
            asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(),
                [&](auto& a) { return a.isOffScreen(H); }), asteroids.end());

            auto& bullets = player.bullets;
            for (size_t i = 0; i < bullets.size(); ++i) {
                sf::FloatRect bb = bullets[i].shape.getGlobalBounds();
                for (size_t j = 0; j < enemies.size(); ++j) {
                    if (bb.findIntersection(enemies[j].getBounds()).has_value()) {
                        score += 10;
                        enemies.erase(enemies.begin() + j);
                        bullets.erase(bullets.begin() + i);
                        --i;
                        break;
                    }
                }
            }

            scoreText.setString("Score: " + std::to_string(score));
            highScoreText.setString("High Score: " + std::to_string(highscore));
        }

        window.clear();
        window.draw(background);

        if (state == GameState::StartScreen) {
            window.draw(startScreenText);
        }
        else if (state == GameState::Playing || state == GameState::Paused) {

            const float shrinkFactor = 0.5f;
            sf::FloatRect pb = player.getBounds();
            for (const auto& e : enemies) {
                auto eb = e.getBounds();
                sf::Vector2f shrink(eb.size.x * shrinkFactor, eb.size.y * shrinkFactor);
                sf::FloatRect hb{
                    eb.position + shrink * 0.5f,
                    eb.size - shrink
                };

                if (pb.findIntersection(hb).has_value()) {
                    state = GameState::GameOver;
                    break;
                }
            }

            if (state == GameState::Playing) {
                for (const auto& a : asteroids) {
                    auto ab = a.getBounds();
                    sf::Vector2f shrink(ab.size.x * shrinkFactor, ab.size.y * shrinkFactor);
                    sf::FloatRect hb{
                        ab.position + shrink * 0.5f,
                        ab.size - shrink
                    };

                    if (pb.findIntersection(hb).has_value()) {
                        state = GameState::GameOver;
                        break;
                    }
                }
            }

            player.draw(window);

            sf::FloatRect playerHitboxBounds = player.getBounds();
            const float playerShrinkFactor = 0.01f;
            sf::Vector2f playerShrink(playerHitboxBounds.size.x * playerShrinkFactor, playerHitboxBounds.size.y * playerShrinkFactor);
            sf::FloatRect hitbox{
                playerHitboxBounds.position + playerShrink * 0.1f,
                playerHitboxBounds.size - playerShrink
            };

            for (auto& e : enemies)    e.draw(window);
            for (auto& a : asteroids)  a.draw(window);
            window.draw(scoreText);
            window.draw(highScoreText);

            if (state == GameState::Paused) {
                sf::RectangleShape overlay({ float(W), float(H) });
                overlay.setFillColor(sf::Color(0, 0, 0, 150));
                window.draw(overlay);
                window.draw(pauseText);
            }
        }

        if (state == GameState::GameOver) {
            sf::Text goText(font, "GAME OVER");
            goText.setCharacterSize(64);
            goText.setFillColor(sf::Color::Red);
            goText.setPosition({ W / 2.f - 150.f, H / 2.f - 100.f });
            window.draw(goText);


            sf::Text goOptions(font, "Press Enter to Restart\nPress Escape to Quit");
            goOptions.setCharacterSize(32);
            goOptions.setFillColor(sf::Color::White);
            auto optB = goOptions.getLocalBounds();
            goOptions.setOrigin({ optB.position.x + optB.size.x / 2.f, optB.position.y + optB.size.y / 2.f });
            goOptions.setPosition({ W / 2.f, H / 2.f + 50.f });
            window.draw(goOptions);

            if (score > highscore) {
                std::ofstream out("highscore.txt");
                if (out.is_open()) out << score;
                highscore = score;
            }
        }

        window.display();
    }
}