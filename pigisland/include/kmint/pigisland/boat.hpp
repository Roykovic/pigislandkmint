#ifndef KMINT_PIGISLAND_BOAT_HPP
#define KMINT_PIGISLAND_BOAT_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"
#include "kmint/pigisland/states/state.hpp"
#include <kmint\pigisland\astar.hpp>

namespace kmint {
namespace pigisland {

class boat : public play::map_bound_actor {
public:
	boat(map::map_graph& g, map::map_node& initial_node, astar* astar);
	// wordt elke game tick aangeroepen
	void act(delta_time dt) override;
	ui::drawable const& drawable() const override { return drawable_; }
	// als incorporeal false is, doet de actor mee aan collision detection
	bool incorporeal() const override { return false; }
	// geeft de lengte van een zijde van de collision box van deze actor terug.
	// Belangrijk voor collision detection
	scalar collision_range() const override { return 32.0; }

	void changeState(State<boat>* pNewState);
	State<boat>* get_state() { return currentstate_; }

	void set_path(std::vector<size_t> path) { path_ = path; }
	std::vector<size_t> get_path() { return path_; }

	void set_damage(int damage) { damage_ = damage; }
	int get_damage() { return damage_; }

	kmint::map::map_graph* graph;

	astar* astar;

	void set_tint(int r, int g, int b);
	void remove_tint();

	delta_time get_t_passed() { return t_passed_; }

	std::unordered_map<int, std::vector<int>> repair_history_ =
	{
		{1,{50}},
		{2,{50}},
		{3,{50}}
	};
private:
	// hoeveel tijd is verstreken sinds de laatste beweging
	delta_time t_passed_{};
	// weet hoe de koe getekend moet worden
	play::image_drawable drawable_;

	State<boat>* currentstate_ = nullptr;

	State<boat>* previousstate_;

	State<boat>* globalstate_;

	std::vector<size_t> path_;

	int damage_ = 0;
};

} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_BOAT_HPP */
