#include <string.h>
#include "emp-tool/utils/com.h"


namespace platon
{
	namespace mpc
	{

		Commitment::Commitment() {}

		void Commitment::commit(Decom decom, Com com, const void * message, int nbytes) {
			prg.random_block(decom, 1);
			h.reset();
			h.put_block(decom, 1);
			h.put(message, nbytes);
			h.digest(com);
		}

		bool Commitment::open(Decom decom, Com com, const void * message, int nbytes) {
			h.reset();
			h.put_block(decom, 1);
			h.put(message, nbytes);

			Com res;
			h.digest(res);
			return strncmp(com, res, Hash::DIGEST_SIZE) == 0; 
		}
	}
}