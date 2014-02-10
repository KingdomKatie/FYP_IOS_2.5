-- Script for the player character

function GetEntityClassType(self)
  return "RPG_PlayerCharacter";
end

function OnInitProperties(self)  
  -- Entity Properties
  self:SetProperty("m_collisionRadius", 40);
  self:SetProperty("m_collisionHeight", 160);
  self:SetProperty("m_sensorSize", 256);
  self:SetProperty("m_desiredSpeed", 350);

  self:SetProperty("m_equipmentAttachmentBoneNames[ES_Weapon]", "right_attachment_jnt");
  self:SetProperty("m_equipmentAttachmentBoneNames[ES_Shoulder]", "left_shouderPad_jnt");
  
  self:SetProperty("m_defaultEquipmentScripts[ES_Weapon]", "Scripts/Pickups/RPG_Barbarian_Axe.lua");
  --self:SetProperty("m_defaultEquipmentScripts[ES_Shoulder]", "Scripts/Pickups/RPG_Barbarian_Shoulder.lua");

  self:SetProperty("m_actionData.m_spawnAnimationDelay", 1.4);

  -- base character stats
  self:SetProperty("m_stats.m_strength", 16);
  self:SetProperty("m_stats.m_dexterity", 12);
  self:SetProperty("m_stats.m_spirit", 10);
  self:SetProperty("m_stats.m_vitality", 12);

  -- health
  self:SetProperty("m_stats.m_baseHealth", 200);
  self:SetProperty("m_stats.m_healthRegenerationEnabled", true);
  self:SetProperty("m_stats.m_healthRegenerationRate", 0.8);
  self:SetProperty("m_stats.m_unlimitedHealth", false);
  self:SetProperty("m_displayHealth", true);

  -- mana
  self:SetProperty("m_stats.m_baseMana", 30);
  self:SetProperty("m_stats.m_manaRegenerationEnabled", true);
  self:SetProperty("m_stats.m_manaRegenerationRate", 0.8);

  -- attacks
  self:SetProperty("m_stats.m_baseDamage", 60);

  self:SetProperty("m_actionData.m_aoeAttackDamageMultiplier", 3);
  self:SetProperty("m_actionData.m_aoeAttackRange", 250);
  self:SetProperty("m_actionData.m_aoeAttackManaCost", 14);
  
  self:SetProperty("m_actionData.m_powerAttackDamageMultiplier", 2);
  self:SetProperty("m_actionData.m_powerAttackRange", 750);
  self:SetProperty("m_actionData.m_powerAttackAngle", 10);
  self:SetProperty("m_actionData.m_powerAttackManaCost", 8);
  
  self:SetProperty("m_actionData.m_rangedAttackImpactSpeedMultiplier", 1);
  self:SetProperty("m_actionData.m_aoeAttackImpactSpeedMultiplier", 1.5);
  self:SetProperty("m_actionData.m_powerAttackImpactSpeedMultiplier", 2);
  self:SetProperty("m_actionData.m_meleeAttackImpactSpeedMultiplier", 1);

  -- debugging
  self:SetProperty("m_debugDisplayStats", false);
  self:SetProperty("m_debugDisplayMovementInfo", false);
  self:SetProperty("m_debugDisplayInventory", false);
  self:SetProperty("m_debugDisplayBehaviorEvents", false);
  self:SetProperty("m_debugDisplayNavmeshStatus", false);
  self:SetProperty("m_debugDisplayEffectInformation", false);

end
