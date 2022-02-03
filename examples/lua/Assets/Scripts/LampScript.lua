-- Copyright 2022 Matteo Grasso
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.


local LampScript =
{
	Properties = {}
}

function LampScript:OnActivate()
	self.maxLightIntensity = AreaLightRequestBus.Event.GetIntensity(self.entityId)

	self:AskForCurrentDayPeriod()

	-- -------------------------------------
	-- [INSTRUCTIONS]: #1
	self.dayNightCycleNotificationsHandler = DayNightCycleNotificationBus.Connect(self)
	-- -------------------------------------
end

function LampScript:OnDeactivate()
	-- -------------------------------------
	-- [INSTRUCTIONS]: #2
	self.dayNightCycleNotificationsHandler = nil
	-- -------------------------------------
end

-- -------------------------------------
-- [INSTRUCTIONS]: #3

function LampScript:OnDayStarted()
	self:TurnOffLight()
end

function LampScript:OnNightStarted()
	self:TurnOnLight()
end

-- -------------------------------------

function LampScript:AskForCurrentDayPeriod()
	local dayPeriod = DayNightCycleRequestBus.Broadcast.GetCurrentDayPeriod()

	if dayPeriod == 0 then
		self:OnDayStarted()
	elseif dayPeriod == 1 then
		self:OnNightStarted()
	end
end

function LampScript:TurnOnLight()
	self:SetLightIntensity(self.maxLightIntensity)
end

function LampScript:TurnOffLight()
	self:SetLightIntensity(0.0)
end

function LampScript:SetLightIntensity(intensity)
	AreaLightRequestBus.Event.SetIntensity(self.entityId, intensity)
end

return LampScript
