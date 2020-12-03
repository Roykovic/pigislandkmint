#include "kmint/pigisland/states/state.hpp"
#include <kmint\pigisland\shark.hpp>

namespace kmint {
	namespace pigisland {

		class tired_state : public State<shark> {
		public:

			void enter(shark* shark) override;
			void execute(shark* shark) override;
			void exit(shark* shark) override;

		private:
		};

	} // namespace pigisland
} // namespace kmint