#pragma once
#include "Behavior.h"
#include "CoreCommon.h"
#include "WorldComponent.h"
#include "RigidBodyComponent.h"

#include <functional>
#include "CameraComponent.h"
#include "GameWorld.h"
#include "PhysicsEngine\PhysicsCommon.h"

class Actor
{
public:
	inline Actor();

public:
	inline void Update(float deltaTime);
	inline void AddBehavior(Behavior* b);

	inline CameraComponent* AddComponent_Camera();
	inline MeshComponent* AddComponent_Mesh(const std::string& modelPath);

	inline void AttachTo(WorldComponent* c);

	inline void Attach(WorldComponent* c);

	inline void AddForce(const mm::vec3& force, const mm::vec3& relPos = { 0, 0, 0 });

	inline void SetName(const std::string& s);

	inline void SetTextureNormal(const std::string& contentPath);
	inline void SetTextureBaseColor(const std::string& contentPath);
	inline void SetTextureAO(const std::string& contentPath);

	// TODO call Core::Destroy
	inline void Kill();

	inline void SetTrigger(bool bTrigger);

	inline void SetGravityScale(float s);

	inline void SetCollisionGroup(u64 ID);

	inline void SetAngularFactor(float f);

	inline void SetKinematic(bool b);

	inline void SetVelocity(const mm::vec3& v);

	inline void SetOnUpdate(const std::function<void(float deltaSeconds)>& callb);

	inline void SetOnCollision(const std::function<void(const Collision& info)>& callb);
	inline void SetOnCollisionEnter(const std::function<void(const Collision& info)>& callb);
	inline void SetOnCollisionExit(const std::function<void(const Collision& info)>& callb);

	inline void SetPos(const mm::vec3& v);
	inline void SetRot(const mm::quat& q);
	inline void SetScale(const mm::vec3& v);
	
	inline void RotX(float angleDegree);
	inline void RotY(float angleDegree);
	inline void RotZ(float angleDegree);

	inline void Move(const mm::vec3& v);
	inline void Rot(const mm::quat& q);
	inline void Scale(const mm::vec3& v);
	inline void ScaleLocal(const mm::vec3& v);
	
	inline void SetRelPos(const mm::vec3& v);
	inline void SetRelRot(const mm::quat& q);
	inline void SetRelScale(const mm::vec3& v);
	inline void MoveRel(const mm::vec3& v);
	inline void RotRel(const mm::quat& q);
	inline void ScaleRel(const mm::vec3& v);
	
	//inline Entity* GetParent() const { return worldEntity->GetParent(); }
	
	inline const mm::vec3  GetScale() const;
	inline const mm::mat3& GetSkew() const;
	inline const mm::vec3& GetPos()	const;
	inline const mm::quat& GetRot()	const;
	
	inline const mm::vec3  GetRelLocalScale() const;
	inline const mm::vec3& GetRelPos() const;
	inline const mm::quat& GetRelRot() const;
	
	
	inline const Transform3D& GetRelTransform()	const;
	inline const Transform3D& GetTransform() const;
	
	inline mm::vec3 GetFrontDir()	const;
	inline mm::vec3 GetRightDir()	const;
	inline mm::vec3 GetDownDir()	const;
	inline mm::vec3 GetLeftDir()	const;
	inline mm::vec3 GetBackDir()	const;
	inline mm::vec3 GetUpDir()	const;
	
	// TODO
	inline mm::vec3 GetVelocity() const;

	inline const std::vector<WorldComponent*> GetComponents() const;

	//inline void GetComponents(std::vector<WorldComponent*>& allComp);

	template<class T>
	inline std::vector<T*> GetComponents() const;

	inline std::vector<Behavior*>& GetBehaviors();

	inline const std::function<void(float deltaSeconds)>& GetOnUpdate() const;
	inline const std::function<void(const Collision& col)>& GetOnCollision() const;
	inline const std::function<void(const Collision& col)>& GetOnCollisionEnter() const;
	inline const std::function<void(const Collision& col)>& GetOnCollisionExit() const;
	inline const std::string& GetName() const;
	inline bool IsKilled() const;
	inline std::vector<Actor*>& GetChilds() const;

	inline WorldComponent* GetRootComponent(u64 idx) const;
	inline std::vector<ContactPoint> GetContactPoints() const;

protected:
	std::string name;
	Transform3D transform;

	mm::vec3 velocity; // non physics velocity

	std::vector<WorldComponent*> rootComponents;

	// Behavior -> ActorScripts
	std::vector<Behavior*> behaviors;

