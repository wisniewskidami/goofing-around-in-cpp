#ifndef GOOFING_AROUND_IN_CPP_RUNNABLE_HPP
#define GOOFING_AROUND_IN_CPP_RUNNABLE_HPP

class Runnable
{
	protected:
		bool isRunning;
	public:
		Runnable() = default;
		virtual ~Runnable() = default;

		virtual void run ( ) = 0;
		virtual void stop ( ) = 0;
		virtual bool connected ( ) = 0;
};

#endif //GOOFING_AROUND_IN_CPP_RUNNABLE_HPP
