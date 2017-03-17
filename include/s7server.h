#ifndef S7SERVER_H
#define S7SERVER_H

#include "snap7.h"

class S7Server : public TS7Server
{
	public:
		S7Server();
		void setAngles(unsigned int*);

		union {
			unsigned char a[256];
			struct {
				unsigned char winkel[14];
				unsigned char lin_ein : 1;
				unsigned char lin_reset : 1;
				unsigned char lin_grundstellung : 1;
			} s;
		} DB_Send;

		union {
			unsigned char a[256];
			struct {
				uint16_t lin_status;

			} s;
		} DB_Recv;
};

#endif
