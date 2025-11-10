#include <SFML/Graphics.hpp>

#include "BatallaFinalScene.h"
#include "CalderoScene.h"
#include "EspadaScene.h"
#include "IglesiaScene.h"
#include "LibroScene.h"
#include "RuletaScene.h"
#include "Scene.h"

int main() {
  setlocale(LC_ALL, "Spanish");

  sf::RenderWindow window(
      sf::VideoMode({1280, 720}), "Minijuego Mago - Estructura de Datos II");
  window.setFramerateLimit(60);

  sf::Clock clock;

  // --- Corazones ---
  sf::Texture heartsTex;
  if (!heartsTex.loadFromFile("assets/ui/hearts.png")) {
    std::cerr << "No se pudo cargar hearts.png\n";
  }
  int vidas = 0;
  std::vector<std::unique_ptr<sf::Sprite>> hearts;
  hearts.reserve(4);
  for (int i = 0; i < 4; ++i) {
    auto sprite = std::make_unique<sf::Sprite>(heartsTex);
    sprite->setTextureRect(sf::IntRect({16, 0}, {16, 16}));  // vacío
    sprite->setScale({3.f, 3.f});
    sprite->setPosition({20.f + i * 55.f, 20.f});
    hearts.push_back(std::move(sprite));
  }

  // Escena inicial
  // --- DEBUG ----
  vidas = 4;
  Scene* currentScene = new BatallaFinalScene(window, vidas);
  // Scene* currentScene = new RuletaScene(window, vidas);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) window.close();
    }
    float deltaTime = clock.restart().asSeconds();
    currentScene->handleInput(deltaTime);
    currentScene->update(deltaTime);
    window.clear();
    currentScene->draw(window);

    // display corazones
    for (int i = 0; i < 4; ++i) {
      hearts[i]->setTextureRect(i < vidas ? sf::IntRect({0, 0}, {16, 16})
                                          : sf::IntRect({16, 0}, {16, 16}));
      window.draw(*hearts[i]);
    }

    window.display();

    // --- Cambio de escena ---
    if (currentScene->wantsToChangeScene()) {
      Scene* nextScene = currentScene->nextScene(window, vidas);
      delete currentScene;       // liberar memoria de escena anterior
      currentScene = nextScene;  // asignar nueva escnea
    }
  }
  delete currentScene;  // librerar memoria de la úlitma escena

  return 0;
}