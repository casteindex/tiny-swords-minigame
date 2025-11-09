#include "CalderoScene.h"

const float CalderoScene::DURACION_FRAME = 0.12f;

static const sf::Vector2f POS_FONDO = {0.f, 0.f};
static const sf::Vector2f POS_CALDERO = {530.f, 300.f};
static const sf::Vector2f POS_PREGUNTA = {100.f, 60.f};
static const sf::Vector2f POS_OPCIONES = {100.f, 170.f};
static const float DISTANCIA_ENTRE_OPCIONES = 40.f;

CalderoScene::CalderoScene(sf::RenderWindow& windowRef, int& vidasJugador,
    std::vector<std::string> categoriasRestantes, int totalPreguntas)
    : categoriasRestantes(std::move(categoriasRestantes)),
      vidasJugador(vidasJugador),
      preguntaActualPtr(nullptr),
      totalPreguntasIniciales(totalPreguntas),
      respuestasCorrectas(0),
      terminado(false),
      gano(false),
      keyLocked(false),
      frameX(0),
      frameY(4),
      tiempoAcumulado(0.f),
      inicioBloque({4, 0}) {
  // clang-format off
  if (!texFondo.loadFromFile("assets/backgrounds/fondo_caldera.jpg"));
  if (!texCaldero.loadFromFile("assets/objects/caldera.png"));
  if (!fuente.openFromFile("assets/fonts/Jacquard24-Regular.ttf"));
  // clang-format on

  // Fondo
  spriteFondo = new sf::Sprite(texFondo);
  spriteFondo->setPosition(POS_FONDO);
  spriteFondo->setColor(sf::Color(255, 255, 255, 180));

  // Caldero
  spriteCaldero = new sf::Sprite(texCaldero);
  spriteCaldero->setPosition(POS_CALDERO);
  spriteCaldero->setScale({6.f, 6.f});

  setFrame(frameY, frameX);
  inicializarPreguntas();
  if (!colaPreguntas.empty()) preguntaActualPtr = &colaPreguntas.front();
}

CalderoScene::~CalderoScene() {
  delete spriteFondo;
  delete spriteCaldero;
}

// clang-format off
void CalderoScene::inicializarPreguntas() {
  colaPreguntas = std::queue<Pregunta>();
  colaPreguntas.push(Pregunta("Entre los siguientes renacentistas seleccione, uno de los precursores\nfilosofo-cientifico del heliocentrismo:", {"A) Tomas Moro", "B) Galileo", "C) Platón", "D) Arquímedes"}, 'B'));
  colaPreguntas.push(Pregunta("El método científico se introduce por el interés de tres filósofos.\nEntre los siguientes uno de los mencionados no es precursor del método científico:", {"A) Francis Bacon", "B) Galileo Galilei", "C) Nicolás Maquiavelo", "D) René Descartes"}, 'C'));
  colaPreguntas.push(Pregunta("Es uno de los precursores del pensamiento Moderno:", {"A) Isaac Newton", "B) René Descartes", "C) Erasmo de Roterdam", "D) Francis Bacon"}, 'B'));
  colaPreguntas.push(Pregunta("De los siguientes filósofos niega el geocentrismo:", {"A) Aristóteles", "B) Nicolás Copérnico", "C) Tomás de Aquino", "D) Isaac Newton"}, 'B'));
  colaPreguntas.push(Pregunta("Uno de los inventos que suscitó un conocimiento ilimitado fue el\nde Gutenberg:", {"A) El astrolabio", "B) La imprenta", "C) La Nao y la Carabela", "D) El Telescopio"}, 'B'));
}
// clang-format on

void CalderoScene::setFrame(int fila, int columna) {
  rectFrame = sf::IntRect({columna * FRAME_WIDTH, fila * FRAME_HEIGHT},
      {FRAME_WIDTH, FRAME_HEIGHT});
  spriteCaldero->setTextureRect(rectFrame);
}

void CalderoScene::handleInput(float dt) {
  bool a = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
  bool b = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B);
  bool c = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C);
  bool d = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

  if (!keyLocked && (a || b || c || d) && preguntaActualPtr) {
    keyLocked = true;
    char tecla = a ? 'A' : b ? 'B' : c ? 'C' : 'D';
    procesarRespuesta(tecla);
  }
  if (!a && !b && !c && !d) keyLocked = false;
}

