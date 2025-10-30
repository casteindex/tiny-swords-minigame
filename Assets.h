// clang-format off
#pragma once
#include <SFML/Graphics.hpp>

// --- Texturas Background ---
inline const sf::Texture TEX_FONDO_RULETA("assets/backgrounds/fondo_ruleta.jpg");
inline const sf::Texture TEX_FONDO_ESPADA("assets/backgrounds/fondo_espada.jpg");
inline const sf::Texture TEX_FONDO_CALDERA("assets/backgrounds/fondo_caldera.jpg");

// --- Texturas Objetos ---
inline const sf::Texture TEX_MAGO_IDLE("assets/player/idle.png");
inline const sf::Texture TEX_MAGO_WALK("assets/player/idle_walk.png");
inline const sf::Texture TEX_RULETA("assets/objects/ruleta.png");
inline const sf::Texture TEX_ESPADA("assets/objects/espada.png");
inline const sf::Texture TEX_CALDERA("assets/objects/caldera.png");

// --- Sprites Background ---
inline sf::Sprite spriteFondoRuleta(TEX_FONDO_RULETA);
inline sf::Sprite spriteFondoEspada(TEX_FONDO_RULETA);
inline sf::Sprite spriteFondoCaldera(TEX_FONDO_RULETA);

// --- Sprites Objetos ---
inline sf::Sprite ruletaSprite(TEX_RULETA);
inline sf::Sprite espadaSprite(TEX_ESPADA);
inline sf::Sprite calderaSprite(TEX_CALDERA);

// --- Font ---
inline const sf::Font FUENTE("assets/fonts/Jacquard24-Regular.ttf");

// clang-format on