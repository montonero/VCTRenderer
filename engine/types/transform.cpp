#include "Transform.h"

#include "../util/const_definitions.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/orthonormalize.inl>

Transform::Transform() : changed(true)
{
    position = Vector3::zero;
    scale = Vector3::one;
    rotation = glm::quat(Vector3::zero);
    forward = Vector3::forward;
    up = Vector3::up;
    right = Vector3::right;
    transformation = ToMatrix();
    changed = false;
}

Transform::~Transform()
{
}

void Transform::Position(const glm::vec3 &val)
{
    changed |= position != val;

    if (!changed) { return; }

    position = val;
}

void Transform::Rotation(const glm::quat &val)
{
    changed |= rotation != val;

    if (!changed) { return; }

    this->angles = eulerAngles(val);
    rotation = val;
    UpdateCoordinates();
}

void Transform::Rotation(const glm::vec3 &angles)
{
    changed |= this->angles != angles;

    if (!changed) { return; }

    this->angles = angles;
    rotation = glm::quat(angles);
    UpdateCoordinates();
}

void Transform::Scale(const glm::vec3 &val)
{
    changed |= scale != val;

    if (!changed) { return; }

    scale = val;
}

const glm::vec3 &Transform::Position() const
{
    return position;
}

const glm::quat &Transform::Rotation() const
{
    return rotation;
}

void Transform::UpdateCoordinates()
{
    forward = Vector3::forward * rotation;
    right = Vector3::right * rotation;
    up = Vector3::up * rotation;
}

const glm::vec3 &Transform::Scale() const
{
    return scale;
}

const glm::vec3 &Transform::Forward() const
{
    return forward;
}

const glm::vec3 &Transform::Right() const
{
    return right;
}

const glm::vec3 &Transform::Up() const
{
    return up;
}

const glm::vec3 &Transform::Angles() const
{
    return angles;
}

const glm::mat4x4 &Transform::ToMatrix()
{
    if (changed)
    {
        transformation = translate(position) *
                         mat4_cast(rotation) *
                         glm::scale(scale);
    }

    return transformation;
}

void Transform::LookAt(const glm::vec3 &pos, const glm::vec3 &up)
{
    forward = normalize(pos);
    forward = orthonormalize(forward, up);
    right = cross(up, forward);
    rotation.w = sqrt(1.0f + right.x + up.y + forward.z) * 0.5f;
    auto recip = 1.0f / (4.0f * rotation.w);
    rotation.x = (up.z - forward.y) * recip;
    rotation.y = (forward.x - right.z) * recip;
    rotation.z = (right.y - up.x) * recip;
    changed = true;
}