#pragma once


#include "emp-tool/utils/hash.h"
#include "emp-tool/utils/prg.h"



namespace platon
{
	namespace mpc
	{

		typedef char Com[Hash::DIGEST_SIZE];
		typedef block Decom[1];

		class Commitment
		{
		public:
			Hash h;
			PRG prg;

			Commitment();

			void commit(Decom decom, Com com, const void * message, int nbytes);

			bool open(Decom decom, Com com, const void * message, int nbytes);
		};
	}
}
