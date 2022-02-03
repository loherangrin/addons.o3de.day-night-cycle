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
#include <AzCore/Component/TickBus.h>
#include <AzCore/Component/TransformBus.h>

#include <DateTime/EBuses/TimeBus.hpp>
#include <DateTime/Types/Time.hpp>

#include <DayNightCycle/EBuses/DayNightCycleBus.hpp>

#include "DayNightCycleComponentConfig.hpp"


namespace Loherangrin::Gems::DayNightCycle
{
	class DayNightCycleComponent
		: public AZ::Component
		, protected AZ::TickBus::Handler
		, protected Loherangrin::Gems::DateTime::TimeNotificationBus::Handler
		, protected DayNightCycleRequestBus::Handler
	{
	private:
		using DurationS = Loherangrin::Gems::DateTime::DurationS;
		using Time = Loherangrin::Gems::DateTime::Time;

	public:
		using ConfigClass = DayNightCycleComponentConfig;

	public:
		AZ_COMPONENT(DayNightCycleComponent, "{07DCFA8E-EAA7-4788-BB2D-56FF56DB3DE5}");
		static void Reflect(AZ::ReflectContext* io_context);

		static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& io_provided);
		static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& io_incompatible);
		static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& io_required);
		static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& io_dependent);

		DayNightCycleComponent();
		DayNightCycleComponent(const ConfigClass& i_config);
		DayNightCycleComponent(ConfigClass&& io_config);

	protected:
		// AZ::Component
		void Init() override;
		void Activate() override;
		void Deactivate() override;

		// AZ::TickBus
		void OnTick(float i_deltaTime, AZ::ScriptTimePoint i_time) override;

		// TimeNotificationBus
		void OnTimeScaleChanged(float i_scale) override;
		void OnTimeSpeedChanged(float i_speed) override;
		void OnTimeReset(const Time& i_startingTime) override;

		// DayNightCycleRequestBus
		DayPeriod GetCurrentDayPeriod() const override;

		Time GetSunriseTime() const override;
		Time GetSunsetTime() const override;

		void SetSunriseTime(const Time& i_time) override;
		void SetSunsetTime(const Time& i_time) override;

		bool IsSynchronizedWithTimeComponent() const override;
		void SetSynchronizationWithTimeComponent(bool i_synchronized) override;
		void SetCurrentTime(const Time& i_time) override;
		void SetTimeScale(float i_scale) override;
		void SetTimeSpeed(float i_speed) override;

	private:
		void CalculateTicking(float i_speed);
		void CalculateRotations();
		void CalculateTimeDurationsAndParameter(const Time& i_nowTime, const Time& i_sunriseTime, const Time& i_sunsetTime);

		bool IsDay(const Time& i_time) const;

		void RotateSky(float i_deltaTime);

		void SetTimeSpeedAndTickBus(float i_speed);

		void SynchronizeWithTimeComponent();

		bool m_isDay = false;
		bool m_isTicking = false;

		bool m_hasSynchronization = true;
		float m_timeSpeed;
		float m_timeScale;
		Time m_nowTime;

		Time m_sunriseTime;
		Time m_sunsetTime;

		DurationS m_dayDuration;
		DurationS m_nightDuration;

		float m_eastOrientation;
		float m_skyRotationParameter;

		AZ::Quaternion m_startDaySkyRotation;
		AZ::Quaternion m_endDaySkyRotation;
		AZ::Quaternion m_startNightSkyRotation;
		AZ::Quaternion m_endNightSkyRotation;

		AZ::TransformBus::BusPtr m_transformRequestBus;
	};

} // Loherangrin::Gems::DayNightCycle
