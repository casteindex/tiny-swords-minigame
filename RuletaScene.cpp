#include "RuletaScene.h"

// Constructor con solo un parámetro llama al otro
RuletaScene::RuletaScene(sf::RenderWindow& window, int& vidasJugador)
    : RuletaScene(
          window, vidasJugador, {"Arte", "Política", "Ciencia", "Historia"}) {}

RuletaScene::RuletaScene(sf::RenderWindow& window, int& vidasJugador,
    std::vector<std::string> categorias)
    : window(window),
      vidasJugador(vidasJugador),
      categoriasRestantes(std::move(categorias)),
      jugador(3.0f),
      terminado(false),
      eligiendoBando(false),
      bandoElegido(false),
      esEmpirista(false) {
  // --- cargar recursos ---
  // clang-format off
  if (!backgroundTex.loadFromFile("assets/backgrounds/fondo_ruleta.jpg"));
  if (!empiristasTex.loadFromFile("assets/objects/empiristas_badge.png"));
  if (!racionalistasTex.loadFromFile("assets/objects/racionalistas_badge.png"));
    if (!fuente.openFromFile("assets/fonts/Jacquard24-Regular.ttf"));
  // clang-format on

  // crear sprites
  background = new sf::Sprite(backgroundTex);
  background->setPosition({0.f, 0.f});
  background->setColor(sf::Color(255, 255, 255, 255));  // opacidad máxima

  jugador.setPosition(200.f, 500.f);
  ruleta = new Ruleta(window, categoriasRestantes, sf::Vector2f(350.f, 250.f));

  empiristasSprite = new sf::Sprite(empiristasTex);
  racionalistasSprite = new sf::Sprite(racionalistasTex);
  empiristasSprite->setScale({6.f, 6.f});
  racionalistasSprite->setScale({6.f, 6.f});
  empiristasSprite->setPosition({350.f, 300.f});
  racionalistasSprite->setPosition({800.f, 300.f});

  // DEBUG: imprimir categorias
  for (const auto& c : ruleta->getCategorias()) {
    std::cout << "[" << c << "]";
  }
  std::cout << std::endl;
}

RuletaScene::~RuletaScene() {
  delete background;
  delete ruleta;
  delete empiristasSprite;
  delete racionalistasSprite;
}

void RuletaScene::handleInput(float deltaTime) {
  if (eligiendoBando) {
    // Detección de clic sobre los emblemas
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      auto mousePos = sf::Mouse::getPosition(window);

      sf::FloatRect empBounds = empiristasSprite->getGlobalBounds();
      sf::FloatRect racBounds = racionalistasSprite->getGlobalBounds();

      if (empBounds.contains({(float)mousePos.x, (float)mousePos.y})) {
        std::cout << "Bando elegido: Empiristas\n";
        terminado = true;
        bandoElegido = true;
        esEmpirista = true;
      } else if (racBounds.contains({(float)mousePos.x, (float)mousePos.y})) {
        std::cout << "Bando elegido: Racionalistas\n";
        terminado = true;
        bandoElegido = true;
        esEmpirista = false;
      }
    }
    return;
  }
  // resto del código normal: cuando NO se está eligiendo bando
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) &&
      !ruleta->estaGirando()) {
    ruleta->iniciarGiro();
  }
  jugador.handleInput(deltaTime);
}

void RuletaScene::update(float deltaTime) {
  if (eligiendoBando) return;  // no actualizar nada más
  jugador.update(deltaTime);
  ruleta->actualizar(deltaTime);
  if (ruleta->haTerminado() && !terminado) {
    terminado = true;
    minijuego = ruleta->getCategoriaSeleccionada();
  }
  // Cuando ya no hay categorías restantes, pasamos al modo de elección
  if (categoriasRestantes.empty()) {
    eligiendoBando = true;
  }
}

void RuletaScene::draw(sf::RenderWindow& window) {
  if (background) window.draw(*background);

  jugador.draw(window);

  if (eligiendoBando) {
    // Fondo semitransparente
    sf::RectangleShape overlay({1280.f, 720.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    // Título local
    sf::Text titulo(fuente);
    titulo.setString("Elige tu bando:\nEmpiristas o Racionalistas");
    titulo.setCharacterSize(48);
    titulo.setFillColor(sf::Color::White);
    titulo.setPosition({300.f, 100.f});
    window.draw(titulo);

    window.draw(*empiristasSprite);
    window.draw(*racionalistasSprite);
  } else {
    ruleta->dibujar();
  }
}

bool RuletaScene::wantsToChangeScene() const {
  return terminado &&
         (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) || bandoElegido);
}

// clang-format off
Scene* RuletaScene::nextScene(sf::RenderWindow& window, int& vidasJugador) {
  if(bandoElegido) return new BatallaFinalScene(window, vidasJugador, esEmpirista);

  categoriasRestantes.erase(std::remove(categoriasRestantes.begin(), categoriasRestantes.end(), minijuego), categoriasRestantes.end());
  
  if (minijuego == "Ciencia") return new CalderoScene(window, vidasJugador, categoriasRestantes);
  if (minijuego == "Política") return new  EspadaScene(window, vidasJugador, categoriasRestantes);
  if (minijuego == "Historia") return new LibroScene(window, vidasJugador, categoriasRestantes);
  if (minijuego == "Arte") return new IglesiaScene(window, vidasJugador, categoriasRestantes);
  
  // no debería pasar:
  else return new RuletaScene(window, vidasJugador, categoriasRestantes);
}
// clang-format on
