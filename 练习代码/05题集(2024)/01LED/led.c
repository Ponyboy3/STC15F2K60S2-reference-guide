#include "reg52.h"


void main()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0x0f;
	while (1) {
		
	}
}