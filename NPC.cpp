#include "NPC.h"

NPC::NPC(const std::string& texturePath, float scale, bool facingLeft,
    float frameDuration)
    : frameTime(frameDuration),
      currentTime(0.f),
      currentFrame(0),
      totalFrames(1),
      facingRight(!facingLeft),
      scaleFactor(scale) {
  // --- cargar textura ---
  // clang format off
  if (!texture.loadFromFile(texturePath))
    ;
  // clang format on

  if (texture.getSize().x > 0 && texture.getSize().y > 0) {
    // se asume que tiene 7 frames (porque los que tengo así son)
    totalFrames = 7;
    int frameWidth = static_cast<int>(texture.getSize().x) / totalFrames;
    int frameHeight = static_cast<int>(texture.getSize().y);

    // inicializar rect de frame como hiciste en Jugador
    frameRect = {{0, 0}, {frameWidth, frameHeight}};
  } else {
    totalFrames = 1;
    frameRect = {{0, 0}, {64, 64}};
  }

  // crear sprite con el rect inicial
  sprite = new sf::Sprite(texture, frameRect);
  sprite->setScale({scaleFactor, scaleFactor});

  // mirror (por si está viendo a la izquierda)
  sprite->setOrigin({frameRect.size.x / 2.f, frameRect.size.y / 2.f});
  sprite->setScale({facingRight ? scaleFactor : -scaleFactor, scaleFactor});
}

NPC::~NPC() {
  delete sprite;
}

void NPC::update(float deltaTime) {
  // actualizar temporizador y avanzar frame si corresponde
  currentTime += deltaTime;
  if (currentTime >= frameTime) {
    currentTime = 0.f;
    currentFrame = (currentFrame + 1) % totalFrames;

    // calcular nuevo rect según currentFrame (misma técnica que Jugador)
    int frameWidth = frameRect.size.x;
    int frameHeight = frameRect.size.y;

    frameRect.position.x = frameWidth * currentFrame;
    frameRect.size = {frameWidth, frameHeight};
    sprite->setTextureRect(frameRect);
  }
}

void NPC::draw(sf::RenderWindow& window) const {
  if (sprite) window.draw(*sprite);
}

void NPC::setPosition(float x, float y) {
  if (sprite) sprite->setPosition({x, y});
}

sf::FloatRect NPC::getGlobalBounds() const {
  return sprite ? sprite->getGlobalBounds() : sf::FloatRect{};
}
