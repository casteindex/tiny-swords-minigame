#pragma once
#include <SFML/Graphics.hpp>

class Scene {
 public:
  virtual ~Scene() = default;
  virtual void handleInput(float deltaTime) = 0;
  virtual void update(float deltaTime) = 0;
  virtual void draw(sf::RenderWindow& window) = 0;
  virtual bool playerWon() const { return false; }
  virtual bool wantsToChangeScene() const { return false; }
  virtual Scene* nextScene(sf::RenderWindow& window, int& vidasJugador) {
    return nullptr;
  }
};
