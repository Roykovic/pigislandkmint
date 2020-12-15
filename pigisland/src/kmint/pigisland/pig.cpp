#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/resources.hpp"

namespace kmint {
namespace pigisland {


pig::pig(math::vector2d location, boat& boat, shark& shark)
  : play::free_roaming_actor{location},
    drawable_{ *this, pig_image() }, boat_(boat), shark_(shark) {
}

void pig::act(delta_time dt) {

        math::vector2d steeringForce = steering->wander(this);

        steeringForce += (steering->seek(boat_.location(), this->location(), maxSpeed_, velocity_) * boatAttraction);
        steeringForce += (steering->flee(shark_.location(), this->location(), maxSpeed_, velocity_) * sharkAttraction);

        steeringForce += (steering->seperation(this, tagNeighbours()) * seperation);
        steeringForce += (steering->cohesion(this, maxSpeed_, velocity_, tagNeighbours()) * cohesion);
        steeringForce += (steering->alignment(this, tagNeighbours(false)) * alignment);

        steeringForce += (steering->wall_avoidance(this, walls_)*10);


        math::vector2d acceleration = steeringForce / mass_;

        velocity_ += acceleration * to_seconds(dt);

        double vector_length = std::sqrt(std::pow(velocity_.x(), 2) + std::pow(velocity_.y(), 2));
        if (vector_length > maxSpeed_) {
            velocity_ = normalize(velocity_);

            velocity_ *= maxSpeed_;
        }

        //update the position
        move(velocity_ *to_seconds(dt));


        heading_ = normalize(velocity_);
}

void pig::move(math::vector2d delta) {
    math::vector2d newLoc;

    if (location().x() + delta.x() > 1024) {
        newLoc.x(location().x() - delta.x());
    }
    else if (location().x() + delta.x() < 0){
        newLoc.x(location().x() - delta.x());
    }
    else {
        newLoc.x(location().x() + delta.x());
    }
    if (location().y() + delta.y() > 768) {
        newLoc.y(location().y() - delta.y());

    }
    else if (location().y() + delta.y() < 0) {
        newLoc.y(location().y() - delta.y());

    }
    else {
        newLoc.y(location().y() + delta.y());
    }
    location(newLoc);
}

std::vector<kmint::math::vector2d> pig::tagNeighbours(bool location) {

    std::vector<kmint::math::vector2d> taggedNeighbours;

	for (pig* pig : pigs_)
	{
        if (!pig->removed()) {
            kmint::math::vector2d to = (this->location() - pig->location());

            double toLengthSq = pow(to.x(), 2) + pow(to.y(), 2);

            double range = 60;

            if (this != pig && toLengthSq < pow(range, 2)) {
                if (location) {
                    taggedNeighbours.push_back(pig->location());
                }
                else {
                    taggedNeighbours.push_back(pig->heading());
                }
            }
        }
	}

    return taggedNeighbours;
}

} // namespace pigisland

} // namespace kmint
