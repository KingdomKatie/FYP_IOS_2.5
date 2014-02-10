-- new script file
local move = "foward"

--function OnSerialize(self, archieve)

 --if archieve:IsLoading() then

 --OnCreate(self)

-- end

--end

function OnThink(self)

  -- check if corner
  if self:GetPosition().x > 2400 and self:GetPosition().y > -4000 then
  move = "right"
  elseif self:GetPosition().y < -4000 then
  move ="stop"
  end
 
  -- move around...
  if move == "foward" then
    self:IncPosition(10,0,0)
 elseif move == "right" then
    self:IncPosition(0,-10,0)
  elseif move == "stop" then
    self:IncPosition(0,0,0)
   
 end
 
-- function OnObjectEnter(self)
 
 --self:SetVisible(false)
-- Debug:PrintLine("pick up", Vision.V_RGBA_RED)
 
 --end

end