#include "Ruleta.h"
using namespace sf;

Ruleta::Ruleta(sf::RenderWindow& windowRef, std::vector<std::string> categorias,
    Vector2f pos)
    : window(windowRef),
      nombresCategorias(std::move(categorias)),
      posicion(pos),
      angulo(0.f),
      velocidadAngular(0.f),
      desaceleracion(0.f),
      girando(false),
      categoriaSeleccionada(-1) {
  // --- Cargar recursos ----
  // clang-format off
  if (!texturaRuleta.loadFromFile("assets/objects/ruleta.png"));
  if (!fuente.openFromFile("assets/fonts/Jacquard24-Regular.ttf"));
  // clang-format on

  // Inicializar random
  std::srand(static_cast<unsigned int>(std::time(0)));

  // Crear sprite
  spriteRuleta = new Sprite(texturaRuleta);
  // Pasar el origen al centro de la imagen
  spriteRuleta->setOrigin(
      {texturaRuleta.getSize().x / 2.f, texturaRuleta.getSize().y / 2.f});
  spriteRuleta->setPosition(posicion);
}

Ruleta::~Ruleta() {
  delete spriteRuleta;
}

void Ruleta::iniciarGiro() {
  if (girando || nombresCategorias.empty()) return;
  girando = true;
  velocidadAngular = 720.f;
  desaceleracion = 300.f;
  categoriaSeleccionada = -1;
}

void Ruleta::actualizar(float dt) {
  if (!girando) return;

  // Girar
  angulo += velocidadAngular * dt;
  spriteRuleta->setRotation(sf::degrees(angulo));

  // Desacelerar
  velocidadAngular -= desaceleracion * dt;
  if (velocidadAngular <= 0.f) {
    velocidadAngular = 0.f;
    girando = false;

    // Elegir categoría final dinámicamente
    int numCategorias = static_cast<int>(nombresCategorias.size());
    float gradosPorSector = 360.f / numCategorias;

    // normalizar ángulo entre 0 y 360
    float anguloNormalizado = fmod(angulo, 360.f);
    if (anguloNormalizado < 0) anguloNormalizado += 360.f;

    // calcular índice
    int idx = static_cast<int>(anguloNormalizado / gradosPorSector);
    // invertir sentido (para que coincida con rotación de imagen)
    categoriaSeleccionada = (numCategorias - 1) - idx;

    // asegurar que esté dentro de rango
    if (categoriaSeleccionada < 0) categoriaSeleccionada = 0;
    else if (categoriaSeleccionada >= numCategorias)
      categoriaSeleccionada = numCategorias - 1;

    std::cout << "Categoría elegida: "
              << nombresCategorias[categoriaSeleccionada] << "\n";
  }
}

void Ruleta::dibujar() {
  if (spriteRuleta) window.draw(*spriteRuleta);
  Text texto(fuente);
  texto.setCharacterSize(28);
  texto.setFillColor(Color::Yellow);
  texto.setOutlineThickness(1.0f);

  if (!girando) {
    // instrucción
    texto.setString("Presiona ESPACIO para girar");
    texto.setPosition({posicion.x - 100.f, posicion.y + 250.f});
    window.draw(texto);

    // mostrar categoría seleccionada
    if (categoriaSeleccionada != -1) {
      texto.setString("Tu destino te lleva hacia el reino de " +
                      nombresCategorias[categoriaSeleccionada]);
      texto.setPosition({posicion.x - 200.f, posicion.y + 300.f});
      window.draw(texto);
    }
  }
}

std::vector<std::string> Ruleta::getCategorias() const {
  return nombresCategorias;
}

bool Ruleta::estaGirando() const {
  return girando;
}

bool Ruleta::haTerminado() const {
  return !girando && categoriaSeleccionada != -1;
}

std::string Ruleta::getCategoriaSeleccionada() const {
  return nombresCategorias[categoriaSeleccionada];
}