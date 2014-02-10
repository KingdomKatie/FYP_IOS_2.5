
#include "GamePluginPCH.h"
#include "PlayerDialog.h"


PlayerDialog::PlayerDialog(PlayerComponent *playerController)
	:VDialog(),
	mController(playerController),
	m_inputMap(NULL),
	m_cursor()
{
	Vision::Message.Add(1, "constructor");
}
 void PlayerDialog::InitInputMap()
{
	  #ifdef SUPPORTS_MOUSE
      m_inputMap->MapTrigger(PI_TargetDest, V_MOUSE, CT_MOUSE_LEFT_BUTTON);
     #endif
	  //if ios
	#if defined(_VISION_MOBILE)
	
		 float const priority = -950.0f;
		 int const width = Vision::Video.GetXRes();
		 int const height = Vision::Video.GetYRes();

		 VTouchArea *touchScreen = new VTouchArea(VInputManager::GetTouchScreen(), VRectanglef(0.0f, 0.0f, width, height), priority);
		 m_inputMap->MapTrigger(PI_TargetDest, touchScreen, CT_TOUCH_ANY);
		 m_inputMap->MapTrigger(PI_PrimaryActionX, touchScreen, CT_TOUCH_ABS_X);
		 m_inputMap->MapTrigger(PI_PrimaryActionY, touchScreen, CT_TOUCH_ABS_Y);
	#endif
}
void PlayerDialog::OnActivate()
{
  VDialog::OnActivate();
  Vision::Message.Add(1, "activate");
  m_inputMap = new VInputMap(64,20);
  InitInputMap();
}
void PlayerDialog::OnDeactivate()
{
	VDialog::OnDeactivate();
    m_inputMap = NULL;
}
void PlayerDialog::OnSetFocus(bool focus)
{
  VDialog::OnSetFocus(focus);
#if defined(_VISION_MOBILE)
  //m_cursor = GetMenuManager()->LoadCursorResource("GUI/Textures/MouseMobile.tga");
#else
  //VASSERT_MSG(!m_cursorTextureFilename.IsEmpty(), "Cursor texture filename hasn't been specified.");
  //m_cursor = GetMenuManager()->LoadCursorResource(m_cursorTextureFilename.AsChar());
  //VASSERT_MSG(m_cursor, "Cursor failed to load.");
#endif

#if defined(SUPPORTS_MULTITOUCH) && (HAVOK_VISION_RESTRUCTURING) && !defined(_VISION_ANDROID)

  m_touchInput = NULL;

  IVInputDevice* inputDevice = &VInputManagerPC::GetInputDevice(INPUT_DEVICE_TOUCHSCREEN);
  if(inputDevice != &VInputManagerPC::s_NoInputDevice)
    m_touchInput = static_cast<IVMultiTouchInput*>(inputDevice);

#endif
}
bool PlayerDialog::GetClosestPointOnNavMeshUnderCursor(hkVector4& point, hkVector4 const& searchPoint)
{
	 IVGUIContext *const context = GetContext();
  VASSERT(context);
  hkvVec2 const mousePos = GetCursorPosition(context);

  hkvVec3 traceDirOut;
  VisRenderContext_cl::GetCurrentContext()->GetTraceDirFromScreenPos(mousePos.x, mousePos.y, traceDirOut, 1.0f);
  hkvVec3 const& camPos = VisRenderContext_cl::GetCurrentContext()->GetCamera()->GetPosition();

  hkVector4 rayStartHavok, rayEndHavok;
  vHavokConversionUtils::VisVecToPhysVecLocal(camPos, rayStartHavok);
  vHavokConversionUtils::VisVecToPhysVecLocal(camPos + traceDirOut * 5000.0f, rayEndHavok);

  hkaiNavMeshQueryMediator::HitDetails hit;
  if(vHavokAiModule::GetInstance()->GetAiWorld()->getDynamicQueryMediator()->castRay(rayStartHavok, rayEndHavok, hit))
  {
    point.setInterpolate(rayStartHavok, rayEndHavok, hit.m_hitFraction);
	//Vision::Message.Add(1, "hit");
    return true;
	
  }
  else
  {
	  //Vision::Message.Add(1, "miss");
    // If the ray missed the nav mesh:
    // 1. Find the nav mesh face closest to the search point
    // 2. Intersect the ray with the plane of this face
    // 3. Find the closest point on the nav mesh to the plane point
    hkVector4 dummyPoint;
    hkaiPackedKey const searchFaceKey = 
      vHavokAiModule::GetInstance()->GetAiWorld()->getDynamicQueryMediator()->getClosestPoint(searchPoint, 1.f, dummyPoint);
    if(searchFaceKey != HKAI_INVALID_PACKED_KEY)
    {
      hkVector4 facePlane;
      hkaiNavMeshInstance const *meshInstance = vHavokAiModule::GetInstance()->GetAiWorld()->getStreamingCollection()->getInstanceAt(hkaiGetRuntimeIdFromPacked(searchFaceKey));
      {
        hkaiNavMeshUtils::calcFacePlane(*meshInstance, hkaiGetIndexFromPacked(searchFaceKey), facePlane);
      }

      hkSimdReal const f = facePlane.dot4xyz1(rayStartHavok);
      hkSimdReal const t = facePlane.dot4xyz1(rayEndHavok);
      if(f.isGreaterEqualZero() && t.isLessZero())
      {
        hkVector4 planePoint;
        {
          hkSimdReal const hitFraction = f / (f - t);
          planePoint.setInterpolate(rayStartHavok, rayEndHavok, hitFraction);
        }

        hkaiPackedKey faceKey = vHavokAiModule::GetInstance()->GetAiWorld()->getDynamicQueryMediator()->getClosestPoint(planePoint, 5.f, point);
        return (faceKey != HKAI_INVALID_PACKED_KEY);
      }
    }
  }

  return false;
}

