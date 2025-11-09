#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <vector>

#include "Pregunta.h"
#include "RuletaScene.h"
#include "Scene.h"

class EspadaScene : public Scene {
 private:
  sf::RenderWindow& window;
  int& vidasJugador;

  // Texturas y sprites
  sf::Texture texFondo, texPiedra, texEspada;
  sf::Sprite* spriteFondo;
  sf::Sprite* spritePiedra;
  sf::Sprite* spriteEspada;

  sf::Font fuente;

  // Preguntas
  std::queue<Pregunta> colaPreguntas;
  Pregunta* preguntaActualPtr;
  std::vector<std::string> categoriasRestantes;  // Ruleta

  int totalPreguntasIniciales;
  int respuestasCorrectas;

  // Espada
  float espadaY;
  float destinoY;
  bool animando;

  bool keyLocked;  // si CapsLock está activado

  // Estado final
  bool terminado;
  bool gano;

  void inicializarPreguntas();
  void procesarRespuesta(char tecla);
  void resetEspada();

  void handleInput(float dt) override;
  void update(float dt) override;

 public:
  EspadaScene(sf::RenderWindow& windowRef, int& vidasJugador,
      std::vector<std::string> categoriasRestantes = {},
      int totalPreguntas = 5);
  ~EspadaScene();

  void draw(sf::RenderWindow& window) override;
  bool wantsToChangeScene() const override;
  Scene* nextScene(sf::RenderWindow& window, int& vidasJugador) override;
  bool isFinished() const;
  bool playerWon() const;
};
