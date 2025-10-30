#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "Scene.h"

class CalderoScene : public Scene {
 public:
  CalderoScene(sf::RenderWindow& window);

  void handleInput(float dt);
  void update(float dt);
  void draw(sf::RenderWindow& window);

 private:
  sf::Texture texFondo;
  sf::Texture texCaldero;
  std::unique_ptr<sf::Sprite> spriteFondo;
  std::unique_ptr<sf::Sprite> spriteCaldero;

  float tiempoAcumulado = 0.f;
  int frameActual = 0;
  const int totalFrames = 4;
  const float duracionFrame = 0.25f;  // segundos entre frames

  bool terminado = false;
};
