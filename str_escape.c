#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int inCharset(const char q, const char *charset) {
  const char *ref;
  for (ref=charset; *ref != '\0'; ref++) {
    if (q == *ref) return 1;
  }

  return 0;
}

// strEscape escapes every character in a character set `charset`
// replacing each occurrance in `src` in with \\<character>
char *strEscape(const char *src, const char *charset) {
  if (src == NULL || charset == NULL)
    return NULL;

  if (strlen(src) < 1 || strlen(charset) < 1)
    return strdup(src);

  size_t srcSize = strlen(src);
  size_t bufferSize = srcSize + (2 + (srcSize / 8));

  char *buffer = malloc(sizeof(*buffer) * bufferSize);
  if (buffer == NULL)
    return NULL;

  char *oldBuffer;
  size_t bufI = 0;

  while (*src) {
    if (inCharset(*src, charset)) {
      // Time for some reallocation here
      size_t budget = bufI + 3; // 2 backslashes and one char

      if (budget >= bufferSize) {
	oldBuffer = buffer;
	bufferSize += budget + 5;
	buffer = realloc(buffer, bufferSize);
	if (buffer == NULL) {
	  buffer = oldBuffer;
	  goto freeUpBuffer;
	}
      }

      // Otherwise now concatenate those two backslashes but also
      // track the new position of the buffer's moving index.
      bufI += sprintf(buffer+bufI, "\\");
    }

    buffer[bufI++] = *src++;
  }

  oldBuffer = buffer;
  buffer = realloc(buffer, bufI+1);
  if (buffer == NULL)
    goto freeUpBuffer;

  buffer[bufI] = '\0';
  return buffer;

freeUpBuffer:
  free(buffer);
  return NULL;
}

#ifdef RUN_TESTS
typedef struct {
  char *src;
  char *set;
  char *want;
  char *comment;
} testCase;

int main() {
  
  testCase testTable[] = {
    {.src="hello", .set=" e", .want="h\\ello", .comment="must escape hello trivially"},
    {.src="", .set="  ", .want="", .comment="should return the empty string"},
    {.src="   ", .set="fl", .want="   ", .comment="untouched"},
    {.src="What is this one called", .set="i ", .want="What\\ \\is\\ th\\is\\ one\\ called", .comment="untouched"},
    {.src="1234", .set="1234", .want="\\1\\2\\3\\4", .comment="escape them all"},
    {.src="", .set="1234", .want="", .comment="escape them all"},
    {
      .set="",
      .src=" and what was that? and how about this one and those ones right thuuurrrr",
      .want=" and what was that? and how about this one and those ones right thuuurrrr",
      .comment="no charset passed in"
    }
  };

  size_t i;
  size_t len = sizeof(testTable)/sizeof(testTable[0]);

  int failures = 0;
  int passes = 0;

  for (i=0; i < len; i++) {
    const testCase tt = testTable[i];
    char *got = strEscape(tt.src, tt.set);
    if (strcmp(got, tt.want) != 0) {
      failures += 1;
      fprintf(stderr, "#%zu got=%s want=%s; comment=%s\n", i, got, tt.want, tt.comment);
    } else {
      passes += 1;
    }

    free(got);
  }

  if (failures >= 1) {
    fprintf(stderr, "\033[32mPasses: %d\n\033[31mFailures: %d\033[00m\n", passes, failures);
    return -1;
  }

  return 0;
}
#endif // RUN_TESTS
