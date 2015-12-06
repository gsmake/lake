#ifndef LEMON_IO_BASIC_IO_SERVICE_HPP
#define LEMON_IO_BASIC_IO_SERVICE_HPP

#include <mutex>
#include <memory>
#include <utility>
#include <unordered_map>

namespace lemon{ namespace io{ namespace async{

	template<typename Impl,typename IrpBase,typename Mutex>
	class basic_io_service 
	{
	public:
		typedef Impl												impl_type;

		typedef Mutex												mutex_type;

		typedef IrpBase												irp_base_type;

		typedef std::unordered_map<
			irp_base_type*,std::unique_ptr<irp_base_type>>			irps_type;

		basic_io_service(basic_io_service && rhs)
			:_impl(rhs._impl)
		{

		}

		basic_io_service():_impl(new impl_type())
		{
			
		}

		template<typename Irp,typename ... Args>
		Irp* create_irp(Args &&... args)
		{
			auto irp = new Irp(std::forward<Args>(args) ...);

			std::lock_guard<mutex_type> lock(_mutex);

			_irps.insert(irp,irp);

			return irp;
		}

		template<typename Irp>
		void close_irp(Irp * irp)
		{
			std::lock_guard<mutex_type> lock(_mutex);

			if(_irps.count(irp) != 0)
			{
				_irps.erase(irp);
			}
		}

	private:

		mutex_type									_mutex;

		std::unique_ptr<impl_type>					_impl;

		irps_type									_irps;
	};

}}}

#endif //LEMON_IO_BASIC_IO_SERVICE_HPP

