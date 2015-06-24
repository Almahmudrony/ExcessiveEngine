#pragma once
#include "mymath/mymath.h"
#include "../Core/src/WorldComponent.h"

namespace physics {

class IEntityRigid
{
public:
	virtual void SetPos(const mm::vec3& v) = 0;
	virtual void SetRot(const mm::quat& q) = 0;
	virtual void SetScaleLocal(const mm::vec3& v) = 0;

	virtual mm::vec3 GetPos() = 0;
	virtual mm::quat GetRot() = 0;
	virtual mm::vec3 GetScaleLocal() = 0;
};
}