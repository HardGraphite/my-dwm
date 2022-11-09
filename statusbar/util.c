#include "util.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

size_t read_file(const char *path, void *buf, size_t buf_sz)
{
	size_t cnt = 0;
	const int fd = open(path, O_RDONLY);
	if (fd == -1)
		return 0;

	assert(buf_sz);
	do {
		const ssize_t rd_sz = read(fd, buf, buf_sz);
		if (rd_sz <= 0)
			break;
		const size_t off = (size_t)rd_sz;
		cnt += off;
		buf = (char *)buf + off;
		buf_sz -= off;
	} while(buf_sz);

	close(fd);
	return cnt;
}

long long read_file_as_ll(const char *path, int base)
{
	char buffer[64];
	const size_t n = read_file(path, buffer, sizeof buffer);
	return n ? strtoll(buffer, &(char *){buffer + n}, base) : 0;
}

size_t str_copy(char *dest, size_t dest_sz, const char *src, size_t src_sz)
{
	char *const dest_end = dest + dest_sz;
	if (dest + src_sz > dest_end)
		src_sz = dest_sz;
	memcpy(dest, src, src_sz);
	return src_sz;
}

__attribute__((pure)) size_t u8str_charlen(const char *s)
{
	const unsigned char byte_1 = (unsigned char)*s;
	assert(byte_1);
	if ((byte_1 & 0x80) == 0)
		return 1;
	if ((byte_1 & 0xe0) == 0xc0)
		return 2;
	if ((byte_1 & 0xf0) == 0xe0)
		return 3;
	if ((byte_1 & 0xf8) == 0xf0)
		return 4;
	return 0;
}

__attribute__((pure)) const char *ramp_select(
	const char *const ramp_arr[], size_t ramp_arr_len,
	unsigned int val, unsigned int val_max)
{
	return ramp_arr[
		val < val_max ?
			val / (val_max / (unsigned int)ramp_arr_len) :
			ramp_arr_len - 1
	];
}
