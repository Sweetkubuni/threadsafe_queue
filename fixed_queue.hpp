#include <utility>
#include <mutex>
#include <condition_variable>

template < typename T, std::size_t N>
class queue
{
	protected:
    struct node
    {
        T val;
        // checks if data has been written to node
        bool dirty;
    };
	node data[N];

	// increments both sides of the queue
	unsigned int front;
	unsigned int end;

    // parameters used for threadsafety
	std::mutex mutex_front;
	std::mutex mutex_end;
	std::condition_variable check;
	int num_elements;
	public:
	queue()
	{
		front = 0;
		end = 0;
		num_elements = 0;
		for( auto & datum : data )
            datum.dirty = false;
	}

	bool push(T datum)
	{
	    std::unique_lock<std::mutex> lk(mutex_end);
	    if( data[end].dirty )
            check.wait(lk,[this]{ return front != end;});

        if( !data[end].dirty )
        {
            data[end].val = std::move(datum);
            data[end].dirty = true;
            ++num_elements;
            ++end;
            if( end  == N )
                end = 0;
            lk.unlock();
            return true;
        }

        lk.unlock();
        return false;
	}

	bool pop(T & datum)
	{
	    std::lock_guard<std::mutex> lock_front(mutex_front);
        if(data[front].dirty)
        {
            datum = std::move(data[front].val);
            data[front].dirty = false;
            ++front;
            if( front == N)
                front = 0;
            --num_elements;
            check.notify_one();
            return true;
        }
        return false;
	}

	bool empty()
	{
        if(num_elements == 0)
            return true;
        else
            return false;
	}
};

