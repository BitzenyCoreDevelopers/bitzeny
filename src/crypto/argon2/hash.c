#include "argon2.h"

/*
 * Type: Argon2d
 * Iterations: 1
 * Memory: 1 GiB
 * Parallelism: 1
 */

void argon2d_hash(const char *input, char *output) {
    argon2d_hash_raw(1, 1048576, 1, input, 80, input, 80, output, 32);
}
