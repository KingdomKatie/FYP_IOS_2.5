/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

//  Basic Component Template
//  

#include "GamePluginPCH.h"
#include "PlayerControlComponent.h"
#include "ComponentManager.h"

//  Register the class in the engine module so it is available for RTTI
V_IMPLEMENT_SERIAL( PlayerComponent, IVObjectComponent,0,Vision::GetEngineModule());

START_VAR_TABLE(PlayerComponent, IVObjectComponent, "Player Controller", 0, "Player Controller")
  DEFINE_VAR_FLOAT_AND_NAME(PlayerComponent, m_sensorSize, "SensorSize", "Character Sensor Size", "128", 0, NULL);
  DEFINE_VAR_FLOAT_AND_NAME(PlayerComponent, m_desiredSpeed, "DesiredSpeed", "Character Desired Speed", "200", 0, NULL);
END_VAR_TABLE
namespace
{
  // AiWorldListener
  class AiWorldListener : public hkaiWorld::Listener
  {
  public:
    void AddController(PlayerComponent *controller);

    void RemoveController(PlayerComponent *controller);

  private:
    void AddListener();

    void RemoveListener();

    // hkaiWorld::Listener
    void postCharacterStepCallback(hkaiWorld::CharacterStepCallbackContext const& ctx) HK_OVERRIDE;

  private:
    VRefCountedCollection<PlayerComponent> m_controllers;
  }
  s_AiWorldListener;

  // LocalSteeringFilter
  class LocalSteeringFilter : public hkaiLocalSteeringFilter
  {
	hkBool32 isCharacterEnabled(hkaiCharacter const *aiCharacter, hkaiCharacter const *otherAiCharacter) const HK_OVERRIDE;

	hkBool32 isObstacleEnabled(hkaiCharacter const *aiCharacter, hkaiObstacleGenerator const *otherObstacle) const HK_OVERRIDE;
  }
  s_SteeringFilter;
}


PlayerComponent::PlayerComponent()
	:m_target(NULL),
	 m_cachedDirection(100.0f,100.100f, 1.0f),
	 m_desiredSpeed(800.f),
	 m_sensorSize(300.f),
     m_cachedSpeed(50.0f),
	 i(0)
{
}

