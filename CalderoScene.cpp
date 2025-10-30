#include "CalderoScene.h"

#include <iostream>

CalderoScene::CalderoScene(sf::RenderWindow& window) {
  // Fondo
  if (!texFondo.loadFromFile("assets/backgrounds/big-mage.jpg")) {
    std::cerr << "No se pudo cargar el fondo\n";
  }

  spriteFondo = std::make_unique<sf::Sprite>(texFondo);

  // Escalar para llenar la ventana
  sf::Vector2u texSize = texFondo.getSize();
  if (texSize.x > 0 && texSize.y > 0) {
    float scaleX = window.getSize().x / float(texSize.x);
    float scaleY = window.getSize().y / float(texSize.y);
    spriteFondo->setScale({scaleX, scaleY});
  }

  // Bajar la opacidad un poco (como hicimos en otras escenas)
  spriteFondo->setColor(sf::Color(255, 255, 255, 180));

  // Caldero
  if (!texCaldero.loadFromFile("assets/objects/caldera.png")) {
    std::cerr << "No se pudo cargar caldera.png\n";
  }

  spriteCaldero = std::make_unique<sf::Sprite>(texCaldero);

  // --- Configuración del spritesheet ---
  int anchoFrame = 128;
  int altoFrame = 128;
  spriteCaldero->setTextureRect(sf::IntRect({0, 0}, {anchoFrame, altoFrame}));
  spriteCaldero->setScale({4.f, 4.f});
  spriteCaldero->setPosition({550, 350});
}

void CalderoScene::handleInput(float dt) {
  if (terminado) return;

  bool a = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
  bool b = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B);
  bool c = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C);
  bool d = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

  // if (!keyLocked && (a || b || c || d) && preguntaActualPtr) {
  //   keyLocked = true;
  //   char tecla = a ? 'A' : b ? 'B' : c ? 'C' : 'D';
  //   procesarRespuesta(tecla);
  // }

  // if (!a && !b && !c && !d) keyLocked = false;
}

void CalderoScene::update(float dt) {
  if (terminado) return;

  tiempoAcumulado += dt;
  if (tiempoAcumulado >= duracionFrame) {
    tiempoAcumulado = 0.f;
    frameActual = (frameActual + 1) % totalFrames;

    int anchoFrame = 128;
    int altoFrame = 128;
    spriteCaldero->setTextureRect(
        sf::IntRect({frameActual * anchoFrame, 0}, {anchoFrame, altoFrame}));
  }
}

void CalderoScene::draw(sf::RenderWindow& window) {
  if (spriteFondo) window.draw(*spriteFondo);
  if (spriteCaldero) window.draw(*spriteCaldero);
}
