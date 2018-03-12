#include "catapult/ionet/AppendContext.h"
#include "tests/TestHarness.h"

namespace catapult { namespace ionet {

#define TEST_CLASS AppendContextTests

	// region constructor

	namespace {
		void AssertAppendBufferSize(size_t initialSize, size_t initialCapacity, size_t appendSize, size_t expectedSize) {
			// Arrange:
			ByteBuffer buffer(initialSize);
			buffer.reserve(initialCapacity);
			AppendContext context(buffer, appendSize);

			// Assert:
			EXPECT_EQ(expectedSize, buffer.size())
					<< "initialSize: " << initialSize
					<< ", initialCapacity: " << initialCapacity
					<< ", appendSize: " << appendSize;
		}
	}

	TEST(TEST_CLASS, ConstructorResizesBufferIfLessThanHalfRequestedSizeIsAvailable) {
		// Assert:
		AssertAppendBufferSize(26, 100, 150, 176); // (100 - 26) < 150 / 2
		AssertAppendBufferSize(12, 12, 100, 112);
	}

	TEST(TEST_CLASS, ConstructorDoesNotResizeBufferIfAtLeastHalfRequestedSizeIsAvailable) {
		// Assert:
		AssertAppendBufferSize(25, 100, 150, 100); // (100 - 25) == 150 / 2
		AssertAppendBufferSize(24, 100, 150, 100); // (100 - 24) > 150 / 2
		AssertAppendBufferSize(8, 100, 50, 58);
	}

	// endregion

	// region mutable buffer

	TEST(TEST_CLASS, MutableBufferCanBeAccessedBeforeCommitWhenBufferIsResized) {
		// Arrange:
		ByteBuffer buffer(12);
		AppendContext context(buffer, 100);
		test::FillWithRandomData(buffer);

		// Assert:
		auto pContextBuffer = boost::asio::buffer_cast<uint8_t*>(context.buffer());
		auto contextBufferSize = boost::asio::buffer_size(context.buffer());
		ASSERT_EQ(100u, contextBufferSize);
		EXPECT_TRUE(std::equal(buffer.begin() + 12, buffer.end(), pContextBuffer, pContextBuffer + contextBufferSize));
	}

	TEST(TEST_CLASS, MutableBufferCanBeAccessedBeforeCommitWhenBufferIsNotResized) {
		// Arrange:
		ByteBuffer buffer(8);
		buffer.reserve(100);
		AppendContext context(buffer, 50);
		test::FillWithRandomData(buffer);

		// Assert:
		auto pContextBuffer = boost::asio::buffer_cast<uint8_t*>(context.buffer());
		auto contextBufferSize = boost::asio::buffer_size(context.buffer());
		ASSERT_EQ(50u, contextBufferSize);
		EXPECT_TRUE(std::equal(buffer.begin() + 8, buffer.end(), pContextBuffer, pContextBuffer + contextBufferSize));
	}

	TEST(TEST_CLASS, MutableBufferCannotBeAccessedAfterCommit) {
		// Arrange:
		ByteBuffer buffer(12);
		AppendContext context(buffer, 100);
		context.commit(100);

		// Act + Assert:
		EXPECT_THROW(context.buffer(), catapult_runtime_error);
	}

	// endregion

	// region commit / abandon

	TEST(TEST_CLASS, CanCommitAllReservedData) {
		// Arrange:
		ByteBuffer buffer(12);
		AppendContext context(buffer, 100);

		// Act:
		context.commit(100);

		// Assert:
		EXPECT_EQ(112u, buffer.size());
	}

	TEST(TEST_CLASS, CanCommitPartialReservedData) {
		// Arrange:
		ByteBuffer buffer(12);
		AppendContext context(buffer, 100);

		// Act:
		context.commit(63);

		// Assert:
		EXPECT_EQ(75u, buffer.size());
	}

	TEST(TEST_CLASS, CannotCommitMoreDataThanReserved) {
		// Arrange:
		ByteBuffer buffer(12);
		AppendContext context(buffer, 100);

		// Act + Assert:
		EXPECT_THROW(context.commit(101), catapult_runtime_error);
	}

	TEST(TEST_CLASS, CannotCommitDataMultipleTimes) {
		// Arrange:
		ByteBuffer buffer(12);
		AppendContext context(buffer, 100);
		context.commit(100);

		// Act + Assert:
		EXPECT_THROW(context.commit(100), catapult_runtime_error);
	}

	TEST(TEST_CLASS, CanAbandonReservedData) {
		// Arrange:
		ByteBuffer buffer(12);

		// Act:
		{
			AppendContext context(buffer, 100);
		}

		// Assert:
		EXPECT_EQ(12u, buffer.size());
	}

	TEST(TEST_CLASS, CanDestroyAfterCommitWithNoAbandonment) {
		// Arrange:
		ByteBuffer buffer(12);

		// Act:
		{
			AppendContext context(buffer, 100);
			context.commit(75);
		}

		// Assert:
		EXPECT_EQ(87u, buffer.size());
	}

	TEST(TEST_CLASS, MoveDoesNotCauseAbandonment) {
		// Arrange:
		ByteBuffer buffer(12);

		// Act:
		auto context2 = [&buffer]() {
			AppendContext context(buffer, 100);
			return context;
		}();

		// Assert:
		EXPECT_EQ(112u, buffer.size());
	}

	// endregion
}}