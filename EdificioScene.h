#pragma once

#include <SFML/Graphics.hpp>

#include "AulaScene.h"
#include "Jugador.h"
#include "Scene.h"

class EdificioScene : public Scene {
 private:
  sf::Texture backgroundTexture;
  sf::Sprite* backgroundSprite;
  Jugador* jugador;  // reutilizado, no se destruye aquí

  bool cambiarEscena;
  int opcionDestino;

 public:
  // el jugador se pasa como puntero existente
  EdificioScene(sf::RenderWindow& window, Jugador* jugadorExistente);
  ~EdificioScene() override;

  void handleInput(float deltaTime) override;
  void update(float deltaTime) override;
  void draw(sf::RenderWindow& window) override;

  bool wantsToChangeScene() const override;
  Scene* nextScene(sf::RenderWindow& window, int& vidasJugador) override;
};