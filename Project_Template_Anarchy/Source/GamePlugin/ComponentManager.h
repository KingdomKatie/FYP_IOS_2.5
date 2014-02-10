
#ifndef COMPONANTMANAGER_H_INCLUDED
#define COMPONANTMANAGER_H_INCLUDED

//#include "GamePluginModule.h"
//#include "PlayerControlComponent.h"
class ComponentManager : public IVisCallbackHandler_cl
{
public:
	
	 virtual  void OnHandleCallback(IVisCallbackDataObject_cl *pData) HKV_OVERRIDE;
    void OneTimeInit();
    void OneTimeDeInit();
    // holds the collection of all instances of MyComponent
      //VRefCountedCollection<PlayerComponent> m_Components;
    
    // access one global instance of the Game manager
    static ComponentManager& GlobalComponentManager() {return g_ComponentManager;}
     // one global instance of our manager
     static ComponentManager g_ComponentManager;
};
#endif