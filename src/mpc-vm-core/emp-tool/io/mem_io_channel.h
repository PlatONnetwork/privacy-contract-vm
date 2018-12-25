#ifndef MEM_IO_CHANNEL
#define MEM_IO_CHANNEL

#include <string>
#include "io_channel.h"
#include "emp-tool/io/file_io_channel.h"
#include <condition_variable>
#include <mutex>



namespace platon
{
	namespace mpc
	{
		class MemIO : public IOChannel {
		public:
			char * buffer = nullptr;
			int64_t size = 0;
			int64_t read_pos = 0;
			int64_t cap;
			std::mutex lock_;
			std::condition_variable cond_;

			MemIO(int64_t _cap = 1024 * 1024) {
				this->cap = _cap;
				buffer = new char[cap];
				size = 0;
			}
			~MemIO() {
				if (buffer != nullptr)
					delete[] buffer;
			}
			void load_from_file(FileIO * fio, int64_t size) {
				delete[] buffer;
				buffer = new char[size];
				this->cap = size;
				this->read_pos = 0;
				this->size = size;
				fio->recv_data(buffer, size);
			}
			void clear() {
				size = 0;
			}
			
			int send_data_impl(const void * data, int len) {
				std::unique_lock<std::mutex> guard(lock_);
				if (size + len >= cap) {
					char * new_buffer = new char[2 * (cap + len)];
					memcpy(new_buffer, buffer, size);
					delete[] buffer;
					buffer = new_buffer;
					cap = 2 * (cap + len);
				}
				memcpy(buffer + size, data, len);
				size += len;

				cond_.notify_one();
				return len;
			}

			int recv_data_impl(void  * data, int len) {
				std::unique_lock<std::mutex> guard(lock_);
				while (!(read_pos + len <= size))
					cond_.wait(guard);

				//read data
				if (read_pos + len <= size) {
					memcpy(data, buffer + read_pos, len);
					read_pos += len;
				}
				else {
					fprintf(stderr, "error: mem_recv_data\n");
				}

				return len;
			}
		};
	}
}
#endif//MEM_IO_CHANNEL_H__
