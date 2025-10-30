#include "Ruleta.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;

Ruleta::Ruleta(sf::RenderWindow& windowRef, Vector2f pos)
    : window(windowRef), posicion(pos), angulo(0.f), velocidadAngular(0.f),
      desaceleracion(0.f), girando(false), categoriaSeleccionada(-1) {
  // Inicializar random
  std::srand(static_cast<unsigned int>(std::time(0)));

  // Textura de la ruleta
  if (!texturaRuleta.loadFromFile("assets/objects/ruleta.png"))
    std::cerr << "No se pudo cargar ruleta.png\n";

  // Crear sprite
  spriteRuleta = std::make_unique<Sprite>(texturaRuleta);
  // Pasar el origen al centro de la imagen
  spriteRuleta->setOrigin(
      {texturaRuleta.getSize().x / 2.f, texturaRuleta.getSize().y / 2.f});
  spriteRuleta->setPosition(posicion);

  // Categorías
  nombresCategorias = {"Arte", "Política", "Ciencia", "Historia"};

  // Fuente
  if (!fuente.openFromFile("assets/fonts/Jacquard24-Regular.ttf"))
    std::cerr << "No se pudo cargar Jacquard24-Regular.ttf\n";
}

void Ruleta::iniciarGiro() {
  if (girando || nombresCategorias.empty()) return;
  girando = true;
  velocidadAngular = 720.f;
  desaceleracion = 300.f;
  categoriaSeleccionada = -1;
}

void Ruleta::actualizar(float dt) {
  if (!girando) return;

  // Girar
  angulo += velocidadAngular * dt;
  spriteRuleta->setRotation(sf::degrees(angulo));

  // Desacelerar
  velocidadAngular -= desaceleracion * dt;
  if (velocidadAngular <= 0.f) {
    velocidadAngular = 0.f;
    girando = false;

    // Elegir categoría final (hard coded por sectores)
    int idx = int(fmod(angulo, 360.f) / 90.f);  // 4 sectores de 90°
    categoriaSeleccionada = 3 - idx;            // invertir sentido

    std::cout << "Categoría elegida: "
              << nombresCategorias[categoriaSeleccionada] << "\n";
  }
}

void Ruleta::dibujar() {
  if (spriteRuleta) window.draw(*spriteRuleta);
  Text texto(fuente);
  texto.setCharacterSize(28);
  texto.setFillColor(Color::Yellow);
  texto.setOutlineThickness(1.0f);

  if (!girando) {
    // instrucción
    texto.setString("Presiona ESPACIO para girar");
    texto.setPosition({posicion.x - 100.f, posicion.y + 250.f});
    window.draw(texto);

    // mostrar categoría seleccionada
    if (categoriaSeleccionada != -1) {
      texto.setString("Tu destino te lleva hacia el reino de " +
                      nombresCategorias[categoriaSeleccionada]);
      texto.setPosition({posicion.x - 200.f, posicion.y + 300.f});
      window.draw(texto);
    }
  }
}

bool Ruleta::estaGirando() const {
  return girando;
}
