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

#include <AzCore/EBus/EBus.h>

#include <DateTime/Types/Time.hpp>
#include <DayNightCycle/Types/DayPeriod.hpp>


namespace Loherangrin::Gems::DayNightCycle
{
	class DayNightCycleRequests
	{
	public:
		AZ_RTTI(DayNightCycleRequests, "{FA49DC0E-3D14-4BE2-9894-EB07BD66DC29}");
		virtual ~DayNightCycleRequests() = default;

		using Time = Loherangrin::Gems::DateTime::Time;

		virtual DayPeriod GetCurrentDayPeriod() const = 0;

		virtual Time GetSunriseTime() const = 0;
		virtual Time GetSunsetTime() const = 0;

		virtual void SetSunriseTime(const Time& i_time) = 0;
		virtual void SetSunsetTime(const Time& i_time) = 0;

		virtual bool IsSynchronizedWithTimeComponent() const = 0;
		virtual void SetSynchronizationWithTimeComponent(bool i_synchronized) = 0;
		virtual void SetCurrentTime(const Time& i_time) = 0;
		virtual void SetTimeScale(float i_scale) = 0;
		virtual void SetTimeSpeed(float i_speed) = 0;
	};
	
	class DayNightCycleBusTraits
		: public AZ::EBusTraits
	{
	public:
		// EBusTraits
		static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
	};

	using DayNightCycleRequestBus = AZ::EBus<DayNightCycleRequests, DayNightCycleBusTraits>;	

	// ---

    class DayNightCycleNotifications
    {
    public:
        AZ_RTTI(DayNightCycleNotifications, "{EE66F47C-140D-46E7-9B0B-1BCF8DE2301A}");
        virtual ~DayNightCycleNotifications() = default;

		virtual void OnDayStarted(){}
		virtual void OnNightStarted(){}
    };
    
    class DayNightCycleNotificationBusTraits
        : public AZ::EBusTraits
    {
    public:
        // EBusTraits
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Multiple;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
    };

    using DayNightCycleNotificationBus = AZ::EBus<DayNightCycleNotifications, DayNightCycleNotificationBusTraits>;

} // Loherangrin::Gems::DayNightCycle
