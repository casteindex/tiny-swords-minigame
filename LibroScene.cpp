#include "LibroScene.h"

static const sf::Vector2f POS_FONDO = {0.f, 0.f};
static const sf::Vector2f POS_PREGUNTA = {120.f, 150.f};
static const sf::Vector2f POS_OPCIONES = {120.f, 330.f};
static const float DISTANCIA_ENTRE_OPCIONES = 50.f;

LibroScene::LibroScene(sf::RenderWindow& windowRef, int& vidasJugador,
    std::vector<std::string> categorias, int totalPreguntas)
    : window(windowRef),
      vidasJugador(vidasJugador),
      categoriasRestantes(std::move(categorias)),
      totalPreguntasIniciales(totalPreguntas),
      preguntaActualPtr(nullptr),
      respuestasCorrectas(0),
      terminado(false),
      gano(false),
      calaveraAlpha(0.f),
      calaveraTarget(0.f),
      showCalavera(false) {
  // clang-format off
  if (!texFondo.loadFromFile("assets/backgrounds/fondo_libro.jpg"));
  if (!texLibro.loadFromFile("assets/objects/libro.png"));
  if (!texCalavera.loadFromFile("assets/objects/calavera.png"));
  if (!fuente.openFromFile("assets/fonts/Jacquard24-Regular.ttf"));

  spriteFondo = new sf::Sprite(texFondo);
  spriteFondo->setPosition(POS_FONDO);
  spriteFondo->setColor(sf::Color(255, 255, 255, 128));

  spriteLibro = new sf::Sprite(texLibro);
  spriteLibro->setOrigin({texLibro.getSize().x / 2.f, texLibro.getSize().y / 2.f});
  spriteLibro->setPosition({window.getSize().x / 2.f + 10.f, window.getSize().y / 2.f + 40.f});
  spriteLibro->setScale({2.5f, 2.5f});

  spriteCalavera = new sf::Sprite(texCalavera);
  spriteCalavera->setOrigin({texCalavera.getSize().x / 2.f, texCalavera.getSize().y / 2.f});
  spriteCalavera->setPosition({window.getSize().x / 2.f, window.getSize().y / 2.f - 20.f});
  spriteCalavera->setColor(sf::Color(255, 255, 255, 0));
  spriteCalavera->setScale({1.1f, 1.1f});

  optionAlpha.assign(4, 0.f);
  optionTarget.assign(4, 0.f);

  inicializarPreguntas();
  if (!colaPreguntas.empty()) preguntaActualPtr = &colaPreguntas.front();
}

LibroScene::~LibroScene() {
  delete spriteFondo;
  delete spriteLibro;
  delete spriteCalavera;
}

void LibroScene::inicializarPreguntas() {
  colaPreguntas = std::queue<Pregunta>();
  colaPreguntas.push(Pregunta("1. Después del feudalismo medieval acudimos al surgimiento\nde una nueva clase social conocida como la:", {"A) La monarquía.", "B) El mercantilismo.", "C) La burguesía.", "D) El proletariado."}, 'C'));
  colaPreguntas.push(Pregunta("2. El renacimiento supone una época de absolutismo y\nnacionalismos, como el nacimiento de fuertes monarquías\neuropeas centralizadas como:", {"A) Grecia.", "B) Inglaterra.", "C) Yugoslavia.", "D) Egipto."}, 'B'));
  colaPreguntas.push(Pregunta("3. Antes de la consolidación del estado moderno, Italia estuvo\ndividida en pequeñas ciudades-estado normalmente enfrentadas\nentre sí, como es el caso de:", {"A) Florencia-Napoli.", "B) Ámsterdam-Cracovia.", "C) Reims-Colonia.", "D) Milán-Lourdes."}, 'A'));
  colaPreguntas.push(Pregunta("4. La toma de Constantinopla supone un bloqueo comercial entre\nEuropa y Asia (la ruta de la seda) y ocurrió en lo que hoy es\nactualmente:", {"A) Eslovaquia.", "B) Estambul en Turquía.", "C) Mesopotamia.", "D) Jerusalén."}, 'B'));
  colaPreguntas.push(Pregunta("5. Resurge el interés por Grecia y Roma, junto al declive del\nsistema feudal, el crecimiento del comercio e innovaciones entre\nlas que mencionamos:", {"A) La imprenta y la brújula.", "B) La rueda y la escritura.", "C) Las máquinas de vapor y la producción en masa.", "D) La pólvora y la rueda."}, 'A'));
}
// clang-format off

