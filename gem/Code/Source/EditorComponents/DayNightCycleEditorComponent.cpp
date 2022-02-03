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

#include <DayNightCycle/Constants/DayNightCycleEditorConstants.hpp>

#include "../Components/DayNightCycleComponent.hpp"

#include "DayNightCycleEditorComponent.hpp"

using Loherangrin::Gems::DayNightCycle::DayNightCycleEditorComponent;


void DayNightCycleEditorComponent::Reflect(AZ::ReflectContext* io_context)
{
	DayNightCycleComponentConfig::Reflect(io_context);

	if(auto serializeContext = azrtti_cast<AZ::SerializeContext*>(io_context))
	{
		serializeContext->Class<DayNightCycleEditorComponent, AzToolsFramework::Components::EditorComponentBase>()
			->Version(0)
			->Field("Configuration", &DayNightCycleEditorComponent::m_config)
			;

		if(AZ::EditContext* editContext = serializeContext->GetEditContext())
		{
			editContext->Class<DayNightCycleEditorComponent>("Day/Night Cycle", "Manage the alternation of day and night according to the scene time")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
					->Attribute(AZ::Edit::Attributes::AutoExpand, true)

				->DataElement(AZ::Edit::UIHandlers::Default, &DayNightCycleEditorComponent::m_config, "Config", "")
					->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
			;

			editContext->Class<DayNightCycleComponentConfig>("Config", "")
				->DataElement(AZ::Edit::UIHandlers::Slider, &DayNightCycleComponentConfig::m_eastOrientation, "East Orientation", "Where all sky objects rise")
					->Attribute(AZ::Edit::Attributes::Min, 0.f)
					->Attribute(AZ::Edit::Attributes::Max, 360.f)

				->ClassElement(AZ::Edit::ClassElements::Group, "Sun")
					->Attribute(AZ::Edit::Attributes::AutoExpand, true)

					->DataElement(AZ::Edit::UIHandlers::Default, &DayNightCycleComponentConfig::m_sunriseTime, "Sunrise", "When the day begins")
						->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
						->Attribute(AZ::Edit::Attributes::Max, &DayNightCycleComponentConfig::GetMaxSunrise)

					->DataElement(AZ::Edit::UIHandlers::Default, &DayNightCycleComponentConfig::m_sunsetTime, "Sunset", "When the day ends")
						->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
						->Attribute(AZ::Edit::Attributes::Min, &DayNightCycleComponentConfig::GetMinSunset)

				->ClassElement(AZ::Edit::ClassElements::Group, "")
					->Attribute(AZ::Edit::Attributes::AutoExpand, true)

					->DataElement(AZ::Edit::UIHandlers::Default, &DayNightCycleComponentConfig::m_hasSynchronization, "Synchronization", "Synchronize time and speed with the main TimeComponent. When disabled, values must be provided manually")
						->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::EntireTree)

					->DataElement(AZ::Edit::UIHandlers::Default, &DayNightCycleComponentConfig::m_timeScale, "Time Scale", "Conversion factor between one second in the real world and <X> seconds in the scene")
						->Attribute(AZ::Edit::Attributes::Min, 0.f)
						->Attribute(AZ::Edit::Attributes::Visibility, &DayNightCycleComponentConfig::IsSynchronizationDisabled)

					->DataElement(AZ::Edit::UIHandlers::Default, &DayNightCycleComponentConfig::m_timeSpeed, "Time Speed", "Counting speed expressed in scene-time seconds. Use 0 to stop the clock, or negative values to count in reverse order")
						->Attribute(AZ::Edit::Attributes::Visibility, &DayNightCycleComponentConfig::IsSynchronizationDisabled)

					->DataElement(AZ::Edit::UIHandlers::Default, &DayNightCycleComponentConfig::m_nowTime, "Start Time", "Initial time value of the counter when the scene is started. It is expressed in scene-time unit")
						->Attribute(AZ::Edit::Attributes::Visibility, &DayNightCycleComponentConfig::IsSynchronizationDisabled)
			;
		}
	}
}

void DayNightCycleEditorComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& io_provided)
{
	io_provided.push_back(EditorServices::DAY_NIGHT_CYCLE);
}

void DayNightCycleEditorComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& io_incompatible)
{
	io_incompatible.push_back(EditorServices::DAY_NIGHT_CYCLE);
}

void DayNightCycleEditorComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& io_required)
{}

void DayNightCycleEditorComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& io_dependent)
{}

void DayNightCycleEditorComponent::Init()
{}

void DayNightCycleEditorComponent::Activate()
{}

void DayNightCycleEditorComponent::Deactivate()
{}

void DayNightCycleEditorComponent::BuildGameEntity(AZ::Entity* io_gameEntity)
{
	DayNightCycleComponent* runtimeComponent = io_gameEntity->CreateComponent<DayNightCycleComponent>(AZStd::move(m_config));
	if(!runtimeComponent)
	{
		AZ_Error(GetNamedEntityId().ToString().c_str(), false, "Unable to create DayNightCycleComponent into entity: %s (%d)", io_gameEntity->GetName().c_str(), io_gameEntity->GetId());
		return;
	}
}
