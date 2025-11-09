#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

class Ruleta {
 private:
  sf::RenderWindow& window;
  sf::Vector2f posicion;

  sf::Sprite* spriteRuleta;
  sf::Texture texturaRuleta;

  std::vector<std::string> nombresCategorias;

  float angulo;
  float velocidadAngular;
  float desaceleracion;
  bool girando;
  int categoriaSeleccionada;

  sf::Font fuente;

 public:
  Ruleta(sf::RenderWindow& windowRef, std::vector<std::string> categorias,
      sf::Vector2f pos);
  ~Ruleta();

  void iniciarGiro();
  void actualizar(float dt);
  void dibujar();

  bool estaGirando() const;
  bool haTerminado() const;
  std::vector<std::string> getCategorias() const;
  std::string getCategoriaSeleccionada() const;
};
