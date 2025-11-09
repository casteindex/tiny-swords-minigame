#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>

#include "Jugador.h"
#include "Pregunta.h"
#include "RuletaScene.h"
#include "Scene.h"

class IglesiaScene : public Scene {
 private:
  int& vidasJugador;
  sf::RenderWindow& window;

  // Assets
  sf::Texture fondosTex[6];
  sf::Texture potionTex;
  sf::Font fuente;

  // Sprites
  sf::Sprite* spriteFondo;
  sf::Sprite* spritePocion1;
  sf::Sprite* spritePocion2;
  sf::Sprite* spritePocion3;
  sf::Sprite* spritePocion4;

  Jugador* jugador;

  // Preguntas
  std::queue<Pregunta> colaPreguntas;
  Pregunta* preguntaActualPtr;

  // Estado del juego
  std::vector<std::string> categoriasRestantes;
  int fondoActual;
  int respuestasCorrectas;
  bool terminado;
  bool gano;
  bool esperandoEnter = false;  // para controlar transición después de mensaje

  void inicializarPreguntas();
  void procesarRespuesta(char tecla);
  bool jugadorSobrePocion(const sf::Sprite& pocion) const;

 public:
  IglesiaScene(sf::RenderWindow& windowRef, int& vidasJugador,
      std::vector<std::string> categorias = {}, int totalPreguntas = 5);
  ~IglesiaScene();

  void handleInput(float dt) override;
  void update(float dt) override;
  void draw(sf::RenderWindow& window) override;

  bool wantsToChangeScene() const override;
  Scene* nextScene(sf::RenderWindow& window, int& vidasJugador) override;

  bool isFinished() const;
  bool playerWon() const;
};
