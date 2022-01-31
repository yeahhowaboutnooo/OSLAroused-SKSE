#include "ArousalManager.h"
#include "Serialization.h"
#include "Utilities.h"
#include "Papyrus.h"
#include "Settings.h"

using namespace Serialization;

namespace ArousalManager
{
	float GetOArousedArousal(RE::Actor* actorRef, float lastCheckTime, float timePassed);
	float GetSexlabArousal(RE::Actor* actorRef, float timePassed);

	float GetArousal(RE::Actor* actorRef)
	{
		RE::FormID actorFormId = actorRef->formID;

		const auto LastCheckTimeData = LastCheckTimeData::GetSingleton();
		auto lastCheckTime = LastCheckTimeData->GetData(actorFormId, 0.f);
		float curTime = RE::Calendar::GetSingleton()->GetCurrentGameTime();
		float timePassed = curTime - lastCheckTime;

		LastCheckTimeData->SetData(actorFormId, curTime);

		switch (Settings::GetSingleton()->GetArousalMode()) {
		case Settings::ArousalMode::kSexlabAroused:
			return GetSexlabArousal(actorRef, timePassed);
		case Settings::ArousalMode::kOAroused:
			return GetOArousedArousal(actorRef, lastCheckTime, timePassed);
		}

		return 0.f;
	}

	float SetArousal(RE::Actor* actorRef, float value)
	{
		value = std::clamp(value, 0.0f, 100.f);

		ArousalData::GetSingleton()->SetData(actorRef->formID, value);

		if (actorRef->IsPlayerRef()) {
			Papyrus::Events::SendPlayerArousalUpdatedEvent(value);
		}

		return value;
	}

	void ModifyArousal(RE::Actor* actorRef, float modValue)
	{
		logger::trace("ModifyArousal: {}  by {}", actorRef->GetDisplayFullName(), modValue);

		if (modValue > 0) {
			modValue *= MultiplierData::GetSingleton()->GetData(actorRef->formID);
		}

		SetArousal(actorRef, ArousalData::GetSingleton()->GetData(actorRef->formID) + modValue);
	}

	float GetActorTimeRate(RE::Actor* actorRef, float timeSinceLastOrgasm)
	{
		float decayRate = Settings::GetSingleton()->GetDecayRate();
		if (decayRate <= 0.1) {
			return 10.f;
		}

		float timeRate = TimeRateData::GetSingleton()->GetData(actorRef->formID, 10.f);
		timeRate *= std::pow(1.5f, -timeSinceLastOrgasm / decayRate);
		return timeRate;
	}

	float GetOArousedArousal(RE::Actor* actorRef, float lastCheckTime, float timePassed)
	{
		RE::FormID actorFormId = actorRef->formID;
		logger::trace("Get Arousal for {}  lastcheck: {}  timePass {}", actorFormId, lastCheckTime, timePassed);

		float newArousal = 0.f;
		//If never calc or old, regen
		if (lastCheckTime <= 0.0f || timePassed > 3.f) {
			newArousal = Utilities::GenerateRandomFloat(0.f, 75.f);

			if (lastCheckTime <= 0.0f) {
				float randomMultiplier = Utilities::GenerateRandomFloat(0.75f, 1.25f);
				MultiplierData::GetSingleton()->SetData(actorFormId, randomMultiplier);
			}
		} else {
			float currentArousal = ArousalData::GetSingleton()->GetData(actorFormId);
			float arousalMultiplier = MultiplierData::GetSingleton()->GetData(actorFormId);
			newArousal = currentArousal + ((timePassed * 25.f) * arousalMultiplier);
		}

		return SetArousal(actorRef, newArousal);
	}

	float GetSexlabArousal(RE::Actor* actorRef, float timePassed)
	{
		logger::trace("GetSexlabArousal timePass {}", timePassed);

		if (!actorRef) {
			return -2;
		}

		float lastOrgasmTime = LastOrgasmTimeData::GetSingleton()->GetData(actorRef->formID, 0.f);
		float daysSinceLastOrgasm = RE::Calendar::GetSingleton()->GetCurrentGameTime() - lastOrgasmTime;

		float arousal = (daysSinceLastOrgasm * GetActorTimeRate(actorRef, daysSinceLastOrgasm)) + GetSexlabExposure(actorRef, timePassed);

		return arousal;
	}

	float GetSexlabExposure(RE::Actor* actorRef, float timePassed)
	{
		RE::FormID actorFormId = actorRef->formID;

		float newExposure = ArousalData::GetSingleton()->GetData(actorFormId, -2);

		//If never calculated regen exposure
		if (newExposure < -1) {
			newExposure = Utilities::GenerateRandomFloat(0.0f, 50.f);
		} else {
			float decayRate = Settings::GetSingleton()->GetDecayRate();
			if (decayRate > 0.1f) {
				//Yikes
				newExposure *= std::pow(1.5f, -timePassed / decayRate);
			}
		}

		//We store exposure, arousal is calculated
		return SetArousal(actorRef, newExposure);
	}
}
