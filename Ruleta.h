#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Ruleta {
 public:
  Ruleta(sf::RenderWindow& windowRef);

  bool isActive() const;
  void activar();
  void actualizar(float dt);
  void dibujar();

 private:
  sf::RenderWindow& window;
  vector<string> nombresPergaminos;
  vector<sf::Texture> pergaminos;
  unique_ptr<sf::Sprite> pergaminoActual;

  int indiceActual;
  int seleccionFinal;
  float timer;
  bool activa;
  float tiempoEntreCambios;
  float factorDesaceleracion;
};
