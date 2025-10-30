#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

class Jugador {
 private:
  std::unique_ptr<sf::Sprite> sprite;
  sf::Texture textureIdle;
  sf::Texture textureWalk;

  sf::IntRect frameRect;
  float frameTime;
  float currentTime;
  int currentFrame;
  int totalFrames;

  bool isWalking;
  bool facingRight;

  float scaleFactor;
  float speed;

 public:
  Jugador(float scale = 1.0f);

  void setPosition(float x, float y);
  void setScale(float factor);
  void handleInput(float deltaTime);
  void update(float deltaTime);
  void draw(sf::RenderWindow& window);
};
