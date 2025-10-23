#include "Ruleta.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

Ruleta::Ruleta(sf::RenderWindow& windowRef)
    : window(windowRef), indiceActual(0), seleccionFinal(-1), timer(0.f),
      activa(false), tiempoEntreCambios(0.05f), factorDesaceleracion(1.f) {
  srand(static_cast<unsigned int>(time(0)));

  nombresPergaminos = {"pergamino-fuego.png", "pergamino-hielo.png",
      "pergamino-planta.png", "pergamino-tierra.png"};

  // Cargar texturas
  for (auto& nombre : nombresPergaminos) {
    sf::Texture tex;
    if (!tex.loadFromFile("assets/" + nombre)) {
      cerr << "Error al cargar " << nombre << endl;
    } else {
      pergaminos.push_back(tex);
    }
  }

  // Crear el sprite con make_unique
  pergaminoActual = make_unique<sf::Sprite>(pergaminos[0]);
  pergaminoActual->setOrigin(
      {pergaminos[0].getSize().x / 2.f, pergaminos[0].getSize().y / 2.f});
  pergaminoActual->setPosition(
      {window.getSize().x / 2.f, window.getSize().y / 2.f});
}

bool Ruleta::isActive() const {
  return activa;
}

void Ruleta::actualizar(float dt) {
  if (!activa) return;

  // Incrementar el timer
  timer += dt * factorDesaceleracion;

  // Cambiar pergamino según tiempo
  if (timer >= tiempoEntreCambios) {
    timer = 0.f;
    indiceActual = (indiceActual + 1) % pergaminos.size();
    pergaminoActual->setTexture(pergaminos[indiceActual]);
    pergaminoActual->setOrigin({pergaminos[indiceActual].getSize().x / 2.f,
        pergaminos[indiceActual].getSize().y / 2.f});
    pergaminoActual->setPosition(
        {window.getSize().x / 2.f, window.getSize().y / 2.f});

    // Reducir velocidad (desaceleración)
    factorDesaceleracion *= 0.95f;

    // Cuando la ruleta se detiene lo suficiente
    if (factorDesaceleracion < 0.1f) {
      activa = false;
      seleccionFinal = indiceActual;
      cout << "Pergamino seleccionado: " << nombresPergaminos[seleccionFinal]
           << "\n";

      // Eliminar de la lista de pergaminos para próximas veces
      pergaminos.erase(pergaminos.begin() + seleccionFinal);
      nombresPergaminos.erase(nombresPergaminos.begin() + seleccionFinal);

      // Resetear valores para la próxima ruleta
      indiceActual = 0;
      factorDesaceleracion = 1.f;
    }
  }
}

void Ruleta::dibujar() {
  if (!activa) return;

  // Fondo semi-transparente
  sf::RectangleShape fondo(
      {(float)window.getSize().x, (float)window.getSize().y});
  fondo.setFillColor(sf::Color(0, 0, 0, 150));
  window.draw(fondo);

  // Dibujar pergamino actual
  window.draw(*pergaminoActual);
}

void Ruleta::activar() {
  if (pergaminos.empty()) {
    cout << "No hay pergaminos disponibles.\n";
    return;
  }

  activa = true;
  // Elegir un índice inicial aleatorio
  indiceActual = rand() % pergaminos.size();
  pergaminoActual->setTexture(pergaminos[indiceActual]);
  pergaminoActual->setOrigin({pergaminos[indiceActual].getSize().x / 2.f,
      pergaminos[indiceActual].getSize().y / 2.f});
  pergaminoActual->setPosition(
      {window.getSize().x / 2.f, window.getSize().y / 2.f});

  // Resetear desaceleración
  factorDesaceleracion = 1.f;
  timer = 0.f;
}