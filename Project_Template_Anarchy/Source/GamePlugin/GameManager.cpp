/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#include "GamePluginPCH.h"
#include "GameManager.h"
#include "Player.h"

#include "VisionHavokBehaviorWorldListener.h"
#include <Vision/Runtime/EnginePlugins/EnginePluginsImport.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/Scene/VPrefab.hpp>
#include <Vision/Samples/Engine/HavokAi/HavokAiPCH.h>
#include <Vision/Samples/Engine/HavokAi/PathPickDlg.hpp>

#include <Vision/Runtime/Framework/VisionApp/VAppImpl.hpp>
#include <Vision/Runtime/Framework/VisionApp/Modules/VHelp.hpp>

#include <Vision/Runtime/EnginePlugins/EnginePluginsImport.hpp>

#include <Vision/Runtime/EnginePlugins/Havok/HavokAiEnginePlugin/vHavokAiModule.hpp>
#include <Ai/Pathfinding/World/hkaiWorld.h>
#include <Vision/Runtime/EnginePlugins/Havok/HavokAiEnginePlugin/vHavokAiNavMeshResource.hpp>
#include <Vision/Runtime/EnginePlugins/Havok/HavokAiEnginePlugin/vHavokAiNavMeshResourceManager.hpp>
#include <Vision/Runtime/EnginePlugins/Havok/HavokAiEnginePlugin/vHavokAiNavMeshInstance.hpp>

#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsModule.hpp>
#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokSync.hpp>

VString const MyGameManager::PLAYER_PREFAB_NAME = "Prefabs\\Demo_Player_Hero.vprefab";
// one global static instance
MyGameManager MyGameManager::g_GameManager;

void MyGameManager::OneTimeInit()
{
  m_bPlayingTheGame = false;

  // the game manager should listen to the following callbacks:
  Vision::Callbacks.OnEditorModeChanged += this;
  Vision::Callbacks.OnBeforeSceneLoaded += this;
  Vision::Callbacks.OnAfterSceneLoaded += this;
  Vision::Callbacks.OnUpdateSceneBegin += this;
  Vision::Callbacks.OnWorldDeInit += this;
}

void MyGameManager::OneTimeDeInit()

{
  Vision::Callbacks.OnEditorModeChanged -= this;
  Vision::Callbacks.OnBeforeSceneLoaded -= this;
  Vision::Callbacks.OnAfterSceneLoaded -= this;
  Vision::Callbacks.OnUpdateSceneBegin -= this;
  Vision::Callbacks.OnWorldDeInit -= this;
}

VisBaseEntity_cl* MyGameManager::SpawnPlayer(const VString& prefabName, hkvVec3 const& position, hkvVec3 const& orientation)
{
	
    VisBaseEntity_cl* playerEntity = CreateEntityFromPrefab(prefabName, position, orientation);
    playerEntity->InitFunction();
    
    if(playerEntity)
    {
        PlayerComponent *const playerController = static_cast<PlayerComponent*>
        (playerEntity->Components().GetComponentOfBaseType(V_RUNTIME_CLASS(PlayerComponent)));
        
        //VASSERT(playerController);
        if(playerController)
        {
            spGUIContext = new VGUIMainContext(NULL);
            // Create a GUI context
            mDialog = new PlayerDialog(playerController);
            mDialog->InitDialog(spGUIContext, NULL, NULL);
            spGUIContext->ShowDialog(mDialog);
            spGUIContext->SetActivate(true);
        }
        
        m_playerEntity = playerEntity->GetWeakReference();
        //Vision::Message.Add(1, "Spawn");
    }
    
    return playerEntity;
}

