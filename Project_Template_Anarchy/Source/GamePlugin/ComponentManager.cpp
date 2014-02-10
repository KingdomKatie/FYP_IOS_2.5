#include "GamePluginPCH.h"
//#include "PlayerControlComponent.h"
#include "ComponentManager.h"

// handles all active instances of MyComponent
///class MyComponentManager : public IVisCallbackHandler_cl
//{
//public:

 // one global static instance
 ComponentManager ComponentManager::g_ComponentManager;
	 

   // should be called at plugin initialization time
  void ComponentManager::OneTimeInit()
  {
    //Vision::Callbacks.OnUpdateSceneFinished += this; // listen to this callback
  }

  // should be called at plugin de-initialization time
  void ComponentManager::OneTimeDeInit()
  {
    //Vision::Callbacks.OnUpdateSceneFinished -= this; // de-register
  }

  // implements IVisCallbackHandler_cl
  void ComponentManager::OnHandleCallback(IVisCallbackDataObject_cl *pData)
  {
    //VASSERT(pData->m_pSender==&Vision::Callbacks.OnUpdateSceneFinished);

    // call update function on every component
    //const int iCount = m_Components.Count();
    //for (int i=0;i<iCount;i++)
     //m_Components.GetAt(i)->PerFrameUpdate();
  }

  
//};
