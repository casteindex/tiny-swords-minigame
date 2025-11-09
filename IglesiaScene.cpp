#include "IglesiaScene.h"

static const sf::Vector2f POS_POCION1 = {300.f, 600.f};
static const sf::Vector2f POS_POCION2 = {500.f, 600.f};
static const sf::Vector2f POS_POCION3 = {700.f, 600.f};
static const sf::Vector2f POS_POCION4 = {900.f, 600.f};

IglesiaScene::IglesiaScene(sf::RenderWindow& windowRef, int& vidasJugador,
    std::vector<std::string> categorias, int totalPreguntas)
    : window(windowRef),
      vidasJugador(vidasJugador),
      categoriasRestantes(std::move(categorias)),
      preguntaActualPtr(nullptr),
      fondoActual(0),
      respuestasCorrectas(0),
      terminado(false),
      gano(false) {
  // clang-format off
  // --- cargar regursos ---
  for (int i = 0; i < 6; ++i) {
    std::string path = "assets/backgrounds/iglesia/iglesia_" + std::to_string(i) + ".jpg";
    if (!fondosTex[i].loadFromFile(path));
  }
  if (!potionTex.loadFromFile("assets/objects/potions.png"));
  if (!fuente.openFromFile("assets/fonts/Jacquard24-Regular.ttf"));
  // clang-format on

  // sprites
  spriteFondo = new sf::Sprite(fondosTex[0]);

  auto crearPocion = [&](int x, int y, sf::IntRect rect) {
    auto s = new sf::Sprite(potionTex, rect);
    s->setScale({0.5f, 0.5f});
    s->setPosition({(float)x, (float)y});
    return s;
  };

  spritePocion1 = crearPocion(300, 600, sf::IntRect({0, 0}, {115, 115}));
  spritePocion2 = crearPocion(500, 600, sf::IntRect({115, 0}, {115, 115}));
  spritePocion3 = crearPocion(700, 600, sf::IntRect({230, 0}, {115, 115}));
  spritePocion4 = crearPocion(900, 600, sf::IntRect({0, 115}, {115, 115}));

  jugador = new Jugador(2.4f);
  jugador->setPosition(600, 505);

  inicializarPreguntas();
  if (!colaPreguntas.empty()) preguntaActualPtr = &colaPreguntas.front();
}

IglesiaScene::~IglesiaScene() {
  delete spriteFondo;
  delete spritePocion1;
  delete spritePocion2;
  delete spritePocion3;
  delete spritePocion4;
  delete jugador;
}

// clang-format off
void IglesiaScene::inicializarPreguntas() {
  colaPreguntas = std::queue<Pregunta>();
  colaPreguntas.push(Pregunta("1. Uno de los siguientes personajes fue el encargado de pintar la capilla Sixtina:", {"A) Miguel Ángel.", "B) Donatello.", "C) Leonardo Da Vinci.", "D) Francis Bacon"}, 'A'));
  colaPreguntas.push(Pregunta("2. Genio del renacimiento que esculpió el Moisés, el David y la Pietá:", {"A) Miguel Ángel Buonarroti.", "B) Leonardo Da Vinci.", "C) Rafael Sanzio.", "D) Galileo Galilei"}, 'A'));
  colaPreguntas.push(Pregunta("3. Durante el renacimiento el estilo artístico que impregnó el arte fue el:", {"A) El Gótico.", "B) El Barroco.", "C) El Clasicismo.", "D) Romanticismo"}, 'B'));
  colaPreguntas.push(Pregunta("4. Nueva visión del hombre reflejada en arte, política y ciencias:", {"A) Antropocentrismo.", "B) Humanismo.", "C) Paradigma antropológico.", "D) Teocentrismo."}, 'B'));
  colaPreguntas.push(Pregunta("5. Cuatro genios del renacimiento han sido llevados a los comics de:", {"A) Las tortugas ninjas.", "B) Los caballeros del Zodiaco.", "C) Los cuatro fantásticos.", "D) Attack on Titan."}, 'A'));
}
// clang-format on

bool IglesiaScene::jugadorSobrePocion(const sf::Sprite& pocion) const {
  sf::FloatRect pj = jugador->getGlobalBounds();
  sf::FloatRect poc = pocion.getGlobalBounds();
  return pj.findIntersection(poc).has_value();
}

