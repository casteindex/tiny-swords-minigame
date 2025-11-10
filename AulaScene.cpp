#include "AulaScene.h"

AulaScene::AulaScene(sf::RenderWindow& windowRef, Jugador* jugadorPtr)
    : window(windowRef),
      jugador(jugadorPtr),
      mostrandoPista(true),
      mostrandoPregunta(false),
      ultimaRespuestaCorrecta(false),
      preguntaActual(nullptr),
      esperandoClick(true),
      finJuego(false),
      cambioEscena(false),
      aciertos(0) {
  // --- cargar recursos ---
  // clang-format off
  if (!texFondo.loadFromFile("assets/backgrounds/fondo_aula.jpg"));
  if (!fuente.openFromFile("assets/fonts/Jacquard24-Regular.ttf"));
  // clang-format on

  fondo = new sf::Sprite(texFondo);
  kant = new NPC("assets/npc/npc_idle.png", 5.f, true);
  kant->setPosition(100.f, 280.f);

  // Cuadro de diálogo
  cuadroDialogo.setSize({(float)window.getSize().x - 100.f, 230.f});
  cuadroDialogo.setPosition({50.f, window.getSize().y - 240.f});
  cuadroDialogo.setFillColor(sf::Color(0, 0, 0, 180));

  textoDialogo = new sf::Text(fuente);
  textoDialogo->setCharacterSize(28);
  textoDialogo->setFillColor(sf::Color::White);
  textoDialogo->setPosition({70.f, window.getSize().y - 220.f});

  inicializarPreguntas();
  inicializarPistas();
  inicializarAreas();  // áreas dónde el usuario debe hacer click
  mostrarSiguientePista();
}

AulaScene::~AulaScene() {
  delete fondo;
  delete kant;
  delete textoDialogo;
  delete preguntaActual;
}

// clang-format off
void AulaScene::inicializarPreguntas() {
  colaPreguntas.push(Pregunta("1. Seleccione el mandato cuya obligación viene del miedo al castigo o la búsqueda de un premio:", {"A) Imperativo Hipotético.", "B) Imperativo categórico.", "C) Ambos.", "D) Ninguno"}, 'A'));
  colaPreguntas.push(Pregunta("2. Para Emanuel Kant, es posible conocer lo que las cosas nos permiten (como lo superficial) a través de\nnuestros sentidos:", {"A) Conocimiento Noumenico.", "B) Conocimiento fenoménico.", "C) conocimiento Empírico.", "D) Conocimiento Racional"}, 'B'));
  colaPreguntas.push(Pregunta("3. Kant decía que el lema de la ilustración era “Sapere aude”, que significa:", {"A) Sopesa tus acciones.", "B) Atrévete a saber por ti mismo.", "C) Saber a la fuerza.", "D) Someterse al conocimiento"}, 'B'));
  colaPreguntas.push(Pregunta("4. Kant (igual que Copérnico cambió el centro del universo de la tierra al sol), cambia el centro del\nconocimiento del objeto al sujeto, a esto se le llama:", {"A) Subjetivismo.", "B) Prejuicio.", "C) giro copernicano.", "D) Suerte"}, 'C'));
  colaPreguntas.push(Pregunta("5. La postura conciliadora de Kant respecto a los empiristas y racionalistas define que los datos experimentales\nson la fuente del conocimiento racional del sujeto:", {"A) Racionalismo.", "B) Empirismo.", "C) Criticismo.", "D) Escepticismo"}, 'C'));
  colaPreguntas.push(Pregunta("6. De las siguientes obras de Emanuel Kant, seleccione aquella que define su epistemología:", {"A) Critica de la razón práctica.", "B) Critica de la razón pura.", "C) Critica del juicio.", "D) Critica fenomenológica"}, 'B'));
}

