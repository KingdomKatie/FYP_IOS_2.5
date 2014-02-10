/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

/// \file VisionHavokBehaviorWorldListener.h

#ifndef MY_PLUGIN_VISION_HAVOK_BEHAVIOR_WORLD_LISTENER_H__
#define MY_PLUGIN_VISION_HAVOK_BEHAVIOR_WORLD_LISTENER_H__

#include <Behavior/Behavior/World/hkbWorldListener.h>
#include <Behavior/Behavior/World/hkbWorld.h>

///
/// \brief
///    Class that implements the hkbWorldListener Interface which is responsible for Havok Behavior callbacks.
/// 
class MyHavokBehaviorWorldListener : public hkbWorldListener
{
public:
  MyHavokBehaviorWorldListener();

  ///
  /// @name vHavokBehaviorWorldListener Virtual Overrides
  /// @{
  ///

  /// hkbWorldListener interface.  Listens for behavior events and raises other behavior events based on the information.		
  virtual void eventRaisedCallback( hkbCharacter* character, const hkbEvent& event, bool raisedBySdk ) HK_OVERRIDE;

  ///
  /// @}
  ///

public:
  
  static MyHavokBehaviorWorldListener s_instance;
};

#endif // RPG_PLUGIN_VISION_HAVOK_BEHAVIOR_WORLD_LISTENER_H__

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
