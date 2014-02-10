/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */


//  This class show how to use the vForge "play the Game" mode

#ifndef PLUGINMANAGER_H_INCLUDED
#define PLUGINMANAGER_H_INCLUDED

#include "GamePluginModule.h"
#include"Player.h"
#include "PlayerDialog.h"


class MyGameManager : public IVisCallbackHandler_cl
{
public:
  virtual void OnHandleCallback(IVisCallbackDataObject_cl *pData) HKV_OVERRIDE;

  // called when plugin is loaded/unloaded
  void OneTimeInit();
  void OneTimeDeInit();

  // switch to play-the-game mode. When not in vForge, this is default
  void SetPlayTheGame(bool bStatus);

    VisBaseEntity_cl* SpawnPlayer(const VString& prefabName, hkvVec3 const& position, hkvVec3 const& orientation);
    VisBaseEntity_cl* CreateEntityFromPrefab(const VString& prefabName, const hkvVec3& position, const hkvVec3& orientation);
    
    
    
    
  // access one global instance of the Game manager
  static MyGameManager& GlobalManager() {return g_GameManager;}

private:
  bool m_bPlayingTheGame;
  bool mStaticsSynced;
  static MyGameManager g_GameManager;
  VWeakPtr<VisBaseEntity_cl> m_playerEntity;
  hkvVec3 m_playerRespawnPosition;
  static VString const PLAYER_PREFAB_NAME;
  VSmartPtr<PlayerDialog> mDialog;
  VSmartPtr<PlayerComponent> mplayerController;
  // VSmartPtr<ComponentManager> mComponentManager;
  VSmartPtr<VGUIMainContext> spGUIContext;//gui Context
};


#endif // PLUGINMANAGER_H_INCLUDED

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
