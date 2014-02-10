#ifndef SAMPLEPLUGIN_PLAYER_H__
#define SAMPLEPLUGIN_PLAYER_H__

#include "GamePluginModule.h"
#include "PlayerControlComponent.h"
#include <Behavior/Behavior/Event/hkbEventQueue.h>
#include <Behavior/Behavior/World/hkbWorld.h>

#include <Vision/Runtime/EnginePlugins/Havok/HavokBehaviorEnginePlugin/vHavokBehaviorComponent.hpp>
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokCharacterController.hpp>
#include<Behavior/Behavior/Event/hkbEvent.h>

//Animation includes
#include <Vision/Runtime/Engine/Animation/VisApiAnimLayerMixerNode.hpp>
#include <Vision/Runtime/Engine/Animation/VisApiAnimNormalizeMixerNode.hpp>
#include <Vision/Runtime/Engine/Animation/VisApiAnimBoneModifierNode.hpp>

class PlayerComponent;
typedef VSmartPtr<PlayerComponent> PlayerComponentPtr;

class Player : public VisBaseEntity_cl
{
int iHealth; // health recovery points


public:
	//needs to be public for mobile 
	Player();
	 // @TODO: make abstract
  VType* GetControllerComponentType();

 virtual void CalcPositionForTargeting(hkvVec3& targetOut) const;

	
 
	// virtual functions inherited from VisBaseEntity_cl
  virtual void InitFunction() HKV_OVERRIDE;    // entity initialization
 // virtual void DeInitFunction() HKV_OVERRIDE;  // entity deinitialization
  virtual void ThinkFunction() HKV_OVERRIDE;   // entity ThinkFunction, called once per frame
  virtual void PostInitialize();
   float GetCollisionRadius() const;
   float GetCollisionHeight() const;

  

 


    // Public Accessor Functions
  VisAnimNormalizeMixerNode_cl *GetNormalizeMixerNode() const { return m_spNormalizeMixerNode; }
  VisAnimLayerMixerNode_cl *GetLayerMixerNode() const { return m_spLayerMixerNode; }
  VisSkeletalAnimControl_cl *GetSingleAnimControl() const { return m_spSingleAnimControl; }
  VisAnimBoneModifierNode_cl *GetBoneModifiedNode() const { return m_spBoneModifierNode; }
  int GetNeckBoneIndex() const { return m_iNeckBoneIndex; }

  
  //@{
  // AI
  void SetController(PlayerComponent *newController);
 PlayerComponent *GetController();
 PlayerComponent const *GetController() const;
  float GetAggroRadius() const;
  bool IsOnNavMesh(float const testRadius = 1.f) const;
  //@}

  
    // Animation

  void StartIdleAnimation(bool bLooped=false); 
  void StartWalkAnimation(bool bLooped=false);

  


  // VisTypedEngineObject_cl
  void MessageFunction(int iID, INT_PTR iParamA, INT_PTR iParamB ) HKV_OVERRIDE;



protected:
  // VisBaseEntity_cl
  //void Serialize(VArchive& ar) HKV_OVERRIDE;
	  float m_fHalfScreenSizeX;
      float m_fHalfScreenSizeY;
	  


  float m_sensorSize;
   float m_desiredSpeed;

private:

  V_DECLARE_SERIAL_DLLEXP(Player, SAMPLEPLUGIN_IMPEXP ) // RTTI (mandatory)
  V_DECLARE_VARTABLE( Player, SAMPLEPLUGIN_IMPEXP )      // Optional variable table

   hkvVec2 pCursor;
   hkvVec3 mPosition;
   hkvVec3 mTarget;
   float mCollisionRadius;
   float mCollisionHeight;
 // private data
  VSmartPtr<VisAnimNormalizeMixerNode_cl> m_spNormalizeMixerNode;
  VSmartPtr<VisAnimLayerMixerNode_cl> m_spLayerMixerNode;
  VSmartPtr<VisSkeletalAnimControl_cl> m_spSingleAnimControl;
  VSmartPtr<VisAnimBoneModifierNode_cl> m_spBoneModifierNode;
  int m_iHeadBoneIndex;
  int m_iNeckBoneIndex;
  int m_iMixerInputWalk, m_iMixerInputRun;

  VString m_sHighlightedBoneName;
  float m_fBoneHighlightDuration;
  int m_iAnimEventFootStepLeft;
  int m_iAnimEventFootStepRight;

   void InitInputMap();
  void SetCursorPosition();
  void GetTargetPosition();
  hkvVec2 GetCursorPosition();
   void SetPlayerPosition();
  hkvVec3 GetPlayerPosition();
   VSmartPtr<VInputMap> m_inputMap;   
  //set up for ios
  #if defined(SUPPORTS_MULTITOUCH)

  IVMultiTouchInput* m_touchInput;

#endif
	

};

#endif // RPG_PLUGIN_BASE_ENTITY_H__