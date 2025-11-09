#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "Jugador.h"
#include "NPC.h"
#include "Pregunta.h"
#include "RuletaScene.h"
#include "Scene.h"

class BatallaFinalScene : public Scene {
 private:
  sf::RenderWindow& window;
  int& vidasJugador;
  int vidasNPC;
  bool esEmpirista;

  sf::Font fuente;
  sf::Texture fondoTexture;
  sf::Sprite* spriteFondo;

  Jugador jugador;
  NPC* npc;
  sf::Texture texHeartsNPC;
  std::vector<sf::Sprite*> heartsNPC;

  std::vector<Pregunta> bancoPreguntas;
  int preguntaActual;
  bool terminado;
  bool gano;
  bool keyLocked;

  // --- Rayo ---
  sf::Sprite* spriteRayo;
  sf::Texture texRayo;
  sf::IntRect frameRayo;
  float frameTimeRayo = 0.06f;
  float tiempoActualRayo = 0.f;
  int frameActualRayo = 0;
  int totalFramesRayo = 12;
  bool mostrandoRayo = false;
  bool rayoSobreJugador = true;
  int columnasRayo = 6;

  // --- Música ---
  sf::Music musicaEmpirista;
  sf::Music musicaRacionalista;

 public:
  BatallaFinalScene(
      sf::RenderWindow& windowRef, int& vidasJugador, bool esEmpirista = false);
  ~BatallaFinalScene();

  void handleInput(float deltaTime) override;
  void update(float deltaTime) override;
  void draw(sf::RenderWindow& window) override;

  bool playerWon() const override;
  bool wantsToChangeScene() const override;
  Scene* nextScene(sf::RenderWindow& window, int& vidasJugador) override;

 private:
  void inicializarPreguntas();
  void procesarRespuesta(char tecla);
  void lanzarRayo(bool sobreJugador);
};