hkvVec2 PlayerDialog::GetCursorPosition(IVGUIContext const *context)const
{
  #if defined(_VISION_MOBILE)
     float const touchX = m_inputMap->GetTrigger(PI_PrimaryActionX);
     float const touchY = m_inputMap->GetTrigger(PI_PrimaryActionY);
     return hkvVec2(touchX, touchY);
  #else
     return context->GetCurrentMousePos();
  #endif
}
void PlayerDialog::OnTick(float deltaTime)
{
  VDialog::OnTick(deltaTime);

   Player *characterEntity = static_cast<Player *>(mController->GetOwner());
  if (!characterEntity)
  {
    // @todo: handle player death properly
    hkvLog::Warning("Player character has probably died, which isn't yet handled. Set the character's Unlimited Health entity property to true to prevent this.");
    return;
  }
   if(m_inputMap->GetTrigger(PI_TargetDest) > 0.0f)
    {	
	
		hkVector4 hitPoint, characterPos;
		hkvVec3 mInput;
		hkvVec3 PlayerPos = characterEntity->GetPosition();
	    vHavokConversionUtils::VisVecToPhysVecLocal(characterEntity->GetPosition(), characterPos);
		 bool isonMesh = false;
		  if(GetClosestPointOnNavMeshUnderCursor(hitPoint,characterPos))
		  {
			  //vec4 to vec3
			   vHavokConversionUtils::PhysVecToVisVecLocal(hitPoint,mInput);
			    vHavokAiModule::GetInstance()->DebugRender(0.1f, false);
			    IVRenderInterface *pRI = Vision::Contexts.GetMainRenderContext()->GetRenderInterface();
				vHavokAiModule::GetInstance()->ComputeAndDrawPath(pRI, &PlayerPos,&mInput, 1.0f, 0.75f, 0.2f, hkColor::YELLOW);

				 mController->RequestPath(mInput);
				 hkvVec3 dir;
			     mController->FaceTowards(mController,mInput,0.25f);
				 float distance = mController->GetPathGoalReachedTolerance();
				  
                 hkvVec3 const targetPointToCharacter(mController->GetCharacter()->GetPosition() - mInput);
                 float const targetPointToCharactedDistSqr = targetPointToCharacter.getLengthSquared();
                 float const pathGoaltoleranceSqr = mController->GetPathGoalReachedTolerance() * mController->GetPathGoalReachedTolerance();

	//	if(targetPointToCharactedDistSqr <= pathGoaltoleranceSqr)
      //   {
			// Vision::Message.Add(1, "at point");
			// mController->RequestPath(mInput);
		// }
		//else
			//mController->RequestPath(mInput);
		 }
		  
   }

}
