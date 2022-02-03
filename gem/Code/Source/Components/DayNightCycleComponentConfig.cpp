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

#include <AzCore/Serialization/SerializeContext.h>

#include "DayNightCycleComponentConfig.hpp"

using Loherangrin::Gems::DayNightCycle::DayNightCycleComponentConfig;


void DayNightCycleComponentConfig::Reflect(AZ::ReflectContext* io_context)
{
	if(auto serializeContext = azrtti_cast<AZ::SerializeContext*>(io_context))
	{
		serializeContext->Class<DayNightCycleComponentConfig>()
			->Version(0)
			->Field("EastOrientation", &DayNightCycleComponentConfig::m_eastOrientation)
			->Field("Sunrise", &DayNightCycleComponentConfig::m_sunriseTime)
			->Field("Sunset", &DayNightCycleComponentConfig::m_sunsetTime)

			->Field("Synchronization", &DayNightCycleComponentConfig::m_hasSynchronization)
			->Field("TimeScale", &DayNightCycleComponentConfig::m_timeScale)
			->Field("TimeSpeed", &DayNightCycleComponentConfig::m_timeSpeed)
			->Field("CurrentTime", &DayNightCycleComponentConfig::m_nowTime)
			;
	}
}

bool DayNightCycleComponentConfig::IsSynchronizationDisabled() const
{
	return !m_hasSynchronization;
}

Time DayNightCycleComponentConfig::GetMaxSunrise() const
{
	if(!m_sunsetTime.IsValid())
	{
		return Time { 23, 59, 58 };
	}

	Time maxSunriseTime = m_sunsetTime;
	maxSunriseTime.SubtractSecond();

	return maxSunriseTime;
}

Time DayNightCycleComponentConfig::GetMinSunset() const
{
	if(!m_sunriseTime.IsValid())
	{
		return Time { 0, 0, 1 };
	}

	Time minSunsetTime = m_sunriseTime;
	minSunsetTime.AddSecond();

	return minSunsetTime;
}
