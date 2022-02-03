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

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <DateTime/Constants/DateTimeConstants.hpp>

#include <DayNightCycle/Constants/DayNightCycleConstants.hpp>

#include "DayNightCycleComponent.hpp"

using Loherangrin::Gems::DateTime::DurationS;
using Loherangrin::Gems::DateTime::Time;
using Loherangrin::Gems::DateTime::TimeNotificationBus;
using Loherangrin::Gems::DateTime::TimeRequestBus;
using Loherangrin::Gems::DateTime::TimeS;
using Loherangrin::Gems::DayNightCycle::DayNightCycleComponent;
using Loherangrin::Gems::DayNightCycle::DayPeriod;


namespace Loherangrin::Gems::DayNightCycle
{
	class DayNightCycleNotificationBusBehaviorHandler
		: public DayNightCycleNotificationBus::Handler
		, public AZ::BehaviorEBusHandler
	{
	public:
		AZ_EBUS_BEHAVIOR_BINDER(DayNightCycleNotificationBusBehaviorHandler, "{AA16E52F-8916-4D58-902E-32E62FA60604}", AZ::SystemAllocator,
			OnDayStarted,
			OnNightStarted
		);

		void OnDayStarted() override
		{
			Call(FN_OnDayStarted);
		}

		void OnNightStarted() override
		{
			Call(FN_OnNightStarted);
		}
	};

} // Loherangrin::Gems::DayNightCycle

void DayNightCycleComponent::Reflect(AZ::ReflectContext* io_context)
{
	if(auto serializeContext = azrtti_cast<AZ::SerializeContext*>(io_context))
	{
		serializeContext->Class<DayNightCycleComponent, AZ::Component>()
			->Version(0)
			->Field("EastOrientation", &DayNightCycleComponent::m_eastOrientation)
			->Field("Sunrise", &DayNightCycleComponent::m_sunriseTime)
			->Field("Sunset", &DayNightCycleComponent::m_sunsetTime)

			->Field("Synchronization", &DayNightCycleComponent::m_hasSynchronization)
			->Field("TimeScale", &DayNightCycleComponent::m_timeScale)
			->Field("TimeSpeed", &DayNightCycleComponent::m_timeSpeed)
			->Field("CurrentTime", &DayNightCycleComponent::m_nowTime)
			;
	}

	if(auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(io_context))
	{
		behaviorContext
			->Enum<(AZ::u8) DayPeriod::DAY>("DayPeriod_Day")
			->Enum<(AZ::u8) DayPeriod::NIGHT>("DayPeriod_Night")
			;

		behaviorContext->EBus<DayNightCycleRequestBus>("DayNightCycleRequestBus")
			->Attribute(AZ::Script::Attributes::Module, ScriptAttributes::MODULE)
			->Attribute(AZ::Script::Attributes::Category, ScriptAttributes::CATEGORY)

			->Event("GetCurrentDayPeriod", &DayNightCycleRequestBus::Events::GetCurrentDayPeriod)

			->Event("GetSunriseTime", &DayNightCycleRequestBus::Events::GetSunriseTime)
			->Event("GetSunsetTime", &DayNightCycleRequestBus::Events::GetSunsetTime)

			->Event("SetSunriseTime", &DayNightCycleRequestBus::Events::SetSunriseTime)
			->Event("SetSunsetTime", &DayNightCycleRequestBus::Events::SetSunsetTime)

			->Event("IsSynchronized", &DayNightCycleRequestBus::Events::IsSynchronizedWithTimeComponent)
			->Event("SetSynchronization", &DayNightCycleRequestBus::Events::SetSynchronizationWithTimeComponent)
			->Event("SetCurrentTime", &DayNightCycleRequestBus::Events::SetCurrentTime)
			->Event("SetTimeScale", &DayNightCycleRequestBus::Events::SetTimeScale)
			->Event("SetTimeSpeed", &DayNightCycleRequestBus::Events::SetTimeSpeed)
		;

		behaviorContext->EBus<DayNightCycleNotificationBus>("DayNightCycleNotificationBus")
			->Attribute(AZ::Script::Attributes::Module, ScriptAttributes::MODULE)
			->Attribute(AZ::Script::Attributes::Category, ScriptAttributes::CATEGORY)

			->Handler<DayNightCycleNotificationBusBehaviorHandler>()
		;
	}
}

void DayNightCycleComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& io_provided)
{
	io_provided.push_back(Services::DAY_NIGHT_CYCLE);
}

void DayNightCycleComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& io_incompatible)
{
	io_incompatible.push_back(Services::DAY_NIGHT_CYCLE);
}

void DayNightCycleComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& io_required)
{
	io_required.push_back(AZ_CRC("DirectionalLightService", 0x5270619f));
	io_required.push_back(AZ_CRC("SkyBoxService", 0x8169a709));
	io_required.push_back(AZ_CRC("TransformService"));
}

void DayNightCycleComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& io_dependent)
{
	io_dependent.push_back(AZ_CRC_CE(Loherangrin::Gems::DateTime::Services::TIME));
}

DayNightCycleComponent::DayNightCycleComponent()
	: DayNightCycleComponent(ConfigClass {})
{}

DayNightCycleComponent::DayNightCycleComponent(const ConfigClass& i_config)
	: m_isDay { false }
	, m_isTicking { false }
	, m_hasSynchronization { i_config.m_hasSynchronization }
	, m_timeSpeed { i_config.m_timeSpeed }
	, m_timeScale { i_config.m_timeScale }
	, m_nowTime { i_config.m_nowTime }
	, m_sunriseTime { i_config.m_sunriseTime }
	, m_sunsetTime { i_config.m_sunsetTime }
	, m_dayDuration { 0.f }
	, m_nightDuration { 0.f }
	, m_eastOrientation { i_config.m_eastOrientation }
	, m_skyRotationParameter { 0.f }
	, m_startDaySkyRotation {}
	, m_endDaySkyRotation {}
	, m_startNightSkyRotation {}
	, m_endNightSkyRotation {}
{}

DayNightCycleComponent::DayNightCycleComponent(ConfigClass&& io_config)
	: m_isDay { false }
	, m_isTicking { false }
	, m_hasSynchronization { AZStd::exchange(io_config.m_hasSynchronization, true) }
	, m_timeSpeed { AZStd::exchange(io_config.m_timeSpeed, 0.f) }
	, m_timeScale { AZStd::exchange(io_config.m_timeScale, 0.f) }
	, m_nowTime { AZStd::move(io_config.m_nowTime) }
	, m_sunriseTime { AZStd::move(io_config.m_sunriseTime) }
	, m_sunsetTime { AZStd::move(io_config.m_sunsetTime) }
	, m_dayDuration { 0.f }
	, m_nightDuration { 0.f }
	, m_eastOrientation { AZStd::exchange(io_config.m_eastOrientation, 0.f) }
	, m_skyRotationParameter { 0.f }
	, m_startDaySkyRotation {}
	, m_endDaySkyRotation {}
	, m_startNightSkyRotation {}
	, m_endNightSkyRotation {}
{}

void DayNightCycleComponent::Init()
{
	CalculateRotations();
	CalculateTimeDurationsAndParameter(m_nowTime, m_sunriseTime, m_sunsetTime);

	if(!m_hasSynchronization)
	{
		CalculateTicking(m_timeSpeed);
	}
}

void DayNightCycleComponent::Activate()
{
	const AZ::EntityId entityId = GetEntityId();
	AZ::TransformBus::Bind(m_transformRequestBus, entityId);

	if(m_hasSynchronization)
	{
		SynchronizeWithTimeComponent();
		TimeNotificationBus::Handler::BusConnect();
	}

	DayNightCycleRequestBus::Handler::BusConnect();

	if(m_isTicking)
	{	
		AZ::TickBus::Handler::BusConnect();
	}
}

void DayNightCycleComponent::Deactivate()
{
	if(m_isTicking)
	{
		AZ::TickBus::Handler::BusDisconnect();
	}

	DayNightCycleRequestBus::Handler::BusDisconnect();

	if(m_hasSynchronization)
	{
		TimeNotificationBus::Handler::BusDisconnect();
	}

	m_transformRequestBus = nullptr;
}

void DayNightCycleComponent::OnTick(float i_deltaTime, AZ::ScriptTimePoint i_time)
{
	AZ_Assert(m_isTicking, "OnTick must not be called when the timer is inactive");

	RotateSky(i_deltaTime);
}

void DayNightCycleComponent::OnTimeReset(const Time& i_startingTime)
{
	if(!i_startingTime.IsValid())
	{
		AZ_Assert(false, "Starting time cannot contain invalid fields");
		return;
	}

	const Time unchangedTime {};
	CalculateTimeDurationsAndParameter(i_startingTime, unchangedTime, unchangedTime);
}

