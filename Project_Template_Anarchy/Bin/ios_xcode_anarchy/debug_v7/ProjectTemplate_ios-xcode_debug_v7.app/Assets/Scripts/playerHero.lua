
g_useRemoteInput = false
g_touchInput = (Application:GetPlatformName() == "ANDROID") or (Application:GetPlatformName() == "IOS")


g_gcScreenOffsetY = 0.3
g_gcAutomationUpdateRate = 40 
g_gcAutomationAngleLimit = 0.4
g_gcAutomationPadThreshold = 0.2
g_gcAutomationWalkingMaxTurnSpeed = 1.0
g_gcAutomationRunningMaxTurnSpeed = 2.5
g_gcAutomationRunningThresholdSquared = math.pow(0.4, 2)


function OnAfterSceneLoaded(self)

	-- Setup input map
	self.map = Input:CreateMap("InputMap")
	if g_touchInput or g_useRemoteInput then
		local w, h = Screen:GetViewportSize()
		self.map:MapTrigger("X", {0, 0, w, h}, "CT_TOUCH_ABS_X")
		self.map:MapTrigger("Y", {0, 0, w, h}, "CT_TOUCH_ABS_Y")
		self.map:MapTrigger("Pick", {0, 0, w, h}, "CT_TOUCH_ANY", {once = false}) 
	else
		self.map:MapTrigger("X", "MOUSE", "CT_MOUSE_ABS_X")
		self.map:MapTrigger("Y", "MOUSE", "CT_MOUSE_ABS_Y")
    self.map:MapTrigger("Pick", "MOUSE", "CT_MOUSE_LEFT_BUTTON", {once = false})
end

	-- Setup Remote Input
	if g_useRemoteInput and not g_touchInput then
		Debug:PrintLine("Using Remote Input...")
		RemoteInput:StartServer('RemoteGui')
		RemoteInput:InitEmulatedDevices()
		RemoteInput:DebugDrawTouchPoints(Vision.VColorRef(255,0,0))
	end

	-- Setup Behavior component
	--local behaviorComponent = self:GetComponentOfType("vHavokBehaviorComponent")
  --  if behaviorComponent ~= nil then
	--	behaviorComponent:SetFloatVar("AutomationUpdateRate", g_gcAutomationUpdateRate)
	--	behaviorComponent:SetFloatVar("AutomationAngleLimit", g_gcAutomationAngleLimit)
	--	behaviorComponent:SetFloatVar("AutomationWalkingMaxTurnSpeed", g_gcAutomationWalkingMaxTurnSpeed)
	--	behaviorComponent:SetFloatVar("AutomationRunningMaxTurnSpeed", g_gcAutomationRunningMaxTurnSpeed)
	--end

end

function OnBeforeSceneUnloaded(self)

	if g_useRemoteInput and not g_touchInput then
		RemoteInput:StopServer()
	end

    Input:DestroyMap(self.map);

end

function OnThink(self)

   -- local behaviorComponent = self:GetComponentOfType("vHavokBehaviorComponent")
   -- if behaviorComponent == nil then
		--return
	--end
  
  --Draw position of mouse/touch
  local cursorX = self.map:GetTrigger("X")
	local cursorY = self.map:GetTrigger("Y")
	local cursorSize = 0
	if self.map:GetTrigger("TOUCH_ANY") ~= 0 then
		cursorSize = 10
	elseif not g_useRemoteInput then
		cursorSize = 12
	end
	if cursorSize ~= 0 then
		Debug.Draw:Line2D( cursorX-cursorSize, cursorY, cursorX+cursorSize, cursorY )
		Debug.Draw:Line2D( cursorX, cursorY-cursorSize, cursorX, cursorY+cursorSize )
	end
  
 
  --
  
  --self:SetPosition(cursorX,cursorY,-3.05)
  --if (self.picked
   if self.map:GetTrigger("Pick")>0 then
   
    Debug:PrintLine("Click", Vision.V_RGBA_RED)
    --self:SetPosition(500,200,0)
    
    self:SetOrientation(4,0,0)
   end

	




	-- Normalize the touch input to [-1, 1] with y == -1 being forward
	--local normX = 0
	--local normY = 0
	--if self.map:GetTrigger("TOUCH_ANY") ~= 0 then
	--	normX, normY = GetJoystickCoords( behaviorComponent, self.map:GetTrigger("X"), self.map:GetTrigger("Y") )
	--end

	
	--local leftTrigger = 0.1
	--local lengthSquared = normX*normX + normY*normY
	--if lengthSquared < g_gcAutomationRunningThresholdSquared then
	--	leftTrigger = 0.8
	--end
	--behaviorComponent:SetFloatVar("LeftTrigger", leftTrigger)
	--behaviorComponent:SetBoolVar("UseAutomation", lengthSquared > g_gcAutomationPadThreshold)
	

	-- The actual input to the graph is always maxed out to avoid behavior script tolerances from kicking in
	--behaviorComponent:SetFloatVar("LeftStickX", normX * 6.0)
--	behaviorComponent:SetFloatVar("LeftStickY", normY * 6.0)
	--behaviorComponent:SetFloatVar("RightStickX", 0)
	--behaviorComponent:SetFloatVar("RightStickY", 0)

end

function OnKeyPressed(self)

--self:SetPosition(cursorX,cursorY,-3.05)
 Debug:PrintLine("Click", Vision.V_RGBA_RED)

  
end

---function GetJoystickCoords(behaviorComponent, screenX, screenY)

--	local width, height = Screen:GetViewportSize()

	--local halfScreenX = width / 2
	--local halfScreenY = height / 2

	--local offsetScreenNormX = screenX - halfScreenX
	--local offsetScreenNormY = (height - screenY) - halfScreenY - (halfScreenY * -g_gcScreenOffsetY)

	--local adjustedScreenX = offsetScreenNormX / halfScreenX
	--local adjustedScreenY = offsetScreenNormY / halfScreenY

	--local normX
	--if adjustedScreenX >= 0 then
	--	normX = math.min( 1, adjustedScreenX )
	--else
	--	normX = math.max( -1, adjustedScreenX )
	--end

	--local normY
	--if adjustedScreenY >= 0 then
	--	normY = math.min( 1, adjustedScreenY )
	--else
	--	if behaviorComponent:IsNodeActive("Take Cover Behavior") then
		--	normY = math.max( -1, adjustedScreenY )
		--else
		--	normY = math.max( 0, adjustedScreenY )
		--end
	--end

	

	--return normX, -normY]
 -- end

---- new script file
