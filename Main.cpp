#include <SFML/Graphics.hpp>
#include <vector>

const int TILE_SIZE = 32;
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;

// Un mapa simple con IDs de tiles
// clang-format off
int mapData[MAP_HEIGHT][MAP_WIDTH] = {
  {0,0,0,0,1,1,1,0,0,0},
  {0,0,1,1,1,2,1,1,0,0},
  {0,1,1,2,2,2,2,1,1,0},
  {0,1,2,2,3,3,2,2,1,0},
  {1,1,2,3,3,3,3,2,1,1},
  {1,2,3,3,3,3,3,3,2,1},
  {0,1,2,3,3,3,3,2,1,0},
  {0,0,1,2,2,2,2,1,0,0},
  {0,0,0,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0}
};
// clang-format off

int main() {
  sf::RenderWindow window(sf::VideoMode({640, 480}), "Mini RPG - Base");
  window.setFramerateLimit(60);

  // === TILESET ===
  sf::Texture tileset;
  if (!tileset.loadFromFile("assets/tileset.png")) {
    return -1;
  }

  // Calculamos cuántos tiles por fila tiene el tileset
  const int tilesPerRow = tileset.getSize().x / TILE_SIZE;

  // === MAPA: ahora con Triangles (6 vértices por tile) ===
  sf::VertexArray map(sf::PrimitiveType::Triangles, MAP_WIDTH * MAP_HEIGHT * 6);

  for (int i = 0; i < MAP_HEIGHT; ++i) {
    for (int j = 0; j < MAP_WIDTH; ++j) {
      int tileNumber = mapData[i][j];

      int tu = tileNumber % tilesPerRow;
      int tv = tileNumber / tilesPerRow;

      // índice del primer vértice de este tile en el vertex array
      int idx = (i * MAP_WIDTH + j) * 6;
      // coordenadas en pantalla
      float x = j * TILE_SIZE;
      float y = i * TILE_SIZE;

      // Primer triángulo (v0, v1, v2)
      map[idx + 0].position = {x, y};
      map[idx + 1].position = {x + TILE_SIZE, y};
      map[idx + 2].position = {x + TILE_SIZE, y + TILE_SIZE};

      // Segundo triángulo (v3, v4, v5)
      map[idx + 3].position = {x, y};
      map[idx + 4].position = {x + TILE_SIZE, y + TILE_SIZE};
      map[idx + 5].position = {x, y + TILE_SIZE};

      // texture coords (misma lógica, usando tu/tv)
      float tx = tu * TILE_SIZE;
      float ty = tv * TILE_SIZE;

      // Primer triángulo texCoords
      map[idx + 0].texCoords = {tx, ty};
      map[idx + 1].texCoords = {tx + TILE_SIZE, ty};
      map[idx + 2].texCoords = {tx + TILE_SIZE, ty + TILE_SIZE};

      // Segundo triángulo texCoords
      map[idx + 3].texCoords = {tx, ty};
      map[idx + 4].texCoords = {tx + TILE_SIZE, ty + TILE_SIZE};
      map[idx + 5].texCoords = {tx, ty + TILE_SIZE};
    }
  }

  // === PLAYER ===
  sf::Texture playerTexture;
  if (!playerTexture.loadFromFile("assets/player.png")) {
    return -1;
  }

  // Nota: en SFML 3 no hay constructor por defecto para Sprite,
  // así que construimos con la textura:
  sf::Sprite player(playerTexture);
  player.setPosition({100.f, 100.f});
  player.setScale({1.0f, 1.0f});

  float speed = 100.f;  // píxeles por segundo
  sf::Clock clock;

  // ==== LOOP PRINCIPAL ====
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) window.close();
    }

    float dt = clock.restart().asSeconds();

    // ==== MOVIMIENTO ====
    sf::Vector2f movement{0.f, 0.f};
    bool movingLeft = false;
    bool movingRight = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
      movement.y -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
      movement.y += speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
      movement.x -= speed * dt;
      movingLeft = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
      movement.x += speed * dt;
      movingRight = true;
    }

    // Aplicar espejo según dirección
    if (movingLeft) {
      // usamos getLocalBounds().size.x (SFML 3: Rect tiene position y size)
      float w = player.getLocalBounds().size.x;
      player.setOrigin({w, 0.f});  // setOrigin espera Vector2f
      player.setScale({-1.f, 1.f});
    } else if (movingRight) {
      // origen normal
      player.setOrigin({0.f, 0.f});
      player.setScale({1.f, 1.f});
    }

    player.move(movement);

    // ==== RENDER ====
    window.clear(sf::Color(80, 160, 255));
    sf::RenderStates states;
    states.texture =
        &tileset;  // esto sigue funcionando para asignar la textura
    window.draw(map, states);
    window.draw(player);
    window.display();
  }

  return 0;
}