void PlayerComponent::RequestPath(hkvVec3 const& dest)
{
   hkVector4 scaledDest;
   vHavokConversionUtils::VisVecToPhysVecLocal(dest, scaledDest);
   
   /*if ( m_aiCharacter->getState() != hkaiCharacter::STATE_FOLLOWING_PATH)
   {
    Player *PlayerAnimate = static_cast<Player*>(GetOwner());
    PlayerAnimate->StartWalkAnimation(true);
   }*/
   m_aiBehavior->requestPath(scaledDest);
 

}
float PlayerComponent::GetPathGoalReachedTolerance() const
{
  hkaiPathFollowingBehavior *pathFollowingBehavior = static_cast<hkaiPathFollowingBehavior*>(m_aiBehavior.val());
  float const radiusMultiplier = hkvMath::Max(0.0f, pathFollowingBehavior->getCharacterRadiusMultiplier() - 1.0f);
  float const tolerance = m_aiCharacter->getRadius() * radiusMultiplier + pathFollowingBehavior->getPathFollowingProperties()->m_goalDistTolerance;
  return HK2VIS_FLOAT_SCALED(tolerance);
}


 void PlayerComponent::PerFrameUpdate()
{
  Player const *const controllerPlayer = static_cast<Player*>(GetOwner());

  VASSERT(controllerPlayer);
  if(!controllerPlayer )
  {
    return;
  }

  // Project Vision entity position to navmesh and update AI character
  {
    hkVector4 characterPos;
	vHavokConversionUtils::VisVecToPhysVecLocal(controllerPlayer->GetPosition(), characterPos);

  
    m_aiCharacter->setPosition(characterPos);
    {
      hkaiWorld *const aiWorld = vHavokAiModule::GetInstance()->GetAiWorld();
      hkaiCharacter *const aiCharacter = m_aiCharacter;

      hkaiCharacterUtil::ProjectToMeshSettings projectToMeshSettings;
      hkaiCharacterUtil::integrateMotionAndProjectToMesh(0.0f, aiWorld, &aiCharacter, 1, projectToMeshSettings); 
    }

	 
	   hkvVec3 aiPos;
	  vHavokConversionUtils::PhysVecToVisVecLocal(m_aiCharacter->getPosition(), aiPos);
	  IVRenderInterface* pRenderer = Vision::Contexts.GetCurrentContext()->GetRenderInterface(); 
	  VSimpleRenderState_t state(VIS_TRANSP_ALPHA, RENDERSTATEFLAG_FRONTFACE);
	  pRenderer->RenderSphere(aiPos, 50.0f, V_RGBA_YELLOW, state, RENDERSHAPEFLAGS_LINES);
      dir = controllerPlayer->GetDirection();
	 

	   Player *PlayerAnimate = static_cast<Player*>(GetOwner());

	  hkaiCharacter::State playerState = m_aiCharacter->getState();
	 if(playerState ==  hkaiCharacter::STATE_GOAL_REACHED)
	 {
		 Vision::Message.Add(1, "STATE_GOAL_REACHED");
		// PlayerAnimate->StartIdleAnimation(false);
	 }
	  else if(playerState ==  hkaiCharacter::STATE_FOLLOWING_PATH)
	 {
		 Vision::Message.Add(1, "STATE_FOLLOWING_PATH");
		 
	 }
	  else if(playerState ==  hkaiCharacter::STATE_PATH_FAILED)
	 {
		 Vision::Message.Add(1, "STATE_PATH_FAILED");
	 }
	 else if(playerState ==  hkaiCharacter::STATE_REPATHING_INCOMPLETE_PATH)
	 {
		 Vision::Message.Add(1, "STATE_REPATHING_INCOMPLETE_PATH");
	 }
	 else if(playerState ==  hkaiCharacter::STATE_WANDERED_OFF_PATH)
	 {
		 Vision::Message.Add(1, "STATE_WANDERED_OFF_PATH");
	 }
	 else if(playerState ==  hkaiCharacter::STATE_SLOWING_TO_GOAL)
	 {
		 Vision::Message.Add(1, "STATE_SLOWING_TO_GOAL");
	 }
	 else if(playerState ==  hkaiCharacter::STATE_NEEDS_NEW_PATH)
	 {
		 Vision::Message.Add(1, "STATE_NEEDS_NEW_PATH");
	 }
	  else if(playerState ==  hkaiCharacter::STATE_MANUAL_CONTROL)
	 {
		 Vision::Message.Add(1, "STATE_MANUAL_CONTROL");
	 }
	 else
	 {
		 Vision::Message.Add(1, "STATE_OTHER");
	 }

	
  }

 
 }
  
  //set set ai charecter and behavior 
 void PlayerComponent::SetOwner(VisTypedEngineObject_cl *pOwner)
{
  IVObjectComponent::SetOwner(pOwner);

  // Insert code here to respond to attaching this component to an object
  // This function is called with pOwner==NULL when de-attaching.
  //the compontnent belongs to the player entity
  if (pOwner!=NULL)
    ComponentManager::g_GlobalManager.m_Components.AddUnique(this);
  else
    ComponentManager::g_GlobalManager.m_Components.SafeRemove(this);

   Player *PlayerOwner = vstatic_cast<Player*>(pOwner);
 
   if(PlayerOwner)
  {
	  //get info for entity to be stored in m_aiCharacter to be used in AI
    hkvVec3 const& pos = PlayerOwner->GetPosition();
    m_cachedDirection = PlayerOwner->GetDirection();

	
    hkaiAvoidanceSolver::AvoidanceProperties *avoidanceProperties = new hkaiAvoidanceSolver::AvoidanceProperties();
   hkReal const sensorSize = VIS2HK_FLOAT_SCALED(m_sensorSize);
    {
     avoidanceProperties->m_localSensorAabb.m_max.setAll3(sensorSize);
     avoidanceProperties->m_localSensorAabb.m_min.setNeg3(avoidanceProperties->m_localSensorAabb.m_max);
    }

	//get charecter info from entity to make Ai character
    hkaiCharacter::Cinfo characterCinfo;
    {
      
	  vHavokConversionUtils::VisVecToPhysVecLocal(pos, characterCinfo.m_initialPosition);
      characterCinfo.m_up = hkVector4::getConstant<HK_QUADREAL_0010>();
      {
        hkReal const radius = VIS2HK_FLOAT_SCALED(40.0f);
        hkReal const desiredSpeed = VIS2HK_FLOAT_SCALED(m_desiredSpeed);
        hkReal const height = VIS2HK_FLOAT_SCALED(160.0f);
			
        {
          characterCinfo.m_radius = radius;
          characterCinfo.m_desiredSpeed = desiredSpeed;
        }
      }
      characterCinfo.m_avoidanceProperties = avoidanceProperties;
      characterCinfo.m_userData = (hkUlong)this;
    }

	//make new ai Charecter for the player 
	 m_aiCharacter = new hkaiCharacter(characterCinfo);
   {
       m_aiCharacter->setSteeringFilter(&s_SteeringFilter);
        avoidanceProperties->removeReference();
  }
   m_aiCharacter->removeReference();

	//set player charecter behavior for path following

   hkaiPathFollowingBehavior::Cinfo behaviorCinfo;
   m_aiBehavior = new hkaiPathFollowingBehavior(m_aiCharacter, vHavokAiModule::GetInstance()->GetAiWorld(), behaviorCinfo);
   m_aiBehavior->removeReference();

    vHavokAiModule::GetInstance()->getCharacterBehaviors().pushBack(m_aiBehavior);

     s_AiWorldListener.AddController(this);
  }
   else
	{  
		s_AiWorldListener.RemoveController(this);

    int const aiBehaviorIndex = vHavokAiModule::GetInstance()->getCharacterBehaviors().indexOf(m_aiBehavior);
    VASSERT(aiBehaviorIndex >= 0);
    if(aiBehaviorIndex >= 0)
    {
      vHavokAiModule::GetInstance()->getCharacterBehaviors().removeAt(aiBehaviorIndex);
    }

    m_aiBehavior = NULL;
    m_aiCharacter = NULL;

  
  }

 }
  

 
  

 void PlayerComponent::OnAfterCharacterStep(float deltaTime)
{
	hkVector4 const& aiVelocity = m_aiCharacter->getVelocity();

  // update m_cachedDirection and m_cachedSpeed
  vHavokConversionUtils::PhysVecToVisVecLocal(aiVelocity, m_cachedDirection);
  m_cachedSpeed = VIS2HK_FLOAT_SCALED(m_cachedDirection.getLengthAndNormalize());

  // calculate displacement for accumulation
  hkVector4 aiDisplacement;
  aiDisplacement.setMul(deltaTime, aiVelocity);

  hkvVec3 motionDelta;
  vHavokConversionUtils::PhysVecToVisVecLocal(aiDisplacement, motionDelta);

  // update character
  Player *controllerPlayer = static_cast<Player*>(GetOwner());
  {
    // vHavokCharacterController consumes motion delta; set world-space displacement from AI
    controllerPlayer->IncMotionDeltaWorldSpace(motionDelta);
  }


}

 BOOL PlayerComponent::CanAttachToObject(VisTypedEngineObject_cl *pObject, VString &sErrorMsgOut)
{
  if (!IVObjectComponent::CanAttachToObject(pObject, sErrorMsgOut))
    return FALSE;

  // Define criteria here that allows the editor to attach this component to the passed object.
  // In our example, the object should be derived from VisObject3D_cl to be positionable.
  bool bIsValidType = pObject->IsOfType(V_RUNTIME_CLASS(Player));

  if (!bIsValidType)
  {
    // Provide detailed information why the component can't be added.
    // This information will be displayed to the level designed in vForge.
    sErrorMsgOut = "Component can only be added to Player entity class.";
    return FALSE;
  }

  return TRUE;
}
 // AiWorldListener
