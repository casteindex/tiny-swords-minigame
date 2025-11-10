#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Jugador {
 private:
  sf::Sprite* sprite;
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
  float minX, maxX;
  bool reachedLeft, reachedRight;

 public:
  Jugador(float scale = 1.0f);
  ~Jugador();

  sf::FloatRect getGlobalBounds() const;
  void setPosition(float x, float y);
  void setScale(float factor);
  void setTextures(const std::string& idlePath, const std::string& walkPath);
  void handleInput(float deltaTime);
  void update(float deltaTime);
  void draw(sf::RenderWindow& window);

  void setMovementBounds(float left, float right);
  bool hasReachedLeft() const;
  bool hasReachedRight() const;
};