void CalderoScene::procesarRespuesta(char tecla) {
  if (!preguntaActualPtr) return;

  bool correcta = preguntaActualPtr->esCorrecta(tecla);

  if (correcta) {
    respuestasCorrectas++;
    switch (respuestasCorrectas) {
      case 1: inicioBloque = {4, 3}; break;
      case 2: inicioBloque = {4, 6}; break;
      case 3: inicioBloque = {4, 9}; break;
      case 4: inicioBloque = {0, 6}; break;
      case 5:
        inicioBloque = {0, 9};
        gano = true;
        vidasJugador++;
        terminado = true;
        break;
    }
  } else {
    inicioBloque = {0, 3};
    terminado = true;
    gano = false;
  }

  // reiniciar frame al inicio del bloque
  frameY = inicioBloque.first;
  frameX = inicioBloque.second;

  colaPreguntas.pop();
  preguntaActualPtr = colaPreguntas.empty() ? nullptr : &colaPreguntas.front();
}

void CalderoScene::update(float dt) {
  tiempoAcumulado += dt;
  if (tiempoAcumulado >= DURACION_FRAME) {
    tiempoAcumulado = 0.f;

    // avanzar frame dentro del bloque 3x3
    frameX++;
    if (frameX >= inicioBloque.second + 3) {
      frameX = inicioBloque.second;
      frameY++;
      if (frameY >= inicioBloque.first + 3) frameY = inicioBloque.first;
    }

    setFrame(frameY, frameX);
  }
}

void CalderoScene::draw(sf::RenderWindow& window) {
  if (spriteFondo) window.draw(*spriteFondo);
  if (spriteCaldero) window.draw(*spriteCaldero);

  if (!terminado && preguntaActualPtr) {
    sf::Text pregunta(fuente);
    pregunta.setString(preguntaActualPtr->getTexto());
    pregunta.setCharacterSize(36);
    pregunta.setFillColor(sf::Color::White);
    pregunta.setOutlineThickness(1.0f);
    pregunta.setPosition(POS_PREGUNTA);
    window.draw(pregunta);

    const auto& opciones = preguntaActualPtr->getOpciones();
    for (size_t i = 0; i < opciones.size(); ++i) {
      sf::Text opt(fuente);
      opt.setString(opciones[i]);
      opt.setCharacterSize(30);
      opt.setFillColor(sf::Color::Yellow);
      opt.setOutlineThickness(1.0f);
      opt.setPosition(
          {POS_OPCIONES.x, POS_OPCIONES.y + i * DISTANCIA_ENTRE_OPCIONES});
      window.draw(opt);
    }

    sf::Text instr(fuente);
    instr.setString("Presiona A, B, C o D");
    instr.setCharacterSize(28);
    instr.setFillColor(sf::Color::White);
    instr.setOutlineThickness(1.0f);
    instr.setPosition(
        {POS_OPCIONES.x, POS_OPCIONES.y + 4 * DISTANCIA_ENTRE_OPCIONES + 20.f});
    window.draw(instr);
  } else if (terminado) {
    sf::Text resultado(fuente);
    resultado.setCharacterSize(40);
    resultado.setOutlineThickness(1.0f);
    // clang-format off
    resultado.setFillColor(gano ? sf::Color::Green : sf::Color::Red);
    resultado.setString(gano ? "¡La poción está lista! Presiona Enter para regresar a La Ruleta del Saber"
                             : "¡La poción se arruinó! Presiona Enter para regresar a La Ruleta del Saber");
    // clang-format on
    resultado.setPosition({100.f, 650.f});
    window.draw(resultado);
  }
}

bool CalderoScene::wantsToChangeScene() const {
  return terminado && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
}

Scene* CalderoScene::nextScene(sf::RenderWindow& window, int& vidasJugador) {
  return new RuletaScene(window, vidasJugador, categoriasRestantes);
}

bool CalderoScene::isFinished() const {
  return terminado;
}
bool CalderoScene::playerWon() const {
  return gano;
}
