#include "BatallaFinalScene.h"

static const sf::Vector2f POS_JUGADOR = {300.f, 500.f};
static const sf::Vector2f POS_NPC = {950.f, 500.f};
static const sf::Vector2f POS_PREGUNTA = {80.f, 90.f};
static const sf::Vector2f POS_OPCIONES = {80.f, 190.f};
static const float DISTANCIA_ENTRE_OPCIONES = 40.f;

BatallaFinalScene::BatallaFinalScene(
    sf::RenderWindow& windowRef, int& vidasJugador, bool esEmpirista)
    : window(windowRef),
      esEmpirista(esEmpirista),
      vidasJugador(vidasJugador),
      vidasNPC(4),
      spriteRayo(nullptr),
      preguntaActual(0),
      terminado(false),
      gano(false),
      keyLocked(false) {
  // clang-format off
  // --- cargar recursos ---
  if(!texRayo.loadFromFile("assets/objects/rayo.png"));
  if(!fuente.openFromFile("assets/fonts/Jacquard24-Regular.ttf"));
  if(!fondoTexture.loadFromFile("assets/backgrounds/fondo_batalla_final.jpg"));
  if(!texHeartsNPC.loadFromFile("assets/ui/hearts.png"));
  if(!musicaEmpirista.openFromFile("assets/music/empiristas.wav"));
  if(!musicaRacionalista.openFromFile("assets/music/racionalistas.wav"));

  // --- fondo ----
  spriteFondo = new sf::Sprite(fondoTexture);
  spriteFondo->setPosition({0.f, 0.f});

  jugador = new Jugador(3.0f);
  // --- cambiar textura del jugador (color de ropa)
  if (esEmpirista) jugador->setTextures("assets/player/idle_empirista.png", "assets/player/walk_empirista.png");
  else jugador->setTextures("assets/player/idle_racionalista.png", "assets/player/walk_racionalista.png");
  jugador->setPosition(POS_JUGADOR.x, POS_JUGADOR.y);
  
  // --- elegir textura del npc (color de ropa)
  if(esEmpirista) npc = new NPC("assets/npc/npc_racionalista.png", 3.0f, true);
  else npc = new NPC("assets/npc/npc_empirista.png", 3.0f, true);
  npc->setPosition(POS_NPC.x, POS_NPC.y);
  
  // --- corazones NPC ---
  heartsNPC.reserve(4);
  for (int i = 0; i < 4; ++i) {
    sf::Sprite* sprite = new sf::Sprite(texHeartsNPC);
    sprite->setTextureRect(sf::IntRect({0, 0}, {16, 16}));  // corazón lleno
    sprite->setScale({3.f, 3.f});
    sprite->setPosition({1180.f - i * 55.f, 20.f}); // esquina superior derecha
    heartsNPC.push_back(sprite);
  }
  inicializarPreguntas();
}

BatallaFinalScene::~BatallaFinalScene() {
  delete npc;
  delete spriteFondo;
  delete spriteRayo;
  for (auto sprite : heartsNPC) delete sprite;
}

