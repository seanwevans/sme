#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t pack(uint8_t *unpacked) {
  uint64_t packed = 0;
  for (int i = 0; i < 8; i++) {
    packed |= ((uint64_t)unpacked[i]) << (i * 8);
  }
  return packed;
}

void unpack(uint64_t packed, uint8_t *unpacked) {
  for (int i = 0; i < 8; i++) {
    unpacked[i] = (packed >> (i * 8)) & 0xFF;
  }
}

int main() {
  uint8_t original[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  for (int i = 0; i < 8; i++)
    printf("%02X ", original[i]);

  printf("\n");

  uint64_t packed = pack(original);
  printf("Packed: %llu\n", (unsigned long long)packed);

  uint8_t unpacked[8];
  unpack(packed, unpacked);
  printf("Unpacked: ");
  for (int i = 0; i < 8; i++)
    printf("%02X ", unpacked[i]);

  printf("\n");

  return 0;
}
