#include "../lib/vecN.hpp"
#include "../src/acme.hpp"


int main () {

    // Define an entity type for players
    ACME::EntityConfig2D PlayerConfig;

    PlayerConfig.typeName = "player";
    PlayerConfig.initialSize = std::vec2(0.9, 1.8);
    PlayerConfig.move = nullptr; // pointer to function


    // Create a world for a player to live in (currently empty)
    ACME::Scene2D world;

    // Summon a player entity
    world.summon(PlayerConfig, 0.0, 0.0);

    // Advance the game physics by 1 second.
    world.tick(1.0);

}
