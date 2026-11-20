#include <stdio.h>
#include <time.h>

/*
1. Unix时间戳（最常用）
 
从 格林威治时间1970-01-01 00:00:00（北京时间1970-01-01 08:00:00） 到当前这一刻
一共经过的秒数（10位）/毫秒数（13位），是一个纯数字
 
例： 1763892000  → 对应某个日期
 
特点：
 
同一瞬间，全世界的时间戳数字一样，不受时区影响，方便程序计算时间差、排序
数据库、日志、聊天记录、文件修改时间底层大多用它
 
2. 你前面看到的这种格式（日志时间戳）
 
 2026-11-20 12:33:56 
属于格式化时间戳，直接写成「年-月-日 时:分:秒」，方便人看懂，多用于日志、聊天记录。
*/


time_t time_cnt;
struct tm time_date;
char *time_str;

int main(){
    time_cnt=time(NULL);//时间戳(当前时间)
    //time(&time_cnt);
    //time_cnt=1795149478;
    printf("%d\n",time_cnt);

    time_date=*localtime(&time_cnt);
    printf("%d\n",time_date.tm_year+1900);//要加上1900
    printf("%d\n",time_date.tm_mon);
    printf("%d\n",time_date.tm_mday);
    printf("%d\n",time_date.tm_hour);
    printf("%d\n",time_date.tm_min);
    printf("%d\n",time_date.tm_sec);
    printf("%d\n",time_date.tm_wday);

    time_cnt=mktime(&time_date);
    printf("%d\n",time_cnt);

    time_str=ctime(&time_cnt);
    printf(time_str);
    
    time_str=asctime(&time_date);
    printf(time_str);


    char t[50];
    strftime(t,50,"%H-%M-%S",&time_date);
    printf(t);

    return 0;
}