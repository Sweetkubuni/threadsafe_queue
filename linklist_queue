#include <utility>
#include <mutex>
#include <memory>

template<typename T>
class queue
{
	private:
	class node
	{
		T val;
		std::unique_ptr<node> next;
	};
	
	std::mutex mutex_front;
	std::mutex mutex_end;
	
	std::unique_ptr<node> front;
	node * end;
	
	node* get_end()
	{
		std::lock_guard<std::mutex> lk(mutex_end);
		return end;
	}
	std::unique_ptr<node> pop_front()
	{
		std::lock_guard<std::mutex> lk(mutex_front);
		if(front.get()==get_end())
			return nullptr;
		std::unique_ptr<node> old_front = std::move(front);
		front = std::move(old_front->next);
		return old_front;
	}
	public:
	queue():front(new node),end(front.get()) {  }
	
	queue(const queue& other)=delete;
	queue& operator=(const queue& other)=delete;
	
	bool pop(T & item)
	{
		std::unique_ptr<node> old_front =pop_front();
		if(old_front)
		{
			item = std::move(old_front->val);
			return true;
		}
			
			return false;
	}
	
	void push(T value )
	{
		std::unique_ptr<node> p(new node);
		const node* new_end=p.get();
		std::lock_guard<std::mutex> lk(mutex_end);
		end->val=value;
		end->next=std::move(p);
		end=new_end;	
	}
};
