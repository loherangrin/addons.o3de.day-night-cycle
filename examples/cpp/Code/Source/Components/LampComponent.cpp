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

#include <AtomLyIntegration/CommonFeatures/CoreLights/AreaLightBus.h>

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>

// -------------------------------------
// [INSTRUCTIONS]: #4
#include <DayNightCycle/Constants/DayNightCycleConstants.hpp>
// -------------------------------------

#include "LampComponent.hpp"

using Loherangrin::Gems::DayNightCycle::LampComponent;


void LampComponent::Reflect(AZ::ReflectContext* io_context)
{
	if(auto serializeContext = azrtti_cast<AZ::SerializeContext*>(io_context))
	{
		serializeContext->Class<LampComponent, AZ::Component>()
			->Version(0)
		;

		if(AZ::EditContext* editContext = serializeContext->GetEditContext())
		{
			editContext->Class<LampComponent>("LampComponent", "Custom component to demonstrate gem features")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
					->Attribute(AZ::Edit::Attributes::AutoExpand, true)
			;
		}
	}
}

void LampComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& io_provided)
{
	io_provided.push_back(AZ_CRC_CE("DayNightCycleCppExampleLampService"));
}

void LampComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& io_incompatible)
{
	io_incompatible.push_back(AZ_CRC_CE("DayNightCycleCppExampleLampService"));
}

void LampComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& io_required)
{
	io_required.push_back(AZ_CRC_CE("AreaLightService"));
}

void LampComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& io_dependent)
{
	// -------------------------------------
	// [INSTRUCTIONS]: #5
	io_dependent.push_back(Services::DAY_NIGHT_CYCLE);
	// -------------------------------------
}

void LampComponent::Init()
{
	m_maxLightIntensity = 50.f;
}

void LampComponent::Activate()
{
	const AZ::EntityId thisEntityId = GetEntityId();
	EBUS_EVENT_ID_RESULT(m_maxLightIntensity, thisEntityId, AZ::Render::AreaLightRequestBus, GetIntensity);

	AskForCurrentDayPeriod();

	// -------------------------------------
	// [INSTRUCTIONS]: #6
	DayNightCycleNotificationBus::Handler::BusConnect();
	// -------------------------------------
}

void LampComponent::Deactivate()
{
	// -------------------------------------
	// [INSTRUCTIONS]: #7
	DayNightCycleNotificationBus::Handler::BusDisconnect();
	// -------------------------------------
}

// -------------------------------------
// [INSTRUCTIONS]: #8

void LampComponent::OnDayStarted()
{
	TurnOffLight();
}

void LampComponent::OnNightStarted()
{
	TurnOnLight();
}

// -------------------------------------

void LampComponent::AskForCurrentDayPeriod()
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

void LampComponent::TurnOnLight() const
{
	SetLightIntensity(m_maxLightIntensity);
}

void LampComponent::TurnOffLight() const
{
	SetLightIntensity(0.f);
}

void LampComponent::SetLightIntensity(float i_intensity) const
{
	const AZ::EntityId& lightEntityId = GetEntityId();
	const auto setIntensityMethod = static_cast<void (AZ::Render::AreaLightRequestBus::Events::*)(float)>(&AZ::Render::AreaLightRequestBus::Events::SetIntensity);
	AZ::Render::AreaLightRequestBus::Event(lightEntityId, setIntensityMethod, i_intensity);
}