void BatallaFinalScene::inicializarPreguntas() {
  bancoPreguntas.push_back(Pregunta("1. Para algunos de los siguientes filósofos, el criterio de verdad es la evidencia sensible:", {"A) Empiristas.", "B) Criticistas.", "C) Racionalistas.", "D) Dogmáticos"}, 'A'));
  bancoPreguntas.push_back(Pregunta("2. De las siguientes, una de ellas es la corriente filosófica que en general tiende a negar la\nposibilidad de la metafísica y a sostener que hay conocimiento únicamente de los fenómenos.", {"A) Racionalistas.", "B) Empiristas.", "C) Escolásticos.", "D) Escépticos"}, 'B'));
  bancoPreguntas.push_back(Pregunta("3. Para unos de los siguientes filósofos, la experiencia como única fuente del conocimiento.", {"A) Epistemólogos.", "B) Racionalistas.", "C) Empiristas.", "D) Escépticos"}, 'C'));
  bancoPreguntas.push_back(Pregunta("4. Filósofos para quienes la única fuente del conocimiento es la razón.", {"A) Epistemólogos.", "B) Racionalistas.", "C) Empiristas.", "D) Escépticos"}, 'B'));
  bancoPreguntas.push_back(Pregunta("5. Filósofos que postulan las ideas innatas en el sujeto.", {"A) Empiristas.", "B) Idealistas.", "C) Racionalistas.", "D) Innatistas"}, 'C'));
  bancoPreguntas.push_back(Pregunta("6. De los siguientes filósofos seleccione el que no se considera Racionalista:", {"A) David Hume.", "B) John Locke.", "C) Nicolas Malebranch.", "D) Francis Bacon."}, 'C'));
  bancoPreguntas.push_back(Pregunta("7. Es la doctrina que establece que todos nuestros conocimientos provienen de la razón.", {"A) Empirismo.", "B) Criticismo.", "C) Racionalismo.", "D) Epistemología"}, 'C'));
  bancoPreguntas.push_back(Pregunta("8. Uno de los siguientes filósofos postula las ideas innatas en el sujeto:", {"A) George Berkeley.", "B) David Hume.", "C) Leibniz.", "D) Hipatía"}, 'C'));
  // Hacer un shuffle después de aregar preguntas para que siempre sean distintas
  // Obtenido de: https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector/
  std::shuffle(bancoPreguntas.begin(), bancoPreguntas.end(), std::mt19937(std::random_device{}()));
}
// clang-format on

void BatallaFinalScene::handleInput(float deltaTime) {
  if (terminado) return;

  jugador->handleInput(deltaTime);

  bool a = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
  bool b = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B);
  bool c = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C);
  bool d = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

  if (!keyLocked && (a || b || c || d)) {
    keyLocked = true;
    char tecla = a ? 'A' : b ? 'B' : c ? 'C' : 'D';
    procesarRespuesta(tecla);
  }
  if (!a && !b && !c && !d) keyLocked = false;
}

void BatallaFinalScene::procesarRespuesta(char tecla) {
  if (bancoPreguntas.empty() || terminado) return;

  Pregunta& pregunta = bancoPreguntas.front();
  bool correcta = pregunta.esCorrecta(tecla);

  // --- Rayo, vidas y música ---
  if (correcta) {
    lanzarRayo(false);  // rayo cae al npc
    if (esEmpirista) {
      musicaEmpirista.play();
      musicaRacionalista.pause();
    } else {
      musicaRacionalista.play();
      musicaEmpirista.pause();
    }

    // actualizar corazones del NPC
    vidasNPC--;
    for (int i = 0; i < heartsNPC.size(); i++) {
      if (i < vidasNPC)
        heartsNPC[i]->setTextureRect(sf::IntRect({0, 0}, {16, 16}));  // lleno
      else
        heartsNPC[i]->setTextureRect(sf::IntRect({16, 0}, {16, 16}));  // vacío
    }
    if (vidasNPC <= 0) {
      gano = true;
      terminado = true;
    }
  } else {
    lanzarRayo(true);  // rayo cae al jugador
    if (esEmpirista) {
      musicaRacionalista.play();
      musicaEmpirista.pause();
    } else {
      musicaEmpirista.play();
      musicaRacionalista.pause();
    }
    vidasJugador--;
    if (vidasJugador <= 0) {
      gano = false;
      terminado = true;
    }
  }

  bancoPreguntas.erase(bancoPreguntas.begin());
  if (bancoPreguntas.empty() && !terminado) {
    gano = vidasNPC < vidasJugador;
    terminado = true;
  }
}

