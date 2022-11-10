#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

#define unused_var(VAR) ((void)(VAR))

#define array_length(ARR) (sizeof (ARR) / sizeof (ARR)[0])

/* Start a subprocess. Return its PID. */
int start_process(const char *prog, char *const argv[]);

/* Send desktop notification. `urgency`: 0~2; `timeout`: expiration timeout, ms. */
int send_notification(const char *summary, const char *body, int urgency, unsigned int timeout);

/* Read file contents. */
size_t read_file(const char *path, void *buf, size_t buf_sz);

/* Read a long long int from file. */
long long read_file_as_ll(const char *path, int base);

/* Simalir to `strncpy()`, but will not append NUL and will return copied size. */
size_t str_copy(char *dest, size_t dest_sz, const char *src, size_t src_sz);

/* Calculate string hash code. */
__attribute__((pure)) size_t str_hash(const char *str, size_t len);

/* Get number of bytes in a UTF-8 character. If it is invalid, return 0. */
__attribute__((pure)) size_t u8str_charlen(const char *s);

/* Select ramp string from an array according to `val` (0 ~ `val_max`). */
__attribute__((pure)) const char *ramp_select(
	const char *const ramp_arr[], size_t ramp_arr_len,
	unsigned int val, unsigned int val_max);

#endif /* UTIL_H */