void DayNightCycleComponent::OnTimeScaleChanged(float i_scale)
{
	if(i_scale < 0.f)
	{
		AZ_Assert(false, "Time scale cannot be negative");
		return;
	}

	m_timeScale = i_scale;
}

void DayNightCycleComponent::OnTimeSpeedChanged(float i_speed)
{
	SetTimeSpeedAndTickBus(i_speed);
}

DayPeriod DayNightCycleComponent::GetCurrentDayPeriod() const
{
	return ((m_isDay) ? DayPeriod::DAY : DayPeriod::NIGHT);
}

Time DayNightCycleComponent::GetSunriseTime() const
{
	return m_sunriseTime;
}

Time DayNightCycleComponent::GetSunsetTime() const
{
	return m_sunsetTime;
}

void DayNightCycleComponent::SetSunriseTime(const Time& i_time)
{
	if(!i_time.IsValid())
	{
		AZ_Assert(false, "Sunrise time cannot contain invalid fields");
		return;
	}

	const Time unchangedTime {};
	CalculateTimeDurationsAndParameter(unchangedTime, i_time, unchangedTime);
}

void DayNightCycleComponent::SetSunsetTime(const Time& i_time)
{
	if(!i_time.IsValid())
	{
		AZ_Assert(false, "Sunset time cannot contain invalid fields");
		return;
	}

	const Time unchangedTime {};
	CalculateTimeDurationsAndParameter(unchangedTime, unchangedTime, i_time);
}

bool DayNightCycleComponent::IsSynchronizedWithTimeComponent() const
{
	return m_hasSynchronization;
}

void DayNightCycleComponent::SetSynchronizationWithTimeComponent(bool i_synchronized)
{
	if(m_hasSynchronization == i_synchronized)
	{
		return;
	}

	m_hasSynchronization = i_synchronized;
	if(m_hasSynchronization)
	{
		SynchronizeWithTimeComponent();
	}
}

void DayNightCycleComponent::SetTimeScale(float i_scale)
{
	if(m_hasSynchronization)
	{
		AZ_Assert(false, "Unable to set a new time scale when the synchronization is active");
		return;
	}

	AZ_Assert(!(i_scale < 0.f), "Time scale cannot be negative");
	m_timeScale = i_scale;
}

void DayNightCycleComponent::SetTimeSpeed(float i_speed)
{
	if(m_hasSynchronization)
	{
		AZ_Assert(false, "Unable to set a new time speed when the synchronization is active");
		return;
	}

	SetTimeSpeedAndTickBus(i_speed);
}

void DayNightCycleComponent::SetCurrentTime(const Time& i_time)
{
	if(m_hasSynchronization)
	{
		AZ_Assert(false, "Unable to set time scale when the synchronization is active");
		return;
	}

	if(!i_time.IsValid())
	{
		AZ_Assert(false, "Time cannot contain invalid fields");
		return;
	}

	const Time unchangedTime {};
	CalculateTimeDurationsAndParameter(i_time, unchangedTime, unchangedTime);
}

void DayNightCycleComponent::CalculateRotations()
{
	const AZ::Quaternion eastRotation = AZ::Quaternion::CreateRotationZ(AZ::DegToRad(m_eastOrientation));

	m_startDaySkyRotation = eastRotation * AZ::Quaternion::CreateRotationX(AZ::DegToRad(360.f));
	m_endDaySkyRotation = eastRotation * AZ::Quaternion::CreateRotationX(AZ::DegToRad(180.f + AZ::Constants::FloatEpsilon));
	m_startNightSkyRotation = eastRotation * AZ::Quaternion::CreateRotationX(AZ::DegToRad(180.f));
	m_endNightSkyRotation = eastRotation * AZ::Quaternion::CreateRotationX(AZ::DegToRad(0.f + AZ::Constants::FloatEpsilon));
}

void DayNightCycleComponent::CalculateTicking(float i_speed)
{
	m_isTicking = !(AZ::IsClose(i_speed, 0.0f, AZ::Constants::FloatEpsilon));
	m_timeSpeed = i_speed;
}