	std::function<void(float deltaSeconds)> onUpdate;
	std::function<void(const Collision& col)> onCollision;
	std::function<void(const Collision& col)> onCollisionEnter;
	std::function<void(const Collision& col)> onCollisionExit;

	// Lifecycle
	bool bKilled;
};


Actor::Actor()
:onUpdate(nullptr), onCollision(nullptr), onCollisionEnter(nullptr), onCollisionExit(nullptr), bKilled(false), velocity(0,0,0)
{
}

void Actor::Update(float deltaTime)
{
	if (onUpdate)
		onUpdate(deltaTime);

	// Null velocity, don't update
	if (velocity.x == 0 && velocity.y == 0 && velocity.z == 0)
		return;

	for (WorldComponent* c : rootComponents)
	{
		c->Move(velocity * deltaTime);
	}
}

void Actor::AddBehavior(Behavior* b) 
{ 
	behaviors.push_back(b); 
}

CameraComponent* Actor::AddComponent_Camera()
{
	CameraComponent* c = World.AddComponent_Camera();
	rootComponents.push_back(c);

	return c;
}

MeshComponent* Actor::AddComponent_Mesh(const std::string& modelPath)
{
	MeshComponent* c = World.AddComponent_Mesh(modelPath);
	rootComponents.push_back(c);

	return c;
}

void Actor::AttachTo(WorldComponent* c) 
{ 
	for(WorldComponent* root : rootComponents)
		root->AttachTo(c);
}

void Actor::Attach(WorldComponent* c)
{
	bool bAlreadyAttached = false;
	for (WorldComponent* rootComp : rootComponents)
	{
		if (c == rootComp)
		{
			break;
			bAlreadyAttached = true;
		}
	}

	rootComponents.push_back(c);
}

template<class T>
void Actor::LambdaOnComponents(const std::function<void(T*)>& lambda)
{
	std::function<void(WorldComponent* c, const std::function<void(T*)>& lambda)> recursiveFunc;
	recursiveFunc = [&](WorldComponent* c, const std::function<void(T*)>& lambda)
	{
		if (c->Is<T>())
			lambda((T*)c);

		for (auto& child : c->GetChilds())
			recursiveFunc(child, lambda);
	};
	recursiveFunc(rootComp, lambda);
}

void Actor::AddForce(const mm::vec3& force, const mm::vec3& relPos /*= { 0, 0, 0 }*/)
{
	std::vector<RigidBodyComponent*> rigidBodyComponents = GetComponents<RigidBodyComponent>();
	for (auto c : rigidBodyComponents)
	{
		c->AddForce(force, relPos);
	}
}

void Actor::SetName(const std::string& s)
{
	name = s;
}

void Actor::SetTextureNormal(const std::string& contentPath)
{
	const std::vector<MeshComponent*>& comps = GetComponents<MeshComponent>();

	for (auto& c : comps)
		c->SetTextureNormal(contentPath);
}

void Actor::SetTextureBaseColor(const std::string& contentPath)
{
	const std::vector<MeshComponent*>& comps = GetComponents<MeshComponent>();

	for (auto& c : comps)
		c->SetTextureBaseColor(contentPath);
}

void Actor::SetTextureAO(const std::string& contentPath)
{
	const std::vector<MeshComponent*>& comps = GetComponents<MeshComponent>();

	for (auto& c : comps)
		c->SetTextureAO(contentPath);
}

void Actor::Kill() 
{
	bKilled = true;
}

void Actor::SetTrigger(bool bTrigger)
{
	std::vector<RigidBodyComponent*> rigidBodyComponents = GetComponents<RigidBodyComponent>();
	for (auto c : rigidBodyComponents)
	{
		c->SetTrigger(bTrigger);
	}
}

void Actor::SetGravityScale(float gravityScale)
{
	std::vector<RigidBodyComponent*> rigidBodyComponents = GetComponents<RigidBodyComponent>();
	for (auto c : rigidBodyComponents)
	{
		c->SetGravityScale(gravityScale);
	}
}

void Actor::SetCollisionGroup(u64 ID)
{
	std::vector<RigidBodyComponent*> rigidBodyComponents = GetComponents<RigidBodyComponent>();
	for (auto c : rigidBodyComponents)
	{
		c->SetCollisionGroup(ID);
	}
}

void Actor::SetAngularFactor(float angularFactor)
{
	std::vector<RigidBodyComponent*> rigidBodyComponents = GetComponents<RigidBodyComponent>();
	for (auto c : rigidBodyComponents)
	{
		c->SetAngularFactor(angularFactor);
	}
}

