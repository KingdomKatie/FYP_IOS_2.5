-- new script file
function OnAfterSceneLoaded(self)
  -- Create the input map that will check for left mouse click and get mouse position
  self.map = Input:CreateMap("InputMap")
  self.map:MapTrigger("X", "MOUSE", "CT_MOUSE_ABS_X")
  self.map:MapTrigger("Y", "MOUSE", "CT_MOUSE_ABS_Y")
end

function OnThink(self)
  -- declare local variables for x,y position
  local x = self.map:GetTrigger("X")
  local y = self.map:GetTrigger("Y")
  
  
  --Quick check to see if running on windows if so draw a cursor at mouse position
  if Application:GetPlatformName() == "WIN32DX9" then
    Debug.Draw:Line2D(x,y,x+10,y+5, Vision.V_RGBA_GREEN)
    Debug.Draw:Line2D(x,y,x+5,y+10, Vision.V_RGBA_GREEN)
    Debug.Draw:Line2D(x+10,y+5,x+5,y+10, Vision.V_RGBA_GREEN)
  end
end