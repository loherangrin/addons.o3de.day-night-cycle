/* Copyright 2022 Matteo Grasso
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>

#include <LyShine/Bus/UiCanvasBus.h>
#include <LyShine/Bus/UiElementBus.h>
#include <LyShine/Bus/UiTextBus.h>

#include <DayNightCycle/Constants/DayNightCycleConstants.hpp>

#include "MainComponent.hpp"

using Loherangrin::Gems::DayNightCycle::MainComponent;


void MainComponent::Reflect(AZ::ReflectContext* io_context)
{
	if(auto serializeContext = azrtti_cast<AZ::SerializeContext*>(io_context))
	{
		serializeContext->Class<MainComponent, AZ::Component>()
			->Version(0)
		;

		if(AZ::EditContext* editContext = serializeContext->GetEditContext())
		{
			editContext->Class<MainComponent>("MainComponent", "Custom component to demonstrate gem features")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
					->Attribute(AZ::Edit::Attributes::AutoExpand, true)
			;
		}
	}
}

void MainComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& io_provided)
{
	io_provided.push_back(AZ_CRC_CE("DayNightCycleCppExampleMainService"));
}

void MainComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& io_incompatible)
{
	io_incompatible.push_back(AZ_CRC_CE("DayNightCycleCppExampleMainService"));
}

void MainComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& io_required)
{
	io_required.push_back(AZ_CRC("UiCanvasRefService", 0xb4cb5ef4));
}

void MainComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& io_dependent)
{
	io_dependent.push_back(Services::DAY_NIGHT_CYCLE);
}

void MainComponent::Init()
{}

void MainComponent::Activate()
{
	const AZ::EntityId thisEntityId = GetEntityId();
	UiCanvasRefNotificationBus::Handler::BusConnect(thisEntityId);

	AZ::EntityId mainCanvas;
	EBUS_EVENT_ID_RESULT(mainCanvas, thisEntityId, UiCanvasRefBus, GetCanvas);

	const bool found = FindAllUiElements(mainCanvas);
	if(found)
	{
		AskForCurrentDayPeriod();
	}

	DayNightCycleNotificationBus::Handler::BusConnect();
}

void MainComponent::Deactivate()
{
	const AZ::EntityId thisEntityId = GetEntityId();

	DayNightCycleNotificationBus::Handler::BusDisconnect();
	UiCanvasRefNotificationBus::Handler::BusDisconnect(thisEntityId);
}

void MainComponent::OnCanvasRefChanged(AZ::EntityId i_uiCanvasRefEntity, AZ::EntityId i_uiCanvasEntity)
{
	const bool changed = FindAllUiElements(i_uiCanvasEntity);
	if(changed)
	{
		AskForCurrentDayPeriod();
	}
}

void MainComponent::OnDayStarted()
{
	EditTextUiElement(m_dayPeriodTextId, "Day");
}

void MainComponent::OnNightStarted()
{
	EditTextUiElement(m_dayPeriodTextId, "Night");
}

void MainComponent::AskForCurrentDayPeriod()
{
	DayPeriod dayPeriod;
	EBUS_EVENT_RESULT(dayPeriod, DayNightCycleRequestBus, GetCurrentDayPeriod);

	switch(dayPeriod)
	{
		case DayPeriod::DAY:
		{
			OnDayStarted();
		}
		break;

		case DayPeriod::NIGHT:
		{
			OnNightStarted();
		}
		break;
	}
}

bool MainComponent::FindAllUiElements(const AZ::EntityId& i_mainCanvasId)
{
	if(!i_mainCanvasId.IsValid() || i_mainCanvasId == m_mainCanvasId)
	{
		return false;
	}

	m_mainCanvasId = i_mainCanvasId;

	m_dayPeriodTextId = FindUiElement(UI_DAY_PERIOD_TEXT);

	return true;
}

AZ::EntityId MainComponent::FindUiElement(const char* i_elementName) const
{
	AZ::EntityId elementId;
	EBUS_EVENT_ID_RESULT(elementId, m_mainCanvasId, UiCanvasBus, FindElementEntityIdByName, AZStd::string { i_elementName });

	return elementId;
}

void MainComponent::EditTextUiElement(const AZ::EntityId& i_textElementId, const AZStd::string& i_textValue) const
{	
	EBUS_EVENT_ID(i_textElementId, UiTextBus, SetText, i_textValue);
}
