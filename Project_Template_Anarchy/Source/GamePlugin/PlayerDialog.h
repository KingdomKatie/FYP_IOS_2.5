
#ifndef PLAYERDIALOG_H__
#define PLAYERDIALOG_H__

#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/VMenuIncludes.hpp>
#include <Vision/Samples/Engine/HavokAi/HavokAiPCH.h>
#include <Vision/Samples/Engine/HavokAi/PathPickDlg.hpp>
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokConversionUtils.hpp>

#include <Vision/Runtime/EnginePlugins/Havok/HavokAiEnginePlugin/vHavokAiModule.hpp>
#include <Ai/Pathfinding/World/hkaiWorld.h>
#include <Vision/Runtime/EnginePlugins/Havok/HavokAiEnginePlugin/vHavokAiNavMeshResource.hpp>
#include "GamePluginModule.h"

#include "PlayerControlComponent.h"

class Player;

class PlayerComponent;
typedef VSmartPtr<PlayerComponent> PlayerComponentPtr;

enum PlayerInputs_e
{
  PI_Invalid = -1,
  PI_TargetDest =0,
  PI_PrimaryActionX,
  PI_PrimaryActionY,
};
class PlayerDialog : public VDialog
{

public:
  PlayerDialog(PlayerComponent *playerController);

  VInputMap *GetInputMap() { return m_inputMap; }
 

private:
  //@{
  // VDialog
   void OnActivate() HKV_OVERRIDE;
  void OnDeactivate() HKV_OVERRIDE;
  void OnSetFocus(bool focus) HKV_OVERRIDE;
  void OnTick(float deltaTime) HKV_OVERRIDE;
  // Input handling
  bool GetClosestPointOnNavMeshUnderCursor(hkVector4& point, hkVector4 const& searchPoint);
  void InitInputMap();
  hkvVec2 GetCursorPosition(IVGUIContext const *context) const;
  VSmartPtr<VInputMap> m_inputMap;
  PlayerComponentPtr mController;
  VCursorPtr m_cursor;

#if defined(SUPPORTS_MULTITOUCH)

  IVMultiTouchInput* m_touchInput;

#endif

};

#endif