#include "Jugador.h"

Jugador::Jugador(float scale)
    : frameTime(0.1f),
      currentTime(0.f),
      currentFrame(0),
      totalFrames(8),
      isWalking(false),
      facingRight(true),
      scaleFactor(scale),
      minX(0.f),
      maxX(0.f),
      reachedLeft(false),
      reachedRight(false),
      speed(350.f) {
  // Cargar texturas y validar carga
  // clang-format off
  if (!textureIdle.loadFromFile("assets/player/idle.png"));
  if (!textureWalk.loadFromFile("assets/player/walk.png"));
  // clang-format on

  totalFrames = 8;  // idle tiene 8 frames
  int frameWidth = textureIdle.getSize().x / totalFrames;
  int frameHeight = textureIdle.getSize().y;

  frameRect = {{0, 0}, {frameWidth, frameHeight}};

  // Crear sprite con la textura idle y rect inicial
  sprite = new sf::Sprite(textureIdle, frameRect);
  sprite->setScale({scaleFactor, scaleFactor});

  // Centrar origen para reflejar sin mover sprite
  sprite->setOrigin({frameWidth / 2.f, frameHeight / 2.f});
}

Jugador::~Jugador() {
  delete sprite;
}

sf::FloatRect Jugador::getGlobalBounds() const {
  return sprite->getGlobalBounds();
}

void Jugador::setPosition(float x, float y) {
  sprite->setPosition({x, y});
}

void Jugador::setScale(float factor) {
  scaleFactor = factor;
  sprite->setScale({facingRight ? factor : -factor, factor});
}

void Jugador::setTextures(
    const std::string& idlePath, const std::string& walkPath) {
  // clang-format off
  if (!textureIdle.loadFromFile(idlePath));
  if (!textureWalk.loadFromFile(walkPath));
  // clang-format on

  // reiniciar el sprite con la textura idle por defecto
  sprite->setTexture(textureIdle, true);

  // ajustar el tamaño de frame al nuevo spritesheet
  totalFrames = 8;  // se asume que tiene 8 igual que el original
  int frameWidth = textureIdle.getSize().x / totalFrames;
  int frameHeight = textureIdle.getSize().y;
  frameRect = {{0, 0}, {frameWidth, frameHeight}};
  sprite->setTextureRect(frameRect);
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

void Jugador::setMovementBounds(float left, float right) {
  minX = left;
  maxX = right;
}

bool Jugador::hasReachedLeft() const {
  float x = sprite->getPosition().x;
  return x <= minX;
}

bool Jugador::hasReachedRight() const {
  float x = sprite->getPosition().x;
  return x >= maxX;
}