void Actor::SetKinematic(bool bKinematic)
{
	std::vector<RigidBodyComponent*> rigidBodyComponents = GetComponents<RigidBodyComponent>();
	for (auto a : rigidBodyComponents)
	{
		a->SetKinematic(bKinematic);
	}
}

void Actor::SetVelocity(const mm::vec3& v)
{
	// TODO non physis velocity !!,  mm::vec3 Actor::velocity;
	std::vector<RigidBodyComponent*> rigidBodyComponents = GetComponents<RigidBodyComponent>();

	// There is no rigid body in actor, use our "velocity" system
	if (rigidBodyComponents.size() == 0)
	{
		velocity = v;
	}
	else
	{
		for (auto a : rigidBodyComponents)
		{
			a->SetVelocity(v);
		}
	}
}

void Actor::SetOnUpdate(const std::function<void(float deltaSeconds)>& callb)
{
	onUpdate = callb;
}

void Actor::SetOnCollision(const std::function<void(const Collision& info)>& callb)
{
	onCollision = callb;
}

void Actor::SetOnCollisionEnter(const std::function<void(const Collision& info)>& callb)
{ 
	onCollisionEnter = callb;
}

void Actor::SetOnCollisionExit(const std::function<void(const Collision& info)>& callb)
{ 
	onCollisionExit = callb;
}

void Actor::SetPos(const mm::vec3& v)
{
	// Move actor
	mm::vec3 dMove = v - transform.GetPos();
	transform.SetPos(v);

	// Move components
	for (WorldComponent* c : rootComponents)
	{
		c->Move(dMove);
	}
}

void Actor::SetRot(const mm::quat& q)
{
	transform.SetRot(q);

	// Delta world rot
	mm::quat dRot = mm::inverse(q) * q;

	for (WorldComponent* c : rootComponents)
	{
		c->Rot(dRot, transform.GetPos());
	}
}

void Actor::SetScale(const mm::vec3& v)
{
	for (WorldComponent* c : rootComponents)
	{
		c->SetScale(v, transform.GetPos(), transform.GetRot());
	}

	transform.SetScale(v);
}

void Actor::RotX(float angleDegree)
{
	Rot(mm::quat(mm::radians(angleDegree), mm::vec3(1, 0, 0)));
}

void Actor::RotY(float angleDegree)
{
	Rot(mm::quat(mm::radians(angleDegree), mm::vec3(0, 1, 0)));
}

void Actor::RotZ(float angleDegree)
{
	Rot(mm::quat(mm::radians(angleDegree), mm::vec3(0, 0, 1)));
}

void Actor::Move(const mm::vec3& v)
{
	for (WorldComponent* c : rootComponents)
	{
		c->Move(v);
	}
	
	transform.Move(v);
}

void Actor::Rot(const mm::quat& q)
{
	for (WorldComponent* c : rootComponents)
	{
		c->Rot(q);
	}

	transform.Rot(q);
}

void Actor::Scale(const mm::vec3& v)
{
	for (WorldComponent* c : rootComponents)
	{
		c->Scale(v, transform.GetPos(), transform.GetRot());
	}

	transform.Scale(v);
}

void Actor::ScaleLocal(const mm::vec3& v)
{
	assert(0);
	//rootComp->ScaleLocal(v);
}

void Actor::SetRelPos(const mm::vec3& v)
{
	assert(0);
	//rootComp->SetRelPos(v);
}

void Actor::SetRelRot(const mm::quat& q)
{
	assert(0);
	//rootComp->SetRelRot(q);
}

void Actor::SetRelScale(const mm::vec3& v)
{
	assert(0);
	//rootComp->SetRelScale(v);
}

void Actor::MoveRel(const mm::vec3& v)
{
	assert(0);
	//rootComp->MoveRel(v);
}

void Actor::RotRel(const mm::quat& q)
{
	assert(0);
	//rootComp->RotRel(q);
}

void Actor::ScaleRel(const mm::vec3& v)
{
	assert(0);
	//rootComp->ScaleRel(v);
}

//inline Entity* GetParent() const { return worldEntity->GetParent(); }

const mm::vec3  Actor::GetScale() const
{
	return transform.GetScale();
}

const mm::mat3& Actor::GetSkew() const
{
	return transform.GetSkew();
}

const mm::vec3& Actor::GetPos()	const
{
	return transform.GetPos();
}

const mm::quat& Actor::GetRot()	const
{
	return transform.GetRot();
}

const mm::vec3 Actor::GetRelLocalScale() const
{
	//TODO
	return transform.GetScale();
}

const mm::vec3& Actor::GetRelPos() const
{
	//TODO
	return transform.GetPos();
}