void BatallaFinalScene::lanzarRayo(bool sobreJugador) {
  if (spriteRayo) delete spriteRayo;
  spriteRayo = new sf::Sprite(texRayo);
  frameActualRayo = 0;
  tiempoActualRayo = 0.f;
  mostrandoRayo = true;
  rayoSobreJugador = sobreJugador;

  int frameWidth = texRayo.getSize().x / columnasRayo;
  int frameHeight = texRayo.getSize().y / 2;

  frameRayo = {{0, 0}, {frameWidth, frameHeight}};
  spriteRayo->setTextureRect(frameRayo);
  spriteRayo->setScale({3.f, 3.f});
  spriteRayo->setOrigin({frameWidth / 2.f, frameHeight / 2.f});

  sf::Vector2f pos;
  if (sobreJugador) {
    auto bbox = jugador->getGlobalBounds();
    pos = {bbox.position.x + bbox.size.x * 0.5f, bbox.position.y - 40.f};
  } else {
    auto bbox = npc->getGlobalBounds();
    pos = {bbox.position.x + bbox.size.x * 0.5f, bbox.position.y - 40.f};
  }
  spriteRayo->setPosition(pos);
}

void BatallaFinalScene::update(float deltaTime) {
  jugador->update(deltaTime);
  npc->update(deltaTime);

  if (mostrandoRayo && spriteRayo) {
    tiempoActualRayo += deltaTime;
    if (tiempoActualRayo >= frameTimeRayo) {
      tiempoActualRayo = 0.f;
      frameActualRayo++;

      if (frameActualRayo >= totalFramesRayo) mostrandoRayo = false;
      else {
        int col = frameActualRayo % columnasRayo;
        int fila = frameActualRayo / columnasRayo;
        int frameWidth = frameRayo.size.x;
        int frameHeight = frameRayo.size.y;
        frameRayo.position.x = col * frameWidth;
        frameRayo.position.y = fila * frameHeight;
        spriteRayo->setTextureRect(frameRayo);
      }
    }
  }
}

void BatallaFinalScene::draw(sf::RenderWindow& window) {
  if (spriteFondo) window.draw(*spriteFondo);
  jugador->draw(window);
  if (npc) npc->draw(window);
  for (auto& heart : heartsNPC) window.draw(*heart);
  if (mostrandoRayo && spriteRayo) window.draw(*spriteRayo);

  if (!terminado && !bancoPreguntas.empty()) {
    const auto& p = bancoPreguntas.front();
    sf::Text txtPregunta(fuente);
    txtPregunta.setString(p.getTexto());
    txtPregunta.setCharacterSize(36);
    txtPregunta.setFillColor(sf::Color::White);
    txtPregunta.setPosition(POS_PREGUNTA);
    window.draw(txtPregunta);

    const auto& opciones = p.getOpciones();
    for (size_t i = 0; i < opciones.size(); ++i) {
      sf::Text txt(fuente);
      txt.setString(opciones[i]);
      txt.setCharacterSize(30);
      txt.setFillColor(sf::Color::Yellow);
      txt.setPosition(
          {POS_OPCIONES.x, POS_OPCIONES.y + i * DISTANCIA_ENTRE_OPCIONES});
      window.draw(txt);
    }
  } else if (terminado) {
    sf::Text resultado(fuente);
    resultado.setCharacterSize(48);
    resultado.setFillColor(gano ? sf::Color::Green : sf::Color::Red);
    resultado.setString(gano ? "¡Has vencido!" : "Has sido derrotado...");
    resultado.setPosition({400.f, 400.f});
    window.draw(resultado);

    sf::Text continuar(fuente);
    continuar.setCharacterSize(32);
    continuar.setFillColor(sf::Color::White);
    continuar.setString("Presiona Enter para continuar");
    continuar.setPosition({420.f, 470.f});
    window.draw(continuar);
  }
}

bool BatallaFinalScene::playerWon() const {
  return gano;
}

bool BatallaFinalScene::wantsToChangeScene() const {
  return terminado && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
}

Scene* BatallaFinalScene::nextScene(
    sf::RenderWindow& window, int& vidasJugador) {
  if (!gano) {
    window.close();
    return nullptr;
  }
  return new EdificioScene(window, jugador);
}
