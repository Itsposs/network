
#ifndef _LST_TIMER_H_
#define _LST_TIMER_H_

#define BUFFER_SIZE 64


#include <time.h>
#include <netinet/in.h>

class util_timer;

struct client_data
{
	sockaddr_in address;
	int sockfd;
	char buf[BUFFER_SIZE];
	util_timer *timer;
};

class util_timer
{
  public:
	util_timer() : prev(NULL), next(NULL) {}
  public:
	time_t expire;
	void (*cb_func) (client_data *);
	client_data *user_data;
	util_timer *prev;
	util_timer *next;
};

class sort_timer_lst
{
  public:
	sort_timer_lst() : head(NULL), tail(NULL) {}
	~sort_timer_lst()
	{
	  util_timer *tmp = head;
	  while (tmp)
	  {
		head = tmp -> next;
		delete tmp;
		tmp = head;
	  }
	}
	
	void add_timer(util_timer *timer)
	{
	  if (!timer) return;
	  
	  if (!head)
	  {
		head = tail = timer;
		return;
	  }
	  // 插入链表头部
	  if (timer -> expire < head -> expire)
	  {
		timer -> next = head;
		head -> prev = timer;
		head = timer;
		return;
	  }
	  // 插入链表当中合适的位置
	  add_timer(timer, head);
	}
	
	void adjust_timer(util_timer *timer)
	{
	  if (!timer) return;

	  // 调整目标在链表尾部或仍然小于下一个定时器的超时值，不调整
	  util_timer *tmp = timer -> next;
	  if (!tmp || (timer -> expire < tmp -> expire))
	  {
		return;
	  }
	  
	  // 调整目标定时器处在链表尾部
	  if (timer == head)
	  {
		head = head -> next;
		head -> prev = NULL;
		timer -> next = NULL;
		add_timer(timer, head);
	  }
	  else   // 目标不在尾部
	  {
		timer -> prev -> next = timer -> next;
		timer -> next -> prev = timer -> prev;
		add_timer(timer, timer -> next);
	  }
	}
	
	void del_timer(util_timer *timer)
	{
	  if (!timer) return;
	  
	  // 只有一个定时器，即目标定时器
	  if ((timer == head) && (timer == tail))
	  {
		delete timer;
		head = NULL;
		tail = NULL;
		return;
	  }
	  // 链表中至少有两个定时器,且目标定时器是链表的头部
	  if (timer == head)
	  {
		head = head -> next;
		head -> prev = NULL;
		delete timer;
		return;
	  }
	  // 链表中至少有两个定时器,且目标定时器是链表的尾部
	  if (timer == tail)
	  {
		tail = tail -> prev;
		tail -> next = NULL;
		delete timer;
		return;
	  }
	  // 目标定时器位于链表的中间
	  timer -> prev -> next = timer -> next;
	  timer -> next -> prev = timer -> prev;
	  delete timer;
	}

	void tick()
	{
	  if (!head)
	  {
		return;
	  }
	  printf("timer tick.\n");
	  time_t cur = time(NULL);
	  util_timer *tmp = head;
	  
	  while (tmp)
	  {
		// 定时期未到期
		if (cur < tmp -> expire)
		{
		  break;
		}
		tmp -> cb_func(tmp -> user_data);
		head = tmp -> next;
		
		// 重置指针
		if (head)
		{
		  head -> prev = NULL;
		}
		
		delete tmp;
		tmp = head;
	  }
	}
  
  private:
	util_timer *head;
	util_timer *tail;

	void add_timer(util_timer *timer, util_timer *lst_head)
	{
	  util_timer *prev = lst_head;
	  util_timer *tmp = prev -> next;
	  // 找到合适的位置,插入节点
	  while (tmp)
	  {
		if (timer -> expire < tmp -> expire)
		{
		  prev -> next = timer;
		  timer -> next = tmp;
		  tmp -> prev = timer;
		  timer -> prev = prev;
		  break;
		}
		prev = tmp;
		tmp = tmp -> next;
	  }
	  // 未找到,插入链表尾部,成为链表新的尾节点
	  if (!tmp)
	  {
		prev -> next = timer;
	    timer -> prev = prev;
		timer -> next = NULL;
		tail = timer;
	  }
	}
};


#endif  //_LST_TIMER_H_
