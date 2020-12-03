#include "kmint/pigisland/states/state.hpp"
#include <kmint\pigisland\shark.hpp>

namespace kmint {
	namespace pigisland {

		class hunting_state : public State<shark> {
		public:

			void enter(shark* shark) override;
			void execute(shark* shark) override;
			void exit(shark* shark) override;

			kmint::map::map_node& getClosestNode(kmint::math::vector2d loc, kmint::map::map_graph* graph);
		private:
		};

	} // namespace pigisland
} // namespace kmint