const mm::quat& Actor::GetRelRot() const
{
	//TODO
	return transform.GetRot();
}


const Transform3D& Actor::GetRelTransform()	const
{
	//TODO
	return transform;
}

const Transform3D& Actor::GetTransform() const
{
	return transform;
}

mm::vec3 Actor::GetFrontDir() const
{
	return GetTransform().GetFrontDir();
}

mm::vec3 Actor::GetBackDir() const
{
	return GetTransform().GetBackDir();
}

mm::vec3 Actor::GetUpDir() const
{
	return GetTransform().GetUpDir();
}

mm::vec3 Actor::GetDownDir() const
{
	return GetTransform().GetDownDir();
}
mm::vec3 Actor::GetRightDir()	const
{
	return GetTransform().GetRightDir();
}

mm::vec3 Actor::GetLeftDir() const
{
	return GetTransform().GetLeftDir();
}

mm::vec3 Actor::GetVelocity() const
{
	// TODO non physis velocity !!,  mm::vec3 Actor::velocity;
	std::vector<RigidBodyComponent*> rigidBodyComponents = GetComponents<RigidBodyComponent>();

	// There is no rigid body in actor, use our "velocity" system
	if (rigidBodyComponents.size() == 0)
	{
		return velocity;
	}
	else
	{
		for (auto a : rigidBodyComponents)
		{
			return a->GetVelocity();
		}
	}

	return mm::vec3(0, 0, 0);
}

const std::vector<WorldComponent*> Actor::GetComponents() const
{
	std::function<void(WorldComponent*, std::vector<WorldComponent*>& comps_out)> collectCompsRecursively;
	collectCompsRecursively = [&](WorldComponent* c, std::vector<WorldComponent*>& comps_out)
	{
		comps_out.push_back(c);

		for (auto& child : c->GetChilds())
			collectCompsRecursively(child, comps_out);
	};

	std::vector<WorldComponent*> comps;
	for (WorldComponent* rootComp : rootComponents)
	{
		collectCompsRecursively(rootComp, comps);
	}
	
	return comps;
}

//inline void GetComponents(std::vector<WorldComponent*>& allComp) 
//{ 
//	static std::function<void(WorldComponent*)> collectCompsRecursively  = [&](WorldComponent* c)
//	{
//		allComp.push_back(c);
//
//		for (auto& child : c->GetChilds())
//			collectCompsRecursively(child);
//	};
//
//	collectCompsRecursively(rootComp);
//}

template<class T>
std::vector<T*> Actor::GetComponents() const
{
	std::function<void(WorldComponent* c, std::vector<T*>& comps_out)> collectCompsRecursively;
	collectCompsRecursively = [&](WorldComponent* c, std::vector<T*>& comps_out)
	{
		if (c->Is<T>())
			comps_out.push_back((T*)c);

		for (auto& child : c->GetChilds())
			collectCompsRecursively(child, comps_out);
	};

	std::vector<T*> comps;
	for (WorldComponent* rootComp : rootComponents)
	{
		collectCompsRecursively(rootComp, comps);
	}

	return comps;
}

std::vector<Behavior*>& Actor::GetBehaviors()
{
	return behaviors;
}

const std::function<void(float deltaSeconds)>& Actor::GetOnUpdate() const
{
	return onUpdate;
}

const std::function<void(const Collision& col)>& Actor::GetOnCollision() const
{
	return onCollision;
}

const std::function<void(const Collision& col)>& Actor::GetOnCollisionEnter() const
{
	return onCollisionEnter;
}

const std::function<void(const Collision& col)>& Actor::GetOnCollisionExit() const
{
	return onCollisionExit;
}

const std::string& Actor::GetName() const
{
	return name;
}

bool Actor::IsKilled() const
{
	return bKilled;
}

std::vector<ContactPoint> Actor::GetContactPoints() const
{
	std::vector<ContactPoint> contactPoints;

	std::vector<RigidBodyComponent*> rigidBodyComponents = GetComponents<RigidBodyComponent>();

	for (auto comp : rigidBodyComponents)
	{
		for (auto physicsContact : (comp->GetContactPoints()))
		{
			ContactPoint contact;
			contact.normalA = physicsContact.normalA;
			contact.normalB = physicsContact.normalB;
			contact.posA = physicsContact.posA;
			contact.posB = physicsContact.posB;
			contactPoints.push_back(contact);
		}
	}

	return contactPoints;
}

WorldComponent* Actor::GetRootComponent(u64 idx) const
{
	assert(idx < rootComponents.size());

	return rootComponents[idx];
}