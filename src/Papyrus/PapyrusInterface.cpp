#include "PapyrusInterface.h"

#include "PersistedData.h"
#include "Managers/ArousalManager.h"
#include "Managers/LibidoManager.h"
#include "Utilities/Utils.h"
#include <Settings.h>
#include <Integrations/DevicesIntegration.h>

float PapyrusInterface::GetArousal(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return ArousalManager::GetArousal(actorRef);
}

std::vector<float> PapyrusInterface::GetArousalMultiple(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::reference_array<RE::Actor*> actorRefs)
{
	std::vector<float> results;

	for (const auto actorRef : actorRefs) {
		if (!actorRef) {
			Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
			results.push_back(0);
			continue;
		}
		results.push_back(ArousalManager::GetArousal(actorRef));
	}

	return results;
}

float PapyrusInterface::GetArousalNoSideEffects(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return ArousalManager::GetArousal(actorRef, false);
}

float PapyrusInterface::SetArousal(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef, float value)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return ArousalManager::SetArousal(actorRef, value);
}

void PapyrusInterface::SetArousalMultiple(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::reference_array<RE::Actor*> actorRefs, float value)
{
	for (const auto actorRef : actorRefs) {
		if (!actorRef) {
			Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
			continue;
		}
		ArousalManager::SetArousal(actorRef, value);
	}
}

float PapyrusInterface::ModifyArousal(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef, float value)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return ArousalManager::ModifyArousal(actorRef, value);
}

void PapyrusInterface::ModifyArousalMultiple(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::reference_array<RE::Actor*> actorRefs, float value)
{
	for (const auto actorRef : actorRefs) {
		if (!actorRef) {
			Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
			continue;
		}
		ArousalManager::ModifyArousal(actorRef, value);
	}
}

float PapyrusInterface::GetArousalMultiplier(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return PersistedData::ArousalMultiplierData::GetSingleton()->GetData(actorRef->formID, 1.f);
}

float PapyrusInterface::SetArousalMultiplier(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef, float value)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	value = std::clamp(value, 0.0f, 100.f);

	PersistedData::ArousalMultiplierData::GetSingleton()->SetData(actorRef->formID, value);
	return value;
}

float PapyrusInterface::ModifyArousalMultiplier(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef, float value)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	float curMult = PersistedData::ArousalMultiplierData::GetSingleton()->GetData(actorRef->formID, 1.f);
	float newVal = curMult + value;
	PersistedData::ArousalMultiplierData::GetSingleton()->SetData(actorRef->formID, newVal);
	return newVal;
}

float PapyrusInterface::GetArousalBaseline(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return LibidoManager::GetSingleton()->GetBaselineArousal(actorRef);
}

float PapyrusInterface::GetLibido(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return LibidoManager::GetSingleton()->GetBaseLibido(actorRef);
}

float PapyrusInterface::SetLibido(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef, float newVal)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return LibidoManager::GetSingleton()->SetBaseLibido(actorRef, newVal);
}

float PapyrusInterface::GetDaysSinceLastOrgasm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	float lastOrgasmTime = PersistedData::LastOrgasmTimeData::GetSingleton()->GetData(actorRef->formID, 0.f);
	if (lastOrgasmTime < 0) {
		lastOrgasmTime = 0;
	}

	return RE::Calendar::GetSingleton()->GetCurrentGameTime() - lastOrgasmTime;
}

bool PapyrusInterface::IsNaked(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return Utilities::Actor::IsNakedCached(actorRef);
}

bool PapyrusInterface::IsViewingNaked(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return Utilities::Actor::IsViewingNaked(actorRef);
}

bool PapyrusInterface::IsInScene(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return Utilities::Actor::IsParticipatingInScene(actorRef);
}

bool PapyrusInterface::IsViewingScene(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return Utilities::Actor::IsViewingScene(actorRef);
}

bool PapyrusInterface::IsWearingEroticArmor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	if (!Utilities::Actor::IsNakedCached(actorRef)) {
		if (const auto eroticKeyword = Settings::GetSingleton()->GetEroticArmorKeyword()) {
			const auto wornKeywords = Utilities::Actor::GetWornArmorKeywords(actorRef);
			return wornKeywords.contains(eroticKeyword->formID);
		}
	}
	return false;
}

float PapyrusInterface::WornDeviceBaselineGain(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	if (!actorRef) {
		Utilities::logInvalidArgsVerbose(a_vm, a_stackID, __FUNCTION__);
		return 0;
	}
	return DevicesIntegration::GetSingleton()->GetArousalBaselineFromDevices(actorRef);
}

bool PapyrusInterface::RegisterFunctions(RE::BSScript::IVirtualMachine* vm)
{
	vm->RegisterFunction("GetArousal", "OSLArousedNative", GetArousal);
	vm->RegisterFunction("GetArousalMultiple", "OSLArousedNative", GetArousalMultiple);
	vm->RegisterFunction("GetArousalNoSideEffects", "OSLArousedNative", GetArousalNoSideEffects);

	vm->RegisterFunction("SetArousal", "OSLArousedNative", SetArousal);
	vm->RegisterFunction("SetArousalMultiple", "OSLArousedNative", SetArousalMultiple);
	vm->RegisterFunction("ModifyArousal", "OSLArousedNative", ModifyArousal);
	vm->RegisterFunction("ModifyArousalMultiple", "OSLArousedNative", ModifyArousalMultiple);

	vm->RegisterFunction("SetArousalMultiplier", "OSLArousedNative", SetArousalMultiplier);
	vm->RegisterFunction("GetArousalMultiplier", "OSLArousedNative", GetArousalMultiplier);
	vm->RegisterFunction("ModifyArousalMultiplier", "OSLArousedNative", ModifyArousalMultiplier);

	vm->RegisterFunction("GetArousalBaseline", "OSLArousedNative", GetArousalBaseline);
	vm->RegisterFunction("GetLibido", "OSLArousedNative", GetLibido);
	vm->RegisterFunction("SetLibido", "OSLArousedNative", SetLibido);

	vm->RegisterFunction("GetDaysSinceLastOrgasm", "OSLArousedNative", GetDaysSinceLastOrgasm);

	//Explainer
	vm->RegisterFunction("IsNaked", "OSLArousedNative", IsNaked);
	vm->RegisterFunction("IsViewingNaked", "OSLArousedNative", IsViewingNaked);
	vm->RegisterFunction("IsInScene", "OSLArousedNative", IsInScene);
	vm->RegisterFunction("IsViewingScene", "OSLArousedNative", IsViewingScene);
	vm->RegisterFunction("IsWearingEroticArmor", "OSLArousedNative", IsWearingEroticArmor);
	vm->RegisterFunction("WornDeviceBaselineGain", "OSLArousedNative", WornDeviceBaselineGain);

	return true;
}
