//
//#include <stdio.h>
//#include <signal.h>
//#include <setjmp.h>
//#include <unistd.h>
//#include <sys/time.h>
//#include <time.h>
//
//sigjmp_buf jmp_env;
//
//static void connect_alarm(int a)
//{
//    siglongjmp(jmp_env, 1);
//    printf("connect_alarm\n");
//}
//
//int test() {
//    // 当超时时间sec_timeout大于等于运行时间run_time时会跳过printf("running...\n");
//    int sec_timeout = 3;
//    int run_time = 4;
//    int ret;
//
//    printf("timeout = %d, run time = %d\n", sec_timeout, run_time);
//    if (sec_timeout)
//    {
//        // 超过用alarm函数设置的时间时产生此信号，调用connect_alarm函数
//        signal(SIGALRM, connect_alarm);
//        alarm(sec_timeout);
//        printf("set timeout\n");
//        if (sigsetjmp(jmp_env, 0))
//        {
//            printf("timeout\n");
//            ret = 1;
//        } else {
//            ret = 2;
//        }
//    }
//
//    sleep(run_time);
//    printf("running...\n");
//
//    return ret;
//
//}
//
//int main()
//{
//    int ret = test();
//    time_t t,t2;
//    time(&t);
//    sleep(10);
//    time(&t2);
//    printf("main quit ret = %d  time= %ld\n", ret, t2-t);
//    return 0;
//}
//
//
//
//