void IglesiaScene::procesarRespuesta(char tecla) {
  if (!preguntaActualPtr) return;

  bool correcta = preguntaActualPtr->esCorrecta(tecla);

  if (correcta) {
    respuestasCorrectas++;
    fondoActual = std::min(5, respuestasCorrectas);
    spriteFondo->setTexture(fondosTex[fondoActual]);

    colaPreguntas.pop();
    preguntaActualPtr =
        colaPreguntas.empty() ? nullptr : &colaPreguntas.front();

    if (!preguntaActualPtr) {
      terminado = true;        // el juego terminó
      esperandoEnter = false;  // esperamos a que el jugador presione Enter
      gano = true;
      vidasJugador++;
    }
  } else {
    fondoActual = 0;
    spriteFondo->setTexture(fondosTex[0]);
    terminado = true;
    esperandoEnter = false;
    gano = false;
  }
}

void IglesiaScene::handleInput(float dt) {
  jugador->handleInput(dt);

  static bool prevEnter = false;
  bool enterPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);

  if (!terminado) {
    char opcion = '\0';
    if (jugadorSobrePocion(*spritePocion1)) opcion = 'A';
    else if (jugadorSobrePocion(*spritePocion2)) opcion = 'B';
    else if (jugadorSobrePocion(*spritePocion3)) opcion = 'C';
    else if (jugadorSobrePocion(*spritePocion4)) opcion = 'D';

    if (enterPressed && !prevEnter && opcion != '\0' && preguntaActualPtr) {
      procesarRespuesta(opcion);
    }
  } else {
    // Juego terminado, esperamos Enter
    if (enterPressed && !prevEnter) {
      esperandoEnter = true;
    }
  }

  prevEnter = enterPressed;
}

void IglesiaScene::update(float dt) {
  jugador->update(dt);
}

void IglesiaScene::draw(sf::RenderWindow& window) {
  if (spriteFondo) window.draw(*spriteFondo);
  if (spritePocion1) window.draw(*spritePocion1);
  if (spritePocion2) window.draw(*spritePocion2);
  if (spritePocion3) window.draw(*spritePocion3);
  if (spritePocion4) window.draw(*spritePocion4);
  jugador->draw(window);

  // Mostrar pregunta y opciones si el juego no ha terminado
  if (!terminado) {
    sf::Text textoPregunta(fuente);
    textoPregunta.setString(preguntaActualPtr->getTexto());
    textoPregunta.setCharacterSize(36);
    textoPregunta.setFillColor(sf::Color::Yellow);
    textoPregunta.setOutlineThickness(0.2f);
    textoPregunta.setPosition({100.f, 120.f});
    window.draw(textoPregunta);

    const auto& opciones = preguntaActualPtr->getOpciones();
    float y = 200.f;
    for (int i = 0; i < opciones.size(); ++i) {
      sf::Text opt(fuente);
      opt.setString(opciones[i]);
      opt.setCharacterSize(30);
      opt.setFillColor(sf::Color::Yellow);
      opt.setOutlineThickness(0.5f);
      opt.setPosition({120.f, y});
      y += 40.f;
      window.draw(opt);
    }

    sf::Text instr(fuente);
    instr.setString("Presiona ENTER para responder");
    instr.setCharacterSize(28);
    instr.setFillColor(sf::Color::White);
    instr.setOutlineThickness(0.5f);
    instr.setPosition({120.f, y + 40.f});
    window.draw(instr);
  }

  // Mensaje de resultado
  if (terminado) {
    sf::Text resultado(fuente);
    resultado.setCharacterSize(48);
    resultado.setFillColor(gano ? sf::Color::Green : sf::Color::Red);
    resultado.setOutlineThickness(0.5f);
    // clang-format off
    resultado.setString(gano ? "¡Has devuelto el color a la iglesia!\nPresiona Enter para regresar a La Ruleta del Saber"
                             : "¡No devolviste el color a la iglesia!\nPresiona Enter para regresar a La Ruleta del Saber");
    resultado.setPosition({200.f, 300});
    window.draw(resultado);
    // clang-format on
  }
}

bool IglesiaScene::wantsToChangeScene() const {
  return terminado && esperandoEnter;
}

Scene* IglesiaScene::nextScene(sf::RenderWindow& window, int& vidasJugador) {
  return new RuletaScene(window, vidasJugador, categoriasRestantes);
}

bool IglesiaScene::isFinished() const {
  return terminado;
}

bool IglesiaScene::playerWon() const {
  return gano;
}