void AiWorldListener::AddController(PlayerComponent *controller)
{
  VASSERT(controller);
  if(controller)
  {
    if(m_controllers.Count() == 0)
    {
      AddListener();
    }

    m_controllers.AddUnique(controller);
  }
}

void AiWorldListener::RemoveController(PlayerComponent *controller)
{
  VASSERT(controller);
  if(controller)
  {
    m_controllers.Remove(controller);

    if(m_controllers.Count() == 0)
    {
      RemoveListener();
    }
  }
}

void AiWorldListener::AddListener()
{
  vHavokAiModule::GetInstance()->GetAiWorld()->addListener(this);
}

void AiWorldListener::RemoveListener()
{
  vHavokAiModule::GetInstance()->GetAiWorld()->removeListener(this);
}

void AiWorldListener::postCharacterStepCallback(hkaiWorld::CharacterStepCallbackContext const& ctx)
{
  int const numControllers = m_controllers.Count();
  for(int i = 0; i < numControllers; i++)
  {

	  
	  PlayerComponent *const controller = m_controllers.GetAt(i);
     controller->OnAfterCharacterStep(ctx.m_timestep);
  }
}

hkBool32 LocalSteeringFilter::isCharacterEnabled(hkaiCharacter const *aiCharacter, hkaiCharacter const *otherAiCharacter) const
{
  PlayerComponent const *controller = (PlayerComponent const*)aiCharacter->m_userData;
 Player const *otherCharacter = ((PlayerComponent const*)otherAiCharacter->m_userData)->GetCharacter();

  // Don't steer around the character we are targetting
  return true;
	  //controller->GetTarget() == otherCharacter ? 0 : 1;
}

