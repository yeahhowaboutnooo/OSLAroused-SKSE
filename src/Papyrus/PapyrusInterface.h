#pragma once
#include "PCH.h"

namespace PapyrusInterface
{
	using VM = RE::BSScript::Internal::VirtualMachine;
	using StackID = RE::VMStackID;

	float GetArousal(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);
	float GetArousalNoSideEffects(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);
	std::vector<float> GetArousalMultiple(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs);

	float SetArousal(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	void SetArousalMultiple(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs, float value);

	float ModifyArousal(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	void ModifyArousalMultiple(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs, float value);

	float GetArousalMultiplier(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);
	float SetArousalMultiplier(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	float ModifyArousalMultiplier(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);

	float GetArousalBaseline(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);

	float GetLibido(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);
	float SetLibido(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef, float newVal);

	
	float GetDaysSinceLastOrgasm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);

	bool IsNaked(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);
	bool IsViewingNaked(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);
	bool IsInScene(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);
	bool IsViewingScene(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);
	bool IsWearingEroticArmor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);
	float WornDeviceBaselineGain(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);


	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);
}
