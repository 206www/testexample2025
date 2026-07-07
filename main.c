#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include "main.h"
int main()
{
    int opt = 0;
    char buf[100]
    showMenu();
    while(1){
        printf("请输入您的选择：\n");
        scanf("%s",buf);
        while(1)
        {
            if(isdigit(buf[0]))
            {
                printf("您输入的选择有误，请重新输入！\n");
                scanf("%s",buf);
            }else if(atoi(&buf[0])<0||atoi(&buf[0])>5){
                printf("您输入的选择有误，请重新输入！\n");
                scanf("%s",buf);
            }else{
                opt = atoi(&buf[0]);
                break;
            }
        }
        
        switch(opt){
            case 0:
                return 0;
            case 1:
                createGraph(&G);
                printf("创建景区景点图成功！\n");
                break;
            case 2:
                printGraph(&G);
                printf("景区景点分布图\n");
                break;
            case 3:
                createGuideGraph(&G,&guidG);
                printf("创建导游路线成功！\n");
                break;
            case 4:
                loopTest(&guidG);
                printf("导游路线中的回路\n");
                break;
            case 5:
                printf("两个景点间的最短路径和距离\n");
                break;
        }
    }
    return 0;
}
#endif
