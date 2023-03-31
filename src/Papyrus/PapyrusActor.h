#pragma once

namespace PapyrusActor
{
	using VM = RE::BSScript::Internal::VirtualMachine;
	using StackID = RE::VMStackID;

	std::vector<RE::TESForm*> GetAllEquippedArmor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);

	std::vector<int> GetActiveDeviceTypeIds(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag* base, RE::Actor* actorRef);


	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);
};
