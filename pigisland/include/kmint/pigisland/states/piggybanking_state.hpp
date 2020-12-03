#include "kmint/pigisland/states/state.hpp"
#include <kmint\pigisland\boat.hpp>

namespace kmint {
	namespace pigisland {

		class piggybanking_state : public State<boat> {
		public:

			void enter(boat* boat) override;
			void execute(boat* boat) override;
			void exit(boat* boat) override;

		private:
		};

	} // namespace pigisland
} // namespace kmint#pragma once
