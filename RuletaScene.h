#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "BatallaFinalScene.h"
#include "CalderoScene.h"
#include "EspadaScene.h"
#include "IglesiaScene.h"
#include "Jugador.h"
#include "LibroScene.h"
#include "Ruleta.h"
#include "Scene.h"

class RuletaScene : public Scene {
 private:
  sf::RenderWindow& window;
  int vidasJugador;
  std::vector<std::string> categoriasRestantes;

  Jugador jugador;
  sf::Sprite* background;
  sf::Texture backgroundTex;
  Ruleta* ruleta;
  bool terminado;
  std::string minijuego;

  sf::Font fuente;
  sf::Texture empiristasTex;
  sf::Texture racionalistasTex;
  sf::Sprite* empiristasSprite;
  sf::Sprite* racionalistasSprite;

  bool esEmpirista;
  bool eligiendoBando;
  bool bandoElegido;

  void handleInput(float deltaTime) override;
  void update(float deltaTime) override;

 public:
  RuletaScene(sf::RenderWindow& window, int& vidasJugador);
  RuletaScene(sf::RenderWindow& window, int& vidasJugador,
      std::vector<std::string> categorias);
  ~RuletaScene();

  void draw(sf::RenderWindow& window) override;
  bool wantsToChangeScene() const override;
  Scene* nextScene(sf::RenderWindow& window, int& vidasJugador) override;
};
