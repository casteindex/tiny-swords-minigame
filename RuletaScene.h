#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "Jugador.h"
#include "Ruleta.h"
#include "Scene.h"

class RuletaScene : public Scene {
 private:
  Jugador jugador;
  std::unique_ptr<sf::Sprite> background;
  sf::Texture backgroundTex;

  std::unique_ptr<Ruleta> ruleta;

 public:
  RuletaScene(sf::RenderWindow& window);

  void handleInput(float deltaTime);
  void update(float deltaTime);
  void draw(sf::RenderWindow& window);
};
