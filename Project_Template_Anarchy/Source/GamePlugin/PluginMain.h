#ifndef MY_PLUGIN_PLUGIN_H__
#define MY_PLUGIN_PLUGIN_H__

#include "myComponent.h"
#include "GameManager.h"
#include "ComponentManager.h"
#include <Common/Base/KeyCode.h>
//============================================================================================================
//  Set up the Plugin Class
//============================================================================================================
class myPlugin_cl : public IVisPlugin_cl
{
public:

  void OnInitEnginePlugin();    
  void OnDeInitEnginePlugin();  

  const char *GetPluginName()
  {
    return "GamePlugin";  // must match DLL name
  }
};

// Plugin user messages
namespace VisionUserMessages
{
  enum Enum
  {
    // General
    kFirstMessage = VIS_MSG_USER,

    // Animation
    kHavokAnimationEvent,
  };
}

// use plugins if supported
VIMPORT IVisPlugin_cl* GetEnginePlugin_vFmodEnginePlugin();
#if defined( HAVOK_PHYSICS_2012_KEYCODE )
VIMPORT IVisPlugin_cl* GetEnginePlugin_vHavok();
#endif
#if defined( HAVOK_AI_KEYCODE )
VIMPORT IVisPlugin_cl* GetEnginePlugin_vHavokAi();
#endif
#if defined( HAVOK_BEHAVIOR_KEYCODE )
VIMPORT IVisPlugin_cl* GetEnginePlugin_vHavokBehavior();
#endif


#endif