#pragma once
#ifndef CUSTOM_RESULT_DEFINITION
#include "catapult/validators/ValidationResult.h"

namespace catapult { namespace validators {

#endif
/// Defines an aggregate validation result with \a DESCRIPTION and \a CODE.
#define DEFINE_AGGREGATE_RESULT(DESCRIPTION, CODE) DEFINE_VALIDATION_RESULT(Failure, Aggregate, DESCRIPTION, CODE, None)

	/// Validation failed because aggregate has too many transactions.
	DEFINE_AGGREGATE_RESULT(Too_Many_Transactions, 0x0001);

	/// Validation failed because aggregate does not have any transactions.
	DEFINE_AGGREGATE_RESULT(No_Transactions, 0x0002);

	/// Validation failed because aggregate has too many cosignatures.
	DEFINE_AGGREGATE_RESULT(Too_Many_Cosignatures, 0x0003);

	/// Validation failed because redundant cosignatures are present.
	DEFINE_AGGREGATE_RESULT(Redundant_Cosignatures, 0x0004);

	/// Validation failed because at least one cosigner is ineligible.
	DEFINE_AGGREGATE_RESULT(Ineligible_Cosigners, 0x1001);

	/// Validation failed because at least one required cosigner is missing.
	DEFINE_AGGREGATE_RESULT(Missing_Cosigners, 0x1002);

#ifndef CUSTOM_RESULT_DEFINITION
}}
#endif