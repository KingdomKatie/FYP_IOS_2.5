

#include "GamePluginPCH.h"
#include"Player.h"
#include"PluginMain.h"
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokCharacterController.hpp>
#include <Vision/Samples/Engine/HavokAi/HavokAiPCH.h>
#include <Vision/Samples/Engine/HavokAi/PathPickDlg.hpp>
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokConversionUtils.hpp>

#include <Vision/Runtime/EnginePlugins/Havok/HavokAiEnginePlugin/vHavokAiModule.hpp>
#include <Ai/Pathfinding/World/hkaiWorld.h>
#include <Vision/Runtime/EnginePlugins/Havok/HavokAiEnginePlugin/vHavokAiNavMeshResource.hpp>




V_IMPLEMENT_SERIAL( Player, VisBaseEntity_cl, 0,&g_myComponentModule);

// Variable table
START_VAR_TABLE(Player,VisBaseEntity_cl,"Player Class",0,"")
DEFINE_VAR_INT(Player, iHealth, "Health recovery percentage", "50", 0, 0);
 // add more parameters here
END_VAR_TABLE

Player::Player()
   :m_spNormalizeMixerNode(NULL)
  , m_spLayerMixerNode(NULL)
  , m_spSingleAnimControl(NULL)
  , m_spBoneModifierNode(NULL)
  , m_iHeadBoneIndex(-1)
  , m_iNeckBoneIndex(-1)
  , m_iMixerInputWalk(-1), m_iMixerInputRun(-1)
  , m_sHighlightedBoneName()
  , m_fBoneHighlightDuration(0.0f)
  , m_iAnimEventFootStepLeft(-1)
  , m_iAnimEventFootStepRight(-1)
{
	mCollisionRadius = 40.0f;
    mCollisionHeight = 160.0f;
    m_sensorSize =128.0f;
    m_desiredSpeed =200.0f;

}

void Player::CalcPositionForTargeting(hkvVec3& targetOut) const
{
  //targetOut = 0.5f * (GetEyePosition() + GetPosition());
}

void Player::InitFunction()
{

  m_iAnimEventFootStepLeft = Vision::Animations.RegisterEvent("footstep_left");
  m_iAnimEventFootStepRight = Vision::Animations.RegisterEvent("footstep_right");

  IncOrientation(-90.0f, 0.0f, 0.0f);

  //StartIdleAnimation(true);



}
void Player::InitInputMap()
{
		
}
void Player::ThinkFunction()
{
	
	VisBaseEntity_cl::DrawBoundingBox(false,VColorRef(255, 255, 255),1.0f);
	
	
	

  
}
void Player::PostInitialize()
{
}


void Player::MessageFunction(int id, INT_PTR paramA, INT_PTR paramB)
{
  VisBaseEntity_cl::MessageFunction(id, paramA, paramB);
 
 /*  switch(id)
  {
  case VisionUserMessages::kHavokAnimationEvent:
    OnHavokAnimationEvent(*(hkbEvent*)(paramA), paramB != 0);
    break;
  }*/
 
}

hkvVec2 Player::GetCursorPosition()
{
	

	return pCursor;

}
void Player::SetCursorPosition()
{
   // pCursor = hkvVec2(Vision::Mouse.GetX(), Vision::Mouse.GetY());

}

hkvVec3 Player::GetPlayerPosition()
{
	

	return mPosition;

}
void Player::SetPlayerPosition()
{
    mPosition =  hkvVec3(GetPosition());

}
void Player::GetTargetPosition()
{
}
float Player::GetCollisionRadius() const
{
  return mCollisionRadius;
}

float Player::GetCollisionHeight() const
{
  return mCollisionHeight;
}
VType* Player::GetControllerComponentType()
{
  return V_RUNTIME_CLASS(PlayerComponent);
}
void Player::StartIdleAnimation(bool bLooped) 
{
  // start a simple skeletal animation
  int iFlags = VSKELANIMCTRL_DEFAULTS;
  if (bLooped)
    iFlags |= VANIMCTRL_LOOP;
  m_spSingleAnimControl = VisAnimConfig_cl::StartSkeletalAnimation(this, "Idle", iFlags);
}
void Player::StartWalkAnimation(bool bLooped) 
{
  // start a simple skeletal animation
  int iFlags = VSKELANIMCTRL_DEFAULTS;
  if (bLooped)
    iFlags |= VANIMCTRL_LOOP;
  m_spSingleAnimControl = VisAnimConfig_cl::StartSkeletalAnimation(this, "Walk", iFlags);
}

