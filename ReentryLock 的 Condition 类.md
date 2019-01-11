### ReentryLock 的 Condition 类



```
public class ConditionObject implements Condition, java.io.Serializable
```



#### await 的实现



```java
public final void await() throws InterruptedException {
    //1.中断异常检测
    if (Thread.interrupted())
        throw new InterruptedException();
    //2.添加等待者节点
    Node node = addConditionWaiter();
    int savedState = fullyRelease(node);
    int interruptMode = 0;
    while (!isOnSyncQueue(node)) {
        //3。等待
        LockSupport.park(this);
        if ((interruptMode = checkInterruptWhileWaiting(node)) != 0)
            break;
    }
    if (acquireQueued(node, savedState) && interruptMode != THROW_IE)
        interruptMode = REINTERRUPT;
    if (node.nextWaiter != null) // clean up if cancelled
        unlinkCancelledWaiters();
    if (interruptMode != 0)
        reportInterruptAfterWait(interruptMode);
}
```

一个结构`Node`，一个主要功能实现`LockSupport.park(this);`

- Node

  ```java
  private Node addConditionWaiter() {
      Node t = lastWaiter;
      // If lastWaiter is cancelled, clean out.
      if (t != null && t.waitStatus != Node.CONDITION) {
          unlinkCancelledWaiters();
          t = lastWaiter;
      }
      Node node = new Node(Thread.currentThread(), Node.CONDITION);
      if (t == null)
          firstWaiter = node;
      else
          t.nextWaiter = node;
      lastWaiter = node;
      return node;
  }
  ```

ConditionObject 中是用 lastWaiter 标记最后一个等待者，维护一个 firstWaiter，和lastWaiter 指向的链表

```java
/** First node of condition queue. */
private transient Node firstWaiter;
/** Last node of condition queue. */
private transient Node lastWaiter;
```

再来看看Node 类的定义，先从构造函数入手



```java
static final class Node {
    ...

    volatile Node prev;
    volatile Node next;

    volatile Thread thread;

	...
    Node(Thread thread, int waitStatus) { // Used by Condition
        this.waitStatus = waitStatus;
        this.thread = thread;
    }
    ...
}
```

thread 是当前线程

waitStatus，的取值有

```java
/** waitStatus value to indicate thread has cancelled */
static final int CANCELLED =  1;
/** waitStatus value to indicate successor's thread needs unparking */
static final int SIGNAL    = -1;
/** waitStatus value to indicate thread is waiting on condition */
static final int CONDITION = -2;
/**
 * waitStatus value to indicate the next acquireShared should
 * unconditionally propagate
 */
static final int PROPAGATE = -3;
```

CONDITION 表示线程正在等待condition



把我们的脑子返回到addConditionWaiter函数

现在我们知道了，把新new的Node 加入到了链表，并被lastWaiter指向，然后反回了这个Node

再看看外面拿到这个Node做了什么操作



把我们的脑子返回到await函数

它调用了int savedState = fullyRelease(node);

？

返回了当前状态，但如果失败？会把Node 的waitStatus = Node.CANCELLED



```
LockSupport.unpark(node.thread);
```





### LockSupport

```
LockSupport.park();
LockSupport.unpark(Thread t);
```

park 可以让线程进入 WAITING 状态，unpark 可以让线程进入RUNNABLE 状态

* 可以不在监视器上下文中使用（不用再 sync 关键字范围内）
* 类似  Reentrylock ，估计内部维护了一个计数器，可以知道 某线程调用 park 的次数，和对某线程调用 unpark 的次数。只要 park计数 > unpark 计数，线程会一直WAITING, 当park计数<=unpark 计数，线程不会进入WAITING （或者从WAITING 变为 RUNNABLE ）。这意味着可以先unpark 1次，然后线程再park 1次，线程不会出现阻塞的情况



```
LockSuprot.park(Object blocker);
```

* 当线程被WAITING时，可以设一个block，以方便做功能扩展，调用LockSuport.getBlocker(Thread t) 可以返回当前线程被哪个 Blocker WAITING 住，在生产者和消费者的例子中，如果一个线程及时生产者也是消费者，因为某种原因，调度生产或消费者2个方法的概率不同，而然这个线程倾向于多生产或消费，直到存储区满或空时，产生阻塞，这时可以为这个线程这是不同的 blocker，在外部可以通过 getBlocker 查看这个线程时阻塞在生产或者消费环节，或者都没阻塞，然后根据阻塞在生产或消费，外部线程对这个工作线程下达不同的唤醒指令和额外的业务指令
*  当线程被唤醒后，Blocker 被置为null