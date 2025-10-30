// clang-format off
#pragma once
#include <queue>
#include <vector>
#include "Pregunta.h"

// Nota: para definir las preguntas se están usando funciones lambda, es decir funciones anónimas
// Para más información: https://www.w3schools.com/cpp/cpp_functions_lambda.asp

inline std::queue<Pregunta> PREGUNTAS_POLITICA = []() {
  std::queue<Pregunta> q;
  q.push(Pregunta("1. Durante el renacimiento, el modelo de gobierno es uno de los siguientes:", {"A) Monarquía absoluta", "B) Tiranía republicana", "C) Democracia participativa", "D) Liberalismo político"}, 'A'));
  q.push(Pregunta("2. De los siguientes acontecimientos, seleccione el que inicia el período moderno:", {"A) Toma de Constantinopla", "B) Tratado de Paz de Westfalia", "C) Toma de la Bastilla", "D) La ruta de la seda"}, 'B'));
  q.push(Pregunta("3. Durante el siglo XV, la sociedad se estratifica en tres estamentos definidos:", {"A) Clase media, baja y alta", "B) Nobleza, clero y estado llano", "C) Artesanos, guardianes y gobernantes", "D) Ninguna de las anteriores"}, 'B'));
  q.push(Pregunta("4. Aparece el realismo político, que se basaba en un orden establecido y de cómo gobernar:", {"A) Tomás Moro", "B) Jean Bodín", "C) Nicolás Maquiavelo", "D) Erasmo de Rotterdam"}, 'C'));
  q.push(Pregunta("4. Terminada la edad media, en el contexto de la política resulta que:", {"A) La Iglesia resalta su poder", "B) La Iglesia pierde el papel rector en la política", "C) La Iglesia evangélica se posiciona en la política", "D) La política desaparece"}, 'B'));
  return q;
}();

inline std::queue<Pregunta> PREGUNTAS_CIENCIA = []() {
  std::queue<Pregunta> q;
  q.push(Pregunta("1. Entre los siguientes renacentistas seleccione, uno de los precursores filósofo-científico del heliocentrismo (teoría que afirma que el sol es el centro del universo):", {"A) Tomas Moro", "B) Galileo", "C) Platón", "D) Arquímedes"}, 'B'));
  q.push(Pregunta("2. El método científico se introduce por el interés de tres filósofos. Entre los siguientes uno de los mencionados no es precursor del método científico:", {"A) Francis Bacon", "B) Galileo Galilei", "C) Nicolás Maquiavelo", "D) René Descartes"}, 'C'));
  q.push(Pregunta("3. Es uno de los precursores del pensamiento Moderno:", {"A) Isaac Newton", "B) René Descartes", "C) Erasmo de Roterdam", "D) Francis Bacon"}, 'B'));
  q.push(Pregunta("4. De los siguientes filósofos niega el geocentrismo (teoría que afirma que el centro de nuestro sistema solar es la tierra):", {"A) Aristóteles", "B) Nicolás Copérnico", "C) Tomás de Aquino", "D) Isaac Newton"}, 'B'));
  q.push(Pregunta("5. Uno de los inventos que suscitó un conocimiento ilimitado, fue el de Gutenberg:", {"A) El astrolabio", "B) La imprenta", "C) La Nao y la Carabela", "D) El Telescopio"}, 'B'));
  return q;
}();

// clang-format on