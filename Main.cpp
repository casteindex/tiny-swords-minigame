#include <SFML/Graphics.hpp>
#include <vector>

#include "Ruleta.h"

const int TILE_SIZE = 16;  // tamaño real de cada tile
const int ESCALA = 4;      // factor de escalado para pantalla
const int DISPLAY_SIZE = TILE_SIZE * ESCALA;

const int MAPA_ANCHO = 10;
const int MAPA_ALTO = 10;

// --- VARIABLES DE ANIMACIÓN ---
const int FRAME_SIZE = 16;  // cada sprite 16x16
const int FRAME_COUNT = 4;  // 4 frames por dirección
const float FRAME_DURATION = 0.15f;

int currentFrame = 0;
float animationTimer = 0.f;
bool isMoving = false;

// Mapa simple con IDs de tiles
int mapa[MAPA_ALTO][MAPA_ANCHO] = {
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 2},
    {5, 6, 6, 6, 6, 6, 6, 6, 6, 7},
    {5, 6, 6, 6, 6, 6, 6, 6, 6, 7},
    {5, 6, 6, 6, 6, 6, 6, 6, 6, 7},
    {5, 6, 6, 6, 6, 6, 6, 6, 6, 7},
    {5, 6, 6, 6, 6, 6, 6, 6, 6, 7},
    {5, 6, 6, 6, 6, 6, 6, 6, 6, 7},
    {5, 6, 6, 6, 6, 6, 6, 6, 6, 7},
    {5, 6, 6, 6, 6, 6, 6, 6, 6, 7},
    {10, 11, 11, 11, 11, 11, 11, 11, 11, 12},
};

bool esSolido(int tileId) {
  return false;
  // return tileId >= 2;
}

enum Direccion { DOWN = 0, UP = 1, LEFT = 2, RIGHT = 3 };

int main() {
  sf::RenderWindow window(sf::VideoMode({MAPA_ANCHO * DISPLAY_SIZE + 5,
                              MAPA_ALTO * DISPLAY_SIZE + 5}),
      "Minijuego Estru 2");
  window.setFramerateLimit(60);

  // ==== Ruleta ====
  Ruleta ruleta(window);

  // ==== TILESET ====
  sf::Texture tileset;
  if (!tileset.loadFromFile("assets/tileset.png")) return -1;
  const int tilesPerRow = tileset.getSize().x / TILE_SIZE;

  sf::VertexArray map(sf::PrimitiveType::Triangles, MAPA_ANCHO * MAPA_ALTO * 6);
  for (int i = 0; i < MAPA_ALTO; ++i) {
    for (int j = 0; j < MAPA_ANCHO; ++j) {
      int tileNumber = mapa[i][j];
      int tu = tileNumber % tilesPerRow;
      int tv = tileNumber / tilesPerRow;
      int idx = (i * MAPA_ANCHO + j) * 6;
      float x = j * DISPLAY_SIZE;
      float y = i * DISPLAY_SIZE;

      // Triángulos
      map[idx + 0].position = {x, y};
      map[idx + 1].position = {x + DISPLAY_SIZE, y};
      map[idx + 2].position = {x + DISPLAY_SIZE, y + DISPLAY_SIZE};
      map[idx + 3].position = {x, y};
      map[idx + 4].position = {x + DISPLAY_SIZE, y + DISPLAY_SIZE};
      map[idx + 5].position = {x, y + DISPLAY_SIZE};

      float tx = tu * TILE_SIZE;
      float ty = tv * TILE_SIZE;

      map[idx + 0].texCoords = {tx, ty};
      map[idx + 1].texCoords = {tx + TILE_SIZE, ty};
      map[idx + 2].texCoords = {tx + TILE_SIZE, ty + TILE_SIZE};
      map[idx + 3].texCoords = {tx, ty};
      map[idx + 4].texCoords = {tx + TILE_SIZE, ty + TILE_SIZE};
      map[idx + 5].texCoords = {tx, ty + TILE_SIZE};
    }
  }

  // ==== PLAYER ====
  sf::Texture playerTexture;
  if (!playerTexture.loadFromFile("assets/player.png")) return -1;
  sf::Sprite player(playerTexture);
  player.setPosition({100.f, 100.f});
  player.setScale({ESCALA, ESCALA});  // 16x16 -> 64x64
  player.setTextureRect(sf::IntRect({0, 0}, {FRAME_SIZE, FRAME_SIZE}));

  float speed = 200.f;  // pixeles por segundo
  sf::Clock clock;

  Direccion facing = DOWN;

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent())
      if (event->is<sf::Event::Closed>()) window.close();

    float dt = clock.restart().asSeconds();

    // ==== MOVIMIENTO ====
    sf::Vector2f movement{0.f, 0.f};
    isMoving = false;

    if (!ruleta.isActive()) {  // solo moverse si la ruleta no está activa
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        movement.y -= speed * dt;
        facing = UP;
        isMoving = true;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        movement.y += speed * dt;
        facing = DOWN;
        isMoving = true;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        movement.x -= speed * dt;
        facing = LEFT;
        isMoving = true;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        movement.x += speed * dt;
        facing = RIGHT;
        isMoving = true;
      }
    }

    // ==== COLISIONES ====
    sf::FloatRect nextPos(player.getPosition(), {DISPLAY_SIZE, DISPLAY_SIZE});
    nextPos.position.x += movement.x;
    nextPos.position.y += movement.y;

    int leftTile = nextPos.position.x / DISPLAY_SIZE;
    int rightTile = (nextPos.position.x + nextPos.size.x) / DISPLAY_SIZE;
    int topTile = nextPos.position.y / DISPLAY_SIZE;
    int bottomTile = (nextPos.position.y + nextPos.size.y) / DISPLAY_SIZE;

    bool collision = false;
    for (int i = topTile; i <= bottomTile && !collision; i++) {
      for (int j = leftTile; j <= rightTile && !collision; j++) {
        if (i < 0 || j < 0 || i >= MAPA_ALTO || j >= MAPA_ANCHO) continue;
        if (esSolido(mapa[i][j])) {
          sf::FloatRect tileRect({static_cast<float>(j * DISPLAY_SIZE),
                                     static_cast<float>(i * DISPLAY_SIZE)},
              {static_cast<float>(DISPLAY_SIZE),
                  static_cast<float>(DISPLAY_SIZE)});
          if (nextPos.findIntersection(tileRect).has_value()) collision = true;
        }
      }
    }
    if (!collision) player.move(movement);

    // ==== ANIMACIÓN ====
    animationTimer += dt;
    if (animationTimer >= FRAME_DURATION) {
      animationTimer = 0.f;
      if (isMoving) currentFrame = (currentFrame + 1) % FRAME_COUNT;
      else currentFrame = 0;

      // frames en columna
      player.setTextureRect(
          sf::IntRect({facing * FRAME_SIZE, currentFrame * FRAME_SIZE},
              {FRAME_SIZE, FRAME_SIZE}));
    }

    // ==== ACTIVAR RULETA ====
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) &&
        !ruleta.isActive()) {
      ruleta.activar();
    }
    ruleta.actualizar(dt);

    // ==== RENDER ====
    window.clear(sf::Color(80, 160, 255));
    sf::RenderStates states;
    states.texture = &tileset;
    window.draw(map, states);
    window.draw(player);
    ruleta.dibujar();
    window.display();
  }

  return 0;
}
