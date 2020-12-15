#include "kmint/pigisland/steering_behaviours.hpp"
#include "../../../../dependencies/SDL2/include/SDL_stdinc.h"

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

		math::vector2d steering_behaviours::wall_avoidance(kmint::play::free_roaming_actor* curActor, std::vector<wall*> walls) {

			std::vector<math::vector2d> wallFeelers = std::vector<math::vector2d>(3);

			float feelerLength = 40;

			wallFeelers[0] = curActor->location() + feelerLength * curActor->heading();

			kmint::math::vector2d temp = curActor->heading();

			C2DMatrix mat;
			mat.Rotate((M_PI * 0.5) * 3.5f);
			mat.TransformVector2Ds(temp);
			wallFeelers[1] = curActor->location() + feelerLength / 2.0f * temp;

			temp = curActor->heading();
			mat.Rotate((M_PI * 0.5) * 0.5f);
			mat.TransformVector2Ds(temp);
			wallFeelers[2] = curActor->location() + feelerLength / 2.0f * temp;

			double distToThisIP = 0.0;
			double distToClosestIP = DBL_MAX;

			//this will hold an index into the vector of walls
			int closestWall = -1;
			kmint::math::vector2d SteeringForce,
				point, //used for storing temporary info
				closestPoint; //holds the closest intersection point

				//examine each feeler in turn
			for (int i = 0; i < wallFeelers.size(); ++i)
			{
				//run through each wall checking for any intersection points
				for (int j = 0; j < walls.size(); ++j)
				{
					if (lines_intersect(curActor->location(),wallFeelers[i],walls[j]->from(), walls[j]->to(), distToThisIP, point))
					{
						//is this the closest found so far? If so keep a record
						if (distToThisIP < distToClosestIP)
						{
							distToClosestIP = distToThisIP;
							closestWall = j;
							closestPoint = point;
						}
					}
				}//next wall
				//if an intersection point has been detected, calculate a force
				//that will direct the agent away
				if (closestWall >= 0)
				{
					//calculate by what distance the projected position of the agent
					//will overshoot the wall
					kmint::math::vector2d overShoot = wallFeelers[i] - closestPoint;
					//create a force in the direction of the wall normal, with a
					//magnitude of the overshoot
					SteeringForce = walls[closestWall]->normal() * std::sqrt(pow(overShoot.x(), 2) + pow(overShoot.y(), 2));;
				}
			}//next feeler
			return SteeringForce;
		}

		bool steering_behaviours::lines_intersect(kmint::math::vector2d curBegin, kmint::math::vector2d curEnd, kmint::math::vector2d targetBegin, kmint::math::vector2d targetEnd, double& intersectionDistanceCurrent, kmint::math::vector2d intersectionPoint) {
			double rTop = (curBegin.y() - targetBegin.y()) * (targetEnd.x() - targetBegin.x()) - (curBegin.x() - targetBegin.x()) * (targetEnd.y() - targetBegin.y());
			double rBot = (curEnd.x() - curBegin.x()) * (targetEnd.y() - targetBegin.y()) - (curEnd.y() - curBegin.y()) * (targetEnd.x() - targetBegin.x());

			double sTop = (curBegin.y() - targetBegin.y()) * (curEnd.x() - curBegin.x()) - (curBegin.x() - targetBegin.x()) * (curEnd.x() - curBegin.x());
			double sBot = (curEnd.x() - curBegin.x()) * (targetEnd.y() - targetBegin.y()) - (curEnd.y() - curBegin.y()) * (targetEnd.x() - targetBegin.x());

			if ((rBot == 0) || (sBot == 0)) {
				//lines are parallel
				return false;
			}

			double r = rTop / rBot;
			double s = sTop / sBot;

			if ((r > 0) && (r < 1) && (s > 0) && (s < 1)) {
				intersectionDistanceCurrent = distance(curBegin, curEnd) * r;
				intersectionPoint = curBegin + r * (curEnd - curBegin);

				return true;
			}
			else {
				intersectionDistanceCurrent = 0;
				return false;
			}
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