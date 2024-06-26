#include "spawn_system.h"
#include "../components/sprite.h"
#include "../components/state.h"
#include "../components/transform.h"
#include "../ecs/coordinator.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

extern Coordinator GCR;

void SpawnSystem::init() {
  GCR.add_listener(
      METHOD_LISTENER(Events::Game::START, SpawnSystem::start_game));
  GCR.add_listener(METHOD_LISTENER(Events::Game::STOP, SpawnSystem::stop_game));
}

void SpawnSystem::update() {
  if (checkpoint == -1)
    return; // the game has not started yet
  if (is_done(checkpoint)) {
    checkpoint += 1;
    spawn_checkpoint(checkpoint);
  }
}

bool SpawnSystem::is_done(int checkpoint) {
  for (int i = 0; i < Entities::E_AMT; i++) {
    int entity = i + (Entities::E_AMT * checkpoint);
    bool active = GCR.get_component<State>(entity + Entities::E_GRUNT).active;
    if (active)
      return false;
  }
  return true;
}

void SpawnSystem::spawn_checkpoint(int checkpoint) {
  if (checkpoint < 5) {
    Event event(Events::Game::WAVE_DONE); // Update UI
    GCR.send_event(event);
    for (int i = 0; i < Entities::E_AMT; i++) {
      int entity = i + (Entities::E_AMT * checkpoint);
      auto &active =
          GCR.get_component<State>(entity + Entities::E_GRUNT).active = true;
    }
  } else if (checkpoint == 5) {
    Event event(Events::Game::BOSS_TIME); // Update UI
    GCR.send_event(event);
    GCR.get_component<State>(Entities::BOSS).active = true;
  }
}

void SpawnSystem::start_game(Event &e) { checkpoint = 0; }

void SpawnSystem::stop_game(Event &e) { checkpoint = -1; }