void LibroScene::handleInput(float dt) {
  if (terminado) return;

  auto mp = sf::Mouse::getPosition(window);
  float mx = static_cast<float>(mp.x);
  float my = static_cast<float>(mp.y);

  const float optionHeight = 30.f;

  for (int i = 0; i < 4; ++i) {
    float top = POS_OPCIONES.y + i * DISTANCIA_ENTRE_OPCIONES;
    float bottom = top + optionHeight;
    if (mx >= POS_OPCIONES.x && mx <= POS_OPCIONES.x + 600.f && my >= top &&
        my <= bottom) {
      optionTarget[i] = 255.f * 0.7f;
    } else {
      optionTarget[i] = 0.f;
    }
  }

  bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
  static bool prevMousePressed = false;
  if (mousePressed && !prevMousePressed && preguntaActualPtr) {
    for (int i = 0; i < 4; ++i) {
      if (optionTarget[i] > 0.f) {
        char tecla = 'A' + static_cast<char>(i);
        procesarRespuesta(tecla);
        break;
      }
    }
  }
  prevMousePressed = mousePressed;
}

void LibroScene::procesarRespuesta(char tecla) {
  if (!preguntaActualPtr) return;
  bool correcta = preguntaActualPtr->esCorrecta(tecla);

  if (correcta) {
    colaPreguntas.pop();
    preguntaActualPtr =
        colaPreguntas.empty() ? nullptr : &colaPreguntas.front();

    for (auto& a : optionAlpha) a = 0.f;
    for (auto& t : optionTarget) t = 0.f;

    if (!preguntaActualPtr) {
      gano = true;
      vidasJugador++;
      terminado = true;
    }
  } else {
    showCalavera = true;
    calaveraTarget = 255.f * 0.3f; // 50%
    terminado = true;
    gano = false;
  }
}

void LibroScene::update(float dt) {
  for (size_t i = 0; i < optionAlpha.size(); ++i) {
    if (optionAlpha[i] < optionTarget[i])
      optionAlpha[i] =
          std::min(optionTarget[i], optionAlpha[i] + fadeSpeed * dt);
    else if (optionAlpha[i] > optionTarget[i])
      optionAlpha[i] =
          std::max(optionTarget[i], optionAlpha[i] - fadeSpeed * dt);
  }

  if (showCalavera) {
    if (calaveraAlpha < calaveraTarget)
      calaveraAlpha = std::min(calaveraTarget, calaveraAlpha + fadeSpeed * dt);
  }
}

void LibroScene::draw(sf::RenderWindow& window) {
  if (spriteFondo) window.draw(*spriteFondo);
  if (spriteLibro) window.draw(*spriteLibro);

  if (preguntaActualPtr && !terminado) {
    sf::Text pregunta(fuente);
    pregunta.setString(preguntaActualPtr->getTexto());
    pregunta.setCharacterSize(48);
    pregunta.setFillColor(sf::Color::Black);
    pregunta.setPosition(POS_PREGUNTA);
    window.draw(pregunta);

    const auto& opciones = preguntaActualPtr->getOpciones();
    for (int i = 0; i < opciones.size(); ++i) {
      sf::Text opt(fuente);
      opt.setString(opciones[i]);
      opt.setCharacterSize(44);
      int a = optionAlpha[i];
      opt.setFillColor(sf::Color(0, 0, 0, a));
      opt.setPosition(
          {POS_OPCIONES.x, POS_OPCIONES.y + i * DISTANCIA_ENTRE_OPCIONES});
      window.draw(opt);
    }

    sf::Text instr(fuente);
    instr.setString("Haz clic sobre una opción para responder");
    instr.setCharacterSize(24);
    instr.setFillColor(sf::Color(0, 0, 0, 255.f * 0.5));
    instr.setPosition(
        {POS_OPCIONES.x, POS_OPCIONES.y + 4 * DISTANCIA_ENTRE_OPCIONES + 40.f});
    window.draw(instr);
  }

  if (showCalavera && spriteCalavera) {
    sf::Color c = spriteCalavera->getColor();
    c.a = calaveraAlpha;
    spriteCalavera->setColor(c);
    window.draw(*spriteCalavera);
  }

  if (terminado) {
    sf::Text resultado(fuente);
    resultado.setCharacterSize(36);
    resultado.setFillColor(sf::Color(0, 0, 0, 255.f * 0.7));
    resultado.setString(
        gano
            ? "¡Has completado el libro! Presiona Enter para volver a la Ruleta del Saber"
            : "Has fallado. Presiona Enter para volver a la Ruleta del Saber");
    resultado.setPosition({100.f, 550});
    window.draw(resultado);
  }
}

bool LibroScene::wantsToChangeScene() const {
  return terminado && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
}

Scene* LibroScene::nextScene(sf::RenderWindow& window, int& vidasJugador) {
  return new RuletaScene(window, vidasJugador,categoriasRestantes);
}

bool LibroScene::isFinished() const {
  return terminado;
}

bool LibroScene::playerWon() const {
  return gano;
}