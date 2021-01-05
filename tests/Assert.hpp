#pragma once
#include <cstdio>

#define Assert(e) ++axl::Assert::NUM_TOTAL_ASSERTIONS; ((e) ? (void)0 : axl::Assert::assertionFailed(false, #e, __FILE__, __LINE__))
#define Asserte(e) ++axl::Assert::NUM_TOTAL_ASSERTIONS; ((e) ? (void)0 : axl::Assert::assertionFailed(true, #e, __FILE__, __LINE__))
#define Assertv(e, verbose) ++axl::Assert::NUM_TOTAL_ASSERTIONS; ((e) ? (verbose ? axl::Assert::assertionSucceeded(#e) : (void)0) : axl::Assert::assertionFailed(false, #e, __FILE__, __LINE__))
#define Assertve(e, verbose) ++axl::Assert::NUM_TOTAL_ASSERTIONS; ((e) ? (verbose ? axl::Assert::assertionSucceeded(#e) : (void)0) : axl::Assert::assertionFailed(true, #e, __FILE__, __LINE__))

namespace axl {
namespace Assert {

static int NUM_FAILED_ASSERTIONS = 0, NUM_TOTAL_ASSERTIONS = 0;

void assertionFailed(bool exit_after, const char* e, const char* file, int line)
{
	fprintf(stderr, "* Fail - %s | %s:%d\n", e, file, line);
	++NUM_FAILED_ASSERTIONS;
	if(exit_after) exit(1);
}

void assertionSucceeded(const char* e)
{
	printf("  Pass + %s\n", e);
}

} // namespace axl.Assert
} // namespace axl