void AulaScene::inicializarPistas() {
  colaPistas.push("Kant: Para conocer el mundo, primero debes atreverte a observarlo en su totalidad.\n\n[Haz click sobre el objeto de la pista]");
  colaPistas.push("Kant: La experiencia nos enseña, pero los conceptos dan forma a lo aprendido.\n\n[Haz click sobre el objeto de la pista.]");
  colaPistas.push("Kant: El tiempo no se ve, pero estructura toda percepción que poseemos.\n\n[Haz click sobre el objeto de la pista]");
  colaPistas.push("Kant: A veces, el conocimiento surge cuando eliminamos los prejuicios.\n\n[Haz click sobre el objeto de la pista]");
  colaPistas.push("Kant: La razón necesita un lugar donde escribir sus juicios y compararlos.\n\[Haz click sobre el objeto de la pista]");
  colaPistas.push("Kant: Tras los velos de la apariencia, sólo la razón puede entrever lo esencial.\n\nHaz click sobre el objeto de la pista]");
}
// clang-format on

void AulaScene::inicializarAreas() {
  colaAreas.push(sf::FloatRect({550.f, 400.f}, {85.f, 110.f}));   // globo
  colaAreas.push(sf::FloatRect({213.f, 325.f}, {73.f, 69.f}));    // libro
  colaAreas.push(sf::FloatRect({650.f, 53.f}, {97.f, 76.f}));     // reloj
  colaAreas.push(sf::FloatRect({1100.f, 450.f}, {85.f, 110.f}));  // basurero
  colaAreas.push(sf::FloatRect({697.f, 626.f}, {171.f, 65.f}));   // cuaderno
  colaAreas.push(sf::FloatRect({115.f, 48.f}, {50.f, 477.f}));    // cortina
}

void AulaScene::mostrarSiguientePista() {
  if (!colaPistas.empty()) {
    std::string texto = "";
    // mostrar feedback si no es la primera pista
    if (preguntaActual != nullptr) {
      texto += ultimaRespuestaCorrecta ? "¡Correcto!\n\n" : "Incorrecto...\n\n";
    }
    texto += colaPistas.front();
    colaPistas.pop();
    textoDialogo->setString(texto);
    mostrandoPista = true;
    mostrandoPregunta = false;
    esperandoClick = true;
  } else {
    finJuego = true;
  }
}

void AulaScene::mostrarPreguntaActual() {
  if (!colaPreguntas.empty()) {
    delete preguntaActual;
    preguntaActual = new Pregunta(colaPreguntas.front());
    colaPreguntas.pop();

    mostrandoPista = false;
    mostrandoPregunta = true;

    // agregar las opciones
    std::string texto = preguntaActual->getTexto() + "\n\n";
    const auto& opciones = preguntaActual->getOpciones();
    for (const auto& o : opciones) {
      texto += o + "\n";
    }
    textoDialogo->setString(texto);
  } else {
    finJuego = true;
  }
}

bool AulaScene::clickEnAreaValida(float x, float y) {
  if (colaAreas.empty()) return false;
  return colaAreas.front().contains({x, y});
}

void AulaScene::manejarRespuesta(char opcion) {
  ultimaRespuestaCorrecta = preguntaActual->esCorrecta(opcion);
  if (ultimaRespuestaCorrecta) aciertos++;
  colaAreas.pop();
  mostrarSiguientePista();
}

void AulaScene::handleInput(float dt) {
  if (finJuego) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
      cambioEscena = true;
    return;
  }

  if (mostrandoPista) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      auto mp = sf::Mouse::getPosition(window);
      float mx = static_cast<float>(mp.x);
      float my = static_cast<float>(mp.y);
      if (clickEnAreaValida(mx, my)) mostrarPreguntaActual();
    }
  } else if (mostrandoPregunta) {
    // clang-format off
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) manejarRespuesta('A');
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) manejarRespuesta('B');
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) manejarRespuesta('C');
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) manejarRespuesta('D');
    // clang-format on
  }
}

void AulaScene::update(float dt) {
  kant->update(dt);
  if (finJuego) {
    std::string finalTexto =
        aciertos >= 2
            ? "Kant: Has comprendido bien las bases del conocimiento y la "
              "ética."
            : "Kant: Aún te falta reflexionar sobre la razón y la moral.";
    textoDialogo->setString(finalTexto);
  }
}

void AulaScene::draw(sf::RenderWindow& window) {
  window.draw(*fondo);
  if (kant) kant->draw(window);
  window.draw(cuadroDialogo);
  window.draw(*textoDialogo);
}

bool AulaScene::wantsToChangeScene() const {
  return cambioEscena;
}

Scene* AulaScene::nextScene(sf::RenderWindow& window, int& vidasJugador) {
  return new EdificioScene(window, jugador);
}
