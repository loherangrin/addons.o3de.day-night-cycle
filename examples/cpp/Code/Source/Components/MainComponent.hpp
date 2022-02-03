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

#pragma once

#include <AzCore/Component/Component.h>

#include <DayNightCycle/EBuses/DayNightCycleBus.hpp>

#include <LyShine/Bus/World/UiCanvasRefBus.h>


namespace Loherangrin::Gems::DayNightCycle
{
	class MainComponent
		: public AZ::Component
		, protected UiCanvasRefNotificationBus::Handler
		, protected DayNightCycleNotificationBus::Handler
	{
	public:
		AZ_COMPONENT(MainComponent, "{9B6D6BF9-B42F-49CC-9521-C48F87B486D5}");
		static void Reflect(AZ::ReflectContext* io_context);

		static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& io_provided);
		static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& io_incompatible);
		static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& io_required);
		static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& io_dependent);

	protected:
		// AZ::Component
		void Init() override;
		void Activate() override;
		void Deactivate() override;

		// UiCanvasRefNotificationBus
		void OnCanvasRefChanged(AZ::EntityId i_uiCanvasRefEntity, AZ::EntityId i_uiCanvasEntity) override;

		// DayNightCycleNotificationBus
		void OnDayStarted() override;
		void OnNightStarted() override;

	private:
		void AskForCurrentDayPeriod();

		bool FindAllUiElements(const AZ::EntityId& i_mainCanvasId);
		AZ::EntityId FindUiElement(const char* i_elementName) const;

		void EditTextUiElement(const AZ::EntityId& i_textElementId, const AZStd::string& i_textValue) const;

		AZ::EntityId m_mainCanvasId;
		AZ::EntityId m_dayPeriodTextId;


		static constexpr const char* UI_DAY_PERIOD_TEXT = "DayPeriodValue";
	};

} // Loherangrin::Gems::DateTime
