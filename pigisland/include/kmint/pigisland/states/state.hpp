#pragma once
#include "kmint/play.hpp"

namespace kmint {
	namespace pigisland {

		template <class entity_type>
		class State
		{
		public:
			virtual void enter(entity_type*)=0;
			virtual void exit(entity_type*)=0;
			virtual void execute(entity_type*)=0;

		private:
		};

	} // namespace pigisland
} // namespace kmint