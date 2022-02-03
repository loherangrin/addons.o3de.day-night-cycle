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

#include <AzCore/RTTI/TypeInfoSimple.h>

#include <DateTime/Types/Time.hpp>


namespace Loherangrin::Gems::DayNightCycle
{
	struct DayNightCycleComponentConfig
	{
	private:
		using Time = Loherangrin::Gems::DateTime::Time;

	public:
		AZ_TYPE_INFO(DayNightCycleComponentConfig, "{B1BB6740-38E2-4A43-BA04-87725F35886A}");
		static void Reflect(AZ::ReflectContext* io_context);

		bool IsSynchronizationDisabled() const;

		Time GetMaxSunrise() const;
		Time GetMinSunset() const;

		Time m_sunriseTime { 6, 0, 0 };
		Time m_sunsetTime { 18, 0, 0 };

		float m_eastOrientation { 0.f };

		bool m_hasSynchronization { true };
		float m_timeSpeed { 0.f };
		float m_timeScale { 1.f };
		Time m_nowTime { 0, 0, 0 };
	};

} // Loherangrin::Gems::DayNightCycle
