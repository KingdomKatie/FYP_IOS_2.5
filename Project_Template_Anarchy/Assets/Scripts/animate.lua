function OnThink(self)
  local behavior = self:GetComponentOfType("vHavokBehaviorComponent")
  if (behavior ~= nil) then
    --if (Input:IsKeyPressed(Vision.KEY_E)) then
       behavior:TriggerEvent("Move")
    --end
  end
end-- new script file
