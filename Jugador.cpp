#include "Jugador.h"

#include <iostream>

Jugador::Jugador(float scale)
    : frameTime(0.1f), currentTime(0.f), currentFrame(0), totalFrames(8),
      isWalking(false), facingRight(true), scaleFactor(scale), speed(350.f) {
  // Cargar texturas y validar carga
  if (!textureIdle.loadFromFile("assets/player/idle.png"))
    std::cerr << "No se pudo cargar el idle.png del player\n";
  if (!textureWalk.loadFromFile("assets/player/walk.png"))
    std::cerr << "No se pudo cargar el walk.png del player\n";

  totalFrames = 8;  // idle tiene 8 frames
  int frameWidth = textureIdle.getSize().x / totalFrames;
  int frameHeight = textureIdle.getSize().y;

  frameRect = {{0, 0}, {frameWidth, frameHeight}};

  // Crear sprite con la textura idle y rect inicial
  sprite = std::make_unique<sf::Sprite>(textureIdle, frameRect);
  sprite->setScale({scaleFactor, scaleFactor});

  // Centrar origen para reflejar sin mover sprite
  sprite->setOrigin({frameWidth / 2.f, frameHeight / 2.f});
}

void Jugador::setPosition(float x, float y) {
  sprite->setPosition({x, y});
}

void Jugador::setScale(float factor) {
  scaleFactor = factor;
  sprite->setScale({facingRight ? factor : -factor, factor});
}

void Jugador::handleInput(float deltaTime) {
  isWalking = false;
  float moveX = 0.f;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
    isWalking = true;
    facingRight = true;
    moveX = speed;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
    isWalking = true;
    facingRight = false;
    moveX = -speed;
  }

  // Usar deltaTime real
  sprite->move({moveX * deltaTime, 0.f});
}

void Jugador::update(float deltaTime) {
  currentTime += deltaTime;

  const sf::Texture& tex = isWalking ? textureWalk : textureIdle;
  sprite->setTexture(tex, true);

  totalFrames = isWalking ? 7 : 8;

  if (currentTime >= frameTime) {
    currentTime = 0.f;
    currentFrame = (currentFrame + 1) % totalFrames;
  }

  int frameWidth = tex.getSize().x / totalFrames;
  int frameHeight = tex.getSize().y;

  frameRect.position.x = frameWidth * currentFrame;
  frameRect.size = {frameWidth, frameHeight};
  sprite->setTextureRect(frameRect);

  // Reflejar sprite según dirección
  sprite->setScale({facingRight ? scaleFactor : -scaleFactor, scaleFactor});
}

void Jugador::draw(sf::RenderWindow& window) {
  window.draw(*sprite);
}
