#pragma once

#ifndef KMINT_PIGISLAND_STEERING_BEHAVIOURS_HPP
#define KMINT_PIGISLAND_STEERING_BEHAVIOURS_HPP

#include <kmint\math\vector2d.hpp>
#include <kmint\play\free_roaming_actor.hpp>
#include "kmint/random.hpp"
#include <kmint\pigisland\C2DMatrix.hpp>
#include <kmint\pigisland\wall.hpp>
namespace kmint {
	namespace pigisland {
		class steering_behaviours {
		public:

			steering_behaviours::steering_behaviours();

			kmint::math::vector2d seek(kmint::math::vector2d targetPos, kmint::math::vector2d curPos, double maxSpeed, kmint::math::vector2d velocity);
			kmint::math::vector2d flee(kmint::math::vector2d targetPos, kmint::math::vector2d curPos, double maxSpeed, kmint::math::vector2d velocity);
			kmint::math::vector2d pursuit(kmint::play::free_roaming_actor* curActor, kmint::play::free_roaming_actor* target);
			kmint::math::vector2d wander(kmint::play::free_roaming_actor* curActor);
			kmint::math::vector2d cohesion(kmint::play::free_roaming_actor* curActor, double maxSpeed, kmint::math::vector2d velocity, std::vector<kmint::math::vector2d> taggedNeighbourLocs);
			kmint::math::vector2d alignment(kmint::play::free_roaming_actor* curActor, const std::vector<kmint::math::vector2d> taggedNeighbourHeadings);
			kmint::math::vector2d seperation(kmint::play::free_roaming_actor* curActor, const std::vector<kmint::math::vector2d> taggedNeighbourLocs);
			kmint::math::vector2d wall_avoidance(kmint::play::free_roaming_actor* curActor, std::vector<wall*> walls);
			kmint::math::vector2d pointToWorldSpace(kmint::math::vector2d& point, kmint::math::vector2d& AgentHeading, kmint::math::vector2d& AgentSide, kmint::math::vector2d& AgentPosition);

		private:
			kmint::math::vector2d wanderTarget;
			bool lines_intersect(kmint::math::vector2d curBegin, kmint::math::vector2d curEnd, kmint::math::vector2d targetBegin, kmint::math::vector2d targetEnd, double& intersectionDistanceCurrent, kmint::math::vector2d intersectionPoint);
		};
	}
}
#endif /* KMINT_PIGISLAND_STEERING_BEHAVIOURS_HPP */