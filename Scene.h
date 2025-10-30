#pragma once
#include <SFML/Graphics.hpp>

class Scene {
 public:
  virtual void handleInput(float deltaTime) = 0;
  virtual void update(float deltaTime) = 0;
  virtual void draw(sf::RenderWindow& window) = 0;
  virtual ~Scene() = default;
};
