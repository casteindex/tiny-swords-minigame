#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <string>

#include "EdificioScene.h"
#include "Jugador.h"
#include "NPC.h"
#include "Pregunta.h"
#include "Scene.h"

class AulaScene : public Scene {
 private:
  sf::RenderWindow& window;
  Jugador* jugador;
  NPC* kant;

  sf::Texture texFondo;
  sf::Sprite* fondo;

  sf::RectangleShape cuadroDialogo;
  sf::Font fuente;
  sf::Text* textoDialogo;

  std::queue<std::string> colaPistas;
  std::queue<Pregunta> colaPreguntas;
  std::queue<sf::FloatRect> colaAreas;  // áreas de elementos a buscar

  Pregunta* preguntaActual;
  bool mostrandoPista;
  bool mostrandoPregunta;
  bool esperandoClick;
  bool ultimaRespuestaCorrecta;
  bool finJuego;
  bool cambioEscena;
  int aciertos;

  void inicializarPreguntas();
  void inicializarPistas();
  void inicializarAreas();

  void mostrarSiguientePista();
  void mostrarPreguntaActual();
  void manejarRespuesta(char opcion);
  bool clickEnAreaValida(float x, float y);

 public:
  AulaScene(sf::RenderWindow& windowRef, Jugador* jugadorPtr);
  ~AulaScene();

  void handleInput(float deltaTime) override;
  void update(float deltaTime) override;
  void draw(sf::RenderWindow& window) override;
  bool wantsToChangeScene() const override;
  Scene* nextScene(sf::RenderWindow& window, int& vidasJugador) override;
};
