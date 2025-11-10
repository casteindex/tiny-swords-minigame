#include "IntroScene.h"

#include <iostream>

IntroScene::IntroScene(sf::RenderWindow& windowRef, int& vidas)
    : window(windowRef),
      vidasJugador(vidas),
      tiempoTotal(0.f),
      tiempoEnImagen(0.f),
      duracionPorImagen(6.f),
      imagenActual(0),
      terminado(false),
      skip(false) {
  // clang-format off
  // --- cargar recursos ---
  if (!fuente.openFromFile("assets/fonts/Jacquard24-Regular.ttf"));
  if (!narracion.openFromFile("assets/music/narracion.wav"));
  fondos.resize(20);
  for (int i = 0; i < 20; ++i) {
    std::string path = "assets/backgrounds/intro/" + std::to_string(i + 1) + ".jpg";
    if (!fondos[i].loadFromFile(path));
  }
  // clang-format on
  if (!fondos.empty()) spriteFondo = new sf::Sprite(fondos[0]);

  // --- overlay para el fade (negro encima con alpha variable) ---
  overlay.setSize({(float)window.getSize().x, (float)window.getSize().y});
  overlay.setFillColor(sf::Color(0, 0, 0, 255));  // empezamos en negro total

  // --- texto de "Saltar" ---
  textoSaltar = new sf::Text(fuente);
  textoSaltar->setString("Saltar");
  textoSaltar->setCharacterSize(32);
  textoSaltar->setFillColor(sf::Color::White);
  textoSaltar->setOutlineThickness(1.0f);
  float tx = window.getSize().x - 150.f;
  float ty = window.getSize().y - 80.f;
  textoSaltar->setPosition({tx, ty});

  narracion.play();
}

IntroScene::~IntroScene() {
  narracion.stop();
  delete spriteFondo;
  delete textoSaltar;
}

void IntroScene::handleInput(float dt) {
  // tecla Enter para saltar
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
    skip = true;
    return;
  }

  // clic en "Saltar"
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    auto mp = sf::Mouse::getPosition(window);
    float mx = static_cast<float>(mp.x), my = static_cast<float>(mp.y);
    if (textoSaltar && textoSaltar->getGlobalBounds().contains({mx, my})) {
      skip = true;
    }
  }
}

void IntroScene::update(float dt) {
  if (terminado || skip) return;

  tiempoTotal += dt;
  tiempoEnImagen += dt;

  // si la imagen actual terminó -> avanzar
  if (tiempoEnImagen >= duracionPorImagen) {
    tiempoEnImagen -= duracionPorImagen;
    imagenActual++;
    if (imagenActual >= (int)fondos.size()) {
      terminado = true;
      return;
    }
    // cambiar textura
    if (fondos[imagenActual].getSize().x > 0)
      spriteFondo->setTexture(fondos[imagenActual], true);
  }

  // calcular alpha (fade in 0-1s, visible 1-5s, fade out 5-6s)
  float a = 255.f;
  if (tiempoEnImagen < 1.f) a = 255.f - (tiempoEnImagen / 1.f) * 255.f;
  else if (tiempoEnImagen < 5.f) a = 0.f;
  else {
    float t = (tiempoEnImagen - 5.f) / 1.f;  // 0..1
    a = t * 255.f;
  }
  overlay.setFillColor(sf::Color(0, 0, 0, a));
}

void IntroScene::draw(sf::RenderWindow& window) {
  if (spriteFondo) window.draw(*spriteFondo);
  window.draw(overlay);
  if (textoSaltar) window.draw(*textoSaltar);
}

bool IntroScene::wantsToChangeScene() const {
  return terminado || skip;
}

Scene* IntroScene::nextScene(sf::RenderWindow& window, int& vidasJugador) {
  narracion.stop();
  return new RuletaScene(window, vidasJugador);
}
