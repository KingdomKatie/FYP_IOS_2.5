/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */


//  Basic Component Template
//  look in the EngineSamples Examples90.sln -> VisionEnginePlugin90 -> components -> VFollowPathCamera.h
//
//  18/01/2011
//  SAMPLEPLUGIN_IMPEXP prefix added + include to new GamePluginModule.h
//

#ifndef PLAYERCOMPONENT_H_INCLUDED
#define PLAYERCOMPONENT_H_INCLUDED


#include "GamePluginModule.h"
#include "Player.h"
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokCharacterController.hpp>
#include <Vision/Samples/Engine/HavokAi/HavokAiPCH.h>
#include <Vision/Samples/Engine/HavokAi/PathPickDlg.hpp>
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokConversionUtils.hpp>

#include <Vision/Runtime/EnginePlugins/Havok/HavokAiEnginePlugin/vHavokAiModule.hpp>
#include <Ai/Pathfinding/World/hkaiWorld.h>
#include <Vision/Runtime/EnginePlugins/Havok/HavokAiEnginePlugin/vHavokAiNavMeshResource.hpp>
#include <Ai/Pathfinding/Character/LocalSteering/hkaiLocalSteeringFilter.h>

// Havok Behavior
#include <Vision/Runtime/EnginePlugins/Havok/HavokBehaviorEnginePlugin/vHavokBehaviorModule.hpp>
// Havok AI
#include <Ai/Pathfinding/World/hkaiWorld.h>
#include <Ai/Pathfinding/Character/hkaiCharacter.h>
#include <Ai/Pathfinding/Character/Behavior/hkaiPathFollowingBehavior.h>
#include <Ai/Physics2012Bridge/LocalSteering/hkaiPhysics2012BodyObstacleGenerator.h>

// Havok Behavior
#include <Behavior/Behavior/Character/hkbCharacter.h>
#include <Behavior/Behavior/Character/hkbCharacterData.h>

class hkaiCharacter;
class hkaiBehavior;
class hkbEvent;

class Player;


//============================================================================================================
//  MyComponent Class
//============================================================================================================
class PlayerComponent : public IVObjectComponent
{

public:
  V_DECLARE_SERIAL  ( PlayerComponent, SAMPLEPLUGIN_IMPEXP ); // for RTTI
  V_DECLARE_VARTABLE( PlayerComponent, SAMPLEPLUGIN_IMPEXP );
  PlayerComponent();

   Player *GetCharacter() const { return vstatic_cast<Player*>(GetOwner()); }


   //path/AI
   void RequestPath(hkvVec3 const& dest);
   void CancelRequestedPath();
   float GetPathGoalReachedTolerance() const;
   hkvVec3 GetDirection() const { return m_cachedSpeed > 0.0f ? m_cachedDirection : dir; }
   float GetSpeed() const { return m_cachedSpeed; }
   void SetSensorSize(float sensorSize);
   void SetDesiredSpeed(float desiredSpeed);
   hkvVec3 dir;
   //like servertick in rpg
   void PerFrameUpdate();
    void FaceTowards(PlayerComponent *controller, hkvVec3 const& toPos, float t);
  

   //target point
   void SetTargetPoint(hkvVec3 const& targetPoint) { m_targetPoint = targetPoint; }
   hkvVec3 const& GetTargetPoint() { return m_targetPoint; } 

     // Called by AI world step with AI step dt
  void OnAfterCharacterStep(float deltaTime);

  void CalcDirection(hkvVec3& resultDir, hkvVec3 const& currentDir, hkvVec3 const& desiredDir, float t);


  protected:
  // IVObjectComponent
  BOOL CanAttachToObject(VisTypedEngineObject_cl *object, VString& errorOut) HKV_OVERRIDE;
  
  void SetOwner(VisTypedEngineObject_cl *newOwner) HKV_OVERRIDE;

   bool CalcFaceTowards(hkvVec3& resultDir, hkvVec3 const& fromDir, hkvVec3 const& fromPos, hkvVec3 const& toPos, float t);

  


  

//----------------------------------------------------------------------------------------------------
//  MyComponent::Serialize(...)
//----------------------------------------------------------------------------------------------------
  //SAMPLEPLUGIN_IMPEXP VOVERRIDE void Serialize( VArchive &ar );

protected:
    bool m_settingState;
    hkRefPtr<hkaiCharacter> m_aiCharacter;
    hkRefPtr<hkaiBehavior> m_aiBehavior;

	float m_sensorSize;
    float m_desiredSpeed;

	hkvVec3 m_cachedDirection;
    float m_cachedSpeed;
	int i;

 VWeakPtr<VisBaseEntity_cl> m_target;
  hkvVec3 m_targetPoint;
 


};
#endif

/*
 * Havok SDK - Base file, BUILD(#20131218)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2013
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available from salesteam@havok.com.
 * 
 */
