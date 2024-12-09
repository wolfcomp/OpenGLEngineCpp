#include "physics.h"

float PhysicsComponent::restitution = 0.5f;

template <>
void ECSGlobalMap::insert<PhysicsComponent>(UUID id, PhysicsComponent *value)
{
    for (int i = 0; i < size; i++)
    {
        if (types[i] == typeid(PhysicsComponent).name())
        {
            data[i].insert(id, value);
            return;
        }
    }
    if (size >= capacity)
    {
        expand();
    }
    types[size] = typeid(PhysicsComponent).name();
    data[size++].insert(id, value);
}

inline void PhysicsComponent::apply_collision(PhysicsComponent *other, glm::vec3 normal)
{
    // Safety check for invalid mass
    if (mass <= 0.0f || other->mass <= 0.0f)
    {
        return;
    }

    // Load vectors into SIMD registers
    __m128 v1 = _mm_load_ps(&velocity.x);
    __m128 v2 = _mm_load_ps(&other->velocity.x);
    __m128 n = _mm_load_ps(&normal.x);

    // Calculate relative velocity using SIMD
    __m128 relVel = _mm_sub_ps(v1, v2);

    // Calculate dot product using SIMD
    __m128 dot = _mm_dp_ps(relVel, n, 0x71);
    float velocityAlongNormal = _mm_cvtss_f32(dot);

    // Early exit check
    if (velocityAlongNormal > 0)
    {
        return;
    }

    // Clamp drag coefficients to valid range [0, 1]
    float clampedDrag1 = glm::clamp(dragCoefficient, 0.0f, 1.0f);
    float clampedDrag2 = glm::clamp(other->dragCoefficient, 0.0f, 1.0f);

    // Calculate restitution with clamped values
    float restitution = 1.0f - (clampedDrag1 + clampedDrag2) * 0.5f;
    restitution = glm::max(0.0f, restitution); // Ensure non-negative restitution

    // Calculate inverse masses with safety
    float invMassSum = 1.0f / mass + 1.0f / other->mass;
    if (invMassSum <= 0.0f)
    {
        return;
    }

    // Calculate impulse scalar
    float impulseScalar = -(1.0f + restitution) * velocityAlongNormal / invMassSum;

    // Create impulse vector using SIMD
    __m128 impulseScalarVec = _mm_set1_ps(impulseScalar);
    __m128 impulse = _mm_mul_ps(n, impulseScalarVec);

    // Calculate velocity changes using SIMD
    __m128 invMass1 = _mm_set1_ps(1.0f / mass);
    __m128 invMass2 = _mm_set1_ps(1.0f / other->mass);

    __m128 deltaV1 = _mm_mul_ps(impulse, invMass1);
    __m128 deltaV2 = _mm_mul_ps(impulse, invMass2);

    // Update velocities
    v1 = _mm_add_ps(v1, deltaV1);
    v2 = _mm_sub_ps(v2, deltaV2);

    // Apply drag using SIMD with clamped values
    __m128 drag1 = _mm_set1_ps(1.0f - clampedDrag1);
    __m128 drag2 = _mm_set1_ps(1.0f - clampedDrag2);
    v1 = _mm_mul_ps(v1, drag1);
    v2 = _mm_mul_ps(v2, drag2);

    // Calculate collision force for acceleration updates with safety checks
    __m128 impulseLengthSq = _mm_dp_ps(impulse, impulse, 0x77);
    float impulseLengthSquared = _mm_cvtss_f32(impulseLengthSq);

    __m128 collisionForce;
    if (impulseLengthSquared > 1e-6f)
    { // Avoid division by very small numbers
        __m128 impulseLength = _mm_sqrt_ps(impulseLengthSq);
        collisionForce = _mm_div_ps(impulse, impulseLength);
    }
    else
    {
        collisionForce = _mm_setzero_ps();
    }

    // Load accelerations into SIMD registers
    __m128 a1 = _mm_load_ps(&acceleration.x);
    __m128 a2 = _mm_load_ps(&other->acceleration.x);

    // Update accelerations with bounded values
    float boundedImpulseScalar1 = glm::clamp(impulseScalar / mass, -1000.0f, 1000.0f);
    float boundedImpulseScalar2 = glm::clamp(impulseScalar / other->mass, -1000.0f, 1000.0f);

    __m128 impulseScalarMassRatio1 = _mm_set1_ps(boundedImpulseScalar1);
    __m128 impulseScalarMassRatio2 = _mm_set1_ps(boundedImpulseScalar2);

    a1 = _mm_add_ps(a1, _mm_mul_ps(collisionForce, impulseScalarMassRatio1));
    a2 = _mm_sub_ps(a2, _mm_mul_ps(collisionForce, impulseScalarMassRatio2));

    // Store results back to particles
    _mm_store_ps(&velocity.x, v1);
    _mm_store_ps(&other->velocity.x, v2);
    _mm_store_ps(&acceleration.x, a1);
    _mm_store_ps(&other->acceleration.x, a2);
}