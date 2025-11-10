#include "EdificioScene.h"

EdificioScene::EdificioScene(
    sf::RenderWindow& window, Jugador* jugadorExistente)
    : jugador(jugadorExistente), cambiarEscena(false), opcionDestino(-1) {
  // clang-format off
  if (!backgroundTexture.loadFromFile("assets/backgrounds/fondo_edificio.jpg"));
  // clang-format on
  backgroundSprite = new sf::Sprite(backgroundTexture);
  jugador->setPosition(640.f, 440.f);
  jugador->setMovementBounds(50.f, window.getSize().x - 50.f);
}

EdificioScene::~EdificioScene() {
  // nota: no borrar jugador porque pertenece a otra escena
  delete backgroundSprite;
}

void EdificioScene::handleInput(float deltaTime) {
  jugador->handleInput(deltaTime);
}

void EdificioScene::update(float deltaTime) {
  jugador->update(deltaTime);

  if (jugador->hasReachedLeft()) {
    cambiarEscena = true;
    opcionDestino = 0;  // opción de aula
  } else if (jugador->hasReachedRight()) {
    cambiarEscena = true;
    opcionDestino = 1;  // opción de laboratorio
  }
}

void EdificioScene::draw(sf::RenderWindow& window) {
  window.draw(*backgroundSprite);
  jugador->draw(window);
}

bool EdificioScene::wantsToChangeScene() const {
  return cambiarEscena;
}

Scene* EdificioScene::nextScene(sf::RenderWindow& window, int& vidasJugador) {
  if (opcionDestino == 0) {
    return new AulaScene(window, jugador);
  } else return new AulaScene(window, jugador);  // TODO: laboratorio
}
