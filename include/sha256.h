#include <stdint.h>

// Creates a SHA-256 hash and returns the last 64bit integer
uint64_t create_hash(char *string);