hkBool32 LocalSteeringFilter::isObstacleEnabled(hkaiCharacter const *aiCharacter, hkaiObstacleGenerator const *otherObstacle) const
{
  PlayerComponent const *controller = (PlayerComponent const*)aiCharacter->m_userData;


  // Don't steer around the character we are targetting
 return false;
	 
}

void PlayerComponent::SetSensorSize(float sensorSize)
{
  m_sensorSize = sensorSize;
}

void PlayerComponent::SetDesiredSpeed(float desiredSpeed)
{
  m_desiredSpeed = desiredSpeed;
}
void PlayerComponent::CalcDirection(hkvVec3& resultDir, hkvVec3 const& currentDir, hkvVec3 const& desiredDir, float t)
{
  if(currentDir.dot(desiredDir) < 0.99f)
  {
    float const dot = currentDir.dot(desiredDir);
    float const theta = hkvMath::acosRad(dot) * hkvMath::clamp(t, 0.f, 1.f);
    hkvVec3 const vec = (desiredDir - currentDir * dot).getNormalized();
    resultDir = currentDir * hkvMath::cosRad(theta) + vec * hkvMath::sinRad(theta);
  }
  else
  {
    resultDir = desiredDir;
  }
}
bool PlayerComponent::CalcFaceTowards(hkvVec3& resultDir, hkvVec3 const& fromDir, hkvVec3 const& fromPos, hkvVec3 const& toPos, float const t)
{
  hkvVec3 const up(0.0f, 0.0f, 1.0f);
  hkvVec3 const fromTo(toPos - fromPos);
  hkvVec3 fromToProjectedDir(fromTo - fromTo.dot(up) * up);
  if(fromToProjectedDir.getLengthAndNormalize() > 0.1f)
  {
    CalcDirection(resultDir, fromDir, fromToProjectedDir, t);
    return true;
  }
  return false;
}
void PlayerComponent::FaceTowards(PlayerComponent *controller, hkvVec3 const& toPos, float t)
{
  hkvVec3 dir;
  if(CalcFaceTowards(dir, controller->GetCharacter()->GetDirection(), controller->GetCharacter()->GetPosition(), toPos, t))    
    controller->GetCharacter()->SetDirection(dir);
}