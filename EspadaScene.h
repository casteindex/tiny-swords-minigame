#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>
#include <vector>

#include "Pregunta.h"
#include "Scene.h"

class EspadaScene : public Scene {
 public:
  EspadaScene(sf::RenderWindow& windowRef, int totalPreguntas = 5);

  void handleInput(float dt);
  void update(float dt);
  void draw(sf::RenderWindow& window);

  bool isFinished() const;
  bool playerWon() const;

 private:
  sf::RenderWindow& window;

  // Texturas y sprites
  sf::Texture texFondo, texPiedra, texEspada;
  std::unique_ptr<sf::Sprite> spriteFondo;
  std::unique_ptr<sf::Sprite> spritePiedra;
  std::unique_ptr<sf::Sprite> spriteEspada;

  sf::Font fuente;

  // Preguntas
  std::queue<Pregunta> colaPreguntas;
  Pregunta* preguntaActualPtr;

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
};
