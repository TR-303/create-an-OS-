typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;

#define SELECTOR_KERNEL_CODE (0x8 | 0b000)
#define SELECTOR_KERNEL_DATA (0x10 | 0b000)
#define SELECTOR_VIDEO (0x18 | 0b011)
#define SELECTOR_USER_CODE (0x20 | 0b011)
#define SELECTOR_USER_DATA (0x28 | 0b011)
#define SELECTOR_TSS (0x30 | 0b000)

void memset(void *ptr, int value, uint32_t num);

void delay(unsigned int ms);
