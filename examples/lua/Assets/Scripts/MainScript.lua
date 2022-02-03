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

local MainScript =
{
	Properties = {}
}

function MainScript:OnActivate()
	self.canvasRefNotificationsHandler = UiCanvasRefNotificationBus.Connect(self, self.entityId)

	local mainCanvasId = UiCanvasRefBus.Event.GetCanvas(self.entityId)
	self:FindAllUiElements(mainCanvasId)

	self:AskForCurrentDayPeriod()

	self.dayNightCycleNotificationsHandler = DayNightCycleNotificationBus.Connect(self)
end

function MainScript:OnDeactivate()
	self.dayNightCycleNotificationsHandler = nil
	self.canvasRefNotificationsHandler = nil
end

function MainScript:OnCanvasRefChanged(uiCanvasRefEntity, uiCanvasEntity)
	self:FindAllUiElements(uiCanvasEntity)
end

function MainScript:OnDayStarted()
	self:EditTextUiElement(self.dayPeriodTextId, "Day")
end

function MainScript:OnNightStarted()
	self:EditTextUiElement(self.dayPeriodTextId, "Night")
end

function MainScript:AskForCurrentDayPeriod()
	local dayPeriod = DayNightCycleRequestBus.Broadcast.GetCurrentDayPeriod()

	if dayPeriod == 0 then
		self:OnDayStarted()
	elseif dayPeriod == 1 then
		self:OnNightStarted()
	end
end

function MainScript:FindAllUiElements(mainCanvasId)
	if (mainCanvasId == self.mainCanvasId) or not mainCanvasId:IsValid() then
		return
	end

	self.mainCanvasId = mainCanvasId

	self.dayPeriodTextId = self:FindUiElement("DayPeriodValue")
end

function MainScript:FindUiElement(elementName)
	local elementId = UiCanvasBus.Event.FindElementByName(self.mainCanvasId, elementName)

	return elementId
end

function MainScript:EditTextUiElement(textElementId, textValue)
	UiTextBus.Event.SetText(textElementId, textValue)
end

return MainScript
