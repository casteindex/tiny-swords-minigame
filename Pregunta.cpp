#include "Pregunta.h"

#include <cctype>

Pregunta::Pregunta(const std::string& texto,
    const std::vector<std::string>& opciones, char respuestaCorrecta)
    : texto(texto),
      opciones(opciones),
      respuestaCorrecta(static_cast<char>(std::toupper(respuestaCorrecta))) {}

bool Pregunta::esCorrecta(char respuesta) const {
  return std::toupper(respuesta) == respuestaCorrecta;
}

const std::string& Pregunta::getTexto() const {
  return texto;
}

const std::vector<std::string>& Pregunta::getOpciones() const {
  return opciones;
}

char Pregunta::getRespuestaCorrecta() const {
  return respuestaCorrecta;
}
