#ifndef DPRINT_HEADER
#define DPRINT_HEADER

#ifdef DEBUG
		#define dprintf(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
		#define dprintf(fmt, ...) do {} while (0)
#endif

#endif

