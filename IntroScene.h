#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "RuletaScene.h"
#include "Scene.h"

class IntroScene : public Scene {
 private:
  sf::RenderWindow& window;
  int& vidasJugador;

  std::vector<sf::Texture> fondos;
  sf::Sprite* spriteFondo = nullptr;
  sf::Font fuente;
  sf::Text* textoSaltar = nullptr;
  sf::RectangleShape overlay;

  sf::Music narracion;

  float tiempoTotal;
  float tiempoEnImagen;
  float duracionPorImagen;
  int imagenActual;
  bool terminado;
  bool skip;

 public:
  IntroScene(sf::RenderWindow& window, int& vidasJugador);
  ~IntroScene() override;

  void handleInput(float dt) override;
  void update(float dt) override;
  void draw(sf::RenderWindow& window) override;

  bool wantsToChangeScene() const override;
  Scene* nextScene(sf::RenderWindow& window, int& vidasJugador) override;
};
