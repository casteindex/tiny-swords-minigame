#pragma once
#include <string>
#include <vector>

class Pregunta {
 private:
  std::string texto;
  std::vector<std::string> opciones;
  char respuestaCorrecta;

 public:
  Pregunta(const std::string& texto, const std::vector<std::string>& opciones,
      char respuestaCorrecta);

  bool esCorrecta(char respuesta) const;

  const std::string& getTexto() const;
  const std::vector<std::string>& getOpciones() const;
  char getRespuestaCorrecta() const;
};
