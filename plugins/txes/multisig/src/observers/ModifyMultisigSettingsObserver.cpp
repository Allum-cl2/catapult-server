#include "Observers.h"
#include "src/cache/MultisigCache.h"

namespace catapult { namespace observers {

	using Notification = model::ModifyMultisigSettingsNotification;

	namespace {
		constexpr uint8_t AddDelta(uint8_t value, int8_t delta) {
			return value + static_cast<uint8_t>(delta);
		}
	}

	DEFINE_OBSERVER(ModifyMultisigSettings, Notification, [](const Notification& notification, const ObserverContext& context) {
		auto& multisigCache = context.Cache.sub<cache::MultisigCache>();
		if (!multisigCache.contains(notification.Signer))
			return;

		auto& multisigEntry = multisigCache.get(notification.Signer);

		int8_t direction = NotifyMode::Commit == context.Mode ? 1 : -1;
		multisigEntry.setMinApproval(AddDelta(multisigEntry.minApproval(), direction * notification.MinApprovalDelta));
		multisigEntry.setMinRemoval(AddDelta(multisigEntry.minRemoval(), direction * notification.MinRemovalDelta));
	});
}}