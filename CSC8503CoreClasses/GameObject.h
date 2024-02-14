#pragma once
#include "Transform.h"
#include "CollisionVolume.h"
#include "PhysicsObject.h"

using std::vector;

namespace NCL::CSC8503 {
	class NetworkObject;
	class RenderObject;
	class PhysicsObject;

	class GameObject	{
	public:
		GameObject(const std::string& name = "");
		~GameObject();

		void SetBoundingVolume(CollisionVolume* vol) {
			boundingVolume = vol;
		}

		const CollisionVolume* GetBoundingVolume() const {
			return boundingVolume;
		}

		bool IsActive() const {
			return isActive;
		}
		void SetActive(bool a) {
			isActive = a;
		}

		Transform& GetTransform() {
			return transform;
		}

		RenderObject* GetRenderObject() const {
			return renderObject;
		}

		PhysicsObject* GetPhysicsObject() const {
			return physicsObject;
		}

		NetworkObject* GetNetworkObject() const {
			return networkObject;
		}

		void SetRenderObject(RenderObject* newObject) {
			renderObject = newObject;
		}

		void SetPhysicsObject(PhysicsObject* newObject) {
			physicsObject = newObject;
		}

		void SetNetworkObject(NetworkObject* newObject) {
			networkObject = newObject;
		}

		const std::string& GetName() const {
			return name;
		}

		virtual void Update(float dt) {};

		virtual void OnCollisionBegin(GameObject* otherObject, float penetration) {
			//std::cout << "OnCollisionBegin event occured!\n";
		}

		virtual void OnCollisionEnd(GameObject* otherObject, float penetration) {
			//std::cout << "OnCollisionEnd event occured!\n";
		}

		bool GetBroadphaseAABB(Vector3&outsize) const;

		void UpdateBroadphaseAABB();

		void SetWorldID(int newID) {
			worldID = newID;
		}

		int		GetWorldID() const {
			return worldID;
		}

		int GetType() const {
			return type;
		}
		void SetType(int t) {
			type = t;
		}

		bool GetCollide() const {
			return hasCollided;
		};
		void SetCollide(bool c) {
			hasCollided = c;
		};

		bool GetCollect() const {
			return hasCollected;
		};
		void SetCollect(bool c) {
			hasCollected = c;
		};

		bool IsTrigger() const {
			return isTrigger;
		};
		void SetTrigger(bool t) {
			isTrigger = t;
		};

		bool GetShot() const {
			return canBeShot;
		};
		void SetShot(bool s) {
			canBeShot = s;
		};

		bool GetHooked() const {
			return canBeHooked;
		};
		void SetHooked(bool h) {
			canBeHooked = h;
		};

		bool GetFlag() const {
			return flag;
		};
		void SetFlag(bool f) {
			flag = f;
		};

		bool GetFlag2() const {
			return flag2;
		};
		void SetFlag2(bool f) {
			flag2 = f;
		};

		Vector3 GetInitPosition() const {
			return initPosition;
		};
		void SetInitPosition(Vector3 p) {
			initPosition = p;
		};

		Vector4 GetInitColour() const {
			return initColour;
		};
		void SetInitColour(Vector4 c) {
			initColour = c;
		};

	protected:
		Transform			transform;

		CollisionVolume*	boundingVolume;
		PhysicsObject*		physicsObject;
		RenderObject*		renderObject;
		NetworkObject*		networkObject;

		bool		isActive;
		bool        hasCollided;
		bool        hasCollected;
		bool        useCounter;
		bool        isTrigger;
		bool        canBeShot;
		bool        canBeHooked;
		bool        flag;
		bool        flag2;
		int			worldID;
		int         type;
		float       counter;
		std::string	name;
		Vector3 initPosition;
		Vector4 initColour;

		Vector3 broadphaseAABB;
	};
}

