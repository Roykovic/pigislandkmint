#include "kmint/pigisland/states/state.hpp"
#include <kmint\pigisland\boat.hpp>

namespace kmint {
	namespace pigisland {

		class repair_state : public State<boat> {
		public:

			void enter(boat* boat) override;
			void execute(boat* boat) override;
			void exit(boat* boat) override;

		private:
			float grassEfficiency = 0.33;
			float treeEfficiency = 0.33;
			float grainEfficiency = 0.33;

			int dock;
		};

	} // namespace pigisland
} // namespace kmint#pragma once
