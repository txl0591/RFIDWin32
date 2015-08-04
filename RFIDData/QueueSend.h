#ifndef _QUEUESEND_
#define _QUEUESEND_

#include <iostream>

using namespace std;

//Queue��ʵ����  
//�������ض���ģ����Ԫ��ϵ���������ֻ��Ȩ�ض���ʵ���ķ���Ȩ������Queue����QueueItem��һ��һ�Ĺ�ϵ  
template<class T> class Queue;  
template <class T>  
class QueueItem  
{  
    //��ΪQueue����Ҫʹ�õ�����������ӿڵ����еĳ�Ա,������Ҫ��Queue����Ϊ��Ԫ��  
    friend class Queue<T>;  
private:  
    //���ƹ��캯��  
    QueueItem<T>( const T &q ): item( q ) , next( 0 )  {}  
      
      
    //Ԫ��ֵ  
    T item;  
      
    //ָ����һ��Ԫ�ص�ָ��  
    QueueItem<T> *next;  
};  
  
//��ģ��  
template <class T>  
class Queue  
{  
public:  
    //��Ҫע���һ����,һ��������ʵ�ֵĺ�����������������  
    //ʹ������������Ҫ����,�ú����Ĵ���һ��ֻ�м���,���Ҿ���ִ��  
      
      
      
    //Ĭ�Ϲ��캯��  
      
    //����������Ҳ�ǿ��Ե�  
    Queue<T>() :head( 0 ) , tail( 0 ) {}  
      
      
      
    //���ƹ��캯��  
      
    //��Ȼ����������Ҳ�ǿ��Ե�  
    //Queue( const Queue &q ): head( q.head ) , tail( q.tail ) { copy_elems( q ); }  
    Queue<T>( const Queue<T> &q) :head( q.head ) , tail( q.tail ) { copy_elem( q ); }  
      
      
      
    //����������  
    Queue<T>& operator= ( const Queue<T>& );  
      
      
    //��������  
    ~Queue(){ destroy(); }    
      
      
    //��ȡͷ�ڵ��Ԫ��  
    T& front() { return head->item; }  
      
      
    //�����  
    void push( const T& );  
      
    //������  
    void pop();  
      
    //�ж��Ƿ�Ϊ��  
    bool empty() const { return NULL == head; }  
      
      
    //��ʾ����Ԫ��  
    void ShowElements() ;  
      
      
      
private:  
    //���е�ͷָ��,βָ��,��Ҫ���ڳ��������  
    QueueItem<T> *head;  
    QueueItem<T> *tail;  
      
    //���ٶ�������  
    void destroy();  
      
    //����Ԫ��  
    void copy_elems( const Queue& );  
};  
  
//������,��ɾ������ͷ����Ԫ��  
template<typename T>   
void Queue<T>::pop()  
{     
    //�ж��Ƿ�Ϊ��ָ��  
    if( NULL == head )  
    {  
        return ;  
    }  
      
    //���浱ǰָ���ֵ  
    QueueItem<T> *p = head;  
      
    //��ͷָ��ָ����һ��Ԫ��  
    head = head->next;  
      
    //ɾ��  
    delete p;  
}  
  
  
//�����,���Ӷ��е�β����������  
template<typename T>  
void Queue<T>::push( const T &t )  
{  
    //����һ������  
    QueueItem<T> *p = new QueueItem<T>( t );  
      
      
    //�ڲ������β����ʱ����Ҫ�ж϶����Ƿ�Ϊ�յģ�  
    if( empty() )  
    {  
        head = tail = p;  
    }  
    else  
    {  
        //��βָ���ָ����һ��Ԫ�ص�ָ��ָ�����ɵ�����  
        tail->next = p;  
          
        //��βָ���ƶ������һ��������ȥ  
        tail = p;  
    }  
}  
  
  
  
//��������  
template<typename T>  
void Queue<T>::destroy()  
{  
    //���ϵس����м���  
    while( !empty() )  
    {  
        pop();  
    }  
}  
  
  
//��ֵ����������  
template<typename T>  
Queue<T>& Queue<T>::operator= (const Queue<T> &q)  
{  
    //���ƶ���Ԫ��,����������headΪ�յ�ʱ��  
    for( QueueItem<T> *p= q.head ; p  ; p = p->next )  
    {  
        push( p->item );  
    }  
      
}  
  
#endif