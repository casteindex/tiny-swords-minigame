#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <vector>

#include "Pregunta.h"
#include "RuletaScene.h"
#include "Scene.h"

class CalderoScene : public Scene {
 private:
  int& vidasJugador;
  std::vector<std::string> categoriasRestantes;

  sf::Texture texFondo;
  sf::Texture texCaldero;
  sf::Sprite* spriteFondo;
  sf::Sprite* spriteCaldero;
  sf::Font fuente;

  std::queue<Pregunta> colaPreguntas;
  Pregunta* preguntaActualPtr;

  int totalPreguntasIniciales;
  int respuestasCorrectas;
  bool terminado;
  bool gano;
  bool keyLocked;

  // Animación
  static const int FRAME_WIDTH = 48;
  static const int FRAME_HEIGHT = 48;
  static const float DURACION_FRAME;
  int frameX;
  int frameY;
  float tiempoAcumulado;
  sf::IntRect rectFrame;
  std::pair<int, int> inicioBloque;  // primer frame del bloque actual

  void inicializarPreguntas();
  void procesarRespuesta(char tecla);
  void setFrame(int fila, int columna);

  void handleInput(float dt) override;
  void update(float dt) override;

 public:
  CalderoScene(sf::RenderWindow& window, int& vidasJugador,
      std::vector<std::string> categoriasRestantes = {},
      int totalPreguntas = 5);
  ~CalderoScene();

  void draw(sf::RenderWindow& window) override;
  bool wantsToChangeScene() const override;
  Scene* nextScene(sf::RenderWindow& window, int& vidasJugador) override;
  bool isFinished() const;
  bool playerWon() const;
};