/// Creates an entity from a prefab file.
VisBaseEntity_cl* MyGameManager::CreateEntityFromPrefab(const VString& prefabName, const hkvVec3& position, const hkvVec3& orientation)
{
    if(!prefabName.IsEmpty())
    {
        VPrefab* prefab = VPrefabManager::GlobalManager().LoadPrefab(prefabName);
        
        VPrefabInstanceInfo info;
        {
            info.m_bOutputInstances = true;
            info.m_vInstancePos = position;
            info.m_vInstanceEuler = orientation;
        }
        
        prefab->Instantiate(info);
        
        VisBaseEntity_cl* entity = vstatic_cast<VisBaseEntity_cl*>(info.m_Instances[0]);
        
        return entity;
    }
    
    return NULL;
}
void MyGameManager::OnHandleCallback(IVisCallbackDataObject_cl *pData)
{

    //physics callbacks
	if (pData->m_pSender == &vHavokPhysicsModule::OnBeforeInitializePhysics)
    {
        vHavokPhysicsModuleCallbackData* pHavokData = static_cast<vHavokPhysicsModuleCallbackData*>(pData);
        VISION_HAVOK_SYNC_STATICS();
        VISION_HAVOK_SYNC_PER_THREAD_STATICS(pHavokData->GetHavokModule());
        // hkaiCharacter created by this dll will have a different vtable than the one expected by the AI module (for vdb viewer)
        // The vtable-class(?) mapping is apparently many-to-one, so re-register here with our vtable
        // (Actually the vtables in this module and the vtables elsewhere are unique, so being many-to-one is a happy accident)
        // TODO redundant in the statically linked case
        hkVtableClassRegistry::getInstance().registerList(hkBuiltinTypeRegistry::StaticLinkedTypeInfos, hkBuiltinTypeRegistry::StaticLinkedClasses);
        mStaticsSynced = true;
    }
    else if(pData->m_pSender == &vHavokPhysicsModule::OnBeforeWorldCreated)
    {
        vHavokPhysicsModule *const havok_module = static_cast<vHavokBeforeWorldCreateDataObject_cl *>(pData)->GetHavokModule();
        {
            havok_module->SetUseAsynchronousPhysics(false);
            
            havok_module->SetEnabledVisualDebugger(true);
        }
        
        // Disable validation checks (that are also performed in dev build)
        vHavokAiModule::GetInstance()->GetAiWorld()->getNavMeshCutter()->m_performValidationChecks = false;
    }
    else if (pData->m_pSender == &vHavokPhysicsModule::OnAfterDeInitializePhysics)
    {
        
        vHavokAiModule::GetInstance()->getCharacterBehaviors().clearAndDeallocate();
        vHavokPhysicsModuleCallbackData* pHavokData = static_cast<vHavokPhysicsModuleCallbackData*>(pData);
        VISION_HAVOK_UNSYNC_STATICS();
        VISION_HAVOK_UNSYNC_PER_THREAD_STATICS(pHavokData->GetHavokModule());
        mStaticsSynced = false;
        
    }
    
  if (pData->m_pSender==&Vision::Callbacks.OnUpdateSceneBegin)
  {
      //This callback will be triggered at the beginning of every frame
      //You can add your own per frame logic here
      // [...]
      if (m_bPlayingTheGame)
      {
       // Vision::Message.Print(1, 200, 100, "The game is running");
      }
      return;
  }
    if(pData->m_pSender==&Vision::Callbacks.OnWorldInit)
    {
        vHavokPhysicsModule* pPhysicsModule = vHavokPhysicsModule::GetInstance();
        if (pPhysicsModule != NULL && !mStaticsSynced)
        {
            // Havok Physics Plugin has already been initialized.
            // Reinitialize the physics integration to be able to synchronize static variables properly.
            pPhysicsModule->OnDeInitPhysics();
            pPhysicsModule->OnInitPhysics();
        }
    }

  if (pData->m_pSender==&Vision::Callbacks.OnEditorModeChanged)
  {
    // when vForge switches back from EDITORMODE_PLAYING_IN_GAME, turn off our play the game mode
    if (((VisEditorModeChangedDataObject_cl *)pData)->m_eNewMode != VisEditorManager_cl::EDITORMODE_PLAYING_IN_GAME)
      SetPlayTheGame(false);
    return;
  }

  if (pData->m_pSender==&Vision::Callbacks.OnBeforeSceneLoaded)
  { // Add our own Havok Behavior world listener in order to listen to Behavior events
      vHavokBehaviorModule *const havok_behavior_module = vHavokBehaviorModule::GetInstance();
      havok_behavior_module->getBehaviorWorld()->addListener(&MyHavokBehaviorWorldListener::s_instance);
      return;
  }

  if (pData->m_pSender==&Vision::Callbacks.OnAfterSceneLoaded)
  {
      
	  //spawn player charecter from prefab
     SpawnPlayer(PLAYER_PREFAB_NAME,hkvVec3(100,-400,30) , hkvVec3::ZeroVector());
    //gets triggered when the play-the-game vForge is started or outside vForge after loading the scene
    if (Vision::Editor.IsPlayingTheGame()) 
      SetPlayTheGame(true);
    return;
  }

  if (pData->m_pSender==&Vision::Callbacks.OnWorldDeInit) 
  {
     // this is important when running outside vForge
    SetPlayTheGame(false);
    return;
  }
    if (pData->m_pSender==&Vision::Callbacks.OnBeforeSceneUnloaded)
    {
        
        return;
    }
    if (pData->m_pSender==&Vision::Callbacks.OnAfterSceneUnloaded)
    {
        // Only try to remove the listener if a scene was actually loaded
        vHavokBehaviorModule *const havok_behavior_module = vHavokBehaviorModule::GetInstance();
        havok_behavior_module->getBehaviorWorld()->removeListener(&MyHavokBehaviorWorldListener::s_instance);
        return;
    }
}


// switch to play-the-game mode
void MyGameManager::SetPlayTheGame(bool bStatus)
{
  if (m_bPlayingTheGame==bStatus)
    return;

  m_bPlayingTheGame = bStatus;

  if (m_bPlayingTheGame)
  {
     // Play the game mode is started
     Vision::Message.Add(1, "Play the game mode has been started");
  } 
  else
  {
    //the play the game mode has been stopped.
    //clean up all your game specific instances, like e.g. particle effects
    //VisParticleGroupManager_cl::GlobalManager().Instances().Purge();
  }
}

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
