#include <SFML/Graphics.hpp>
#include <memory>

// #include "Assets.h"
#include "CalderoScene.h"
// #include "ColasPreguntas.h"
#include "EspadaScene.h"
#include "RuletaScene.h"
#include "Scene.h"

int main() {
  sf::RenderWindow window(sf::VideoMode({1280, 720}), "Minijuego - Estru 2");
  window.setFramerateLimit(60);

  sf::Clock clock;

  // Escena inicial
  // std::unique_ptr<Scene> currentScene =
  // std::make_unique<RuletaScene>(window);
  std::unique_ptr<Scene> currentScene = std::make_unique<EspadaScene>(window);
  // std::unique_ptr<Scene> currentScene =
  // std::make_unique<CalderoScene>(window);

  while (window.isOpen()) {
    while (window.isOpen()) {
      while (const std::optional event = window.pollEvent())
        if (event->is<sf::Event::Closed>()) window.close();
      float deltaTime = clock.restart().asSeconds();

      // Actualizar escena
      currentScene->handleInput(deltaTime);
      currentScene->update(deltaTime);

      // Dibujar
      window.clear();
      currentScene->draw(window);
      window.display();
    }

    return 0;
  }
}