#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

class Ruleta {
 private:
  sf::RenderWindow& window;
  sf::Vector2f posicion;

  std::unique_ptr<sf::Sprite> spriteRuleta;
  sf::Texture texturaRuleta;

  std::vector<std::string> nombresCategorias;

  float angulo;
  float velocidadAngular;
  float desaceleracion;
  bool girando;
  int categoriaSeleccionada;

  sf::Font fuente;

 public:
  Ruleta(sf::RenderWindow& windowRef, sf::Vector2f pos);

  void iniciarGiro();
  void actualizar(float dt);
  void dibujar();

  bool estaGirando() const;
};
