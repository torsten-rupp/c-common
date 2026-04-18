#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/global.h"
#include "errors.h"

#include "common/patterns.h"

CTEST(patterns,init_done)
{
  Pattern pattern;
  ASSERT_TRUE(Pattern_initCString(&pattern, "foo*", PATTERN_TYPE_GLOB, PATTERN_FLAG_NONE) == ERROR_NONE);
  Pattern_done(&pattern);
  ASSERT_TRUE(Pattern_initCString(&pattern, "^foo.*bar$", PATTERN_TYPE_REGEX, PATTERN_FLAG_NONE) == ERROR_NONE);
  Pattern_done(&pattern);
  ASSERT_TRUE(Pattern_initCString(&pattern, "foo+(bar)?", PATTERN_TYPE_EXTENDED_REGEX, PATTERN_FLAG_NONE) == ERROR_NONE);
  Pattern_done(&pattern);
  ASSERT_FALSE(Pattern_initCString(&pattern, "[unclosed", PATTERN_TYPE_REGEX, PATTERN_FLAG_NONE) == ERROR_NONE);
}

CTEST(patterns,new_delete)
{
  String string = String_newCString("foo*");
  Pattern *pattern = Pattern_new(string, PATTERN_TYPE_GLOB, PATTERN_FLAG_NONE);
  ASSERT_TRUE(pattern != NULL);
  String string2 = String_newCString("foobar");
  ASSERT_TRUE(Pattern_match(pattern, string2, STRING_BEGIN, PATTERN_MATCH_MODE_EXACT, NULL, NULL));
  String_delete(string2);
  Pattern_delete(pattern);
  String_delete(string);

  /* invalid pattern */
  string = String_newCString("[bad");
  ASSERT_FALSE(Pattern_new(string, PATTERN_TYPE_REGEX, PATTERN_FLAG_NONE) != NULL);
  String_delete(string);
}

CTEST(patterns,duplicate_copy)
{
  Pattern sourcePattern;
  Pattern_initCString(&sourcePattern, "foo*", PATTERN_TYPE_GLOB, PATTERN_FLAG_NONE);

  Pattern *duplicatedPattern = Pattern_duplicate(&sourcePattern);
  ASSERT_TRUE(duplicatedPattern != NULL);
  Pattern_delete(duplicatedPattern);

  Pattern copiedPattern;
  ASSERT_TRUE(Pattern_copy(&copiedPattern, &sourcePattern) == ERROR_NONE);
  Pattern_done(&copiedPattern);

  Pattern_done(&sourcePattern);
}

CTEST(patterns,patternTypeToString)
{
  ASSERT_STR("glob",Pattern_patternTypeToString(PATTERN_TYPE_GLOB, "?"));
  ASSERT_STR("regex",Pattern_patternTypeToString(PATTERN_TYPE_REGEX, "?"));
  ASSERT_STR("extended_regex",Pattern_patternTypeToString(PATTERN_TYPE_EXTENDED_REGEX, "?"));
  ASSERT_STR("default",Pattern_patternTypeToString(PATTERN_TYPE_UNKNOWN, "default"));
  ASSERT_TRUE(Pattern_patternTypeToString(PATTERN_TYPE_UNKNOWN, NULL) == NULL);
}

CTEST(patterns,parsePatternType)
{
  PatternTypes patternType;
  ASSERT_TRUE(Pattern_parsePatternType("glob", &patternType, NULL) && patternType == PATTERN_TYPE_GLOB);
  ASSERT_TRUE(Pattern_parsePatternType("regex", &patternType, NULL) && patternType == PATTERN_TYPE_REGEX);
  ASSERT_TRUE(Pattern_parsePatternType("extended_regex", &patternType, NULL) && patternType == PATTERN_TYPE_EXTENDED_REGEX);
  ASSERT_TRUE(Pattern_parsePatternType("GLOB", &patternType, NULL) && patternType == PATTERN_TYPE_GLOB);
  ASSERT_FALSE(Pattern_parsePatternType("unknown_type", &patternType, NULL));
  ASSERT_FALSE(Pattern_parsePatternType("", &patternType, NULL));
}

CTEST(patterns,match)
{
  String string = String_newCString("foo*");
  Pattern *pattern = Pattern_new(string, PATTERN_TYPE_GLOB, PATTERN_FLAG_NONE);
  ASSERT_TRUE(pattern != NULL);
  String string2 = String_newCString("foobar");
  ASSERT_TRUE(Pattern_match(pattern, string2, STRING_BEGIN, PATTERN_MATCH_MODE_EXACT, NULL, NULL));
  String_delete(string2);
  Pattern_delete(pattern);
  String_delete(string);
}

CTEST(patterns,checkIsPattern)
{
  String globStarString = String_newCString("foo*");
  ASSERT_TRUE(Pattern_checkIsPattern(globStarString));
  String_delete(globStarString);

  String globQuestionString = String_newCString("foo?");
  ASSERT_TRUE(Pattern_checkIsPattern(globQuestionString));
  String_delete(globQuestionString);

  String charClassString = String_newCString("foo[bar]");
  ASSERT_TRUE(Pattern_checkIsPattern(charClassString));
  String_delete(charClassString);

  String braceString = String_newCString("foo{bar}");
  ASSERT_TRUE(Pattern_checkIsPattern(braceString));
  String_delete(braceString);

  String plainString = String_newCString("foobar");
  ASSERT_FALSE(Pattern_checkIsPattern(plainString));
  String_delete(plainString);

  String emptyString = String_newCString("");
  ASSERT_FALSE(Pattern_checkIsPattern(emptyString));
  String_delete(emptyString);

  String escapedStarString = String_newCString("foo\\*");
  ASSERT_FALSE(Pattern_checkIsPattern(escapedStarString));
  String_delete(escapedStarString);
}

int main(int argc, const char *argv[])
{
  Pattern_initAll();
  int exitcode = ctest_main(argc, argv);
  Pattern_doneAll();

  return exitcode;
}
