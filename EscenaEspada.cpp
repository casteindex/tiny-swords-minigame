#include <iostream>

#include "EspadaScene.h"

static const sf::Vector2f POS_FONDO = {0.f, 0.f};
static const sf::Vector2f POS_PIEDRA = {640.f, 520.f};
static const sf::Vector2f POS_ESPADA_BASE = {640.f, 510.f};
static const sf::Vector2f POS_PREGUNTA = {120.f, 60.f};
static const sf::Vector2f POS_OPCIONES = {120.f, 120.f};
static const float DISTANCIA_ENTRE_OPCIONES = 40.f;
static const float SUBIDA_POR_ACIERTO = 25.f;  // movimiento por acirto
static const float VELOCIDAD_ANIMACION = 100.f;

EspadaScene::EspadaScene(sf::RenderWindow& windowRef, int totalPreguntas)
    : window(windowRef), totalPreguntasIniciales(totalPreguntas),
      respuestasCorrectas(0), espadaY(0.f), animando(false), terminado(false),
      gano(false), keyLocked(false) {
  if (!texFondo.loadFromFile("assets/backgrounds/fondo_espada.jpg"))
    std::cerr << "No se pudo cargar fondo_espada.jpg\n";
  if (!texPiedra.loadFromFile("assets/objects/rock.png"))
    std::cerr << "No se pudo cargar rock.png\n";
  if (!texEspada.loadFromFile("assets/objects/espada.png"))
    std::cerr << "No se pudo cargar espada_redimensionada.png\n";
  if (!fuente.openFromFile("assets/fonts/Jacquard24-Regular.ttf"))
    std::cerr << "No se pudo cargar Jacquard24-Regular.ttf\n";

  // Fondo
  spriteFondo = std::make_unique<sf::Sprite>(texFondo);
  spriteFondo->setPosition(POS_FONDO);
  spriteFondo->setColor(sf::Color(255, 255, 255, 128));  // 4: Alfa (opacidad)

  // Piedra
  spritePiedra = std::make_unique<sf::Sprite>(texPiedra);
  spritePiedra->setOrigin(
      {texPiedra.getSize().x / 2.f, texPiedra.getSize().y / 2.f});
  spritePiedra->setPosition(POS_PIEDRA);

  // Espada
  spriteEspada = std::make_unique<sf::Sprite>(texEspada);
  spriteEspada->setOrigin(
      {texEspada.getSize().x / 2.f, texEspada.getSize().y * 0.8f});
  spriteEspada->setPosition(POS_ESPADA_BASE);

  inicializarPreguntas();
  if (!colaPreguntas.empty()) preguntaActualPtr = &colaPreguntas.front();
}

void EspadaScene::inicializarPreguntas() {
  colaPreguntas = std::queue<Pregunta>();

  colaPreguntas.push(
      Pregunta("Durante el renacimiento, el modelo de gobierno es uno de los "
               "siguientes:",
          {"A) Monarquía absoluta", "B) Tiranía republicana",
              "C) Democracia participativa", "D) Liberalismo político"},
          'A'));

  colaPreguntas.push(
      Pregunta("De los siguientes acontecimientos, seleccione el que inicia el "
               "período moderno:",
          {"A) Toma de Constantinopla", "B) Tratado de Paz de Westfalia",
              "C) Toma de la Bastilla", "D) La ruta de la seda"},
          'B'));

  colaPreguntas.push(
      Pregunta("Durante el siglo XV, la sociedad se estratifica en tres "
               "estamentos definidos:",
          {"A) Clase media, baja y alta", "B) Nobleza, clero y estado llano",
              "C) Artesanos, guardianes y gobernantes",
              "D) Ninguna de las anteriores"},
          'B'));

  colaPreguntas.push(
      Pregunta("Aparece el realismo político, que se basaba en un orden "
               "establecido y recomendaciones de cómo gobernar:",
          {"A) Tomás Moro", "B) Jean Bodín", "C) Nicolás Maquiavelo",
              "D) Erasmo de Rotterdam"},
          'C'));

  colaPreguntas.push(Pregunta(
      "Terminada la edad media, en el contexto de la política resulta que:",
      {"A) La Iglesia resalta su poder",
          "B) La Iglesia pierde el papel rector en la política",
          "C) La Iglesia evangélica se posiciona en la política",
          "D) La política desaparece"},
      'B'));
}

void EspadaScene::handleInput(float dt) {
  if (terminado || animando) return;

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

void EspadaScene::procesarRespuesta(char tecla) {
  if (!preguntaActualPtr) return;

  bool correcta = preguntaActualPtr->esCorrecta(tecla);
  if (correcta) {
    respuestasCorrectas++;
    animando = true;
    destinoY = espadaY + SUBIDA_POR_ACIERTO;

    colaPreguntas.pop();
    preguntaActualPtr =
        colaPreguntas.empty() ? nullptr : &colaPreguntas.front();

    if (respuestasCorrectas >= totalPreguntasIniciales) gano = true;
  } else {
    resetEspada();
    terminado = true;
    gano = false;
  }
}

void EspadaScene::update(float dt) {
  if (animando) {
    espadaY += VELOCIDAD_ANIMACION * dt;
    if (espadaY >= destinoY) {
      espadaY = destinoY;
      animando = false;

      if (gano) terminado = true;
    }
  }
}

void EspadaScene::resetEspada() {
  espadaY = 0.f;
  animando = false;
  while (!colaPreguntas.empty())
    colaPreguntas.pop();
  preguntaActualPtr = nullptr;
}

void EspadaScene::draw(sf::RenderWindow& window) {
  if (spriteFondo) window.draw(*spriteFondo);

  // Espada (se eleva con cada acierto)
  if (spriteEspada) {
    sf::Vector2f pos = POS_ESPADA_BASE;
    pos.y -= espadaY;
    spriteEspada->setPosition(pos);
    window.draw(*spriteEspada);
  }

  if (spritePiedra) window.draw(*spritePiedra);

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
    instr.setString("Presiona A, B, C o D)");
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
    resultado.setFillColor(gano ? sf::Color::White : sf::Color::Red);
    resultado.setString(gano ? "¡Has sacado la espada! Regresa a la ruleta..."
                             : "Has fallado. La espada vuelve a la piedra...");
    resultado.setPosition({180.f, 650.f});
    window.draw(resultado);
  }
}

bool EspadaScene::isFinished() const {
  return terminado;
}
bool EspadaScene::playerWon() const {
  return gano;
}