void DayNightCycleComponent::CalculateTimeDurationsAndParameter(const Time& i_nowTime, const Time& i_sunriseTime, const Time& i_sunsetTime)
{
	Time nowTime;
	if(!i_nowTime.IsValid())
	{
		const TimeS nowSeconds = (m_isDay)
			? m_sunriseTime.ToSeconds() + (m_skyRotationParameter * m_dayDuration)
			: m_sunsetTime.ToSeconds() + (m_skyRotationParameter * m_nightDuration);

		nowTime = Time(nowSeconds);
	}
	else
	{
		nowTime = i_nowTime;
	}

	if(i_sunriseTime.IsValid())
	{
		m_sunriseTime = i_sunriseTime;
	}

	if(i_sunsetTime.IsValid())
	{
		m_sunsetTime = i_sunsetTime;
	}

	AZ_Assert(m_sunsetTime > m_sunriseTime, "Sunset must be greater than sunrise");
	m_dayDuration = m_sunsetTime - m_sunriseTime;
	m_nightDuration = Time::DAY_TO_SECONDS - m_dayDuration;

	m_isDay = IsDay(nowTime);
	if(m_isDay)
	{
		m_skyRotationParameter = (nowTime - m_sunriseTime) / m_dayDuration;
	}
	else
	{
		m_skyRotationParameter = (nowTime - m_sunsetTime) / m_nightDuration;
	}
}

bool DayNightCycleComponent::IsDay(const Time& i_time) const
{
	return (i_time > m_sunriseTime && i_time < m_sunsetTime);
}

void DayNightCycleComponent::RotateSky(float i_deltaTime)
{
	float offset = i_deltaTime * m_timeSpeed * m_timeScale;
	do
	{
		const float duration = (m_isDay) ? m_dayDuration : m_nightDuration;

		if(m_skyRotationParameter > 1.f || m_skyRotationParameter < 0.f)
		{
			m_isDay = !m_isDay;
			if(m_isDay)
			{
				EBUS_EVENT(DayNightCycleNotificationBus, OnDayStarted);
			}
			else
			{
				EBUS_EVENT(DayNightCycleNotificationBus, OnNightStarted);
			}

			float overflow;
			if(m_skyRotationParameter < 0.f)
			{
				overflow = m_skyRotationParameter;
				m_skyRotationParameter = 1.f;
			}
			else
			{
				overflow = m_skyRotationParameter - 1.f;
				m_skyRotationParameter = 0.f;
			}

			offset = overflow * duration;
			continue;
		}

		m_skyRotationParameter += offset / duration;
		break;
	}
	while(true);

	const AZ::Quaternion* startRotation;
	const AZ::Quaternion* endRotation;
	if(m_isDay)
	{
		startRotation = &m_startDaySkyRotation;
		endRotation = &m_endDaySkyRotation;
	}
	else
	{
		startRotation = &m_startNightSkyRotation;
		endRotation = &m_endNightSkyRotation;
	}

	const AZ::Quaternion skyRotation = startRotation->Slerp(*endRotation, m_skyRotationParameter);
	EBUS_EVENT_PTR(m_transformRequestBus, AZ::TransformBus, SetLocalRotationQuaternion, skyRotation);
}

void DayNightCycleComponent::SetTimeSpeedAndTickBus(float i_speed)
{
	const bool wasTicking = m_isTicking;

	CalculateTicking(i_speed);

	if(wasTicking == m_isTicking)
	{
		return;
	}

	if(m_isTicking)
	{
		AZ::TickBus::Handler::BusConnect();
	}
	else
	{
		AZ::TickBus::Handler::BusDisconnect();
	}
}

void DayNightCycleComponent::SynchronizeWithTimeComponent()
{
	AZ_Assert(m_hasSynchronization, "Unable to synchronize with Time Manager when the synchronization is disabled");

	Time nowTime;
	EBUS_EVENT_RESULT(nowTime, TimeRequestBus, GetCurrentTime);
	if(!nowTime.IsValid())
	{
		AZ_Assert(false, "Unable to find a valid TimeComponent instance to synchronize with. Time counter will fallback to manual values");
		m_hasSynchronization = false;

		return;
	}

	EBUS_EVENT_RESULT(m_timeScale, TimeRequestBus, GetTimeScale);
	AZ_Assert(!(m_timeScale < 0.f), "Time scale cannot be negative");

	float timeSpeed;
	EBUS_EVENT_RESULT(timeSpeed, TimeRequestBus, GetTimeSpeed);

	const Time unchangedTime {};
	CalculateTimeDurationsAndParameter(nowTime, unchangedTime, unchangedTime);
	CalculateTicking(timeSpeed);
}
