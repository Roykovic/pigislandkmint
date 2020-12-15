#pragma once
#include <kmint/math/vector2d.hpp>

namespace kmint {
	namespace pigisland {

		class wall{
		public:
			wall(kmint::math::vector2d beginPoint, kmint::math::vector2d endPoint);
			kmint::math::vector2d from() { return beginPoint_; }
			kmint::math::vector2d to() { return endPoint_; }
			kmint::math::vector2d normal() { return normalVec_; }

		private:
			kmint::math::vector2d beginPoint_;
			kmint::math::vector2d endPoint_;
			kmint::math::vector2d normalVec_;
		};
	}
}
