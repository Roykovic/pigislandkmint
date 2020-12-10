#pragma once

#ifndef KMINT_PIGISLAND_STEERING_BEHAVIOURS_HPP
#define KMINT_PIGISLAND_STEERING_BEHAVIOURS_HPP

#include <kmint\math\vector2d.hpp>
#include <kmint\play\free_roaming_actor.hpp>
#include "kmint/random.hpp"
#include <kmint\pigisland\C2DMatrix.hpp>
namespace kmint {
	namespace pigisland {
		class steering_behaviours {
		public:

			steering_behaviours::steering_behaviours();

			kmint::math::vector2d seek(kmint::math::vector2d targetPos, kmint::math::vector2d curPos, double maxSpeed, kmint::math::vector2d velocity);
			kmint::math::vector2d flee(kmint::math::vector2d targetPos, kmint::math::vector2d curPos, double maxSpeed, kmint::math::vector2d velocity);
			kmint::math::vector2d pursuit(kmint::play::free_roaming_actor* curActor, kmint::play::free_roaming_actor* target);
			kmint::math::vector2d wander(kmint::play::free_roaming_actor* curActor);
			kmint::math::vector2d pointToWorldSpace(kmint::math::vector2d& point, kmint::math::vector2d& AgentHeading, kmint::math::vector2d& AgentSide, kmint::math::vector2d& AgentPosition);

		private:
			kmint::math::vector2d wanderTarget;
		};
	}
}
#endif /* KMINT_PIGISLAND_STEERING_BEHAVIOURS_HPP */