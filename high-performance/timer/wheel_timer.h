
#ifndef _WHEEL_TIMER_H_
#define _WHEEL_TIMER_H_

#include <time.h>
#include <stdio.h>
#include <netinet/in.h>

#define BUFFER_SIZE 64;

class tw_timer;

struct client_data
{
	sockaddr_in address;
	int sockfd;
	char buf[BUFFER_SIZE];
	tw_timer *timer;
};

class tw_timer
{
	public:
		tw_timer(int rot, int ts)
			: next(NULL), prev(NULL), rotation(rot), time_slot(ts) {}

	public:
		int rotation;  // 记录定时器在时间轮转多少圈后生效
		int time_slot; // 记录定时器属于时间轮上那个槽
		void (*cb_func)(client_data *);
		client_data *user_data;
		
		tw_timer *next;
		tw_timer *prev;
};

class time_wheel
{
	public:
		time_wheel() : cur_slot(0)
		{
			// 初始化每个槽的头结点
			for (int i = 0; i < N; ++i)  slots[i] = NULL;
		}
		~time_wheel()
		{
			// 遍历每个槽,并销毁其中的定时器
			for (int i = 0; i < N; ++i)
			{
				tw_timer *tmp = slots[i];
				while (tmp)
				{
					slots[i] = tmp -> next;
					delete tmp;
					tmp = slots[i];
				}
			}
		}
		// 创建一个定时器,并把它插入合适的槽中
		tw_timer* add_timer(int timeout)
		{
			if (timeout < 0) return NULL:
			
			// 计算它将在时间轮转动多少个滴答后被触发,并将该滴答存储于变量ticks中
			int ticks = 0;
			if (timeout < SI) 
				ticks = 1;
			else
				ticks = timeout / SI;
			// 计算待插入定时器在时间轮转动多少圈后被触发
			int rotation = ticks / N;
			// 计算待插入的定时器应该被插入那个槽中
			int ts = (cur_slot + (ticks % N)) % N;
			// 创建新的定时器,它在时间轮转动rotation圈之后被触发,且位于第ts个槽上
			tw_timer *timer = new tw_timer(rotation, ts);
			
			// 如果第ts个槽中尚无任何定时器,则把新建的定时器插入其中,
			// 并将该定时器设置为该槽的头结点
			if (!slots[ts])
			{
				printf("add timer, rotation is %d, ts is %d, cur_slot is %d\n",
						rotation, ts, cur_slot);
				slots[ts] = timer;
			}
			else // 否则,将定时器插入第ts个槽中
			{
				timer -> next = slots[ts];
				slots[ts] -> prev = timer;
				slots[ts] = timer;
			}
			return timer;
		}
		// 删除目标定时器timer
		void del_timer(tw_timer *timer)
		{
			if (!timer) return;

			int ts = timer -> time_slot;
			// slots[ts]目标定时器所在槽的头结点.如果目标定时器就是
			// 该头结点,则需要重置第ts个槽的头结点
			if (timer == slots[ts])
			{
				slots[ts] = slots[ts] -> next;
				if (slots[ts])
				{
					slots[ts] -> prev = NULL:
				}
				delete timer;
			}
			else
			{
				timer -> prev -> next = timer -> next;
				if (timer -> next)
				{
					timer -> next -> prev = timer -> prev;
				}
				delete timer;
			}
		}
		// SI时间到后,调用该函数,时间轮向前滚动一个槽的间隔
		void tick() 
		{
			tw_timer *tmp = slots[cur_slot]; // 取得时间轮上当前槽的头结点
			printf("current slot is %d\n", cur_slot);
			while (tmp)
			{
				printf("tick the timer once.\n");
				// 如果定时器的rotation值大于0,则它在这一轮不起作用
				if (tmp -> rotation > 0)
				{
					tmp -> rotation--;
					tmp = tmp -> next;
				}
				else
				{
					// 否则,说明定时器已经到期,于是执行定时任务,然后删除该定时器
					tmp -> cb_func(tmp -> user_data);
					// 如果该定时器是时间轮上当前槽的头结点
					if (tmp == slots[cur_slot])
					{
						printf("delete header in cur_slot.\n");
						slots[cur_slot] = tmp -> next;
						delete tmp;
						if (slots[cur_slot])
						{
							slots[cur_slot] -> prev = NULL;
						}
						tmp = slots[cur_slot];
					}
					else // 否则
					{
						tmp -> prev -> next = tmp -> next;
						if (tmp -> next)
						{
							tmp -> next -> prev = tmp -> prev;
						}
						tw_timer * tmp2 = tmp -> next;
						delete tmp;
						tmp = tmp2;
					}
				}
			}
			cur_slot = ++cur_slot % N;  // 更新时间轮的当前槽,以反映时间轮的转动
		}

	private:
		// 时间轮上槽的数目
		static const int N = 60;
		// 每1s转动一次，即时间间隔为1s
		static const int SI = 1;
		// 实践轮的槽，其中每个元素指向一个定时器链表，链表无序
		tw_timer *slots[N];  
		int cur_slot;        // 时间轮当前槽

};

#endif // _WHEEL_TIMER_H_
