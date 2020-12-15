#include "kmint/pigisland/steering_behaviours.hpp"

namespace kmint {
	namespace pigisland {
		steering_behaviours::steering_behaviours()
		{
		}

		math::vector2d steering_behaviours::seek(kmint::math::vector2d targetPos, kmint::math::vector2d currentPos, double maxSpeed, kmint::math::vector2d velocity)
		{
			math::vector2d deltaPos = normalize(targetPos - currentPos);
			math::vector2d desiredVelocity = deltaPos * maxSpeed;
			return (desiredVelocity - velocity);
		}

		math::vector2d steering_behaviours::flee(math::vector2d targetPos, math::vector2d curPos, double maxSpeed, math::vector2d velocity)
		{
			math::vector2d deltaPos = normalize(curPos - targetPos);
			math::vector2d desiredVelocity = deltaPos * maxSpeed;
			return (desiredVelocity - velocity);
		}

		math::vector2d steering_behaviours::pursuit(kmint::play::free_roaming_actor* curActor, kmint::play::free_roaming_actor* target)
		{
			return kmint::math::vector2d();
		}

		math::vector2d steering_behaviours::wander(kmint::play::free_roaming_actor* curActor) {
			double wanderRadius = 0.35;
			double wanderDistance = 0.25;
			double wanderJitter = 1.0;

			scalar randomJitteryScalar = random_int(-1, 1) * wanderJitter;
			scalar randomJitteryScalar2 = random_int(-1, 1) * wanderJitter;

			math::vector2d wanderTarget = math::vector2d(randomJitteryScalar, randomJitteryScalar2);
			
			normalize(wanderTarget);

			wanderTarget *= wanderRadius;

			kmint::math::vector2d targetLocal = wanderTarget + kmint::math::vector2d(wanderDistance, 0);

			kmint::math::vector2d targetWorld = pointToWorldSpace(targetLocal,
				curActor->heading(),
				perp(curActor->heading()),
				curActor->location());

			return targetWorld - curActor->location();
		}

		math::vector2d steering_behaviours::seperation(kmint::play::free_roaming_actor* curActor, const std::vector<kmint::math::vector2d> taggedNeighbourLocs) {

			kmint::math::vector2d steeringForce;

			for (int i = 0; i < taggedNeighbourLocs.size(); ++i)
			{
					Vector2D ToAgent = curActor->location() - taggedNeighbourLocs[i];
					//scale the force inversely proportional to the agent's distance
					//from its neighbor.

					double toAgentLength = std::sqrt(pow(ToAgent.x(), 2) + pow(ToAgent.y(), 2));
					steeringForce += normalize(ToAgent) / toAgentLength;
			}
			return steeringForce;
		}

		math::vector2d steering_behaviours::alignment(kmint::play::free_roaming_actor* curActor, const std::vector<kmint::math::vector2d> taggedNeighbourHeadings) {

			kmint::math::vector2d averageHeading;
			int neighbourCount = taggedNeighbourHeadings.size();

			for (int i = 0; i < taggedNeighbourHeadings.size(); ++i)
			{
				averageHeading += taggedNeighbourHeadings[i];
			}

			if (neighbourCount > 0)
			{
				averageHeading /= (double)neighbourCount;
				averageHeading -= curActor->heading();
			}
			return averageHeading;
		}

		math::vector2d steering_behaviours::cohesion(kmint::play::free_roaming_actor* curActor, double maxSpeed, kmint::math::vector2d curVel, std::vector<kmint::math::vector2d> taggedNeighbourLocs) {

			kmint::math::vector2d centerOfMass, steeringForce;
			int neighbourCount = taggedNeighbourLocs.size();

			for (int i = 0; i < taggedNeighbourLocs.size(); ++i)
			{
				centerOfMass += taggedNeighbourLocs[i];
			}

			if (neighbourCount > 0) {
				centerOfMass /= neighbourCount;
				steeringForce = seek(centerOfMass, curActor->location(), maxSpeed, curVel);
			}

			return steeringForce;
		}

		kmint::math::vector2d steering_behaviours::pointToWorldSpace(kmint::math::vector2d& target, kmint::math::vector2d& heading, kmint::math::vector2d& side, kmint::math::vector2d& loc)
		{
			//make a copy of the point
			kmint::math::vector2d TransPoint = target;

			//create a transformation matrix
			C2DMatrix matTransform;

			//rotate
			matTransform.Rotate(heading, side);

			//and translate
			matTransform.Translate(loc.x(), loc.y());

			//now transform the vertices
			matTransform.TransformVector2Ds(TransPoint);

			return TransPoint;
		}
	}
}