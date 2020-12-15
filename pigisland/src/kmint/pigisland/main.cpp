#include "kmint/main.hpp" // voor de main loop
#include "kmint/math/intersection.hpp"
#include "kmint/math/rectangle.hpp"
#include "kmint/pigisland/boat.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/pigisland/shark.hpp"
#include "kmint/play.hpp"
#include "kmint/ui.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <kmint\pigisland\states\wandering_state.hpp>
#include <kmint\random.hpp>
#include <kmint\pigisland\kill_message.hpp>

using namespace kmint;

int main() {
  // een app object is nodig om
  ui::app app{};

  //  maak een venster aan
  ui::window window{app.create_window({1024, 768}, "pigisland", 1.0)};

  // maak een podium aan
  play::stage s{{1024, 768}};

  bool kill = false;

 kmint::pigisland::kill_message* message = new kmint::pigisland::kill_message();

  auto map = pigisland::map();
  auto &graph = map.graph();
  s.build_actor<play::background>(math::size(1024, 768),
                                  graphics::image{map.background_image()});
  s.build_actor<play::map_actor>(math::vector2d{0.f, 0.f}, map.graph());
 auto& boat = s.build_actor<pigisland::boat>(graph,
                                 pigisland::find_node_of_kind(graph, '1'));



  auto locs = pigisland::random_pig_locations(100);


  auto& shark = s.build_actor<pigisland::shark>(graph,
      pigisland::find_node_of_kind(graph, 'K'), message);

  std::vector<pigisland::pig*> pigs;
  for (auto loc : locs) {
    auto& pig = s.build_actor<pigisland::pig>(loc, boat, shark);
    pigs.push_back(&pig);
  }

#pragma-region walls

  std::vector<kmint::pigisland::wall*> walls;

    #pragma-region edges
      kmint::math::vector2d beginLeft = {0, 0};
      kmint::math::vector2d endLeft = {0, 768};
      walls.push_back(&kmint::pigisland::wall(endLeft, beginLeft));

      kmint::math::vector2d beginRight = { 1024, 0 };
      kmint::math::vector2d endRight = { 1024, 768 };
      walls.push_back(&kmint::pigisland::wall(beginRight, endRight));

      kmint::math::vector2d beginTop = { 0, 0 };
      kmint::math::vector2d endTop = { 1024, 0 };
      walls.push_back(&kmint::pigisland::wall(beginTop, endTop));

      kmint::math::vector2d beginBottom = { 0, 768 };
      kmint::math::vector2d endBottom = { 1024, 768 };
      walls.push_back(&kmint::pigisland::wall(endBottom, beginBottom));
    #pragma endregion

    #pragma-region islands
      //NW
      kmint::math::vector2d beginNWBottom = { 0, 80 };
      kmint::math::vector2d endNWBottom = { 100, 80 };
      walls.push_back(&kmint::pigisland::wall(beginNWBottom, endNWBottom));

      kmint::math::vector2d beginNWRight = { 160, 0 };
      kmint::math::vector2d endNWRight = { 160, 80 };
      walls.push_back(&kmint::pigisland::wall(endNWRight, beginNWRight));

      //SW
      kmint::math::vector2d beginSWLeft = { 80, 688 };
      kmint::math::vector2d endSWleft = { 80, 566 };
      walls.push_back(&kmint::pigisland::wall(endSWleft, beginSWLeft));

      kmint::math::vector2d beginSWBottom = { 80, 688 };
      kmint::math::vector2d endSWBottom = { 202, 688 };
      walls.push_back(&kmint::pigisland::wall(beginSWBottom, endSWBottom));

      kmint::math::vector2d beginSWTop = { 80, 566 };
      kmint::math::vector2d endSWTop = { 202, 566 };
      walls.push_back(&kmint::pigisland::wall(endSWTop, beginSWTop));

      kmint::math::vector2d beginSWRight = { 202, 566 };
      kmint::math::vector2d endSWRight = { 202, 688 };
      walls.push_back(&kmint::pigisland::wall(endSWRight, beginSWRight));

      //SE
      kmint::math::vector2d beginSELeft = { 816, 768 };
      kmint::math::vector2d endSELeft = { 816, 624 };
      walls.push_back(&kmint::pigisland::wall(endSELeft, beginSELeft));

      kmint::math::vector2d beginSETop = { 1024, 624 };
      kmint::math::vector2d endSETop = { 816, 624 };
      walls.push_back(&kmint::pigisland::wall(beginSETop, endSETop));

      //NE
      kmint::math::vector2d beginNEBottom = { 912, 48 };
      kmint::math::vector2d endNEBottom = { 1024, 48 };
      walls.push_back(&kmint::pigisland::wall(beginNEBottom, endNEBottom));

      kmint::math::vector2d beginNELeft = { 912, 0 };
      kmint::math::vector2d endNELeft = { 912, 48 };
      walls.push_back(&kmint::pigisland::wall(beginNELeft, endNELeft));

    #pragma endregion

  for (auto pig : pigs) {
      pig->set_pigs(pigs);
      pig->set_walls(walls);
  }

#pragma endregion


  message->addCallback([&]()
  {
      kill = true;
  });




  // Maak een event_source aan (hieruit kun je alle events halen, zoals
  // toetsaanslagen)
  ui::events::event_source event_source{};

  // main_loop stuurt alle actors aan.
  main_loop(s, window, [&](delta_time dt, loop_controls &ctl) {
    // gebruik dt om te kijken hoeveel tijd versterken is
    // sinds de vorige keer dat deze lambda werd aangeroepen
    // loop controls is een object met eigenschappen die je kunt gebruiken om de
    // main-loop aan te sturen.
      if (kill) {
          kill = false;
          for (auto& pig : pigs) {
              pig->remove();
          }

          locs = pigisland::random_pig_locations(100);

          pigs.clear();
          for (auto loc : locs) {
              auto& pig = s.build_actor<pigisland::pig>(loc, boat, shark);
              pigs.push_back(&pig);
          }

          for (auto pig : pigs) {
              pig->set_pigs(pigs);
              pig->set_walls(walls);
          }
      }

    for (ui::events::event &e : event_source) {
      // event heeft een methode handle_quit die controleert
      // of de gebruiker de applicatie wilt sluiten, en zo ja
      // de meegegeven functie (of lambda) aanroept om met het
      // bijbehorende quit_event
      //

      e.handle_quit([&ctl](ui::events::quit_event qe) { ctl.quit = true; });
      e.handle_key_up([&](auto ke) {
        switch (ke.key) {
        case ui::events::key::p:
          ctl.pause = !ctl.pause;
          break;
        case ui::events::key::r:
          ctl.render = !ctl.render;
          break;
        case ui::events::key::opening_bracket:
          ctl.time_scale /= 2.0;
          break;
        case ui::events::key::closing_bracket:
          ctl.time_scale *= 2.0;
          break;
        case ui::events::key::d:
          ctl.debug_overlay = !ctl.debug_overlay;
        default:
          break;
        }
      });
    }
  });
}
