#include "RuletaScene.h"

#include <iostream>

RuletaScene::RuletaScene(sf::RenderWindow& window) : jugador(3.0f) {
  // Fondo
  if (!backgroundTex.loadFromFile("assets/backgrounds/fondo_ruleta.jpg")) {
    std::cerr << "No se pudo cargar el fondo\n";
  }
  background = std::make_unique<sf::Sprite>(backgroundTex);
  background->setPosition({0.f, 0.f});
  background->setColor(sf::Color(255, 255, 255, 255));  // 4: opacidad (max)

  // Jugador
  jugador.setPosition(200.f, 500.f);

  // Ruleta (Nota: el parámetro es la posición de la ruleta)
  ruleta = std::make_unique<Ruleta>(window, sf::Vector2f(350.f, 250.f));
}

void RuletaScene::handleInput(float deltaTime) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) &&
      !ruleta->estaGirando()) {
    ruleta->iniciarGiro();
  }
  jugador.handleInput(deltaTime);
}

void RuletaScene::update(float deltaTime) {
  jugador.update(deltaTime);
  ruleta->actualizar(deltaTime);
}

void RuletaScene::draw(sf::RenderWindow& window) {
  if (background) window.draw(*background);
  jugador.draw(window);
  ruleta->dibujar();
}
