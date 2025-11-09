#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class NPC {
 private:
  sf::Texture texture;
  sf::Sprite* sprite;

  sf::IntRect frameRect;
  float frameTime;
  float currentTime;
  int currentFrame;
  int totalFrames;
  bool facingRight;
  float scaleFactor;

 public:
  NPC(const std::string& texturePath, float scale = 3.0f,
      bool facingLeft = false, float frameDuration = 0.12f);
  ~NPC();

  void update(float deltaTime);  // avanzar animación
  void draw(sf::RenderWindow& window) const;
  void setPosition(float x, float y);
  sf::FloatRect getGlobalBounds() const;
};
