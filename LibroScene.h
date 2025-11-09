#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>

#include "Pregunta.h"
#include "RuletaScene.h"
#include "Scene.h"

class LibroScene : public Scene {
 private:
  int& vidasJugador;
  sf::RenderWindow& window;

  // Assets
  sf::Texture texFondo;
  sf::Texture texLibro;
  sf::Texture texCalavera;
  sf::Font fuente;

  sf::Sprite* spriteFondo;
  sf::Sprite* spriteLibro;
  sf::Sprite* spriteCalavera;

  std::vector<std::string> categoriasRestantes;

  std::queue<Pregunta> colaPreguntas;
  Pregunta* preguntaActualPtr;

  int totalPreguntasIniciales;
  int respuestasCorrectas;
  bool terminado;
  bool gano;

  // Animaciones / Hover
  std::vector<float> optionAlpha;   // alfa actual por opción
  std::vector<float> optionTarget;  // alfa objetivo
  float fadeSpeed = 350.f;

  float calaveraAlpha;
  float calaveraTarget;
  bool showCalavera;

  void inicializarPreguntas();
  void procesarRespuesta(char tecla);

 public:
  LibroScene(sf::RenderWindow& windowRef, int& vidasJugador,
      std::vector<std::string> categoriasRestantes = {},
      int totalPreguntas = 5);
  ~LibroScene();

  void handleInput(float dt) override;
  void update(float dt) override;
  void draw(sf::RenderWindow& window) override;

  bool wantsToChangeScene() const override;
  Scene* nextScene(sf::RenderWindow& window, int& vidasJugador) override;

  bool isFinished() const;
  bool playerWon() const;
};