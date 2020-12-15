#include "kmint/pigisland/wall.hpp"

namespace kmint {
	namespace pigisland {
		wall::wall(kmint::math::vector2d beginPoint, kmint::math::vector2d endPoint) : beginPoint_(beginPoint), endPoint_(endPoint) {

			kmint::math::vector2d  temp = normalize(endPoint - beginPoint);

			normalVec_.x(-temp.y());
			normalVec_.y(temp.x());
		}
	}
}